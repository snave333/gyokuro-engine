#ifndef AABB_WIREFRAME_H
#define AABB_WIREFRAME_H

#include <glm/glm.hpp>

#include <drawable/IDrawable.h>
#include <math/AABB.h>

class Shader;

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
  
#endif // AABB_WIREFRAME_H
