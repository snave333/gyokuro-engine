
#include <gyo/mesh/Skybox.h>
#include <gyo/shading/Shader.h>
#include <gyo/resources/Resources.h>
#include <gyo/shading/Texture2D.h>
#include <gyo/shading/TextureCube.h>
#include <gyo/utilities/GetError.h>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace gyo {

Skybox::Skybox(TextureCube* cubeMap) {
    this->cubeMap = cubeMap;

    CreateDefaultResources();
}

Skybox::Skybox(Texture2D* hdrTexture) {
    CreateDefaultResources();

    // save our initial viewport size

    GLint vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);
    
    // create frame buffer and render buffer objects for rendering the hdr
    // equirectangular texture into 6 cube faces

    int size = 512;

    unsigned int captureFBO;
    unsigned int captureRBO;

    glGenFramebuffers(1, &captureFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    
    glGenRenderbuffers(1, &captureRBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, size, size);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

    // generate our cubemap color textures

    unsigned int envCubemap;
    glGenTextures(1, &envCubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
    for(unsigned int i = 0; i < 6; i++) {
        // store each face with a floating point value
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB32F, size, size, 0, GL_RGB, GL_FLOAT, nullptr);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // get our shader program for generating the faces

    Shader* equirectangularToCubemapShader = Resources::GetShader("eqRectToCubemap.vert", "eqRectToCubemap.frag");

    // now capture the faces

    glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    glm::mat4 captureViews[] = {
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
    };

    equirectangularToCubemapShader->Use();
    equirectangularToCubemapShader->SetInt("equirectangularMap", 0);
    equirectangularToCubemapShader->SetMat4("projection", captureProjection);
    
    hdrTexture->Bind();

    glViewport(0, 0, size, size);
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);

    for(unsigned int i = 0; i < 6; i++) {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        equirectangularToCubemapShader->SetMat4("view", captureViews[i]);
        
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // reset
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(vp[0],vp[1], vp[2], vp[3]);
    
    // dispose of the resources we no longer need
    glDeleteFramebuffers(1, &captureFBO);
    glDeleteRenderbuffers(1, &captureRBO);

    this->cubeMap = new TextureCube(envCubemap, size, size);
    this->shouldDisposeCubemap = true;
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

Skybox::~Skybox() {
    glDeleteVertexArrays(1, &VAO);
    glCheckError();
    glDeleteBuffers(1, &VBO);
    glCheckError();

    if(shouldDisposeCubemap) {
        cubeMap->Dispose();
    }

    cubeMap = nullptr;
    shader = nullptr;
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
