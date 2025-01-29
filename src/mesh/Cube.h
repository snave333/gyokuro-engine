#ifndef CUBE_H
#define CUBE_H

#include <mesh/Geometry.h>

namespace gyo {

struct Cube : public Geometry {
    Cube(float halfSize = 0.5f) {
        glm::vec3 positions[] = {
            // +x
            { halfSize, -halfSize,  halfSize }, // 3
            { halfSize, -halfSize, -halfSize }, // 7
            { halfSize,  halfSize, -halfSize }, // 6
            { halfSize,  halfSize,  halfSize }, // 2
            // -x
            {-halfSize, -halfSize, -halfSize }, // 4
            {-halfSize, -halfSize,  halfSize }, // 0
            {-halfSize,  halfSize,  halfSize }, // 1
            {-halfSize,  halfSize, -halfSize }, // 5
            // +y
            {-halfSize,  halfSize,  halfSize }, // 1
            { halfSize,  halfSize,  halfSize }, // 2
            { halfSize,  halfSize, -halfSize }, // 6
            {-halfSize,  halfSize, -halfSize }, // 5
            // -y
            {-halfSize, -halfSize, -halfSize }, // 4
            { halfSize, -halfSize, -halfSize }, // 7
            { halfSize, -halfSize,  halfSize }, // 3
            {-halfSize, -halfSize,  halfSize }, // 0
            // +z
            {-halfSize, -halfSize,  halfSize }, // 0
            { halfSize, -halfSize,  halfSize }, // 3
            { halfSize,  halfSize,  halfSize }, // 2
            {-halfSize,  halfSize,  halfSize }, // 1
            // -z
            { halfSize, -halfSize, -halfSize }, // 7
            {-halfSize, -halfSize, -halfSize }, // 4
            {-halfSize,  halfSize, -halfSize }, // 5
            { halfSize,  halfSize, -halfSize }  // 6
        };

        glm::vec3 normals[] = {
            // +x
            { 1.0f,  0.0f,  0.0f },
            { 1.0f,  0.0f,  0.0f },
            { 1.0f,  0.0f,  0.0f },
            { 1.0f,  0.0f,  0.0f },
            // -x
            {-1.0f,  0.0f,  0.0f },
            {-1.0f,  0.0f,  0.0f },
            {-1.0f,  0.0f,  0.0f },
            {-1.0f,  0.0f,  0.0f },
            // +y
            { 0.0f,  1.0f,  0.0f },
            { 0.0f,  1.0f,  0.0f },
            { 0.0f,  1.0f,  0.0f },
            { 0.0f,  1.0f,  0.0f },
            // -y
            { 0.0f, -1.0f,  0.0f },
            { 0.0f, -1.0f,  0.0f },
            { 0.0f, -1.0f,  0.0f },
            { 0.0f, -1.0f,  0.0f },
            // +z
            { 0.0f,  0.0f,  1.0f },
            { 0.0f,  0.0f,  1.0f },
            { 0.0f,  0.0f,  1.0f },
            { 0.0f,  0.0f,  1.0f },
            // -z
            { 0.0f,  0.0f, -1.0f },
            { 0.0f,  0.0f, -1.0f },
            { 0.0f,  0.0f, -1.0f },
            { 0.0f,  0.0f, -1.0f }
        };

        glm::vec2 texCoords[] = {
            // +x
            { 0.0f, 0.0f },
            { 1.0f, 0.0f },
            { 1.0f, 1.0f },
            { 0.0f, 1.0f },
            // -x
            { 0.0f, 0.0f },
            { 1.0f, 0.0f },
            { 1.0f, 1.0f },
            { 0.0f, 1.0f },
            // +y
            { 0.0f, 0.0f },
            { 1.0f, 0.0f },
            { 1.0f, 1.0f },
            { 0.0f, 1.0f },
            // -y
            { 0.0f, 0.0f },
            { 1.0f, 0.0f },
            { 1.0f, 1.0f },
            { 0.0f, 1.0f },
            // +z
            { 0.0f, 0.0f },
            { 1.0f, 0.0f },
            { 1.0f, 1.0f },
            { 0.0f, 1.0f },
            // -z
            { 0.0f, 0.0f },
            { 1.0f, 0.0f },
            { 1.0f, 1.0f },
            { 0.0f, 1.0f }
        };

        vertices = {};
        for(int i = 0; i < 24; i++) {
            vertices.push_back({ positions[i], normals[i], texCoords[i] });
        }

        indices = {
            0,  1,  2,  0,  2,  3,
            4,  5,  6,  4,  6,  7,
            8,  9, 10,  8, 10, 11,
            12, 13, 14, 12, 14, 15,
            16, 17, 18, 16, 18, 19,
            20, 21, 22, 20, 22, 23
        };
    }
};

} // namespace gyo

#endif // CUBE_H
