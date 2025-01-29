
#include <renderer/RenderState.h>

namespace gyo {

RenderState::RenderState() {
}

void RenderState::SetBlendingEnabled(bool enable, GLenum srcFactor, GLenum dstFactor) {
    if(enable && !isBlendingEnabled) {
        glEnable(GL_BLEND);
        isBlendingEnabled = true;
    }
    else if(!enable && isBlendingEnabled) {
        glDisable(GL_BLEND);
        isBlendingEnabled = false;
    }

    if(srcFactor != blendSrcFactor || dstFactor != blendDstFactor) {
        glBlendFunc(srcFactor, dstFactor);
        blendSrcFactor = srcFactor;
        blendDstFactor = dstFactor;
    }
}

void RenderState::SetDepthTestingEnabled(bool enable, GLenum function) {
    if(enable && !isDepthTestingEnabled) {
        glEnable(GL_DEPTH_TEST);
        isDepthTestingEnabled = true;
    }
    else if(!enable && isDepthTestingEnabled) {
        glDisable(GL_DEPTH_TEST);
        isDepthTestingEnabled = false;
    }

    if(function != depthFunction) {
        glDepthFunc(function);
        depthFunction = function;
    }
}

void RenderState::SetFaceCullingEnabled(bool enable, GLenum face) {
    if(enable && !isFaceCullingEnabled) {
        glEnable(GL_CULL_FACE);
        isFaceCullingEnabled = true;
    }
    else if(!enable && isFaceCullingEnabled) {
        glDisable(GL_CULL_FACE);
        isFaceCullingEnabled = false;
    }

    if(face != cullFace) {
        glCullFace(face);
        cullFace = face;
    }
}

} // namespace gyo
