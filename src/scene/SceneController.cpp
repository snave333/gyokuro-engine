
#include <iostream>

#include <scene/SceneController.h>
#include <renderer/Renderer.h>
#include <renderer/Shader.h>
#include <renderer/Texture2D.h>

#include <glad/glad.h>

SceneController::SceneController(Renderer* r) {
    renderer = r;

    ourShader = new Shader("shader.vert", "shader.frag");

    // Texture2D* texture = new Texture2D("/Users/spencerevans/pm-repos/gyokuro-engine/build/resources/textures/wall.jpg");
    // delete texture;
    // texture = nullptr;

    // create the vertex/index buffers and vertex array object
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);
    
    // bind Vertex Array Object first
    glBindVertexArray(VAO);
    
    // copy our vertices array in a buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // copy our indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    // link the vertex attribute pointers
    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // clean up and unbind
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

SceneController::~SceneController() {
    renderer = nullptr;

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    delete ourShader;
    ourShader = nullptr;
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
    
    ourShader->Use();

    // set any shader uniforms
    // ourShader->SetFloat("asdf", 1.0f);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
