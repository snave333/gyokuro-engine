#ifndef FLY_CAMERA_H
#define FLY_CAMERA_H

#include <gyo/camera/CameraNode.h>

namespace gyo {

class FlyCamera : public CameraNode {
public:
    FlyCamera(Camera* camera) : CameraNode(camera) { };
    ~FlyCamera();

    void OnLook(float xOffset, float yOffset);

private:
    float yaw;
    float pitch;
    const float mouseSensitivity = 0.1f;
};

} // namespace gyo

#endif // FLY_CAMERA_H
