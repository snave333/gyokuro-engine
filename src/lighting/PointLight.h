#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include <lighting/Light.h>

struct PointLight : public Light {
    // attenuation
    float constant;
    float linear;
    float quadratic;

    PointLight(glm::vec3 color, float constant, float linear, float quadratic) : Light(color),
        constant(constant),
        linear(linear),
        quadratic(quadratic) {}
};

#endif // POINT_LIGHT_H
