#ifndef RENDERER_H
#define RENDERER_H

#include <glm/glm.hpp>

struct GLFWwindow;
class ScreenQuad;

enum RenderType {
    Opaque,
    Transparent,
    Additive
};

class Renderer {
public:
    Renderer(GLFWwindow* window, const int& width, const int& height);
    ~Renderer();

    void CreateFrameBuffer();

    void BeginFrame();
    // void Clear();
    // void RenderOpaque();
    // void RenderTransparent();
    void EndFrame();

private:
    GLFWwindow* window = nullptr;
    glm::vec2 size;

    const glm::vec3 clearColor = { 0.39f, 0.58f, 0.93f };

    // frame buffer
    ScreenQuad* screenQuad;
    unsigned int framebuffer;
    unsigned int textureColorbuffer;
    unsigned int depthRenderbuffer;

    void PrintGLInfo();
};

#endif // RENDERER_H