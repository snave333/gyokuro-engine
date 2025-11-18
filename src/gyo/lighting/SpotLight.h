#ifndef SPOT_LIGHT_H
#define SPOT_LIGHT_H

#include <gyo/lighting/Light.h>

namespace gyo {

struct SpotLight : public Light {
    float cosAngle;

    SpotLight(glm::vec3 color = glm::vec3(1), float angleDeg = 45) : Light(color) {
        cosAngle = glm::cos(glm::radians(angleDeg));
    }
};

} // namespace gyo

#endif // SPOT_LIGHT_H
