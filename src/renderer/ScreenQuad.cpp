
#include <renderer/ScreenQuad.h>
#include <shading/Shader.h>
#include <resources/Resources.h>

#include <glad/glad.h>

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
    glGenBuffers(1, &VBO);

    // bind Vertex Array Object first
    glBindVertexArray(VAO);

    // copy our vertices array in a buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

    // link the vertex attribute pointers
    // screen-space positions
    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    // texture coords
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    // clean up and unbind
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // now create and configure our shader
    shader = Resources::GetShader("screen.vert", "screen.frag");
    shader->Use();
    shader->SetInt("screenTexture", 0);
}

ScreenQuad::~ScreenQuad() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    shader = nullptr;
}

void ScreenQuad::Draw(unsigned int textureColorbuffer) {
    shader->Use();

    glBindVertexArray(VAO);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}
