
#include <scene/SceneNode.h>


const glm::mat4& SceneNode::Transform() {
    if(!isDirty) {
        return transform;
    }

    glm::mat4 t = glm::mat4(1.0f);
    t = glm::translate(t, position);
    
    glm::mat4 r = glm::toMat4(rotation);

    glm::mat4 s = glm::mat4(1.0f);
    s = glm::scale(s, scale);

    transform = s * r * t;
    isDirty = false;

    return transform;
}

void SceneNode::Translate(float x, float y, float z) {
    Translate(glm::vec3(x, y, z));
}

void SceneNode::Translate(glm::vec3 translation) {
    position += translation;
    isDirty = true;
}

void SceneNode::Rotate(float pitchDeg, float yawDeg, float rollDeg) {
    rotation = glm::normalize(rotation * glm::quat(glm::radians(glm::vec3(pitchDeg, yawDeg, rollDeg))));
    isDirty = true;
}

void SceneNode::Rotate(float angleDeg, glm::vec3 axis) {
    rotation = glm::normalize(rotation * glm::angleAxis(glm::radians(angleDeg), axis));
    isDirty = true;
}

void SceneNode::Scale(float scale) {
    this->scale *= scale;
    isDirty = true;
}

void SceneNode::Scale(float x, float y, float z) {
    Scale(glm::vec3(x, y, z));
}

void SceneNode::Scale(glm::vec3 scale) {
    this->scale *= scale;
    isDirty = true;
}
