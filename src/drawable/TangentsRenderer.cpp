
#include <drawable/TangentsRenderer.h>
#include <resources/Resources.h>
#include <shading/Shader.h>

#include <glad/glad.h>

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
    glGenBuffers(1, &VBO);
    
    // bind Vertex Array Object first
    glBindVertexArray(VAO);
    
    // copy our vertices array in a buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(TangentVertex), &vertices[0], GL_STATIC_DRAW);

    // link the vertex attribute pointers
    // positions
    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TangentVertex), (void*)0);
    // normals
    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(TangentVertex), (void*)offsetof(TangentVertex, normal));
    // tangents
    glEnableVertexAttribArray(2);	
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(TangentVertex), (void*)offsetof(TangentVertex, tangent));

    // clean up and unbind
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // create shader
    shader = Resources::GetShader("tangents.vert", "tangents.geom", "vertexColor.frag");
    shader->Use();
    shader->SetUniformBlockBinding("Camera", 0);
    shader->SetFloat("lineLength", this->lineLength);
}

TangentsRenderer::~TangentsRenderer() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

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
    // draw points, and let the geometry shader draw the lines
    glDrawArrays(GL_POINTS, 0, vertexCount);
    glBindVertexArray(0);
}
