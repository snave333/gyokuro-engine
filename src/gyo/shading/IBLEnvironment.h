#ifndef IBL_ENVIRONMENT_H
#define IBL_ENVIRONMENT_H

#include <gyo/shading/Texture2D.h>
#include <gyo/shading/TextureCube.h>

namespace gyo {

struct IBLEnvironment {
    TextureCube* cubeMap;
    TextureCube* irradianceMap;
    TextureCube* prefilteredEnvMap;
    Texture2D* brdfLUT;
};

} // namespace gyo

#endif // IBL_ENVIRONMENT_H
