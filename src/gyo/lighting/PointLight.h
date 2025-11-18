#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include <gyo/lighting/Light.h>

namespace gyo {

struct PointLight : public Light {
    PointLight(glm::vec3 color = glm::vec3(1)) : Light(color) {}
};

} // namespace gyo

#endif // POINT_LIGHT_H
