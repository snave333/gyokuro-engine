
#include <camera/Camera.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera* Camera::PerspectiveCamera(float fovYDeg, float aspectRatio, float near, float far) {
    return new Camera(glm::perspective(glm::radians(fovYDeg), aspectRatio, near, far));
}

Camera* Camera::OrthographicCamera(float height, float aspectRatio, float near, float far) {
    return new Camera(glm::ortho(
        -(height * aspectRatio) / 2.0f, // left
        (height * aspectRatio) / 2.0f,  // right
        -height / 2.0f,                 // bottom
        height / 2.0f,                  // top
        near, far));
}

Camera::Camera(glm::mat4 projection) {
    this->projection = projection;

    // create our matrices uniform buffer object, and bind for initialization
    glGenBuffers(1, &uboMatrices);
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    
    // allocate enough memory for the 2 matrices    
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);

    // link the range of the entire buffer to binding point 0
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4));

    // store the first half of the uniform buffer with the projection matrix
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

Camera::~Camera() {
    glDeleteBuffers(1, &uboMatrices);
}

void Camera::UpdateViewMatrixUniform(const glm::mat4& view) {
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
