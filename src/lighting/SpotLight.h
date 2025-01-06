#ifndef SPOT_LIGHT_H
#define SPOT_LIGHT_H

#include <lighting/Light.h>

struct SpotLight : public Light {
    float spotAngle;
    float spotOuterAngle;
    // attenuation
    float constant;
    float linear;
    float quadratic;

    SpotLight(glm::vec3 color, float innerAngle, float outerAngle, float constant, float linear, float quadratic) : Light(color),
        spotAngle(innerAngle),
        spotOuterAngle(outerAngle),
        constant(constant),
        linear(linear),
        quadratic(quadratic) {}
};

#endif // SPOT_LIGHT_H
