#ifndef SCENE_CONTROLLER_H
#define SCENE_CONTROLLER_H

#include <glm/glm.hpp>

class Renderer;
class Shader;
class Texture2D;
class Mesh;

class SceneController {
public:
    SceneController(Renderer* renderer);
    ~SceneController();

    void Update(float dt);
    void Render();

private:
    Renderer* renderer;

    Mesh* mesh;
    Texture2D* texture1;
    Texture2D* texture2;
    Shader* shader;

    glm::mat4 model;

    void RenderScene();
};

#endif // SCENE_CONTROLLER_H
