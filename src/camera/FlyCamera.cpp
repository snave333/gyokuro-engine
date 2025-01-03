
#include <camera/FlyCamera.h>

FlyCamera::~FlyCamera() {}

void FlyCamera::OnLook(float yawDelta, float pitchDelta) {
    yaw += yawDelta;
    pitch += pitchDelta;

    // clamp our pitch
    pitch = glm::clamp(pitch, -89.0f, 89.0f);

    SetRotation(pitch, yaw, 0);
}
