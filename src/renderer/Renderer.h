#ifndef RENDERER_H
#define RENDERER_H

struct GLFWwindow;

enum RenderType {
    Opaque,
    Transparent,
    Additive
};

class Renderer {
public:
    Renderer(GLFWwindow* window);
    ~Renderer();

    void BeginFrame();
    void Clear();
    void EndFrame();

private:
    GLFWwindow* window = nullptr;

    void PrintGLInfo();
};

#endif // RENDERER_H