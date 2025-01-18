#ifndef RENDERER_H
#define RENDERER_H

#include <glm/glm.hpp>

#include <renderer/RenderState.h>

struct GLFWwindow;
class ScreenQuad;
class DrawCall;

class Renderer {
public:
    Renderer(GLFWwindow* window, const int& width, const int& height);
    ~Renderer();

    void CreateFrameBuffer();

    void BeginFrame();
    void RenderOpaque(std::vector<DrawCall> drawCalls);
    void RenderTransparent(std::vector<DrawCall> drawCalls);
    void EndGeometryPass();
    void RenderImageEffects();
    void RenderUI();
    void EndFrame();

private:
    GLFWwindow* window = nullptr;
    glm::ivec2 size;

    RenderState state;

    const glm::vec3 clearColor = { 0.0f, 0.0f, 0.0f };

    // frame buffer
    ScreenQuad* screenQuad = nullptr;
    unsigned int framebuffer;
    unsigned int textureColorbuffer;
    unsigned int depthRenderbuffer;

    void PrintGLInfo();
};

#endif // RENDERER_H