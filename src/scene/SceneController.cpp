
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
#include <mesh/ModelNode.h>
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

    textRenderer = new Text("SourceCodePro-Regular.ttf", size, 12);
}

SceneController::~SceneController() {
    renderer = nullptr;

    delete camera;
    camera = nullptr;

    delete lightsUBO;
    lightsUBO = nullptr;

    delete skybox;
    skybox = nullptr;

    for(const auto& modelNode : models) {
        delete modelNode;
    }
    models.clear();

    for(const auto& drawable : drawables) {
        delete drawable;
    }
    drawables.clear();

    for(const auto& lightNode : lights) {
        delete lightNode;
    }
    lights.clear();

    delete textRenderer;
    textRenderer = nullptr;
}

void SceneController::AddNode(SceneNode* node) {
    ModelNode* modelNode = dynamic_cast<ModelNode*>(node);
    LightNode* lightNode = dynamic_cast<LightNode*>(node);

    if(modelNode) {
        models.push_back(modelNode);

        const std::vector<Mesh*>& meshes = modelNode->GetModel().GetMeshes();
        for(Mesh* mesh : meshes) {
            const Material* material = mesh->GetMaterial();
            const Shader& shader = material->GetShader();

            // bind the Camera uniform block
            shader.Use();
            shader.SetUniformBlockBinding("Camera", 0);

            // if the model uses lighting, set the lighting uniforms
            if(material->usesDirectLighting) {
                shader.SetUniformBlockBinding("Lights", 1);
            }
        }
    }
    else if(lightNode) {
        lights.push_back(lightNode);

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
    for(const auto& modelNode : visibleModels) {
        const std::vector<Mesh*>& meshes = modelNode->GetModel().GetMeshes();
        for(Mesh* mesh : meshes) {
            if(mesh->GetRenderType() == OPAQUE) {
                opaqueDrawCalls.push_back(DrawCall{
                    mesh,
                    mesh->GetMaterial(),
                    modelNode->GetTransform(),
                    modelNode->GetNormalMatrix()
                });
            }
            else {
                alphaDrawCalls.push_back(DrawCall{
                    mesh,
                    mesh->GetMaterial(),
                    modelNode->GetTransform(),
                    modelNode->GetNormalMatrix()
                });
            }

            stats.drawCalls++;
            stats.tris += mesh->GetNumTris();
        }
    }

    // opaque pass
    {
        CLOCKT(render_opaque, &stats.opaqueMs);

        renderer->RenderOpaque(opaqueDrawCalls);

        if(skybox != nullptr) {
            renderer->RenderSkybox(skybox, camera->GetView(), camera->GetProjection());
            stats.drawCalls++;
            stats.tris += 12;
        }
    }

    for(IDrawable* drawable : drawables) {
        drawable->Draw();
        stats.drawCalls++;
    }

    // transparency pass
    {
        CLOCKT(render_alpha, &stats.alphaMs);

        // sort furthest to closest length squared from camera position
        // NOTE this doesn't take rotation or scale into account

        const glm::vec3& camPosition = camera->GetPosition();
        std::sort(alphaDrawCalls.begin(), alphaDrawCalls.end(), [camPosition](const DrawCall& a, const DrawCall& b) {
            glm::vec3 aPos = { a.transform[3][0], a.transform[3][1], a.transform[3][2] };
            glm::vec3 bPos = { b.transform[3][0], b.transform[3][1], b.transform[3][2] };

            return glm::length2(aPos - camPosition) > glm::length2(bPos - camPosition);
        });

        renderer->RenderTransparent(alphaDrawCalls);
    }
}

void SceneController::FrustumCull(
    const Frustum& cameraFrustum,
    const std::vector<ModelNode*>& sceneModels,
    std::vector<ModelNode*>& visibleSceneModels
) {
    std::array<std::pair<int, int>, 6> frustumLUT = cameraFrustum.ComputeAABBTestLUT();

    // FIXME: this is a niave approach where all models in the scene are
    // iterated through.
    // TODO Look into using a BVH to quickly cull large swathes of scene models.
    for(const auto& modelNode : sceneModels) {
        const AABB& bounds = modelNode->GetBounds();
        const std::array<glm::vec3, 8>& boundsLUT = modelNode->GetLUT();

        // FrustumTestResult result = cameraFrustum.TestAABBIntersection(bounds);
        // FrustumTestResult result = cameraFrustum.TestAABBIntersection(bounds, boundsLUT, frustumLUT);
        FrustumTestResult result = cameraFrustum.TestAABBIntersection(bounds, boundsLUT, frustumLUT, &modelNode->boundsLastFailedFrustumPlane);

        if(result != OUTSIDE) {
            visibleSceneModels.push_back(modelNode);
        }
    }
}

void SceneController::RenderStats() {
    // CLOCK(render_ui);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    std::vector<std::string> strings = {};

    // view frustum culling

    std::ostringstream stream;
    stream.precision(2);
    stream << std::fixed << stats.vfcMs;
    strings.push_back(std::string("vfc: ") + stream.str() + std::string(" ms"));

    // opaque pass

    stream.str("");
    stream.clear();
    stream << std::fixed << stats.opaqueMs;
    strings.push_back(std::string("opaque pass: ") + stream.str() + std::string(" ms"));

    // alpha pass

    stream.str("");
    stream.clear();
    stream << std::fixed << stats.alphaMs;
    strings.push_back(std::string("alpha pass: ") + stream.str() + std::string(" ms"));

    // total geometry pass

    stream.str("");
    stream.clear();
    stream << std::fixed << stats.geometryMs;
    strings.push_back(std::string("total: ") + stream.str() + std::string(" ms"));

    // now draw the stats

    unsigned int edgeBuffer = 8;
    unsigned int fontSize = textRenderer->GetFontSize();
    unsigned int spacing = 2;

    unsigned int y = edgeBuffer;
    for(int i = strings.size() - 1; i >= 0; i--) {
        textRenderer->RenderText(strings[i], edgeBuffer, y);
        y += fontSize + spacing;
    }

    // number of draw calls

    textRenderer->RenderText(std::string("draw calls: ") + std::to_string(stats.drawCalls), 180, edgeBuffer + fontSize + spacing);

    // number of tris

    textRenderer->RenderText(std::string("tris: ") + std::to_string(stats.tris), 180, edgeBuffer);

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
