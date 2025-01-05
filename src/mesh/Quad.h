#ifndef QUAD_H
#define QUAD_H

#include <mesh/Geometry.h>

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

#endif // QUAD_H
