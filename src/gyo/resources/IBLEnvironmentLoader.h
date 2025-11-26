#ifndef IBL_ENVIRONMENT_LOADER_H
#define IBL_ENVIRONMENT_LOADER_H

#include <glm/glm.hpp>

namespace gyo {

struct IBLEnvironment;

class Mesh;
class Shader;
class ShaderMaterial;
class Texture2D;
class TextureCube;

class IBLEnvironmentLoader {
public:
    static const unsigned int       EnvMapSize =            1024U;

    static const unsigned int       IrradianceTexSize =     32U;
    static constexpr const float    IrradianceSampleDelta = 0.025f;

    static const unsigned int       PrefilteredTexSize =    128U;
    static const unsigned int       PrefilterMipLevels =    6U;
    static const unsigned int       PrefilterSampleCount =  1024U;

    static const unsigned int       BRDFLUTSize =           512U;
    static const unsigned int       BRDFSampleCount =       1024U;

public:
    IBLEnvironmentLoader();
    ~IBLEnvironmentLoader();

    TextureCube GetCubemap(Texture2D* hdrTexture);
    TextureCube GetIrradianceMap(TextureCube* cubeMap);
    TextureCube GetPrefilteredEnvMap(TextureCube* cubeMap);
    Texture2D GetBRDFLUT();

private:
    // framebuffer for cubemap rendering
    unsigned int captureFBO;
    unsigned int captureRBO;
    glm::mat4 captureProjection;
    glm::mat4 captureViews[6];

    ShaderMaterial* eqRectToCubemapMaterial;
    ShaderMaterial* irradianceConvolutionMaterial;
    ShaderMaterial* prefilterConvolutionMaterial;
    ShaderMaterial* brdfConvolutionMaterial;

    Mesh* cube;
    Mesh* ndcQuad;

    TextureCube RenderTexCube(const Shader& shader, unsigned int size, std::function<void()> setUniforms);
    TextureCube RenderPrefilteredTexCube(const Shader& shader, unsigned int size, unsigned int maxMipLevels, std::function<void()> setUniforms);
    Texture2D PreComputeBRDFLUT(unsigned int size);
};
  
} // namespace gyo

#endif // IBL_ENVIRONMENT_LOADER_H
