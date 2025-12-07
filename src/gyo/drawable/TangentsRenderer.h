#ifndef TANGENTS_RENDERER_H
#define TANGENTS_RENDERER_H

#include <glm/glm.hpp>
#include <gyo/drawable/IDrawable.h>
#include <gyo/geometry/Geometry.h>
#include <gyo/mesh/Vertex.h>

namespace gyo {

class Shader;
struct Geometry;

// TODO move this to a gyo/mesh/TangentVertex class?
struct TangentVertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 tangent;

    TangentVertex() {}
    
    TangentVertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec3& tangent)
        : position(position), normal(normal), tangent(tangent) {}
};

class TangentsRenderer : public IDrawable {
public:
    // constructor
    TangentsRenderer(Geometry& geometry, float lineLength = 0.1f);
    ~TangentsRenderer() override;

    void Update(const glm::mat4& modelMatrix, const glm::mat4& normalMatrix);
    void Draw() const override;

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
