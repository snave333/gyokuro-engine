
#include <gyo/renderer/ScreenQuad.h>
#include <gyo/shading/Shader.h>
#include <gyo/resources/Resources.h>
#include <gyo/utilities/GetError.h>

#include <glad/glad.h>

namespace gyo {

ScreenQuad::ScreenQuad() {
    float vertices[] = {  
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    // create the vertex/index buffers and vertex array object
    glGenVertexArrays(1, &VAO);
    glCheckError();
    glGenBuffers(1, &VBO);
    glCheckError();

    // bind Vertex Array Object first
    glBindVertexArray(VAO);
    glCheckError();

    // copy our vertices array in a buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glCheckError();
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
    glCheckError();

    // link the vertex attribute pointers
    // screen-space positions
    glEnableVertexAttribArray(0);	
    glCheckError();
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glCheckError();
    // texture coords
    glEnableVertexAttribArray(1);
    glCheckError();
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glCheckError();

    // clean up and unbind
    glBindVertexArray(0);
    glCheckError();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glCheckError();

    // now create and configure our shader
    shader = Resources::GetShader("screen.vert", "screen.frag");
    shader->Use();
    shader->SetInt("screenTexture", 0);
}

ScreenQuad::~ScreenQuad() {
    glDeleteVertexArrays(1, &VAO);
    glCheckError();
    glDeleteBuffers(1, &VBO);
    glCheckError();

    shader = nullptr;
}

void ScreenQuad::Draw(unsigned int textureColorbuffer) {
    shader->Use();

    glBindVertexArray(VAO);
    glCheckError();
    
    glActiveTexture(GL_TEXTURE0);
    glCheckError();
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glCheckError();

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glCheckError();
}

} // namespace gyo
