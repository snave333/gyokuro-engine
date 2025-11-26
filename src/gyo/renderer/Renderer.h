#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <glm/glm.hpp>

#include <gyo/renderer/RenderState.h>
#include <gyo/scene/IBLEnvironment.h>
#include <gyo/utilities/FrameStats.h>

namespace gyo {

class ScreenQuad;
class DrawCall;
class Skybox;

class Renderer {
public:
    FrameStats stats;

    Renderer(const int& width, const int& height);
    ~Renderer();

    void CreateFrameBuffer();

    void BeginFrame();
    void RenderOpaque(std::vector<DrawCall> drawCalls, const IBLEnvironment& environment);
    void RenderSkybox(Skybox* skybox, glm::mat4 cameraView, glm::mat4 cameraProjection);
    void RenderTransparent(std::vector<DrawCall> drawCalls);
    void EndGeometryPass();
    void RenderImageEffects();
    void RenderUI();
    void EndFrame();

private:
    glm::ivec2 size;

    RenderState state;

    const glm::vec3 clearColor = { 0.0008f, 0.0008f, 0.0004f };

    // frame buffer
    ScreenQuad* screenQuad = nullptr;
    unsigned int framebuffer;
    unsigned int textureColorbuffer;
    unsigned int depthRenderbuffer;

    void PrintGLInfo();
};

} // namespace gyo

#endif // RENDERER_H
