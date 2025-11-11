#ifndef QUAD_H
#define QUAD_H

#include <gyo/mesh/Geometry.h>

namespace gyo {

struct Quad : public Geometry {
    Quad() {
        vertices = {
            { { -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } }, // bottom left
            { {  0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } }, // bottom right
            { {  0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } }, // top right
            { { -0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } }  // top left
        };
        indices = {
            0, 1, 2, // first triangle
            0, 2, 3  // second triangle
        };
    }
};

} // namespace gyo

#endif // QUAD_H
