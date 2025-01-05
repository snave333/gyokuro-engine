#ifndef RENDERER_H
#define RENDERER_H

#include <glm/glm.hpp>

struct GLFWwindow;
class ScreenQuad;

class Renderer {
public:
    Renderer(GLFWwindow* window, const int& width, const int& height);
    ~Renderer();

    void CreateFrameBuffer();

    void BeginFrame();
    // void RenderOpaque();
    // void RenderTransparent();
    void EndFrame();

private:
    GLFWwindow* window = nullptr;
    glm::vec2 size;

    const glm::vec3 clearColor = { 0.2f, 0.2f, 0.2f };

    // frame buffer
    ScreenQuad* screenQuad = nullptr;
    unsigned int framebuffer;
    unsigned int textureColorbuffer;
    unsigned int depthRenderbuffer;

    void PrintGLInfo();
};

#endif // RENDERER_H