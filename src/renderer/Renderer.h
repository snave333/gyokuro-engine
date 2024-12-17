#ifndef RENDERER_H
#define RENDERER_H

struct GLFWwindow;

class Renderer {
public:
    Renderer(GLFWwindow* window);
    ~Renderer();

    void BeginFrame();
    void Clear();
    void EndFrame();

private:
    GLFWwindow* window;
};

#endif // RENDERER_H