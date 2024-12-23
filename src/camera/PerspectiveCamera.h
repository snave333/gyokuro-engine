#ifndef PERSPECTIVE_CAMERA_H
#define PERSPECTIVE_CAMERA_H

#include <camera/Camera.h>

#include <glm/gtc/matrix_transform.hpp>

class PerspectiveCamera : public Camera {
public:
    PerspectiveCamera(float fovDeg, float aspectRatio, float near, float far)
        : Camera(glm::perspective(glm::radians(fovDeg), aspectRatio, near, far)) {}
};

#endif // PERSPECTIVE_CAMERA_H
