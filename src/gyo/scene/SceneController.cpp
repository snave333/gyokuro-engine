
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>

#include <gyo/scene/SceneController.h>
#include <gyo/scene/SceneNode.h>
#include <gyo/renderer/Renderer.h>
#include <gyo/renderer/DrawCall.h>
#include <gyo/drawable/IDrawable.h>
#include <gyo/resources/Resources.h>
#include <gyo/shading/Shader.h>
#include <gyo/lighting/LightNode.h>
#include <gyo/lighting/LightsUBO.h>
#include <gyo/mesh/ModelNode.h>
#include <gyo/mesh/Skybox.h>
#include <gyo/camera/FlyCamera.h>
#include <gyo/ui/Text.h>
#include <gyo/utilities/Clock.h>
#include <gyo/utilities/GetError.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace gyo {

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

    // textRenderer = new Text("SourceCodePro-Regular-MSDF", size, 33.9375f, 6);
    textRenderer = new Text("Ubuntu-Regular-MSDF", size, 33.125f, 6);
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

    visibleModels.clear();
    opaqueDrawCalls.clear();
    alphaDrawCalls.clear();

    renderer->BeginFrame(); // set frame buffer, clear
    
    RenderScene(); // opaque geometry, skybox, and transparent geometry passes
    
    renderer->EndGeometryPass(); // render our full screen quad

    // renderer->RenderImageEffects();

    // renderer->RenderUI();
    RenderStats();

    renderer->EndFrame(); // final tonemapping / gamma correction, swap buffers
}

void SceneController::RenderScene() {
    CLOCKT(geometry_pass, &renderer->stats.geometryMs);
    
    // view frustum culling
    {
        CLOCKT(frustum_culling, &renderer->stats.vfcMs);

        FrustumCull(camera->GetFrustum(), models, visibleModels);
    }

    // update the camera view matrix for our shaders
    camera->UpdateViewMatrixUniform();

    // separate our visible objects into two vectors - opaque and blended
    for(const auto& modelNode : visibleModels) {
        const std::vector<Mesh*>& meshes = modelNode->GetModel().GetMeshes();
        for(Mesh* mesh : meshes) {
            if(mesh->GetRenderType() == RenderType::OPAQUE) {
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

            renderer->stats.drawCalls++;
            renderer->stats.tris += mesh->GetNumTris();
        }
    }

    // opaque pass
    {
        CLOCKT(render_opaque, &renderer->stats.opaqueMs);

        renderer->RenderOpaque(opaqueDrawCalls, this->skybox);

        if(skybox != nullptr) {
            renderer->RenderSkybox(skybox, camera->GetView(), camera->GetProjection());
            renderer->stats.drawCalls++;
            renderer->stats.tris += 12;
        }
    }

    for(IDrawable* drawable : drawables) {
        drawable->Draw();
        renderer->stats.drawCalls++;
    }

    // transparency pass
    {
        CLOCKT(render_alpha, &renderer->stats.alphaMs);

        // sort furthest to closest length squared from camera position
        // NOTE this doesn't take rotation or scale into account, and only uses
        // the mesh position for comparison
        // TODO investigate more robust methods for sorting

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

        if(result != FrustumTestResult::OUTSIDE) {
            visibleSceneModels.push_back(modelNode);
        }
    }
}

void SceneController::RenderStats() {
    CLOCKT(render_ui, &renderer->stats.uiMs);

    std::vector<std::string> strings = {};
    std::ostringstream stream;
    stream.precision(0);

    // fps

    stream << std::fixed << renderer->stats.frame;
    strings.push_back(std::string("frame: ") + stream.str() + std::string(" ms"));

    // view frustum culling

    stream.str("");
    stream.clear();
    stream << std::fixed << renderer->stats.vfcMs;
    strings.push_back(std::string("- vfc: ") + stream.str() + std::string(" ms"));

    // opaque pass

    stream.str("");
    stream.clear();
    stream << std::fixed << renderer->stats.opaqueMs;
    strings.push_back(std::string("- opaque pass: ") + stream.str() + std::string(" ms"));

    // alpha pass

    stream.str("");
    stream.clear();
    stream << std::fixed << renderer->stats.alphaMs;
    strings.push_back(std::string("- alpha pass: ") + stream.str() + std::string(" ms"));

    // total ui pass

    stream.str("");
    stream.clear();
    stream << std::fixed << renderer->stats.uiMs;
    strings.push_back(std::string("- ui: ") + stream.str() + std::string(" ms"));
    
    // total geometry pass

    stream.str("");
    stream.clear();
    stream << std::fixed << renderer->stats.geometryMs;
    strings.push_back(std::string("total: ") + stream.str() + std::string(" ms"));
    
    // now draw the stats

    unsigned int edgeBuffer = 8;
    unsigned int fontSize = 20;
    unsigned int spacing = 2;

    unsigned int y = edgeBuffer;
    for(int i = strings.size() - 1; i >= 0; i--) {
        textRenderer->QueueStringRender(strings[i], edgeBuffer, y);
        y += fontSize + spacing;
    }
    // the ui draw call below
    renderer->stats.drawCalls++;

    // number of draw calls

    textRenderer->QueueStringRender(std::string("draw calls: ") + std::to_string(renderer->stats.drawCalls), 180, edgeBuffer + fontSize + spacing);

    // number of tris

    textRenderer->QueueStringRender(std::string("tris: ") + std::to_string(renderer->stats.tris), 180, edgeBuffer);

    // finally, execute the render

    glEnable(GL_BLEND);
    glCheckError();
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glCheckError();

    textRenderer->ExecuteRender();

    glDisable(GL_BLEND);
    glCheckError();
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

} // namespace gyo
