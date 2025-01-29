#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

namespace gyo {

struct Light {
    glm::vec3 color;
    
    Light(glm::vec3 color) : color(color) {}
    virtual ~Light() {}

    // https://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation
    // (range, constant, linear, quadratic)
    static constexpr float ATTENUATION_LUT[12][4] = {
        {3250, 1.0, 0.0014, 0.000007},
        {600, 1.0, 0.007, 0.0002},
        {325, 1.0, 0.014, 0.0007},
        {200, 1.0, 0.022, 0.0019},
        {160, 1.0, 0.027, 0.0028},
        {100, 1.0, 0.045, 0.0075},
        {65, 1.0, 0.07, 0.017},
        {50, 1.0, 0.09, 0.032},
        {32, 1.0, 0.14, 0.07},
        {20, 1.0, 0.22, 0.20},
        {13, 1.0, 0.35, 0.44},
        {7, 1.0, 0.7, 1.8},
    };

    void GetAttenuation(float range, float& constant, float& linear, float& quadratic) {
        const int lutSize = sizeof(ATTENUATION_LUT) / sizeof(ATTENUATION_LUT[0]);

        constant = 1.0f;

        // handle edge cases: range out of bounds
        if (range >= ATTENUATION_LUT[0][0]) {
            linear = ATTENUATION_LUT[0][2];
            quadratic = ATTENUATION_LUT[0][3];
            return;
        }

        if (range <= ATTENUATION_LUT[lutSize - 1][0]) {
            linear = ATTENUATION_LUT[lutSize - 1][2];
            quadratic = ATTENUATION_LUT[lutSize - 1][3];
            return;
        }

        // interpolate between the 2 bounding entries in the LUT
        for (int i = 0; i < lutSize - 1; ++i) {
            if (range <= ATTENUATION_LUT[i][0] && range >= ATTENUATION_LUT[i + 1][0]) {
                // interpolation factor
                float t = (range - ATTENUATION_LUT[i + 1][0]) / 
                        (ATTENUATION_LUT[i][0] - ATTENUATION_LUT[i + 1][0]);

                constant = (1 - t) * ATTENUATION_LUT[i + 1][1] + t * ATTENUATION_LUT[i][1];
                linear = (1 - t) * ATTENUATION_LUT[i + 1][2] + t * ATTENUATION_LUT[i][2];
                quadratic = (1 - t) * ATTENUATION_LUT[i + 1][3] + t * ATTENUATION_LUT[i][3];
                return;
            }
        }
    }
};

} // namespace gyo

#endif // LIGHT_H
