#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <glm/glm.hpp>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
};

struct Geometry {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
};

#endif // GEOMETRY_H