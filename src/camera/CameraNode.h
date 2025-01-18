#ifndef CAMERA_NODE_H
#define CAMERA_NODE_H

#include <camera/Camera.h>
#include <scene/SceneNode.h>
#include <math/Frustum.h>

struct Frustum;

class CameraNode : public SceneNode {
public:
    CameraNode(Camera* camera) : SceneNode() { this->camera = camera; };
    ~CameraNode();

    void UpdateViewMatrixUniform() { camera->UpdateViewMatrixUniform(GetView(), GetPosition()); };
    
    const glm::mat4& GetProjection() { return camera->GetProjection(); };
    glm::mat4 GetView();
    
    Frustum GetFrustum();

protected:
    Camera* camera = nullptr;
};

#endif // CAMERA_NODE_H
