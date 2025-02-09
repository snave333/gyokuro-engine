#ifndef SPOT_LIGHT_H
#define SPOT_LIGHT_H

#include <gyo/internal/lighting/Light.h>

namespace gyo {

struct SpotLight : public Light {
    float cosAngle;

    SpotLight(glm::vec3 color, float angleDeg) : Light(color) {
        cosAngle = glm::cos(glm::radians(angleDeg));
    }
};

} // namespace gyo

#endif // SPOT_LIGHT_H
