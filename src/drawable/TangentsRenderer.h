#ifndef TANGENTS_RENDERER_H
#define TANGENTS_RENDERER_H

#include <glm/glm.hpp>
#include <drawable/IDrawable.h>
#include <mesh/Geometry.h>

namespace gyo {

class Shader;
struct Geometry;

struct TangentVertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 tangent;

    TangentVertex() {}

    TangentVertex(const Vertex& other) {
        this->position = glm::vec3(other.position);
        this->normal = glm::vec3(other.normal);
        this->tangent = glm::vec3(other.tangent);
    }
};

class TangentsRenderer : public IDrawable {
public:
    // constructor
    TangentsRenderer(Geometry& geometry, float lineLength = 0.1f);
    ~TangentsRenderer() override;

    void Update(const glm::mat4& modelMatrix, const glm::mat4& normalMatrix);
    void Draw() override;

private:
    float lineLength;

    Shader* shader = nullptr;

    // render data
    unsigned int VAO;
    unsigned int VBO;

    unsigned int vertexCount;
};

} // namespace gyo

#endif // TANGENTS_RENDERER_H
