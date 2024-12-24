#ifndef SCENE_CONTROLLER_H
#define SCENE_CONTROLLER_H

#include <glm/glm.hpp>

class Renderer;
class Shader;
class Texture2D;
class Model;
class FlyCamera;

class SceneController {
public:
    SceneController(Renderer* renderer);
    ~SceneController();

    void Update(float dt);
    void Render();

    void OnKeyPressed(int key, float dt);
    void OnKeyReleased(int key, float dt);

private:
    Renderer* renderer = nullptr;

    FlyCamera* camera = nullptr;

    Model* model = nullptr;
    Texture2D* texture1 = nullptr;
    Texture2D* texture2 = nullptr;
    Shader* shader = nullptr;

    void RenderScene();
};

#endif // SCENE_CONTROLLER_H
