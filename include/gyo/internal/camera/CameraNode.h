#ifndef CAMERA_NODE_H
#define CAMERA_NODE_H

#include <gyo/internal/camera/Camera.h>
#include <gyo/internal/scene/SceneNode.h>
#include <math/Frustum.h>

namespace gyo {

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

} // namespace gyo

#endif // CAMERA_NODE_H
