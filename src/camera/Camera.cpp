
#include <camera/Camera.h>

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::mat4 projection) {
    this->projection = projection;
}

glm::mat4 Camera::GetView() {
    glm::vec3 position(0.0f, 0.0f, 3.0f);
    glm::vec3 forward(0.0f, 0.0f, -1.0f);
    glm::vec3 up(0.0f, 1.0f, 0.0f);

    return glm::lookAt(position, position + forward, up);
}
