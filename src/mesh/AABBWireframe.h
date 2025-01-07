#ifndef AABB_WIREFRAME_H
#define AABB_WIREFRAME_H

#include <glm/glm.hpp>

#include <math/AABB.h>

class Shader;

class AABBWireframe {
public:
    // constructor
    AABBWireframe(const AABB& aabb, glm::vec3 color = glm::vec3(1, 0, 1));
    ~AABBWireframe();

    void Update(const AABB& aabb);
    void Draw();

private:
    Shader* shader = nullptr;

    // render data
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    std::vector<glm::vec3> GetVertices(const glm::vec3& min, const glm::vec3& max);
};
  
#endif // AABB_WIREFRAME_H
