#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include <gyo/internal/lighting/Light.h>

namespace gyo {

struct DirectionalLight : public Light {
    DirectionalLight(glm::vec3 color) : Light(color) {}
};

} // namespace gyo

#endif // DIRECTIONAL_LIGHT_H
