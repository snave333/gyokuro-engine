#ifndef PLANE_H
#define PLANE_H

#include <glm/glm.hpp>

struct Plane {
    glm::vec3 normal;
    float distance;

    void Normalize() {
        float length = glm::length(normal);
        normal = normal / length;
        distance /= length;
    }
};

#endif // PLANE_H
