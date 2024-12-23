#ifndef SCENE_NODE_H
#define SCENE_NODE_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

class SceneNode {
public:
    SceneNode() {};

    const glm::vec3& Position() { return position; }
    const glm::fquat& Rotation() { return rotation; }
    const glm::vec3& Scale() { return scale; }
    const glm::mat4& Transform();

    void Translate(float x, float y, float z);
    void Translate(glm::vec3 translation);

    void Rotate(float pitchDeg, float yawDeg, float rollDeg);
    void Rotate(float angleDeg, glm::vec3 axis);

    void Scale(float scale);
    void Scale(float x, float y, float z);
    void Scale(glm::vec3 scale);
protected:
    glm::vec3 position = glm::vec3();
    glm::fquat rotation = glm::quat(1, 0, 0, 0);
    glm::vec3 scale = glm::vec3(1);
    glm::mat4 transform = glm::mat4(1.0f);

private:
    bool isDirty = true;
};

#endif // SCENE_NODE_H
