#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

namespace gyo {

struct Light {
    glm::vec3 color;
    
    Light(glm::vec3 color) : color(color) {}
    virtual ~Light() {}
};

} // namespace gyo

#endif // LIGHT_H
