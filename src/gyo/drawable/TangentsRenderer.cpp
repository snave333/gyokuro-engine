
#include <gyo/drawable/TangentsRenderer.h>
#include <gyo/resources/Resources.h>
#include <gyo/shading/Shader.h>

#include <glad/glad.h>
#include <gyo/utilities/GetError.h>

namespace gyo {

TangentsRenderer::TangentsRenderer(Geometry& geometry, float lineLength) : lineLength(lineLength) {
    this->lineLength = glm::max(lineLength, 0.01f);

    geometry.ComputeTangents();

    // translate to TangentVertex types
    std::vector<TangentVertex> vertices;
    for(const auto& v : geometry.vertices) {
        vertices.push_back(TangentVertex(v));
    }
    vertexCount = vertices.size();

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
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(TangentVertex), &vertices[0], GL_STATIC_DRAW);
    glCheckError();

    // link the vertex attribute pointers
    // positions
    glEnableVertexAttribArray(0);	
    glCheckError();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TangentVertex), (void*)0);
    glCheckError();
    // normals
    glEnableVertexAttribArray(1);	
    glCheckError();
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(TangentVertex), (void*)offsetof(TangentVertex, normal));
    glCheckError();
    // tangents
    glEnableVertexAttribArray(2);	
    glCheckError();
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(TangentVertex), (void*)offsetof(TangentVertex, tangent));
    glCheckError();

    // clean up and unbind
    glBindVertexArray(0);
    glCheckError();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glCheckError();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glCheckError();

    // create shader
    shader = Resources::GetShader("tangents.vert", "tangents.geom", "vertexColor.frag");
    shader->Use();
    shader->SetUniformBlockBinding("Camera", 0);
    shader->SetFloat("lineLength", this->lineLength);
}

TangentsRenderer::~TangentsRenderer() {
    glDeleteVertexArrays(1, &VAO);
    glCheckError();
    glDeleteBuffers(1, &VBO);
    glCheckError();

    shader = nullptr;
}

void TangentsRenderer::Update(const glm::mat4& modelMatrix, const glm::mat4& normalMatrix) {
    shader->Use();
    shader->SetMat4("model", modelMatrix);
    shader->SetMat4("normalMatrix", normalMatrix);
}

void TangentsRenderer::Draw() {
    shader->Use();

    glBindVertexArray(VAO);
    glCheckError();
    // draw points, and let the geometry shader draw the lines
    glDrawArrays(GL_POINTS, 0, vertexCount);
    glCheckError();
    glBindVertexArray(0);
    glCheckError();
}

} // namespace gyo
