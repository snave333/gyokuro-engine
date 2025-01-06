#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

struct Light {
    glm::vec3 color;
    
    Light(glm::vec3 color) : color(color) {}
};

#endif // LIGHT_H
