#ifndef CAMERA_NODE_H
#define CAMERA_NODE_H

#include <camera/Camera.h>
#include <scene/SceneNode.h>
// #include <glm/gtc/matrix_transform.hpp>

class CameraNode : public SceneNode {
public:
    CameraNode(Camera* camera) : SceneNode() { this->camera = camera; };
    ~CameraNode();

    Camera* camera = nullptr;

    glm::mat4 GetView();
    glm::mat4 GetProjection() const {
        return camera->GetProjection();
    };

};

#endif // CAMERA_NODE_H
