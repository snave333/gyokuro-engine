
#include <iostream>

#include <scene/SceneController.h>
#include <renderer/Renderer.h>
#include <shading/UnlitMaterial.h>
#include <shading/Shader.h>
#include <shading/Texture2D.h>
#include <mesh/Quad.h>
#include <mesh/Cube.h>
#include <mesh/Torus.h>
#include <mesh/Model.h>
#include <camera/FlyCamera.h>
#include <utilities/Clock.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

SceneController::SceneController(Renderer* r, const int& width, const int& height) {
    renderer = r;
    size = glm::vec2(width, height);

    camera = new FlyCamera(Camera::PerspectiveCamera(60, width / height));
    // camera = new FlyCamera(Camera::OrthographicCamera(3, width / height));
    camera->Translate(0, 0, 3);

    // spawn an array of models for frustum culling testing
    /*
    int w = 10;
    int h = 5;
    int d = 10;
    float spacing = 2;
    float halfW = w / 2.0f;
    float halfH = h / 2.0f;
    float halfD = d / 2.0f;
    for(int x = -floorf(halfW); x < ceilf(halfW); x++) {
        for(int y = -floorf(halfH); y < ceilf(halfH); y++) {
            for(int z = -floorf(halfD); z < ceilf(halfD); z++) {
                Model* model = new Model(new Mesh(new Torus(), new UnlitMaterial()));
                model->SetPosition(x * spacing, y * spacing, z * spacing);
                model->SetScale(0.5f);
                models.push_back(model);
            }
        }
    }
    */
    
    Model* model = new Model(new Mesh(new Torus(), new UnlitMaterial(glm::vec4(1, 0, 0, 1), "wall.jpg")));
    models.push_back(model);

    model->Translate(0, -1, 0);
    model->Rotate(90, 0, 0);
    // model->Scale(2);
    // glm::vec3 dir = model->GetRight();
    // std::cout << dir.x << ", " << dir.y << ", " << dir.z << std::endl;
}

SceneController::~SceneController() {
    renderer = nullptr;

    delete camera;
    camera = nullptr;

    for(const auto& model : models) {
        delete model;
    }
    models.clear();
}

void SceneController::Update(float dt) {
    // model->Translate(dt / 2, 0, 0);
    // model->Translate(glm::vec3(dt / 2, 0, 0));
    // model->Rotate(dt * 30, glm::normalize(glm::vec3(0.5, 1.0, 0.0)));
    // model->Rotate(0, dt * 60, 0);
    // model->SetScale(glm::sin(glfwGetTime()) + 1);

    // model->SetPosition(glm::sin(glfwGetTime()), 0, 0);
    // model->SetRotation(glm::sin(glfwGetTime()) * 30, 0, 0);
    // model->SetScale(glm::sin(glfwGetTime()) / 2 + 1);
}

void SceneController::Render() {
    if(renderer == nullptr) {
        return;
    }
    
    renderer->BeginFrame(); // set render targets, etc

    RenderScene();
    
    renderer->EndFrame(); // swap buffers
}

void SceneController::RenderScene() {
    /**
     * vector<Mesh*> visible
     * vector<Mesh*> opaque
     * vector<Mesh*> transparent
     * 
     * - update view matrix uniform block
     * - frustum culling
     * - separate opaque/transparent
     * - render opaque :: renderer->RenderOpaque(opaque)
     * - skybox pass
     * - sort transparent back to front
     * - render transparent :: renderer->RenderOpaque(opaque)
     */

    std::vector<Model*> visibleModels = {};
    {
        // CLOCK(frustum_culling);

        const Frustum& cameraFrustum = camera->GetFrustum();
        std::array<std::pair<int, int>, 6> frustumLUT = cameraFrustum.ComputeAABBTestLUT();

        for(const auto& model : models) {
            const AABB& bounds = model->GetBounds();
            const std::array<glm::vec3, 8>& boundsLUT = model->GetLUT();

            // FrustumTestResult result = cameraFrustum.TestAABBIntersection(bounds);
            FrustumTestResult result = cameraFrustum.TestAABBIntersection(bounds, boundsLUT, frustumLUT);

            if(result != Outside) {
                visibleModels.push_back(model);
            }
        }
    }

    // FIXME this should be done as models are added to the scene
    std::vector<Model*> opaqueModels = {};
    std::vector<Model*> transparentModels = {};
    for(const auto& model : visibleModels) {
        if(model->GetRenderType() == Opaque) {
            opaqueModels.push_back(model);
        }
        else {
            transparentModels.push_back(model);
        }
    }

    {
        // CLOCK(render_opaque);

        for(const auto& model : opaqueModels) {
            model->Queue();
            const Shader& shader = model->GetShader();

            // set any shader uniforms
            shader.SetMat4("model", model->GetTransform());
            shader.SetMat4("view", camera->GetView());
            shader.SetMat4("projection", camera->GetProjection());

            // TODO set uniform data for any scene direct lights, if applicable

            model->Draw();
        }
    }

    // int totalCount = models.size();
    // int drawCount = visibleModels.size();
    // std::cout << "Culled " << (totalCount - drawCount) << " meshes" << std::endl;
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
            velocity -= camera->GetRight();
            break;
        case GLFW_KEY_D:
            velocity += camera->GetRight();
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

    float xOffset = x - lastMouseX;
    float yOffset = y - lastMouseY;
    lastMouseX = x;
    lastMouseY = y;

    if(camera != nullptr) {
        camera->OnLook(xOffset, yOffset);
    }
}
