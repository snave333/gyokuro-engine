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
    static const unsigned int       EnvMapSize =            512U;

    static const unsigned int       IrradianceTexSize =     64U;
    static constexpr const float    IrradianceSampleDelta = 0.0125f;

    static const unsigned int       PrefilteredTexSize =    256U;
    static const unsigned int       PrefilterMipLevels =    9U;

    static const unsigned int       BRDFLUTSize =           512U;
    static const unsigned int       BRDFSampleCount =       4096U;

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

    float medianHDRLuminance;

    Mesh* cube;
    Mesh* ndcQuad;

    float ComputeLuminance(Texture2D* hdrTexture);
    TextureCube RenderTexCube(const Shader& shader, unsigned int size, std::function<void()> setUniforms);
    TextureCube RenderPrefilteredTexCube(const Shader& shader, unsigned int size, unsigned int maxMipLevels, std::function<void()> setUniforms);
    Texture2D PreComputeBRDFLUT(unsigned int size);
};
  
} // namespace gyo

#endif // IBL_ENVIRONMENT_LOADER_H
