#ifndef SCENE_CONTROLLER_H
#define SCENE_CONTROLLER_H

#include <glm/glm.hpp>

struct FrameStats {
    float vfcMs = 0;
    float opaqueMs = 0;
    unsigned int drawCalls = 0;
};

class Renderer;
class SceneNode;
class Model;
class AABBWireframe;
class TangentsRenderer;
class FlyCamera;
class Text;
struct LightNode;

class SceneController {
public:
    SceneController(Renderer* renderer, const int& width, const int& height);
    ~SceneController();

    void Update(float dt);
    void Render();

    void AddNode(SceneNode* node);

    void OnKeyPressed(int key, float dt);
    // void OnKeyReleased(int key, float dt);
    void OnMouseMove(float x, float y);

private:
    Renderer* renderer = nullptr;
    glm::ivec2 size;

    FlyCamera* camera = nullptr;

    const glm::vec3 ambientLight = { 0.001f, 0.001f, 0.0005f };
    std::vector<LightNode*> lights = {};

    std::vector<Model*> models = {};

    Text* textRenderer;

    float lastMouseX;
    float lastMouseY;

    void RenderScene();
};

#endif // SCENE_CONTROLLER_H
