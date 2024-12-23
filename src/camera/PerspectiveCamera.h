#ifndef PERSPECTIVE_CAMERA_H
#define PERSPECTIVE_CAMERA_H

#include <camera/Camera.h>

#include <glm/gtc/matrix_transform.hpp>

class PerspectiveCamera : public Camera {
public:
    PerspectiveCamera(float fovYDeg, float aspectRatio, float near = 0.1f, float far = 100)
        : Camera(glm::perspective(glm::radians(fovYDeg), aspectRatio, near, far)) {}
};

#endif // PERSPECTIVE_CAMERA_H
