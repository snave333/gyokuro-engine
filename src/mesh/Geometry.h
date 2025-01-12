#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <glm/glm.hpp>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
    glm::vec3 tangent = glm::vec3(0.0f);
    glm::vec3 bitangent = glm::vec3(0.0f);

    // we calculate the tangent later
    Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texCoord) :
        position(position), normal(normal), texCoord(texCoord) {}
};

struct Geometry {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
};

#endif // GEOMETRY_H