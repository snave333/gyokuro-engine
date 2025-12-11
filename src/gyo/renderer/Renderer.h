#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <glm/glm.hpp>

#include <gyo/renderer/RenderState.h>
#include <gyo/shading/IBLEnvironment.h>
#include <gyo/utilities/FrameStats.h>

namespace gyo {

class ScreenQuad;
class DrawCall;
class Skybox;

class Renderer {
public:
    FrameStats stats;

    Renderer(const int& width, const int& height, int msaaSamples, float pixelScale);
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

    const float& GetPixelScale() { return pixelScale; }

private:
    glm::ivec2 size;
    unsigned int msaaSamples;
    float pixelScale;

    RenderState state;

    const glm::vec3 clearColor = { 0.0008f, 0.0008f, 0.0004f };

    // frame buffer
    ScreenQuad* screenQuad = nullptr;

    unsigned int framebuffer;
    // non-MSAA
    unsigned int textureColorbuffer; // we re-use this if MSAA is enabled
    unsigned int depthRenderbuffer;
    // MSAA
    unsigned int textureColorbufferMS;
    unsigned int depthRenderbufferMS;
    unsigned int intermediateFramebuffer;

    void PrintGLInfo();
};

} // namespace gyo

#endif // RENDERER_H
