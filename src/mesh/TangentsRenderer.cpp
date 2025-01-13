
#include <mesh/TangentsRenderer.h>
#include <resources/Resources.h>

#include <glad/glad.h>

TangentsRenderer::TangentsRenderer(Geometry& geometry, float lineLength) : lineLength(lineLength) {
    this->lineLength = glm::max(lineLength, 0.01f);

    geometry.CalculateTangents();

    // generate our line vertices and indices
    int numVerts = geometry.vertices.size();
    std::vector<TangentVertex> vertices = std::vector<TangentVertex>(numVerts * 6);
    std::vector<unsigned int> indices = std::vector<unsigned int>(numVerts * 6);
    GetLines(geometry, vertices, indices);

    // create the vertex/index buffers and vertex array object
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    // bind Vertex Array Object first
    glBindVertexArray(VAO);
    
    // copy our vertices array in a buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(TangentVertex), &vertices[0], GL_STATIC_DRAW);

    // copy our indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    
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
    // bitangents
    glEnableVertexAttribArray(3);	
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(TangentVertex), (void*)offsetof(TangentVertex, bitangent));

    // clean up and unbind
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // create shader
    shader = Resources::GetShader("tangents.vert", "tangents.frag");
    shader->Use();
    shader->SetUniformBlockBinding("Camera", 0);
    shader->SetFloat("lineLength", this->lineLength);
}

TangentsRenderer::~TangentsRenderer() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    shader = nullptr;
}

void TangentsRenderer::GetLines(Geometry& geometry, std::vector<TangentVertex>& vertices, std::vector<unsigned int>& indices) {
    int size = geometry.vertices.size();
    for(int i = 0; i < size; i++) {
        const Vertex& v = geometry.vertices[i];

        // Offset the positions in the vertex shader instead of precomputing
        // them here so we can have a constant line length, reguardless of
        // model scale

        vertices[i * 6 + 0] = TangentVertex(v);
        vertices[i * 6 + 1] = TangentVertex(v);
        vertices[i * 6 + 2] = TangentVertex(v);
        vertices[i * 6 + 3] = TangentVertex(v);
        vertices[i * 6 + 4] = TangentVertex(v);
        vertices[i * 6 + 5] = TangentVertex(v);

        indices[i * 6 + 0] = i * 6 + 0;
        indices[i * 6 + 1] = i * 6 + 1;
        indices[i * 6 + 2] = i * 6 + 2;
        indices[i * 6 + 3] = i * 6 + 3;
        indices[i * 6 + 4] = i * 6 + 4;
        indices[i * 6 + 5] = i * 6 + 5;
    }

    indexCount = size * 6;
}

void TangentsRenderer::Update(const glm::mat4& modelMatrix, const glm::mat4& normalMatrix) {
    shader->Use();
    shader->SetMat4("model", modelMatrix);
    shader->SetMat4("normalMatrix", normalMatrix);
}

void TangentsRenderer::Draw() {
    shader->Use();

    glBindVertexArray(VAO);
    glDrawElements(GL_LINES, indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
