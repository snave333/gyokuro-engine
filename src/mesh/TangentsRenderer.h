#ifndef TANGENTS_RENDERER_H
#define TANGENTS_RENDERER_H

#include <glm/glm.hpp>
#include <mesh/Geometry.h>

class Shader;
struct Geometry;

struct TangentVertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec3 bitangent;

    TangentVertex() {}

    TangentVertex(const Vertex& other) {
        this->position = glm::vec3(other.position);
        this->normal = glm::vec3(other.normal);
        this->tangent = glm::vec3(other.tangent);
        this->bitangent = glm::vec3(other.bitangent);
    }
};

class TangentsRenderer {
public:
    // constructor
    TangentsRenderer(Geometry& geometry, float lineLength = 0.1f);
    ~TangentsRenderer();

    void Update(const glm::mat4& modelMatrix, const glm::mat4& normalMatrix);
    void Draw();

private:
    float lineLength;

    Shader* shader = nullptr;

    // render data
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    unsigned int indexCount;

    void GetLines(Geometry& geometry, std::vector<TangentVertex>& vertices, std::vector<unsigned int>& indices);
};
  
#endif // TANGENTS_RENDERER_H
