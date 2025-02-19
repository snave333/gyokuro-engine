#ifndef SCENE_NODE_H
#define SCENE_NODE_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace gyo {

class SceneNode {
public:
    SceneNode() {}
    virtual ~SceneNode() {}

    const glm::vec3& GetPosition() const { return position; }
    const glm::fquat& GetRotation() const { return rotation; }
    const glm::vec3& GetScale() const { return scale; }
    
    void SetPosition(float x, float y, float z);
    void SetPosition(const glm::vec3 &position);
    void SetRotation(float pitchDeg, float yawDeg, float rollDeg);
    void SetRotation(float angleDeg, const glm::vec3 &axis);
    void SetRotation(const glm::fquat &rotation);
    void SetScale(float scale);
    void SetScale(float x, float y, float z);
    void SetScale(const glm::vec3 &scale);

    const glm::mat4& GetTransform();
    const glm::mat4& GetNormalMatrix();

    glm::vec3 GetForward();
    glm::vec3 GetRight();
    glm::vec3 GetUp();

    void Translate(float x, float y, float z);
    void Translate(const glm::vec3 &translation);

    void Rotate(float pitchDeg, float yawDeg, float rollDeg);
    void Rotate(float angleDeg, const glm::vec3 &axis);

    void Scale(float scale);
    void Scale(float x, float y, float z);
    void Scale(const glm::vec3 &scale);

protected:
    glm::vec3 position = glm::vec3();
    glm::fquat rotation = glm::quat(1, 0, 0, 0);
    glm::vec3 scale = glm::vec3(1);
    
    glm::mat4 transform = glm::mat4(1.0f);
    glm::mat4 normalMatrix = glm::mat4(1.0f);

    virtual void SetDirty();

private:
    bool isTransformDirty = true;

    void UpdateMatrices();
};

} // namespace gyo

#endif // SCENE_NODE_H
