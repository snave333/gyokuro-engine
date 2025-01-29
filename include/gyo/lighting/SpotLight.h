#ifndef SPOT_LIGHT_H
#define SPOT_LIGHT_H

#include <gyo/internal/lighting/Light.h>

namespace gyo {

struct SpotLight : public Light {
    float cosAngle;
    // attenuation
    float constant;
    float linear;
    float quadratic;

    SpotLight(glm::vec3 color, float angleDeg, float range) : Light(color) {
        cosAngle = glm::cos(glm::radians(angleDeg));
        GetAttenuation(range, constant, linear, quadratic);
    }
};

} // namespace gyo

#endif // SPOT_LIGHT_H
