#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include <lighting/Light.h>

struct PointLight : public Light {
    // attenuation
    float constant;
    float linear;
    float quadratic;

    /**
     * See here for more attenuation values:
     * https://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation
     */
    PointLight(glm::vec3 color, float constant = 1.0f, float linear = 0.7f, float quadratic = 1.8f) : Light(color),
        constant(constant),
        linear(linear),
        quadratic(quadratic) {}
};

#endif // POINT_LIGHT_H
