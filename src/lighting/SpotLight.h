#ifndef SPOT_LIGHT_H
#define SPOT_LIGHT_H

#include <lighting/Light.h>

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

#endif // SPOT_LIGHT_H
