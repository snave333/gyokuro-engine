#ifndef SPOT_LIGHT_H
#define SPOT_LIGHT_H

#include <lighting/Light.h>

struct SpotLight : public Light {
    float cosAngle;
    // attenuation
    float constant;
    float linear;
    float quadratic;

    SpotLight(glm::vec3 color, float angleDeg, float constant = 1.0f, float linear = 0.7f, float quadratic = 1.8f) : Light(color),
        cosAngle(glm::cos(glm::radians(angleDeg))),
        constant(constant),
        linear(linear),
        quadratic(quadratic) {}
};

#endif // SPOT_LIGHT_H
