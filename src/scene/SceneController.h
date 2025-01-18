#ifndef SCENE_CONTROLLER_H
#define SCENE_CONTROLLER_H

#include <functional>
#include <glm/glm.hpp>

struct FrameStats {
    float geometryMs = 0;
    float vfcMs = 0;
    float opaqueMs = 0;
    float alphaMs = 0;
    unsigned int drawCalls = 0;

    void Reset() {
        geometryMs = 0;
        vfcMs = 0;
        opaqueMs = 0;
        alphaMs = 0;
        drawCalls = 0;
    }
};

class Renderer;
class DrawCall;
class SceneNode;
class Model;
class AABBWireframe;
class TangentsRenderer;
class FlyCamera;
class Text;
struct Frustum;
struct LightNode;

class SceneController {
public:
    SceneController(Renderer* renderer, const int& width, const int& height);
    ~SceneController();

    void Update(float dt);
    void Render();

    void AddNode(SceneNode* node);
    void AddUpdateFunction(std::function<void(float)> f) { updateFunctions.push_back(f); }

    void OnKeyPressed(int key, float dt);
    // void OnKeyReleased(int key, float dt);
    void OnMouseMove(float x, float y);

private:
    Renderer* renderer = nullptr;
    glm::ivec2 size;

    std::vector<std::function<void(float)>> updateFunctions;

    FlyCamera* camera = nullptr;

    const glm::vec3 ambientLight = { 0.001f, 0.001f, 0.0005f };
    std::vector<LightNode*> lights = {};

    std::vector<Model*> models = {};

    Text* textRenderer;

    float lastMouseX;
    float lastMouseY;

    // per-frame
    FrameStats stats;
    std::vector<Model*> visibleModels = {};
    std::vector<DrawCall> opaqueDrawCalls = {};
    std::vector<DrawCall> alphaDrawCalls = {};

    void RenderScene();
    void FrustumCull(
        const Frustum& cameraFrustum,
        const std::vector<Model*>& sceneModels,
        std::vector<Model*>& visibleSceneModels
    );
    void RenderStats();
};

#endif // SCENE_CONTROLLER_H
