
/**
 * Camera uniform buffer object, found in camera.glsl, has the following signature:
 * layout (std140) uniform Camera {
 *     mat4 projection;
 *     mat4 view;
 *     vec4 viewPos;        // .xyz: camera position in world space, .w = 0
 * }; // total size with std140 layout: 144 bytes
 */

#include <gyo/camera/Camera.h>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <gyo/utilities/GetError.h>

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
    glCheckError();
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glCheckError();

    // the size of our ubo
    buffer.resize(bufferSize);
    
    // allocate enough memory for the 2 matrices and position
    glBufferData(GL_UNIFORM_BUFFER, bufferSize, NULL, GL_STATIC_DRAW);
    glCheckError();

    // link the range of the entire buffer to binding point 0
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, bufferSize);
    glCheckError();

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glCheckError();
}

Camera::~Camera() {
    glDeleteBuffers(1, &uboMatrices);
    glCheckError();
}

void Camera::UpdateViewMatrixUniform(const glm::mat4& view, const glm::vec3& viewPos) {
    // update the camera properties in our uniform buffer

    size_t offset = 0L;

    memcpy(buffer.data() + offset, glm::value_ptr(projection), sizeof(glm::mat4));
    offset += sizeof(glm::mat4);

    memcpy(buffer.data() + offset, glm::value_ptr(view), sizeof(glm::mat4));
    offset += sizeof(glm::mat4);

    memcpy(buffer.data() + offset, glm::value_ptr(glm::vec4(viewPos, 0)), sizeof(glm::vec4));
    offset += sizeof(glm::vec4);

    // copy the data to the gpu

    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glCheckError();

    void* gpuPtr = glMapBufferRange(GL_UNIFORM_BUFFER, 0, buffer.size(), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
    glCheckError();
    if (gpuPtr) {
        memcpy(gpuPtr, buffer.data(), buffer.size());
        glUnmapBuffer(GL_UNIFORM_BUFFER);
        glCheckError();
    }

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glCheckError();
}

} // namespace gyo
