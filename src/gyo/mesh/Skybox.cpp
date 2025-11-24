
#include <gyo/mesh/Skybox.h>
#include <gyo/geometry/Quad.h>
#include <gyo/mesh/Mesh.h>
#include <gyo/resources/Resources.h>
#include <gyo/shading/Shader.h>
#include <gyo/shading/ShaderMaterial.h>
#include <gyo/shading/ShaderSemantics.h>
#include <gyo/shading/Texture2D.h>
#include <gyo/shading/TextureCube.h>
#include <gyo/utilities/Clock.h>
#include <gyo/utilities/GetError.h>
#include <gyo/utilities/ImageWriter.h>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace gyo {

Skybox::Skybox(TextureCube* cubeMap) {
    this->cubeMap = cubeMap;

    CreateDefaultResources();
}

Skybox::Skybox(Texture2D* hdrTexture) {
    // creates vertex array object VAO for rendering a cube
    CreateDefaultResources();

    // first render our hdr texture into an environment cubemap

    unsigned int envMapSize = 1024U;
    Shader* eqRectToCubemapShader = Resources::GetShader("cubemap.vert", "eqRectToCubemap.frag");
    eqRectToCubemapShader->Use();
    eqRectToCubemapShader->SetInt("equirectangularMap", 0);
    cubeMap = RenderTexCube(eqRectToCubemapShader, envMapSize, [&]() {
        hdrTexture->Bind();
    });
    shouldDisposeCubemap = true;

    // now generate mipmaps to help reduce artifacts in pre-filter convolution
    cubeMap->Bind();
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glCheckError();
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    glCheckError();

    // now convolute our environment map into a diffuse irradiance map

    std::set<std::string> irradianceDefines = { "SAMPLE_DELTA 0.025" };
    Shader* irradianceConvolutionShader = Resources::GetShader(
        "cubemap.vert",
        "irradianceConvolution.frag",
        irradianceDefines
    );
    irradianceConvolutionShader->Use();
    irradianceConvolutionShader->SetInt("environmentMap", 0);
    {
        CLOCK(Irradiance_Convolution);
        irradianceMap = RenderTexCube(irradianceConvolutionShader, 64, [&]() {
            cubeMap->Bind();
        });
    }

    // next, pre-filter our environment map into different roughness mip levels

    std::set<std::string> prefilterDefines = { 
        "SAMPLE_COUNT 2048u",
        "ENV_MAP_RESOLUTION " + std::to_string(envMapSize) + ".0"
    };
    Shader* prefilterConvolutionShader = Resources::GetShader(
        "cubemap.vert",
        "prefilterConvolution.frag",
        prefilterDefines
    );
    prefilterConvolutionShader->Use();
    prefilterConvolutionShader->SetInt("environmentMap", 0);
    {
        CLOCK(Prefilter_Convolution);
        prefilteredEnvMap = RenderPrefilteredTexCube(prefilterConvolutionShader, 128, [&]() {
            cubeMap->Bind();
        });
    }

    // finally, pre-compute the BRDF LUT

    brdfLUT = PreComputeBRDFLUT(512);
}

void Skybox::CreateDefaultResources() {
    float vertices[] = {  
        // positions
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
    };

    // create the vertex/index buffers and vertex array object
    glGenVertexArrays(1, &VAO);
    glCheckError();
    glGenBuffers(1, &VBO);
    glCheckError();

    // bind Vertex Array Object first
    glBindVertexArray(VAO);
    glCheckError();

    // copy our vertices array in a buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glCheckError();
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
    glCheckError();

    // link the vertex attribute pointers
    // positions
    glEnableVertexAttribArray(0);
    glCheckError();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glCheckError();

    // clean up and unbind
    glBindVertexArray(0);
    glCheckError();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glCheckError();

    // now create and configure our shader
    shader = Resources::GetShader("skybox.vert", "skybox.frag");
    shader->Use();
    shader->SetInt("skybox", 0);
}

void Skybox::InitCaptureFramebuffer() {
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

    captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    captureViews[0] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f));
    captureViews[1] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f));
    captureViews[2] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f));
    captureViews[3] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f));
    captureViews[4] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f));
    captureViews[5] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f));

}

TextureCube* Skybox::RenderTexCube(const Shader* captureShader, unsigned int size, std::function<void()> setUniforms) {
    // save our initial viewport size

    GLint vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);
    glCheckError();
    
    // create frame buffer and render buffer objects for rendering into the cube faces
    if(!captureFBO) {
        InitCaptureFramebuffer();
    }

    // bind and resize our frame buffer

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glCheckError();
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glCheckError();

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
    
    captureShader->Use();
    captureShader->SetMat4("projection", captureProjection);
    if (setUniforms) {
        setUniforms();
    }
    
    glViewport(0, 0, size, size);
    glCheckError();
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glCheckError();

    // now capture the faces

    for(unsigned int i = 0; i < 6; i++) {
        captureShader->SetMat4("view", captureViews[i]);
        
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, texId, 0);
        glCheckError();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glCheckError();

        // finally, render the cube
        
        glBindVertexArray(VAO);
        glCheckError();
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glCheckError();
        glBindVertexArray(0);
        glCheckError();
    }

    // unbind our framebuffer, and restore our initial viewport

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glCheckError();
    glViewport(vp[0],vp[1], vp[2], vp[3]);
    glCheckError();

    return new TextureCube(texId, size, size);
}

TextureCube* Skybox::RenderPrefilteredTexCube(const Shader* captureShader, unsigned int size, std::function<void()> setUniforms) {
    // save our initial viewport size

    GLint vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);
    glCheckError();
    
    // create frame buffer and render buffer objects for rendering into the cube faces
    if(!captureFBO) {
        InitCaptureFramebuffer();
    }

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

    captureShader->Use();
    captureShader->SetMat4("projection", captureProjection);
    if (setUniforms) {
        setUniforms();
    }

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glCheckError();

    // now capture the faces

    unsigned int maxMipLevels = 5U;
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
        captureShader->SetFloat("roughness", roughness);
        for(unsigned int i = 0; i < 6; i++) {
            captureShader->SetMat4("view", captureViews[i]);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, texId, mip);
            glCheckError();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glCheckError();

            // finally, render the cube

            glBindVertexArray(VAO);
            glCheckError();
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glCheckError();
            glBindVertexArray(0);
            glCheckError();
        }
    }

    // unbind our framebuffer, and restore our initial viewport

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glCheckError();
    glViewport(vp[0],vp[1], vp[2], vp[3]);
    glCheckError();

    return new TextureCube(texId, size, size);
}

Texture2D* Skybox::PreComputeBRDFLUT(unsigned int size) {
    std::set<std::string> brdfDefines = { "SAMPLE_COUNT 1024u" };
    Mesh* mesh = new Mesh(new Quad(), new ShaderMaterial(
        Resources::GetShader(
            "screen.vert",
            "brdfConvolution.frag",
            brdfDefines
        ),
        {
            { "aPos", SEMANTIC_POSITION },
            { "aTexCoord", SEMANTIC_TEXCOORD0 }
        }
    ));

    // save our initial viewport size

    GLint vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);
    glCheckError();

    // bind and resize our frame buffer

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);

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
    
    mesh->GetMaterial()->Queue();
    
    glViewport(0, 0, size, size);
    
    // use the same framebuffer object and run this shader over an NDC screen-space quad

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUTTexture, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mesh->Draw();

    // unbind our framebuffer, and restore our initial viewport
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(vp[0],vp[1], vp[2], vp[3]);

    // clean up our resources, then return the texture

    delete mesh;

    return new Texture2D(brdfLUTTexture, size, size, false);
}

Skybox::~Skybox() {
    glDeleteVertexArrays(1, &VAO);
    glCheckError();
    glDeleteBuffers(1, &VBO);
    glCheckError();

    if(shouldDisposeCubemap) {
        cubeMap->Dispose();
        irradianceMap->Dispose();
        prefilteredEnvMap->Dispose();
        brdfLUT->Dispose();
    }

    cubeMap = nullptr;
    shader = nullptr;

    if(captureFBO) {
        // dispose of the resources we no longer need
        glDeleteFramebuffers(1, &captureFBO);
        glCheckError();
        glDeleteRenderbuffers(1, &captureRBO);
        glCheckError();
    }
}

void Skybox::Draw(glm::mat4 view, glm::mat4 projection) {
    // remove translation from the view matrix
    glm::mat4 viewNoTranslation = glm::mat4(glm::mat3(view));

    shader->Use();
    shader->SetMat4("view", viewNoTranslation);
    shader->SetMat4("projection", projection);

    glBindVertexArray(VAO);
    glCheckError();
    cubeMap->Bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glCheckError();
}

} // namespace gyo
