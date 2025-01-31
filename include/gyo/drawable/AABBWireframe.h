#ifndef AABB_WIREFRAME_H
#define AABB_WIREFRAME_H

#include <vector>
#include <glm/glm.hpp>

#include <gyo/internal/drawable/IDrawable.h>

namespace gyo {

class Shader;
struct AABB;

class AABBWireframe : public IDrawable {
public:
    // constructor
    AABBWireframe(const AABB& aabb, glm::vec4 color = glm::vec4(1, 0, 1, 1));
    ~AABBWireframe() override;

    void Update(const AABB& aabb);
    void Draw() override;

private:
    glm::vec4 color;
    Shader* shader = nullptr;

    // render data
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    std::vector<glm::vec3> GetVertices(const glm::vec3& min, const glm::vec3& max);
};
  
} // namespace gyo

#endif // AABB_WIREFRAME_H
