
#include <gyo/renderer/RenderState.h>
#include <gyo/utilities/GetError.h>

namespace gyo {

RenderState::RenderState() {
}

void RenderState::SetBlendingEnabled(bool enable, GLenum srcFactor, GLenum dstFactor) {
    if(enable && !isBlendingEnabled) {
        glEnable(GL_BLEND);
        glCheckError();
        isBlendingEnabled = true;
    }
    else if(!enable && isBlendingEnabled) {
        glDisable(GL_BLEND);
        glCheckError();
        isBlendingEnabled = false;
    }

    if(srcFactor != blendSrcFactor || dstFactor != blendDstFactor) {
        glBlendFunc(srcFactor, dstFactor);
        glCheckError();
        blendSrcFactor = srcFactor;
        blendDstFactor = dstFactor;
    }
}

void RenderState::SetDepthTestingEnabled(bool enable, GLenum function) {
    if(enable && !isDepthTestingEnabled) {
        glEnable(GL_DEPTH_TEST);
        glCheckError();
        isDepthTestingEnabled = true;
    }
    else if(!enable && isDepthTestingEnabled) {
        glDisable(GL_DEPTH_TEST);
        glCheckError();
        isDepthTestingEnabled = false;
    }

    if(function != depthFunction) {
        glDepthFunc(function);
        glCheckError();
        depthFunction = function;
    }
}

void RenderState::SetFaceCullingEnabled(bool enable, GLenum face) {
    if(enable && !isFaceCullingEnabled) {
        glEnable(GL_CULL_FACE);
        glCheckError();
        isFaceCullingEnabled = true;
    }
    else if(!enable && isFaceCullingEnabled) {
        glDisable(GL_CULL_FACE);
        glCheckError();
        isFaceCullingEnabled = false;
    }

    if(face != cullFace) {
        glCullFace(face);
        glCheckError();
        cullFace = face;
    }
}

} // namespace gyo
