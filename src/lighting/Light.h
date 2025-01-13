#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

// https://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation
// (range, constant, linear, quadratic)
glm::vec4 ATTENUATION_LUT[] = {
    glm::vec4(3250, 1.0, 0.0014, 0.000007),
    glm::vec4(600, 1.0, 0.007, 0.0002),
    glm::vec4(325, 1.0, 0.014, 0.0007),
    glm::vec4(200, 1.0, 0.022, 0.0019),
    glm::vec4(160, 1.0, 0.027, 0.0028),
    glm::vec4(100, 1.0, 0.045, 0.0075),
    glm::vec4(65, 1.0, 0.07, 0.017),
    glm::vec4(50, 1.0, 0.09, 0.032),
    glm::vec4(32, 1.0, 0.14, 0.07),
    glm::vec4(20, 1.0, 0.22, 0.20),
    glm::vec4(13, 1.0, 0.35, 0.44),
    glm::vec4(7, 1.0, 0.7, 1.8),
};

struct Light {
    glm::vec3 color;
    
    Light(glm::vec3 color) : color(color) {}
    virtual ~Light() {}

    void GetAttenuation(float range, float& constant, float& linear, float& quadratic) {
        const int lutSize = sizeof(ATTENUATION_LUT) / sizeof(ATTENUATION_LUT[0]);

        constant = 1.0f;

        // handle edge cases: range out of bounds
        if (range >= ATTENUATION_LUT[0].x) {
            linear = ATTENUATION_LUT[0].z;
            quadratic = ATTENUATION_LUT[0].w;
            return;
        }

        if (range <= ATTENUATION_LUT[lutSize - 1].x) {
            linear = ATTENUATION_LUT[lutSize - 1].z;
            quadratic = ATTENUATION_LUT[lutSize - 1].w;
            return;
        }

        // interpolate between the 2 bounding entries in the LUT
        for (int i = 0; i < lutSize - 1; ++i) {
            const glm::vec4& current = ATTENUATION_LUT[i];
            const glm::vec4& next = ATTENUATION_LUT[i + 1];

            if (range <= current.x && range >= next.x) {
                // interpolation factor
                float t = (range - next.x) / (current.x - next.x);

                linear = glm::mix(next.z, current.z, t);
                quadratic = glm::mix(next.w, current.w, t);
                return;
            }
        }
    }
};

#endif // LIGHT_H
