#ifndef SPHERE_H
#define SPHERE_H

#include <mesh/Geometry.h>
#include <glm/gtc/constants.hpp>

namespace gyo {

struct Sphere : public Geometry {
    Sphere(float radius = 0.5f, int stacks = 20, int slices = 20) {
        // generate vertices
        for (int stack = 0; stack <= stacks; ++stack) {
            float phi = glm::pi<float>() * stack / stacks; // from 0 to π (latitude)
            float y = cos(phi);                            // y coordinate
            float r = sin(phi);                            // radius of circle at this latitude

            for (int slice = 0; slice <= slices; ++slice) {
                float theta = 2 * glm::pi<float>() * slice / slices; // from 0 to 2π (longitude)
                float x = r * cos(theta);
                float z = -r * sin(theta); // negate z to account for OpenGL's right-handed system

                glm::vec3 position = glm::vec3(x, y, z) * radius;
                glm::vec3 normal = glm::normalize(position); // sphere normals point outward
                glm::vec2 uv = glm::vec2(
                    static_cast<float>(slice) / slices,       // u coordinate
                    -static_cast<float>(stack) / stacks);     // v coordinate

                vertices.push_back({ position, normal, uv });
            }
        }

        // generate indices with CCW winding
        for (int stack = 0; stack < stacks; ++stack) {
            for (int slice = 0; slice < slices; ++slice) {
                int first = (stack * (slices + 1)) + slice;
                int second = first + slices + 1;

                // add two triangles for each quad with CCW order
                indices.push_back(first);
                indices.push_back(second);
                indices.push_back(second + 1);

                indices.push_back(first);
                indices.push_back(second + 1);
                indices.push_back(first + 1);
            }
        }
    }
};

} // namespace gyo

#endif // SPHERE_H
