
#include <iostream>

#include <scene/SceneController.h>
#include <renderer/Renderer.h>
#include <renderer/Shader.h>
#include <renderer/Texture2D.h>
#include <mesh/Quad.h>
#include <mesh/Cube.h>
#include <camera/PerspectiveCamera.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>

SceneController::SceneController(Renderer* r) {
    renderer = r;

    camera = new PerspectiveCamera(60, 1, 0.1f, 100);

    mesh = new Cube();

    shader = new Shader("shader.vert", "shader.frag");

    texture1 = new Texture2D("wall.jpg");
    texture2 = new Texture2D("awesomeface.png", true);

    // assigned each sampler to a texture unit
    shader->Use();
    shader->SetInt("texture1", 0);
    shader->SetInt("texture2", 1);

    // set our MVP matrices

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));

    shader->SetMat4("model", model);
    shader->SetMat4("view", camera->GetView());
    shader->SetMat4("projection", camera->GetProjection());
}

SceneController::~SceneController() {
    renderer = nullptr;

    delete camera;
    camera = nullptr;

    delete mesh;
    mesh = nullptr;

    delete shader;
    shader = nullptr;

    delete texture1;
    texture1 = nullptr;
    delete texture2;
    texture2 = nullptr;
}

void SceneController::Update(float dt) {
    model = glm::rotate(model, dt, glm::normalize(glm::vec3(0.5, 1.0, 0.0)));
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
    // shader->SetFloat("asdf", 1.0f);
    shader->SetMat4("model", model);

    texture1->Bind(0);
    texture2->Bind(1);

    mesh->Draw(*shader);
}
