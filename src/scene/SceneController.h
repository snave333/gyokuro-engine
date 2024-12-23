#ifndef SCENE_CONTROLLER_H
#define SCENE_CONTROLLER_H

#include <glm/glm.hpp>

class Renderer;
class Shader;
class Texture2D;
class Mesh;
class CameraNode;

class SceneController {
public:
    SceneController(Renderer* renderer);
    ~SceneController();

    void Update(float dt);
    void Render();

    void OnKeyboardInput(int key, int action, int mods);

private:
    Renderer* renderer = nullptr;

    CameraNode* camera = nullptr;

    Mesh* mesh = nullptr;
    Texture2D* texture1 = nullptr;
    Texture2D* texture2 = nullptr;
    Shader* shader = nullptr;

    glm::mat4 model;

    void RenderScene();
};

#endif // SCENE_CONTROLLER_H
