#ifndef RENDERER_H
#define RENDERER_H

#include <glm/glm.hpp>

#include <renderer/RenderState.h>

namespace gyo {

class ScreenQuad;
class DrawCall;
class Skybox;

class Renderer {
public:
    Renderer(const int& width, const int& height);
    ~Renderer();

    void CreateFrameBuffer();

    void BeginFrame();
    void RenderOpaque(std::vector<DrawCall> drawCalls);
    void RenderSkybox(Skybox* skybox, glm::mat4 cameraView, glm::mat4 cameraProjection);
    void RenderTransparent(std::vector<DrawCall> drawCalls);
    void EndGeometryPass();
    void RenderImageEffects();
    void RenderUI();
    void EndFrame();

private:
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

} // namespace gyo

#endif // RENDERER_H
