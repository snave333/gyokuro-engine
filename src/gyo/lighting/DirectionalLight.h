#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include <gyo/lighting/Light.h>

namespace gyo {

struct DirectionalLight : public Light {
    DirectionalLight(glm::vec3 color = glm::vec3(1)) : Light(color) {}
};

} // namespace gyo

#endif // DIRECTIONAL_LIGHT_H
