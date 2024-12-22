
#include <iostream>

#include <scene/SceneController.h>
#include <renderer/Renderer.h>
#include <renderer/Shader.h>
#include <renderer/Texture2D.h>
#include <mesh/Quad.h>
#include <mesh/Cube.h>

#include <glm/gtc/matrix_transform.hpp>

#include <glad/glad.h>

SceneController::SceneController(Renderer* r) {
    renderer = r;

    mesh = new Cube();

    shader = new Shader("shader.vert", "shader.frag");

    texture1 = new Texture2D("/Users/spencerevans/pm-repos/gyokuro-engine/build/resources/textures/wall.jpg");
    texture2 = new Texture2D("/Users/spencerevans/pm-repos/gyokuro-engine/build/resources/textures/awesomeface.png", true);

    // assigned each sampler to a texture unit
    shader->Use();
    shader->SetInt("texture1", 0);
    shader->SetInt("texture2", 1);

    // set our MVP matrices

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));

    // move the camera back
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), 512.0f / 512.0f, 0.1f, 100.0f);

    shader->SetMat4("model", model);
    shader->SetMat4("view", view);
    shader->SetMat4("projection", projection);
}

SceneController::~SceneController() {
    renderer = nullptr;

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
