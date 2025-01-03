
#include <iostream>

#include <scene/SceneController.h>
#include <renderer/Renderer.h>
#include <renderer/Shader.h>
#include <renderer/Texture2D.h>
#include <mesh/Quad.h>
#include <mesh/Cube.h>
#include <mesh/Model.h>
#include <camera/FlyCamera.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

SceneController::SceneController(Renderer* r, const int& width, const int& height) {
    renderer = r;

    camera = new FlyCamera(Camera::PerspectiveCamera(60, width / height));
    // camera = new FlyCamera(Camera::OrthographicCamera(3, width / height));

    camera->Translate(0, 0, 5);

    // spawn an array of models for frustum culling testing
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
                Model* model = new Model(new Cube());
                model->SetPosition(x * spacing, y * spacing, z * spacing);
                model->SetScale(0.5f);
                models.push_back(model);
            }
        }
    }
    // model = new Model(new Cube());

    // model->Translate(2, 0, 0);
    // model->Rotate(0, 0, 30);
    // model->Scale(2);
    // glm::vec3 dir = model->GetRight();
    // std::cout << dir.x << ", " << dir.y << ", " << dir.z << std::endl;

    shader = new Shader("shader.vert", "shader.frag");

    texture1 = new Texture2D("wall.jpg");
    texture2 = new Texture2D("awesomeface.png", true);

    // assigned each sampler to a texture unit
    shader->Use();
    shader->SetInt("texture1", 0);
    shader->SetInt("texture2", 1);

    // set our MVP matrices

    // shader->SetMat4("model", model->GetTransform());
    // shader->SetMat4("view", camera->GetView());
    shader->SetMat4("projection", camera->GetProjection());
}

SceneController::~SceneController() {
    renderer = nullptr;

    delete camera;
    camera = nullptr;

    for(const auto& model : models) {
        delete model;
        // model = nullptr;
    }
    models.clear();

    delete shader;
    shader = nullptr;

    delete texture1;
    texture1 = nullptr;
    delete texture2;
    texture2 = nullptr;
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
    renderer->Clear();

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

    const Frustum& cameraFrustum = camera->GetFrustum();

    int totalCount = models.size();
    int drawCount = 0;
    for(const auto& model : models) {
        AABB bounds = model->GetBounds();

        FrustumTestResult result = cameraFrustum.TestAABBIntersection(bounds);

        if(result == Outside) {
            continue;
        }

        // draw scene objects
        
        shader->Use();

        // set any shader uniforms
        shader->SetMat4("model", model->GetTransform());
        shader->SetMat4("view", camera->GetView());

        switch (result)
        {
            case Inside:
                shader->SetVec4("tint", glm::vec4(1));
                break;
            case Intersecting:
                shader->SetVec4("tint", glm::vec4(0.5f, 0.5f, 1, 1));
                break;
            case Outside:
                shader->SetVec4("tint", glm::vec4(1, 0.5f, 0.5f, 1));
                break;
        }

        texture1->Bind(0);
        texture2->Bind(1);

        model->Draw(*shader);

        drawCount++;
    }

    std::cout << "Culled " << (totalCount - drawCount) << " meshes" << std::endl;
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

    xOffset *= mouseSensitivity;
    yOffset *= mouseSensitivity;

    if(camera != nullptr) {
        camera->OnLook(xOffset, yOffset);
    }
}
