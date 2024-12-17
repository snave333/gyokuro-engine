#ifndef SCENE_CONTROLLER_H
#define SCENE_CONTROLLER_H

class Renderer;

class SceneController {
public:
    SceneController(Renderer* renderer);
    ~SceneController();

    void Update(double dt);
    void Render();

private:
    Renderer* renderer;

    float vertices[9] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };
    unsigned int VBO;

    const char *vertShaderSource =
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main() {\n"
        "    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";
    unsigned int vertShader;

    const char *fragShaderSource =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main() {\n"
        "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\0";
    unsigned int fragShader;

    unsigned int shaderProgram;

    unsigned int VAO;

    void RenderScene();
};

#endif // SCENE_CONTROLLER_H
