
#include <camera/CameraNode.h>

CameraNode::~CameraNode() {
    delete camera;
    camera = nullptr;
}

glm::mat4 CameraNode::GetView() {
    glm::vec3 forward(0.0f, 0.0f, -1.0f); // TODO get from this->rotation

    return glm::lookAt(
        Position(),
        Position() + forward,
        glm::vec3(0, 1, 0));
}
