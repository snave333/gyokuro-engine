#ifndef SCENE_CONTROLLER_H
#define SCENE_CONTROLLER_H

#include <functional>
#include <glm/glm.hpp>

namespace gyo {

class Renderer;
class DrawCall;
class SceneNode;
class ModelNode;
class AABBWireframe;
class TangentsRenderer;
class FlyCamera;
class Skybox;
class SDFText;
class LightsUBO;
struct Frustum;
struct LightNode;
struct IDrawable;

class SceneController {
public:
    static const unsigned int MAX_POINT_LIGHTS = 4;
    static const unsigned int MAX_SPOT_LIGHTS = 4;

public:
    SceneController(Renderer* renderer, const int& width, const int& height);
    ~SceneController();

    void Update(float dt);
    void Render();

    void AddNode(SceneNode* node);
    void AddDrawable(IDrawable* drawable);
    void SetSkybox(Skybox* skybox = nullptr);
    void AddUpdateFunction(std::function<void(float)> f) { updateFunctions.push_back(f); }

    void OnKeyPressed(int key, float dt);
    // void OnKeyReleased(int key, float dt);
    void OnMouseMove(float x, float y);

private:
    Renderer* renderer = nullptr;
    glm::ivec2 size;

    std::vector<std::function<void(float)>> updateFunctions;

    FlyCamera* camera = nullptr;

    Skybox* skybox = nullptr;

    const glm::vec3 ambientLight = { 0, 0, 0 };
    std::vector<LightNode*> lights = {};
    LightsUBO* lightsUBO = nullptr;

    std::vector<ModelNode*> models = {};
    std::vector<IDrawable*> drawables = {};

    SDFText* textRenderer;

    float lastMouseX;
    float lastMouseY;

    // per-frame
    std::vector<ModelNode*> visibleModels = {};
    std::vector<DrawCall> opaqueDrawCalls = {};
    std::vector<DrawCall> alphaDrawCalls = {};

    void RenderScene();
    void FrustumCull(
        const Frustum& cameraFrustum,
        const std::vector<ModelNode*>& sceneModels,
        std::vector<ModelNode*>& visibleSceneModels
    );
    void RenderStats();
};

} // namespace gyo

#endif // SCENE_CONTROLLER_H
