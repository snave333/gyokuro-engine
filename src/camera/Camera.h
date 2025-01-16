#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera {
public:
    // static
    static Camera* PerspectiveCamera(float fovYDeg, float aspectRatio, float near = 0.1f, float far = 100);
    static Camera* OrthographicCamera(float height, float aspectRatio, float near = 0.1f, float far = 100);

    // constructor
    Camera(glm::mat4 projection);
    ~Camera();

    const glm::mat4& GetProjection() { return projection; };
    
    void UpdateViewMatrixUniform(const glm::mat4& view, const glm::vec3& viewPos);

protected:
    glm::mat4 projection;

private:
    unsigned int uboMatrices;
};

#endif // CAMERA_H
