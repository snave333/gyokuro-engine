
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>

#include <scene/SceneController.h>
#include <scene/SceneNode.h>
#include <renderer/Renderer.h>
#include <renderer/DrawCall.h>
#include <drawable/IDrawable.h>
#include <resources/Resources.h>
#include <shading/Shader.h>
#include <lighting/LightNode.h>
#include <lighting/LightsUBO.h>
#include <mesh/Model.h>
#include <mesh/Skybox.h>
#include <camera/FlyCamera.h>
#include <ui/Text.h>
#include <utilities/Clock.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

SceneController::SceneController(Renderer* r, const int& width, const int& height) {
    renderer = r;
    size = glm::ivec2(width, height);

    lightsUBO = new LightsUBO();
    lightsUBO->UpdateValues(ambientLight, lights);

    // setup our default camera

    camera = new FlyCamera(Camera::PerspectiveCamera(60, (float)width / height));
    // camera = new FlyCamera(Camera::OrthographicCamera(5, (float)width / height));
    camera->Translate(0, 0, -3);

    // our ui layer

    textRenderer = new Text("SourceCodePro-Regular.ttf", size, 14);
}

SceneController::~SceneController() {
    renderer = nullptr;

    delete camera;
    camera = nullptr;

    delete lightsUBO;
    lightsUBO = nullptr;

    delete skybox;
    skybox = nullptr;

    for(const auto& model : models) {
        delete model;
    }
    models.clear();

    for(const auto& drawable : drawables) {
        delete drawable;
    }
    drawables.clear();

    for(const auto& light : lights) {
        delete light;
    }
    lights.clear();

    delete textRenderer;
    textRenderer = nullptr;
}

void SceneController::AddNode(SceneNode* node) {
    Model* model = dynamic_cast<Model*>(node);
    LightNode* light = dynamic_cast<LightNode*>(node);

    if(model) {
        models.push_back(model);

        const Material* material = model->GetMaterial();
        const Shader& shader = material->GetShader();

        // bind the Camera uniform block
        shader.Use();
        shader.SetUniformBlockBinding("Camera", 0);

        // if the model uses lighting, set the lighting uniforms
        if(material->usesDirectLighting) {
            shader.SetUniformBlockBinding("Lights", 1);
        }
    }
    else if(light) {
        lights.push_back(light);

        // update the light uniform block

        lightsUBO->UpdateValues(ambientLight, lights);
    }
}

void SceneController::AddDrawable(IDrawable* drawable) {
    drawables.push_back(drawable);
}

void SceneController::SetSkybox(Skybox* skybox) {
    // clear any existing skybox if we have one
    if(this->skybox != nullptr) {
        delete this->skybox;
        this->skybox = nullptr;
    }

    // set the new skybox if it isn't null
    if(skybox != nullptr) {
        this->skybox = skybox;
    }
}

void SceneController::Update(float dt) {
    for (const auto& func : updateFunctions) {
        func(dt);
    }
}

void SceneController::Render() {
    if(renderer == nullptr) {
        return;
    }

    renderer->BeginFrame(); // set frame buffer, clear
    
    RenderScene(); // opaque geometry, skybox, and transparent geometry passes
    
    renderer->EndGeometryPass(); // render our full screen quad

    // renderer->RenderImageEffects();

    // renderer->RenderUI();
    RenderStats();

    renderer->EndFrame(); // final tonemapping / gamma correction, swap buffers

    stats.Reset();
    visibleModels.clear();
    opaqueDrawCalls.clear();
    alphaDrawCalls.clear();
}

void SceneController::RenderScene() {
    CLOCKT(geometry_pass, &stats.geometryMs);
    
    // view frustum culling
    {
        CLOCKT(frustum_culling, &stats.vfcMs);

        FrustumCull(camera->GetFrustum(), models, visibleModels);
    }

    // update the camera view matrix for our shaders
    camera->UpdateViewMatrixUniform();

    // separate our visible objects into two vectors - opaque and blended
    for(const auto& model : visibleModels) {
        if(model->GetRenderType() == OPAQUE) {
            opaqueDrawCalls.push_back(DrawCall{ model->GetMesh(), model->GetMaterial(), model->GetTransform(), model->GetNormalMatrix() });
        }
        else {
            alphaDrawCalls.push_back(DrawCall{ model->GetMesh(), model->GetMaterial(), model->GetTransform(), model->GetNormalMatrix() });
        }
        stats.drawCalls++;
    }

    // opaque pass
    {
        CLOCKT(render_opaque, &stats.opaqueMs);

        renderer->RenderOpaque(opaqueDrawCalls);

        if(skybox != nullptr) {
            renderer->RenderSkybox(skybox, camera->GetView(), camera->GetProjection());
            stats.drawCalls++;
        }
    }

    for(IDrawable* drawable : drawables) {
        drawable->Draw();
    }

    // transparency pass
    {
        CLOCKT(render_alpha, &stats.alphaMs);

        // sort furthest to closest length squared from camera position
        // NOTE this doesn't take rotation or scale into account

        const glm::vec3& camPosition = camera->GetPosition();
        std::sort(alphaDrawCalls.begin(), alphaDrawCalls.end(), [camPosition](const DrawCall a, const DrawCall b) {
            const glm::vec3& aPos = glm::vec3(a.transform[3][0], a.transform[3][1], a.transform[3][2]);
            const glm::vec3& bPos = glm::vec3(b.transform[3][0], b.transform[3][1], b.transform[3][2]);;

            return glm::length2(aPos - camPosition) > glm::length2(bPos - camPosition);
        });

        renderer->RenderTransparent(alphaDrawCalls);
    }
}

void SceneController::FrustumCull(
    const Frustum& cameraFrustum,
    const std::vector<Model*>& sceneModels,
    std::vector<Model*>& visibleSceneModels
) {
    std::array<std::pair<int, int>, 6> frustumLUT = cameraFrustum.ComputeAABBTestLUT();

    // FIXME: this is a niave approach where all models in the scene are
    // iterated through.
    // TODO Look into using a BVH to quickly cull large swathes of scene models.
    for(const auto& model : sceneModels) {
        const AABB& bounds = model->GetBounds();
        const std::array<glm::vec3, 8>& boundsLUT = model->GetLUT();

        // FrustumTestResult result = cameraFrustum.TestAABBIntersection(bounds);
        // FrustumTestResult result = cameraFrustum.TestAABBIntersection(bounds, boundsLUT, frustumLUT);
        FrustumTestResult result = cameraFrustum.TestAABBIntersection(bounds, boundsLUT, frustumLUT, &model->boundsLastFailedFrustumPlane);

        if(result != OUTSIDE) {
            visibleSceneModels.push_back(model);
        }
    }
}

void SceneController::RenderStats() {
    // CLOCK(render_ui);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // number of draw calls

    textRenderer->RenderText(std::string("draw calls: ") + std::to_string(stats.drawCalls), 10, 90);
    stats.drawCalls++;

    // view frustum culling

    std::ostringstream stream;
    stream.precision(2);
    stream << std::fixed << stats.vfcMs;

    std::string vfcMs = stream.str();
    textRenderer->RenderText(std::string("vfc: ") + vfcMs + std::string(" ms"), 10, 70);
    stats.drawCalls++;

    // opaque pass

    stream.str("");
    stream.clear();
    stream << std::fixed << stats.opaqueMs;

    std::string opaqueMs = stream.str();
    textRenderer->RenderText(std::string("opaque pass: ") + opaqueMs + std::string(" ms"), 10, 50);
    stats.drawCalls++;

    // alpha pass

    stream.str("");
    stream.clear();
    stream << std::fixed << stats.alphaMs;

    std::string alphaMs = stream.str();
    textRenderer->RenderText(std::string("alpha pass: ") + opaqueMs + std::string(" ms"), 10, 30);
    stats.drawCalls++;

    // total geometry pass

    stream.str("");
    stream.clear();
    stream << std::fixed << stats.geometryMs;

    std::string geometryMs = stream.str();
    textRenderer->RenderText(std::string("total: ") + geometryMs + std::string(" ms"), 10, 10);
    stats.drawCalls++;

    glDisable(GL_BLEND);
}

void SceneController::OnKeyPressed(int key, float dt) {
    float cameraSpeed = 4.0f;
    glm::vec3 velocity(0);

    switch(key) {
        case GLFW_KEY_W:
            velocity += camera->GetForward();
            break;
        case GLFW_KEY_S:
            velocity -= camera->GetForward();
            break;
        case GLFW_KEY_A:
            velocity += camera->GetRight();
            break;
        case GLFW_KEY_D:
            velocity -= camera->GetRight();
            break;
        case GLFW_KEY_E:
            velocity += glm::vec3(0, 1, 0);
            break;
        case GLFW_KEY_Q:
            velocity -= glm::vec3(0, 1, 0);
            break;
    }

    if(velocity != glm::vec3(0)) {
        camera->Translate(velocity * dt * cameraSpeed);
    }
}

void SceneController::OnMouseMove(float x, float y) {
    // the first mouse move; initialize the position values
    if(lastMouseX == 0 && lastMouseY == 0) {
        lastMouseX = x;
        lastMouseY = y;
    }

    // calculate our mouse delta

    float xOffset = lastMouseX - x;
    float yOffset = y - lastMouseY;
    lastMouseX = x;
    lastMouseY = y;

    if(camera != nullptr) {
        camera->OnLook(xOffset, yOffset);
    }
}
