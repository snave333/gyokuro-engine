
#include <camera/Camera.h>

Camera* Camera::PerspectiveCamera(float fovYDeg, float aspectRatio, float near, float far) {
    return new Camera(glm::perspective(glm::radians(fovYDeg), aspectRatio, near, far));
};
Camera* Camera::OrthographicCamera(float height, float aspectRatio, float near, float far) {
    return new Camera(glm::ortho(
        -(height * aspectRatio) / 2.0f, // left
        (height * aspectRatio) / 2.0f,  // right
        -height / 2.0f,                 // bottom
        height / 2.0f,                  // top
        near, far));
};
