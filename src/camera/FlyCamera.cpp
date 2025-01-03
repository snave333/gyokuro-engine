
#include <camera/FlyCamera.h>

FlyCamera::~FlyCamera() {}

void FlyCamera::OnLook(float xOffset, float yOffset) {
    xOffset *= mouseSensitivity;
    yOffset *= mouseSensitivity;

    yaw += xOffset;
    pitch += yOffset;

    // clamp our pitch
    pitch = glm::clamp(pitch, -89.0f, 89.0f);

    SetRotation(pitch, yaw, 0);
}
