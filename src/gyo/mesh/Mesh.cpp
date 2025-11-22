
#include <map>

#include <gyo/mesh/Mesh.h>
#include <gyo/mesh/Vertex.h>
#include <gyo/geometry/Geometry.h>
#include <gyo/shading/Material.h>
#include <gyo/utilities/GetError.h>

#include <glad/glad.h>

namespace gyo {

Mesh::Mesh(Geometry* geometry, Material* material, bool computeTangents) {
    this->geometry = geometry;
    this->material = material;

    if(computeTangents) {
        this->geometry->ComputeTangents();
    }

    Initialize();
    ComputeBounds();

    indexCount = this->geometry->indices.size();
    numTris = indexCount / 3;
}

void Mesh::Initialize() {
    if(VAO != 0) {
        return;
    }

    // create the vertex/index buffers and vertex array object
    glGenVertexArrays(1, &VAO);
    glCheckError();
    glGenBuffers(1, &VBO);
    glCheckError();
    glGenBuffers(1, &EBO);
    glCheckError();
    
    // bind Vertex Array Object first
    glBindVertexArray(VAO);
    glCheckError();
    
    // copy our vertices array in a buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glCheckError();
    glBufferData(GL_ARRAY_BUFFER, geometry->vertices.size() * sizeof(Vertex), &geometry->vertices[0], GL_STATIC_DRAW);
    glCheckError();

    // copy our indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glCheckError();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, geometry->indices.size() * sizeof(unsigned int), &geometry->indices[0], GL_STATIC_DRAW);
    glCheckError();
    
    // link the vertex attribute pointers
    // positions
    glEnableVertexAttribArray(0);	
    glCheckError();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glCheckError();
    // normals
    glEnableVertexAttribArray(1);	
    glCheckError();
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glCheckError();
    // texture coords
    glEnableVertexAttribArray(2);	
    glCheckError();
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
    glCheckError();
    // tangents
    glEnableVertexAttribArray(3);	
    glCheckError();
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
    glCheckError();

    // clean up and unbind
    glBindVertexArray(0);
    glCheckError();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glCheckError();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glCheckError();
}

void Mesh::ComputeBounds() {
    // init min and max with extreme values
    glm::vec3 minPoint(std::numeric_limits<float>::max());
    glm::vec3 maxPoint(std::numeric_limits<float>::lowest());

    // iterate through the vertex array
    for (const auto& vertex : geometry->vertices) {
        minPoint = glm::min(minPoint, vertex.position);
        maxPoint = glm::max(maxPoint, vertex.position);
    }

    bounds = { minPoint, maxPoint };
}

Mesh::~Mesh() {
    delete geometry;
    geometry = nullptr;

    delete material;
    material = nullptr;

    glDeleteVertexArrays(1, &VAO);
    glCheckError();
    glDeleteBuffers(1, &VBO);
    glCheckError();
    glDeleteBuffers(1, &EBO);
    glCheckError();
}

void Mesh::SetMaterial(Material* newMaterial) {
    if(this->material) {
        delete this->material;
        this->material = nullptr;
    }

    if(newMaterial) {
        this->material = newMaterial;
    }
}

void Mesh::Draw() {
    if(VAO == 0) {
        return;
    }

    glBindVertexArray(VAO);
    glCheckError();
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    glCheckError();
    glBindVertexArray(0);
    glCheckError();
}

} // namespace gyo
