
#include <gyo/internal/scene/SceneNode.h>

namespace gyo {

const glm::mat4& SceneNode::GetTransform() {
    if(isTransformDirty) {
        UpdateMatrices();
    }

    return transform;
}

const glm::mat4& SceneNode::GetNormalMatrix() {
    if(isTransformDirty) {
        UpdateMatrices();
    }

    return normalMatrix;
}

void SceneNode::SetDirty() {
    isTransformDirty = true;
}

void SceneNode::UpdateMatrices() {
    transform = glm::mat4(1.0f);
    
    transform = glm::translate(transform, position);
    transform *= glm::toMat4(rotation);
    transform = glm::scale(transform, scale);

    normalMatrix = glm::transpose(glm::inverse(transform));
    
    isTransformDirty = false;
}

glm::vec3 SceneNode::GetForward() {
    return glm::rotate(rotation, glm::vec3(0, 0, 1));
}

glm::vec3 SceneNode::GetRight() {
    return glm::rotate(rotation, glm::vec3(1, 0, 0));
}

glm::vec3 SceneNode::GetUp() {
    return glm::rotate(rotation, glm::vec3(0, 1, 0));
}

void SceneNode::SetPosition(float x, float y, float z) {
    position = glm::vec3(x, y, z);
    SetDirty();
}

void SceneNode::SetPosition(const glm::vec3 &position) {
    this->position = position;
    SetDirty();
}

void SceneNode::SetRotation(float pitchDeg, float yawDeg, float rollDeg) {
    glm::vec3 angles = glm::radians(glm::vec3(pitchDeg, yawDeg, rollDeg));
    rotation = glm::quat(angles);
    SetDirty();
}

void SceneNode::SetRotation(float angleDeg, const glm::vec3 &axis) {
    rotation = glm::angleAxis(glm::radians(angleDeg), axis);
    SetDirty();
}

void SceneNode::SetRotation(const glm::fquat &rotation) {
    this->rotation = rotation;
    SetDirty();
}

void SceneNode::SetScale(float scale) {
    this->scale = glm::vec3(scale, scale, scale);
    SetDirty();
}

void SceneNode::SetScale(float x, float y, float z) {
    this->scale = glm::vec3(x, y, z);
    SetDirty();
}

void SceneNode::SetScale(const glm::vec3 &scale) {
    this->scale = scale;
    SetDirty();
}

void SceneNode::Translate(float x, float y, float z) {
    position += glm::vec3(x, y, z);
    SetDirty();
}

void SceneNode::Translate(const glm::vec3 &translation) {
    position += translation;
    SetDirty();
}

void SceneNode::Rotate(float pitchDeg, float yawDeg, float rollDeg) {
    glm::vec3 angles = glm::radians(glm::vec3(pitchDeg, yawDeg, rollDeg));
    if(glm::length2(angles) > 0.0f) {
        rotation = rotation * glm::quat(angles);
        SetDirty();
    }
}

void SceneNode::Rotate(float angleDeg, const glm::vec3 &axis) {
    float angleRad = glm::radians(angleDeg);
    if(angleRad != 0.0f) {
        rotation = rotation * glm::angleAxis(angleRad, axis);
        SetDirty();
    }
}

void SceneNode::Scale(float scale) {
    this->scale *= scale;
    SetDirty();
}

void SceneNode::Scale(float x, float y, float z) {
    scale *= glm::vec3(x, y, z);
    SetDirty();
}

void SceneNode::Scale(const glm::vec3 &scale) {
    this->scale *= scale;
    SetDirty();
}

} // namespace gyo
