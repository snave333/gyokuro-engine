#ifndef PYRAMID_H
#define PYRAMID_H

#include <gyo/geometry/Geometry.h>

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
        glm::vec3 normal0 = glm::normalize(-glm::cross(base2 - top0, base1 - top0)); // front face
        glm::vec3 normal1 = glm::normalize(-glm::cross(base3 - top0, base2 - top0)); // right face
        glm::vec3 normal2 = glm::normalize(-glm::cross(base4 - top0, base3 - top0)); // back face
        glm::vec3 normal3 = glm::normalize(-glm::cross(base1 - top0, base4 - top0)); // left face
        glm::vec3 baseNormal(0.0f, -1.0f, 0.0f);                                     // base face

        // uv coordinates
        glm::vec2 uvTop(0.5f, 1.0f);
        glm::vec2 uvBase0(0.0f, 0.0f);
        glm::vec2 uvBase1(1.0f, 0.0f);
        glm::vec2 uvBase2(1.0f, 1.0f);
        glm::vec2 uvBase3(0.0f, 1.0f);

        positions = {
            top0, base1, base2,
            top0, base2, base3,
            top0, base3, base4,
            top0, base4, base1,
            // base quad
            base1, base2, base3, base4
        };

        normals = {
            normal0, normal0, normal0,
            normal1, normal1, normal1,
            normal2, normal2, normal2,
            normal3, normal3, normal3,
            // base quad
            baseNormal, baseNormal, baseNormal, baseNormal
        };

        texCoords = {
            uvTop, uvBase0, uvBase1,
            uvTop, uvBase0, uvBase1,
            uvTop, uvBase0, uvBase1,
            uvTop, uvBase0, uvBase1,
            // base quad
            uvBase0, uvBase1, uvBase2, uvBase3
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
