#ifndef FLY_CAMERA_H
#define FLY_CAMERA_H

#include <camera/CameraNode.h>

class FlyCamera : public CameraNode {
public:
    FlyCamera(Camera* camera) : CameraNode(camera) { };
    ~FlyCamera();

    
};

#endif // FLY_CAMERA_H
