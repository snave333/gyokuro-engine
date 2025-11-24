#ifndef VERTEX_H
#define VERTEX_H

#include <glm/glm.hpp>

namespace gyo {

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
    glm::vec3 tangent = glm::vec3(0.0f);

    // we calculate the tangent later
    Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texCoord) :
        position(position), normal(normal), texCoord(texCoord) {}

    Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texCoord, glm::vec3 tangent) :
        position(position), normal(normal), texCoord(texCoord), tangent(tangent) {}
};

} // namespace gyo

#endif // VERTEX_H
