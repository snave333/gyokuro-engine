
#include <iostream>

#include <scene/SceneController.h>
#include <renderer/Renderer.h>
#include <renderer/Shader.h>
#include <renderer/Texture2D.h>
#include <renderer/Mesh.h>

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

    texture = new Texture2D("/Users/spencerevans/pm-repos/gyokuro-engine/build/resources/textures/wall.jpg");
}

SceneController::~SceneController() {
    renderer = nullptr;

    delete mesh;
    mesh = nullptr;

    delete shader;
    shader = nullptr;

    delete texture;
    texture = nullptr;
}

void SceneController::Update(float dt) {
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

    glBindTexture(GL_TEXTURE_2D, texture->ID);

    mesh->Draw(*shader);
}
