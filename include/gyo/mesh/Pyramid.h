#ifndef PYRAMID_H
#define PYRAMID_H

#include <gyo/internal/mesh/Geometry.h>

namespace gyo {

struct Pyramid : public Geometry {
    Pyramid(float halfBase = 0.5f, float height = 1.0f) {
        // define the vertices of the pyramid
        glm::vec3 top0(0.0f, height, 0.0f);             // 0
        glm::vec3 base1(-halfBase, 0.0f, halfBase);     // 1
        glm::vec3 base2(halfBase, 0.0f, halfBase);      // 2
        glm::vec3 base3(halfBase, 0.0f, -halfBase);     // 3
        glm::vec3 base4(-halfBase, 0.0f, -halfBase);    // 4

        // calculate normals for each face
        glm::vec3 normal0 = glm::normalize(glm::cross(base2 - top0, base1 - top0)); // front face
        glm::vec3 normal1 = glm::normalize(glm::cross(base3 - top0, base2 - top0)); // right face
        glm::vec3 normal2 = glm::normalize(glm::cross(base4 - top0, base3 - top0)); // back face
        glm::vec3 normal3 = glm::normalize(glm::cross(base1 - top0, base4 - top0)); // left face
        glm::vec3 baseNormal(0.0f, -1.0f, 0.0f);                                    // base face

        // uv coordinates
        glm::vec2 uvTop(0.5f, 1.0f);
        glm::vec2 uvBase0(0.0f, 0.0f);
        glm::vec2 uvBase1(1.0f, 0.0f);
        glm::vec2 uvBase2(1.0f, 1.0f);
        glm::vec2 uvBase3(0.0f, 1.0f);

        // vertices
        vertices = {
            // top point shared across all triangular faces
            { top0, normal0, uvTop },
            { base1, normal0, uvBase0 },
            { base2, normal0, uvBase1 },

            { top0, normal1, uvTop },
            { base2, normal1, uvBase0 },
            { base3, normal1, uvBase1 },

            { top0, normal2, uvTop },
            { base3, normal2, uvBase0 },
            { base4, normal2, uvBase1 },

            { top0, normal3, uvTop },
            { base4, normal3, uvBase0 },
            { base1, normal3, uvBase1 },

            // base quad
            { base1, baseNormal, uvBase0 },
            { base2, baseNormal, uvBase1 },
            { base3, baseNormal, uvBase2 },
            { base4, baseNormal, uvBase3 },
        };

        // indices
        indices = {
            0, 1, 2,    // front face
            3, 4, 5,    // right face
            6, 7, 8,    // back face
            9, 10, 11,  // left face
            12, 14, 13, // base tri 1
            12, 15, 14, // base tri 2
        };
    }
};

} // namespace gyo

#endif // PYRAMID_H
