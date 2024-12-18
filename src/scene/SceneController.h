#ifndef SCENE_CONTROLLER_H
#define SCENE_CONTROLLER_H

class Renderer;

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

    const char *vertShaderSource =
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec3 aColor;\n"
        "out vec3 ourColor;\n"
        "void main() {\n"
        "    gl_Position = vec4(aPos, 1.0);\n"
        "    ourColor = aColor;\n"
        "}\0";
    unsigned int vertShader;

    const char *fragShaderSource =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "in vec3 ourColor;\n"
        "void main() {\n"
        "    FragColor = vec4(ourColor, 1.0);\n"
        "}\0";
    unsigned int fragShader;

    unsigned int shaderProgram;

    unsigned int VAO;

    void RenderScene();
};

#endif // SCENE_CONTROLLER_H
