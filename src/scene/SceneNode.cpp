
#include <scene/SceneNode.h>


const glm::mat4& SceneNode::GetTransform() {
    if(isDirty) {
        UpdateMatrices();
    }

    return transform;
}

void SceneNode::UpdateMatrices() {
    glm::mat4 t = glm::mat4(1.0f);
    t = glm::translate(t, position);
    
    glm::mat4 r = glm::toMat4(rotation);

    glm::mat4 s = glm::mat4(1.0f);
    s = glm::scale(s, scale);

    transform = t * r * s;
    isDirty = false;
}

glm::vec3 SceneNode::GetForward() {
    return glm::normalize(glm::vec3(0, 0, -1) * rotation);
}

glm::vec3 SceneNode::GetRight() {
    return glm::normalize(glm::vec3(1, 0, 0) * rotation);
}

glm::vec3 SceneNode::GetUp() {
    return glm::normalize(glm::vec3(0, 1, 0) * rotation);
}

void SceneNode::SetPosition(float x, float y, float z) {
    position = glm::vec3(x, y, z);
    isDirty = true;
}

void SceneNode::SetPosition(const glm::vec3 &position) {
    this->position = position;
    isDirty = true;
}

void SceneNode::SetRotation(float pitchDeg, float yawDeg, float rollDeg) {
    rotation = glm::normalize(glm::quat(glm::radians(glm::vec3(pitchDeg, yawDeg, rollDeg))));
    isDirty = true;
}

void SceneNode::SetRotation(float angleDeg, const glm::vec3 &axis) {
    rotation = glm::normalize(glm::angleAxis(glm::radians(angleDeg), axis));
    isDirty = true;
}

void SceneNode::SetRotation(const glm::fquat &rotation) {
    this->rotation = rotation;
    isDirty = true;
}

void SceneNode::SetScale(float scale) {
    this->scale = glm::vec3(scale, scale, scale);
    isDirty = true;
}

void SceneNode::SetScale(float x, float y, float z) {
    this->scale = glm::vec3(x, y, z);
    isDirty = true;
}

void SceneNode::SetScale(const glm::vec3 &scale) {
    this->scale = scale;
    isDirty = true;
}

void SceneNode::Translate(float x, float y, float z) {
    position += glm::vec3(x, y, z);
    isDirty = true;
}

void SceneNode::Translate(const glm::vec3 &translation) {
    position += translation;
    isDirty = true;
}

void SceneNode::Rotate(float pitchDeg, float yawDeg, float rollDeg) {
    rotation = glm::normalize(rotation * glm::quat(glm::radians(glm::vec3(pitchDeg, yawDeg, rollDeg))));
    isDirty = true;
}

void SceneNode::Rotate(float angleDeg, const glm::vec3 &axis) {
    rotation = glm::normalize(rotation * glm::angleAxis(glm::radians(angleDeg), axis));
    isDirty = true;
}

void SceneNode::Scale(float scale) {
    this->scale *= scale;
    isDirty = true;
}

void SceneNode::Scale(float x, float y, float z) {
    scale *= glm::vec3(x, y, z);
    isDirty = true;
}

void SceneNode::Scale(const glm::vec3 &scale) {
    this->scale *= scale;
    isDirty = true;
}
