
#include <iostream>

#include <scene/SceneController.h>
#include <renderer/Renderer.h>
#include <renderer/Shader.h>
#include <renderer/Texture2D.h>
#include <renderer/Mesh.h>

#include <glm/gtc/matrix_transform.hpp>

#include <glad/glad.h>

SceneController::SceneController(Renderer* r) {
    renderer = r;

    std::vector<Vertex> quadVertices = {
        { {  0.5f,  0.5f, 0.0f }, { 1.0f, 0.5f, 0.0f }, { 1.0f, 1.0f } }, // top right
        { {  0.5f, -0.5f, 0.0f }, { 0.5f, 1.0f, 0.0f }, { 1.0f, 0.0f } }, // bottom right
        { { -0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f, 0.5f }, { 0.0f, 0.0f } }, // bottom left
        { { -0.5f,  0.5f, 0.0f }, { 0.0f, 0.5f, 1.0f }, { 0.0f, 1.0f } }  // top left
    };
    std::vector<unsigned int> quadIndices = {
        0, 1, 3, // first triangle
        1, 2, 3 // second triangle
    };

    mesh = new Mesh(quadVertices, quadIndices);

    shader = new Shader("shader.vert", "shader.frag");

    texture1 = new Texture2D("/Users/spencerevans/pm-repos/gyokuro-engine/build/resources/textures/wall.jpg");
    texture2 = new Texture2D("/Users/spencerevans/pm-repos/gyokuro-engine/build/resources/textures/awesomeface.png", true);

    // assigned each sampler to a texture unit
    shader->Use();
    shader->SetInt("texture1", 0);
    shader->SetInt("texture2", 1);



    trans = glm::mat4(1.0f);
    trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
    // trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
    // trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));

    shader->SetMat4("transform", trans);
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
    trans = glm::rotate(trans, dt, glm::vec3(0.0, 0.0, 1.0));
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
    shader->SetMat4("transform", trans);

    texture1->Bind(0);
    texture2->Bind(1);

    mesh->Draw(*shader);
}
