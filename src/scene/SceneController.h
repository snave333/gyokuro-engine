#ifndef SCENE_CONTROLLER_H
#define SCENE_CONTROLLER_H

#include <glm/glm.hpp>

class Renderer;
class Shader;
class Texture2D;
class Model;
class FlyCamera;
class Text;
struct LightNode;

class SceneController {
public:
    SceneController(Renderer* renderer, const int& width, const int& height);
    ~SceneController();

    void Update(float dt);
    void Render();

    void OnKeyPressed(int key, float dt);
    // void OnKeyReleased(int key, float dt);
    void OnMouseMove(float x, float y);

private:
    Renderer* renderer = nullptr;
    glm::ivec2 size;

    FlyCamera* camera = nullptr;

    const glm::vec3 ambientLight = { 0.1f, 0.1f, 0.1f };
    LightNode* dirLight;

    std::vector<Model*> models = {};

    Text* textRenderer;

    float lastMouseX;
    float lastMouseY;

    void RenderScene();
};

#endif // SCENE_CONTROLLER_H
