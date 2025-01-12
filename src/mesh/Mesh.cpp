
#include <map>

#include <mesh/Mesh.h>
#include <mesh/Geometry.h>
#include <shading/Material.h>

#include <glad/glad.h>
#include <glm/gtc/epsilon.hpp>

Mesh::Mesh(Geometry* geometry, Material* material) {
    this->geometry = geometry;
    this->material = material;

    CalculateTangents();
    Initialize();
    ComputeBounds();
}

void Mesh::CalculateTangents() {
    // variables for calculating tangent/bitangent
    unsigned int i0, i1, i2;
    glm::vec3 edge1, edge2;
    glm::vec2 deltaUV1, deltaUV2;
    glm::vec3 tangent, bitangent;

    // accumulate per-vertex the tangent and bitangent of each associated tri

    int size = geometry->indices.size();
    for(int i = 0; i < size; i += 3) {
        i0 = geometry->indices[i];
        i1 = geometry->indices[i+1];
        i2 = geometry->indices[i+2];

        Vertex& v0 = geometry->vertices[i0];
        Vertex& v1 = geometry->vertices[i1];
        Vertex& v2 = geometry->vertices[i2];

        // compute our variables needed for the change-of-basis matrix
        edge1 = v1.position - v0.position;
        edge2 = v2.position - v0.position;
        deltaUV1 = v1.texCoord - v0.texCoord;
        deltaUV2 = v2.texCoord - v0.texCoord;

        // fractional part
        float det = deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x;
        if (glm::epsilonEqual(det, 0.0f, 1e-6f)) {
            continue;
        }
        float invDet = 1.0f / det;

        // now do matrix multiplication to generate the tangent and bitangent
        tangent =   invDet * ( deltaUV2.y * edge1 - deltaUV1.y * edge2);
        bitangent = invDet * (-deltaUV2.x * edge1 + deltaUV1.x * edge2);

        // accumulate our results

        v0.tangent += tangent;
        v0.bitangent += bitangent;
        v1.tangent += tangent;
        v1.bitangent += bitangent;
        v2.tangent += tangent;
        v2.bitangent += bitangent;
    }

    // normalize the results per vert to account for verts that are shared
    // between multiple triangles

    for(auto& vertex : geometry->vertices) {
        vertex.tangent = glm::normalize(vertex.tangent);
        vertex.bitangent = glm::normalize(vertex.bitangent);
    }
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

void Mesh::Draw() {
    if(VAO == 0) {
        return;
    }

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, geometry->indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
