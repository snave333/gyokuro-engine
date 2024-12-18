#ifndef SCENE_CONTROLLER_H
#define SCENE_CONTROLLER_H

class Renderer;
class Shader;

class SceneController {
public:
    SceneController(Renderer* renderer);
    ~SceneController();

    void Update(float dt);
    void Render();

private:
    Renderer* renderer;

    float vertices[24] = {
        // positions         // colors
         0.5f,  0.5f, 0.0f,  1.0f, 0.5f, 0.0f, // top right
         0.5f, -0.5f, 0.0f,  0.5f, 1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.5f, // bottom left
        -0.5f,  0.5f, 0.0f,  0.0f, 0.5f, 1.0f  // top left 
    };
    unsigned int VBO;

    unsigned int indices[6] = {
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };
    unsigned int EBO;

    Shader* ourShader;

    unsigned int VAO;

    void RenderScene();
};

#endif // SCENE_CONTROLLER_H
