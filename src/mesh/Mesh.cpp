
#include <map>

#include <gyo/mesh/Mesh.h>
#include <gyo/internal/mesh/Geometry.h>
#include <gyo/internal/shading/Material.h>

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
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    // bind Vertex Array Object first
    glBindVertexArray(VAO);
    
    // copy our vertices array in a buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, geometry->vertices.size() * sizeof(Vertex), &geometry->vertices[0], GL_STATIC_DRAW);

    // copy our indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, geometry->indices.size() * sizeof(unsigned int), &geometry->indices[0], GL_STATIC_DRAW);
    
    // link the vertex attribute pointers
    // positions
    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // normals
    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    // texture coords
    glEnableVertexAttribArray(2);	
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
    // tangents
    glEnableVertexAttribArray(3);	
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

    // clean up and unbind
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
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
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

} // namespace gyo
