
#include <gyo/mesh/Skybox.h>
#include <gyo/shading/Shader.h>
#include <gyo/resources/Resources.h>
#include <gyo/shading/TextureCube.h>

#include <glad/glad.h>

namespace gyo {

Skybox::Skybox(TextureCube* cubeMap) {
    this->cubeMap = cubeMap;

    float vertices[] = {  
        // positions
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
    };

    // create the vertex/index buffers and vertex array object
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // bind Vertex Array Object first
    glBindVertexArray(VAO);

    // copy our vertices array in a buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

    // link the vertex attribute pointers
    // positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // clean up and unbind
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // now create and configure our shader
    shader = Resources::GetShader("skybox.vert", "skybox.frag");
    shader->Use();
    shader->SetInt("skybox", 0);
}

Skybox::~Skybox() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    cubeMap = nullptr;
    shader = nullptr;
}

void Skybox::Draw(glm::mat4 view, glm::mat4 projection) {
    // remove translation from the view matrix
    glm::mat4 viewNoTranslation = glm::mat4(glm::mat3(view));

    shader->Use();
    shader->SetMat4("view", viewNoTranslation);
    shader->SetMat4("projection", projection);

    glBindVertexArray(VAO);
    cubeMap->Bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

} // namespace gyo
