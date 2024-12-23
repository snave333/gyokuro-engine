#ifndef ORTHOGRAPHIC_CAMERA_H
#define ORTHOGRAPHIC_CAMERA_H

#include <camera/Camera.h>

#include <glm/gtc/matrix_transform.hpp>

class OrthographicCamera : public Camera {
public:
    OrthographicCamera(float height, float aspectRatio, float near = 0.1f, float far = 100)
        : Camera(glm::ortho(
            -(height * aspectRatio) / 2.0f, // left
            (height * aspectRatio) / 2.0f,  // right
            -height / 2.0f,                 // bottom
            height / 2.0f,                  // top
            near, far)) {}
};

#endif // ORTHOGRAPHIC_CAMERA_H
