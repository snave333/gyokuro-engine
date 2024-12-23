#ifndef SCENE_NODE_H
#define SCENE_NODE_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

class SceneNode {
public:
    SceneNode() {};

    glm::vec3 position;
    glm::fquat rotation;
    glm::vec3 scale;

    glm::mat4 GetTransform() {
        // TODO create transform from position, rotation, scale
        // glm::mat4 translation = glm::

        return glm::mat4(1.0f);
    }
};

#endif // SCENE_NODE_H
