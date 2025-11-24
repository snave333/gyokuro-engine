#ifndef QUAD_H
#define QUAD_H

#include <gyo/geometry/Geometry.h>

namespace gyo {

struct Quad : public Geometry {
    Quad(float halfSize = 1.0f) {
        positions = {
            { -halfSize, -halfSize, 0.0f },
            {  halfSize, -halfSize, 0.0f },
            {  halfSize,  halfSize, 0.0f },
            { -halfSize,  halfSize, 0.0f }
        };

        normals = {
            { 0.0f, 0.0f, 1.0f },
            { 0.0f, 0.0f, 1.0f },
            { 0.0f, 0.0f, 1.0f },
            { 0.0f, 0.0f, 1.0f }
        };

        texCoords = {
            { 0.0f, 0.0f },
            { 1.0f, 0.0f },
            { 1.0f, 1.0f },
            { 0.0f, 1.0f }
        };

        indices = {
            0, 1, 2, // first triangle
            0, 2, 3  // second triangle
        };
    }
};

} // namespace gyo

#endif // QUAD_H
