
#include <gyo/resources/IBLEnvironmentLoader.h>
#include <gyo/resources/Resources.h>
#include <gyo/geometry/InvertedCube.h>
#include <gyo/geometry/Quad.h>
#include <gyo/mesh/Mesh.h>
#include <gyo/shading/IBLEnvironment.h>
#include <gyo/shading/Shader.h>
#include <gyo/shading/ShaderMaterial.h>
#include <gyo/shading/ShaderSemantics.h>
#include <gyo/shading/Texture2D.h>
#include <gyo/shading/TextureCube.h>
#include <gyo/utilities/Clock.h>
#include <gyo/utilities/GetError.h>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace gyo {

IBLEnvironmentLoader::IBLEnvironmentLoader() {
    // generate our frame buffer and render buffer objects

    glGenFramebuffers(1, &captureFBO);
    glCheckError();
    glGenRenderbuffers(1, &captureRBO);
    glCheckError();

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glCheckError();
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glCheckError();

    // for now just set it to 1x1; it'll have to be resized anyway
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 1, 1);
    glCheckError();
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);
    glCheckError();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glCheckError();

    // our 6 cube face directions for rendering

    captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);

    captureViews[0] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f));
    captureViews[1] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f));
    captureViews[2] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f));
    captureViews[3] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f));
    captureViews[4] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f));
    captureViews[5] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f));

    // our shaders

    Shader* eqRectToCubemapShader = Resources::GetShader("cubemap.vert", "eqRectToCubemap.frag");
    eqRectToCubemapShader->Use();
    eqRectToCubemapShader->SetInt("equirectangularMap", 0);
    eqRectToCubemapMaterial = new ShaderMaterial(
        eqRectToCubemapShader, { { "aPos", SEMANTIC_POSITION } }
    );


    std::set<std::string> irradianceDefines = {
        "SAMPLE_DELTA " + std::to_string(IrradianceSampleDelta)
    };
    Shader* irradianceConvolutionShader = Resources::GetShader(
        "cubemap.vert",
        "irradianceConvolution.frag",
        irradianceDefines
    );
    irradianceConvolutionShader->Use();
    irradianceConvolutionShader->SetInt("environmentMap", 0);
    irradianceConvolutionMaterial = new ShaderMaterial(
        irradianceConvolutionShader, { { "aPos", SEMANTIC_POSITION } }
    );


    std::set<std::string> prefilterDefines = { 
        "ENV_MAP_RESOLUTION " + std::to_string(EnvMapSize) + ".0"
    };
    Shader* prefilterConvolutionShader = Resources::GetShader(
        "cubemap.vert",
        "prefilterConvolution.frag",
        prefilterDefines
    );
    prefilterConvolutionShader->Use();
    prefilterConvolutionShader->SetInt("environmentMap", 0);
    prefilterConvolutionMaterial = new ShaderMaterial(
        prefilterConvolutionShader, { { "aPos", SEMANTIC_POSITION } }
    );


    std::set<std::string> brdfDefines = {
        "SAMPLE_COUNT " + std::to_string(BRDFSampleCount) + "u"
    };
    brdfConvolutionMaterial = new ShaderMaterial(
        Resources::GetShader(
            "screen.vert",
            "brdfConvolution.frag",
            brdfDefines
        ),
        {
            { "aPos", SEMANTIC_POSITION },
            { "aTexCoord", SEMANTIC_TEXCOORD0 }
        }
    );

    // our meshes

    cube = new Mesh(new InvertedCube(), nullptr);

    ndcQuad = new Mesh(new Quad(), brdfConvolutionMaterial);
}

IBLEnvironmentLoader::~IBLEnvironmentLoader() {
    glDeleteFramebuffers(1, &captureFBO);
    glCheckError();
    glDeleteRenderbuffers(1, &captureRBO);
    glCheckError();

    delete cube;
    cube = nullptr;
    delete ndcQuad;
    ndcQuad = nullptr;
}

TextureCube IBLEnvironmentLoader::GetCubemap(Texture2D* hdrTexture) {
    // save our initial viewport size

    GLint vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);
    glCheckError();

    // bind our frame buffer

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glCheckError();
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glCheckError();

    // render our hdr texture into an environment cubemap

    cube->SetMaterial(eqRectToCubemapMaterial);
    eqRectToCubemapMaterial->Queue();
    eqRectToCubemapMaterial->GetShader().SetInt("equirectangularMap", 0);

    TextureCube cubeMap = RenderTexCube(
        eqRectToCubemapMaterial->GetShader(),
        EnvMapSize, 
        [&]() { hdrTexture->Bind(0U); }
    );

    // now generate mipmaps to help reduce artifacts in pre-filter convolution
    cubeMap.Bind();
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glCheckError();
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    glCheckError();

    // unbind our framebuffer, and restore our initial viewport

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glCheckError();
    glViewport(vp[0],vp[1], vp[2], vp[3]);
    glCheckError();

    return cubeMap;
}

TextureCube IBLEnvironmentLoader::GetIrradianceMap(TextureCube* cubeMap) {
    // save our initial viewport size

    GLint vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);
    glCheckError();

    // bind our frame buffer

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glCheckError();
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glCheckError();

    // now convolute our environment map into a diffuse irradiance map

    cube->SetMaterial(irradianceConvolutionMaterial);
    TextureCube irradianceMap = RenderTexCube(
        irradianceConvolutionMaterial->GetShader(),
        IrradianceTexSize,
        [&]() { cubeMap->Bind(); }
    );

    // unbind our framebuffer, and restore our initial viewport

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glCheckError();
    glViewport(vp[0],vp[1], vp[2], vp[3]);
    glCheckError();

    return irradianceMap;
}

TextureCube IBLEnvironmentLoader::GetPrefilteredEnvMap(TextureCube* cubeMap) {
    // save our initial viewport size

    GLint vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);
    glCheckError();

    // bind our frame buffer

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glCheckError();
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glCheckError();

    // pre-filter our environment map into different roughness mip levels

    cube->SetMaterial(prefilterConvolutionMaterial);
    TextureCube prefilteredEnvMap = RenderPrefilteredTexCube(
        prefilterConvolutionMaterial->GetShader(),
        PrefilteredTexSize,
        PrefilterMipLevels,
        [&]() { cubeMap->Bind(); }
    );

    // unbind our framebuffer, and restore our initial viewport

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glCheckError();
    glViewport(vp[0],vp[1], vp[2], vp[3]);
    glCheckError();

    return prefilteredEnvMap;
}

Texture2D IBLEnvironmentLoader::GetBRDFLUT() {
    // save our initial viewport size

    GLint vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);
    glCheckError();

    // bind our frame buffer

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glCheckError();
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glCheckError();

    // generate the LUT

    Texture2D brdfLUT = PreComputeBRDFLUT(BRDFLUTSize);

    // unbind our framebuffer, and restore our initial viewport

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glCheckError();
    glViewport(vp[0],vp[1], vp[2], vp[3]);
    glCheckError();

    return brdfLUT;
}

TextureCube IBLEnvironmentLoader::RenderTexCube(const Shader& captureShader, unsigned int size, std::function<void()> setUniforms) {
    // resize our frame buffer

    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, size, size);
    glCheckError();

    // generate our cubemap color textures

    unsigned int texId;
    glGenTextures(1, &texId);
    glCheckError();
    glBindTexture(GL_TEXTURE_CUBE_MAP, texId);
    glCheckError();
    for(unsigned int i = 0; i < 6; i++) {
        // store each face with a floating point value
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, size, size, 0, GL_RGB, GL_FLOAT, nullptr);
        glCheckError();
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glCheckError();
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glCheckError();
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glCheckError();
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glCheckError();
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glCheckError();

    // prepare the shader and viewport
    
    captureShader.Use();
    captureShader.SetMat4("projection", captureProjection);
    if (setUniforms) {
        setUniforms();
    }
    
    glViewport(0, 0, size, size);
    glCheckError();
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glCheckError();

    // now capture the faces

    for(unsigned int i = 0; i < 6; i++) {
        captureShader.SetMat4("view", captureViews[i]);
        
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, texId, 0);
        glCheckError();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glCheckError();

        // finally, render the cube
        cube->Draw();
    }

    return TextureCube(texId, size, size);
}

TextureCube IBLEnvironmentLoader::RenderPrefilteredTexCube(const Shader& captureShader, unsigned int size, unsigned int maxMipLevels, std::function<void()> setUniforms) {
    // create our float cube texture with mip maps

    unsigned int texId;
    glGenTextures(1, &texId);
    glCheckError();
    glBindTexture(GL_TEXTURE_CUBE_MAP, texId);
    glCheckError();
    for (unsigned int i = 0; i < 6; ++i) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, size, size, 0, GL_RGB, GL_FLOAT, nullptr);
        glCheckError();
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glCheckError();
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glCheckError();
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glCheckError();
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
    glCheckError();
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glCheckError();

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    glCheckError();

    // prepare the shader and viewport

    captureShader.Use();
    captureShader.SetMat4("projection", captureProjection);
    if (setUniforms) {
        setUniforms();
    }

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glCheckError();

    // now capture the faces

    for(unsigned int mip = 0; mip < maxMipLevels; mip++) {
        // resize our frame buffer and viewport to the mip size

        unsigned int mipSize = size * std::pow(0.5, mip);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glCheckError();
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipSize, mipSize);
        glCheckError();
        glViewport(0, 0, mipSize, mipSize);
        glCheckError();

        // set our roughness level, and render all cube faces

        float roughness = (float)mip / (float)(maxMipLevels - 1);
        captureShader.SetFloat("roughness", roughness);
        captureShader.SetInt("mipLevel", mip);
        for(unsigned int i = 0; i < 6; i++) {
            captureShader.SetMat4("view", captureViews[i]);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, texId, mip);
            glCheckError();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glCheckError();

            // finally, render the cube
            cube->Draw();
        }
    }

    return TextureCube(texId, size, size);
}

Texture2D IBLEnvironmentLoader::PreComputeBRDFLUT(unsigned int size) {
    // resize our frame buffer

    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, size, size);

    // create our float LUT texture

    unsigned int brdfLUTTexture;
    glGenTextures(1, &brdfLUTTexture);
    glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, size, size, 0, GL_RG, GL_FLOAT, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // prepare the shader and viewport
    
    ndcQuad->GetMaterial()->Queue();
    
    glViewport(0, 0, size, size);
    
    // use the same framebuffer object and run this shader over an NDC screen-space quad

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUTTexture, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ndcQuad->Draw();

    return Texture2D(brdfLUTTexture, size, size, false);
}

} // namespace gyo
