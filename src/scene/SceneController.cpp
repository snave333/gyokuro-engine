
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

SceneController::SceneController(Renderer* r) {
    renderer = r;

    float w = 800;
    float h = 600;
    camera = new FlyCamera(Camera::PerspectiveCamera(60, w / h));
    // camera = new FlyCamera(Camera::OrthographicCamera(3, w / h));

    camera->Translate(0, 0, 5);

    model = new Model(new Cube());

    shader = new Shader("shader.vert", "shader.frag");

    texture1 = new Texture2D("wall.jpg");
    texture2 = new Texture2D("awesomeface.png", true);

    // assigned each sampler to a texture unit
    shader->Use();
    shader->SetInt("texture1", 0);
    shader->SetInt("texture2", 1);

    // set our MVP matrices

    shader->SetMat4("model", model->GetTransform());
    shader->SetMat4("view", camera->GetView());
    shader->SetMat4("projection", camera->GetProjection());
}

SceneController::~SceneController() {
    renderer = nullptr;

    delete camera;
    camera = nullptr;

    delete model;
    model = nullptr;

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
    // model->Rotate(dt * 60, glm::normalize(glm::vec3(0.5, 1.0, 0.0)));
    // model->Rotate(0, dt * 60, 0);
    // model->SetScale(glm::sin(glfwGetTime()) + 1);

    model->SetPosition(glm::sin(glfwGetTime()), 0, 0);
    model->SetRotation(glm::sin(glfwGetTime()) * 30, 0, 0);
    model->SetScale(glm::sin(glfwGetTime()) + 1);
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

    // draw scene objects
    
    shader->Use();

    // set any shader uniforms
    shader->SetMat4("model", model->GetTransform());

    texture1->Bind(0);
    texture2->Bind(1);

    model->Draw(*shader);
}

void SceneController::OnKeyboardInput(int key, int action, int mods) {
    // TODO forward key presses to fly camera for movement

    switch(key) {
        case GLFW_KEY_W:
            break;
        case GLFW_KEY_S:
            break;
    }
}