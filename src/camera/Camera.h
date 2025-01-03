#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    // static
    static Camera* PerspectiveCamera(float fovYDeg, float aspectRatio, float near = 0.1f, float far = 100);
    static Camera* OrthographicCamera(float height, float aspectRatio, float near = 0.1f, float far = 100);

    // constructor
    Camera(glm::mat4 projection) { this->projection = projection; };

    glm::mat4 GetProjection() const {
        return projection;
    };

protected:
    glm::mat4 view;
    glm::mat4 projection;
};

#endif // CAMERA_H
