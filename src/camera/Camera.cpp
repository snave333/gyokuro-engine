
/**
 * Camera uniform buffer object, found in camera.glsl, has the following signature:
 * layout (std140) uniform Camera {
 *     mat4 projection;
 *     mat4 view;
 *     vec4 viewPos;        // .xyz: camera position in world space, .w = 0
 * }; // total size with std140 layout: 144 bytes
 */

#include <camera/Camera.h>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace gyo {

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

    // the size of our ubo
    unsigned long size = 144;
    
    // allocate enough memory for the 2 matrices and position
    glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STATIC_DRAW);

    // link the range of the entire buffer to binding point 0
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, size);

    // store the first part of the uniform buffer with the projection matrix
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

Camera::~Camera() {
    glDeleteBuffers(1, &uboMatrices);
}

void Camera::UpdateViewMatrixUniform(const glm::mat4& view, const glm::vec3& viewPos) {
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);

    // update the view matrix and view position in our uniform buffer
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::vec4), glm::value_ptr(glm::vec4(viewPos, 0)));

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

} // namespace gyo
