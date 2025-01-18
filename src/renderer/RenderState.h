#ifndef RENDER_STATE_H
#define RENDER_STATE_H

/**
 * Cache our current OpenGL state here to reduce the number of commands we send
 * per draw call – only send the commands if they are different from what is
 * currently set.
 */

#include <glad/glad.h>

class RenderState {
public:
    // constructor
    RenderState();

    void SetBlendingEnabled(bool enable, GLenum srcFactor = GL_SRC_ALPHA, GLenum dstFactor = GL_ONE_MINUS_SRC_ALPHA);
    void SetDepthTestingEnabled(bool enable, GLenum function = GL_LESS);
    void SetFaceCullingEnabled(bool enable, GLenum face = GL_BACK);

private:
    bool isBlendingEnabled;
    bool isDepthTestingEnabled;
    bool isFaceCullingEnabled;

    GLenum blendSrcFactor;
    GLenum blendDstFactor;
    GLenum depthFunction;
    GLenum cullFace;
};

#endif // RENDER_STATE_H