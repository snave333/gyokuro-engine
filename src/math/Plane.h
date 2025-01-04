#ifndef PLANE_H
#define PLANE_H

#include <glm/glm.hpp>

struct Plane {
    glm::vec3 normal;
    float distance;
};

#endif // PLANE_H
