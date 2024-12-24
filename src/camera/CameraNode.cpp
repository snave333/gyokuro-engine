
#include <camera/CameraNode.h>

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
