#ifndef TORUS_H
#define TORUS_H

#include <gyo/geometry/Geometry.h>
#include <glm/gtc/constants.hpp>

namespace gyo {

struct Torus : public Geometry {
    Torus(float majorRadius = 0.5f,
          float minorRadius = 0.2f,
          int majorSegments = 20,
          int minorSegments = 20)
    {
        // generate vertices
        for (int i = 0; i <= majorSegments; ++i) {
            float majorAngle = 2.0f * glm::pi<float>() * i / majorSegments; // angle around the major circle
            float cosMajor = cos(majorAngle);
            float sinMajor = sin(majorAngle);

            for (int j = 0; j <= minorSegments; ++j) {
                float minorAngle = 2.0f * glm::pi<float>() * j / minorSegments; // angle around the minor circle
                float cosMinor = cos(minorAngle);
                float sinMinor = sin(minorAngle);

                // calculate vertex position
                glm::vec3 center(majorRadius * cosMajor, majorRadius * sinMajor, 0.0f); // center of the minor circle
                glm::vec3 position = center + glm::vec3(cosMajor * minorRadius * cosMinor,
                                                        sinMajor * minorRadius * cosMinor,
                                                        minorRadius * sinMinor);

                // calculate normal
                glm::vec3 normal = glm::normalize(position - center);

                // uv coordinates
                glm::vec2 uv(static_cast<float>(i) / majorSegments, static_cast<float>(j) / minorSegments);

                // add vertex
                vertices.push_back({ position, normal, uv });
            }
        }

        // generate indices with CCW winding
        for (int i = 0; i < majorSegments; ++i) {
            for (int j = 0; j < minorSegments; ++j) {
                int current = i * (minorSegments + 1) + j;
                int next = (i + 1) * (minorSegments + 1) + j;

                // two triangles per quad, in CCW order
                indices.push_back(current);
                indices.push_back(next);
                indices.push_back(current + 1);

                indices.push_back(next);
                indices.push_back(next + 1);
                indices.push_back(current + 1);
            }
        }
    }
};

} // namespace gyo

#endif // TORUS_H
