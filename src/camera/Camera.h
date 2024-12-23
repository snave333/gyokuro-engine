#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera {
public:
    Camera(glm::mat4 projection);

    glm::mat4 GetView();
    glm::mat4 GetProjection() const {
        return projection;
    };

protected:
    glm::mat4 view;
    glm::mat4 projection;
};

#endif // CAMERA_H
