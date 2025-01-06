#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include <lighting/Light.h>

struct DirectionalLight : public Light {
    DirectionalLight(glm::vec3 color) : Light(color) {}
};

#endif // DIRECTIONAL_LIGHT_H
