
#include <camera/CameraNode.h>

namespace gyo {

CameraNode::~CameraNode() {
    delete camera;
    camera = nullptr;
}

glm::mat4 CameraNode::GetView() {
    glm::vec3 forward = GetForward();

    return glm::lookAt(
        GetPosition(),
        GetPosition() + forward,
        glm::vec3(0, 1, 0));
}

/**
 * Adapted from https://www8.cs.umu.se/kurser/5DV051/HT12/lab/plane_extraction.pdf
 */
Frustum CameraNode::GetFrustum() {
    Frustum frustum;

    // transform the projection matrix by the view matrix
    const glm::mat4& projectionMatrix = GetProjection();
    glm::mat4 viewMatrix = GetView();
    glm::mat4 clipMatrix = projectionMatrix * viewMatrix;

    // extract planes from the clip matrix using Gribb/Hartmann method

    //left
    frustum.planes[0] = {
        { clipMatrix[0][3] + clipMatrix[0][0], 
          clipMatrix[1][3] + clipMatrix[1][0], 
          clipMatrix[2][3] + clipMatrix[2][0] },
          clipMatrix[3][3] + clipMatrix[3][0]
    };

    // right
    frustum.planes[1] = {
        { clipMatrix[0][3] - clipMatrix[0][0], 
          clipMatrix[1][3] - clipMatrix[1][0], 
          clipMatrix[2][3] - clipMatrix[2][0] },
          clipMatrix[3][3] - clipMatrix[3][0]
    };

    // bottom
    frustum.planes[2] = {
        { clipMatrix[0][3] + clipMatrix[0][1], 
          clipMatrix[1][3] + clipMatrix[1][1], 
          clipMatrix[2][3] + clipMatrix[2][1] },
          clipMatrix[3][3] + clipMatrix[3][1]
    };

    // top
    frustum.planes[3] = {
        { clipMatrix[0][3] - clipMatrix[0][1], 
          clipMatrix[1][3] - clipMatrix[1][1], 
          clipMatrix[2][3] - clipMatrix[2][1] },
          clipMatrix[3][3] - clipMatrix[3][1]
    };

    // near
    frustum.planes[4] = {
        { clipMatrix[0][3] + clipMatrix[0][2], 
          clipMatrix[1][3] + clipMatrix[1][2], 
          clipMatrix[2][3] + clipMatrix[2][2] },
          clipMatrix[3][3] + clipMatrix[3][2]
    };

    // far
    frustum.planes[5] = {
        { clipMatrix[0][3] - clipMatrix[0][2], 
          clipMatrix[1][3] - clipMatrix[1][2], 
          clipMatrix[2][3] - clipMatrix[2][2] },
          clipMatrix[3][3] - clipMatrix[3][2]
    };

    return frustum;
}

} // namespace gyo
