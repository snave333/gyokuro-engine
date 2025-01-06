
#include <scene/SceneNode.h>


const glm::mat4& SceneNode::GetTransform() {
    if(isDirty) {
        UpdateMatrices();
    }

    return transform;
}

const glm::mat4& SceneNode::GetNormalMatrix() {
    if(isDirty) {
        UpdateMatrices();
    }

    return normalMatrix;
}

void SceneNode::UpdateMatrices() {
    glm::mat4 t = glm::mat4(1.0f);
    t = glm::translate(t, position);
    
    glm::mat4 r = glm::toMat4(rotation);

    glm::mat4 s = glm::mat4(1.0f);
    s = glm::scale(s, scale);

    transform = t * r * s;
    normalMatrix = glm::transpose(glm::inverse(t * r));
    
    isDirty = false;
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
    isDirty = true;
}

void SceneNode::SetPosition(const glm::vec3 &position) {
    this->position = position;
    isDirty = true;
}

void SceneNode::SetRotation(float pitchDeg, float yawDeg, float rollDeg) {
    glm::vec3 angles = glm::radians(glm::vec3(pitchDeg, yawDeg, rollDeg));
    rotation = glm::quat(angles);
    isDirty = true;
}

void SceneNode::SetRotation(float angleDeg, const glm::vec3 &axis) {
    rotation = glm::angleAxis(glm::radians(angleDeg), axis);
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
    glm::vec3 angles = glm::radians(glm::vec3(pitchDeg, yawDeg, rollDeg));
    if(glm::length2(angles) > 0.0f) {
        rotation = rotation * glm::quat(angles);
        isDirty = true;
    }
}

void SceneNode::Rotate(float angleDeg, const glm::vec3 &axis) {
    float angleRad = glm::radians(angleDeg);
    if(angleRad != 0.0f) {
        rotation = rotation * glm::angleAxis(angleRad, axis);
        isDirty = true;
    }
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
