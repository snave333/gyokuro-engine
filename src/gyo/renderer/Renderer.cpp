
#include <iostream>

#include <gyo/renderer/Renderer.h>
#include <gyo/renderer/ScreenQuad.h>
#include <gyo/renderer/DrawCall.h>
#include <gyo/mesh/Mesh.h>
#include <gyo/mesh/Skybox.h>
#include <gyo/shading/TextureCube.h>
#include <gyo/shading/Texture2D.h>
#include <gyo/utilities/GetError.h>

#include <glad/glad.h>

namespace gyo {

Renderer::Renderer(const int& width, const int& height, int msaaSamples) {
    size = glm::ivec2(width, height);

    PrintGLInfo();

    // validate our requested msaa sample count

    GLint maxMsaaSamples;
    glGetIntegerv(GL_MAX_SAMPLES, &maxMsaaSamples);
    glCheckError();
    this->msaaSamples = std::min(maxMsaaSamples, std::max(0, msaaSamples));

    // setup our render state

    state = RenderState();
    state.SetFaceCullingEnabled(true);
    state.SetDepthTestingEnabled(true);
    state.SetBlendingEnabled(false);

    CreateFrameBuffer();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glCheckError();
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    glCheckError();
}

Renderer::~Renderer() {
    delete screenQuad;
    screenQuad = nullptr;

    glDeleteFramebuffers(1, &framebuffer);
    glCheckError();

    if(msaaSamples > 0) {
        glDeleteTextures(1, &textureColorbufferMS);
        glCheckError();
        glDeleteRenderbuffers(1, &depthRenderbufferMS);
        glCheckError();

        glDeleteFramebuffers(1, &intermediateFramebuffer);
        glCheckError();
        glDeleteTextures(1, &textureColorbuffer);
        glCheckError();
    }
    else {
        glDeleteTextures(1, &textureColorbuffer);
        glCheckError();
        glDeleteRenderbuffers(1, &depthRenderbuffer);
        glCheckError();
    }
}

void Renderer::CreateFrameBuffer() {
    screenQuad = new ScreenQuad();

    // create and bind our main framebuffer
    glGenFramebuffers(1, &framebuffer);
    glCheckError();
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glCheckError();

    if(msaaSamples > 0) {
        // create our ms texture object and image
        glGenTextures(1, &textureColorbufferMS);
        glCheckError();
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureColorbufferMS);
        glCheckError();
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, msaaSamples, GL_RGB16F, size.x, size.y, GL_TRUE);
        glCheckError();
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
        glCheckError();
        
        // create a (also multisampled) renderbuffer object for depth and stencil attachments
        glGenRenderbuffers(1, &depthRenderbufferMS);
        glCheckError();
        glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbufferMS);
        glCheckError();
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, msaaSamples, GL_DEPTH24_STENCIL8, size.x, size.y);
        glCheckError();
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glCheckError();

        // attach color and depth/stencil buffers to currently bound framebuffer object
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureColorbufferMS, 0);
        glCheckError();
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthRenderbufferMS);
        glCheckError();

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "Error: framebuffer not complete" << std::endl;
        }
        glCheckError();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glCheckError();

        // now configure our secondary post-process framebuffer
        glGenFramebuffers(1, &intermediateFramebuffer);
        glCheckError();
        glBindFramebuffer(GL_FRAMEBUFFER, intermediateFramebuffer);
        glCheckError();

        // generate intermediate hdr color texture
        glGenTextures(1, &textureColorbuffer);
        glCheckError();
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
        glCheckError();
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, size.x, size.y, 0, GL_RGB, GL_FLOAT, NULL);
        glCheckError();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glCheckError();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glCheckError();
        glBindTexture(GL_TEXTURE_2D, 0);
        glCheckError();

        // attach color buffer (we don't need depth/stencil) to currently bound framebuffer object
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
        glCheckError();

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "Error: framebuffer not complete" << std::endl;
        }
        glCheckError();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glCheckError();
    }
    else {
        // generate hdr color texture
        glGenTextures(1, &textureColorbuffer);
        glCheckError();
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
        glCheckError();
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, size.x, size.y, 0, GL_RGB, GL_FLOAT, NULL);
        glCheckError();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glCheckError();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glCheckError();
        glBindTexture(GL_TEXTURE_2D, 0);
        glCheckError();
    
        // generate render buffer object for depth / stencil
        glGenRenderbuffers(1, &depthRenderbuffer);
        glCheckError();
        glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer); 
        glCheckError();
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size.x, size.y);
        glCheckError();
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glCheckError();
    
        // attach color and depth/stencil buffers to currently bound framebuffer object
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
        glCheckError();
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
        glCheckError();

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "Error: framebuffer not complete" << std::endl;
        }
        glCheckError();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glCheckError();
    }
}

void Renderer::PrintGLInfo() {
    std::cout << "Renderer Info & Capabilities: " << std::endl;

    // Query and print renderer and version information
    const GLubyte* renderer = glGetString(GL_RENDERER);  // GPU name
    glCheckError();
    const GLubyte* version = glGetString(GL_VERSION);    // OpenGL version
    glCheckError();
    const GLubyte* vendor = glGetString(GL_VENDOR);      // Vendor name
    glCheckError();
    const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION); // GLSL version
    glCheckError();

    std::cout << "- Renderer: " << renderer << "\n";
    std::cout << "- OpenGL Version: " << version << "\n";
    std::cout << "- Vendor: " << vendor << "\n";
    std::cout << "- GLSL Version: " << glslVersion << "\n";

    // Query and print OpenGL limits and capabilities
    GLint value;
    
    // Shader and Program Limits
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &value);
    glCheckError();
    std::cout << "- Max Vertex Attributes: " << value << "\n";

    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &value);
    glCheckError();
    std::cout << "- Max Vertex Uniform Components: " << value << "\n";

    glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &value);
    glCheckError();
    std::cout << "- Max Fragment Uniform Components: " << value << "\n";

    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &value);
    glCheckError();
    std::cout << "- Max Texture Image Units: " << value << "\n";

    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &value);
    glCheckError();
    std::cout << "- Max Combined Texture Image Units: " << value << "\n";

    // this triggers a INVALID_ENUM for some reason
    // glGetIntegerv(GL_MAX_VARYING_COMPONENTS, &value);
    // glCheckError();
    // std::cout << "- Max Varying Components: " << value << "\n";

    glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &value);
    glCheckError();
    std::cout << "- Max Uniform Block Size: " << value << " bytes\n";

    // Texture Capabilities
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &value);
    glCheckError();
    std::cout << "- Max 2D Texture Size: " << value << "x" << value << "\n";

    glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &value);
    glCheckError();
    std::cout << "- Max 3D Texture Size: " << value << "x" << value << "x" << value << "\n";

    glGetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE, &value);
    glCheckError();
    std::cout << "- Max Cube Map Texture Size: " << value << "x" << value << "\n";

    glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &value);
    glCheckError();
    std::cout << "- Max Array Texture Layers: " << value << "\n";

    // Framebuffer and Renderbuffer
    glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &value);
    glCheckError();
    std::cout << "- Max Renderbuffer Size: " << value << "x" << value << "\n";

    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &value);
    glCheckError();
    std::cout << "- Max Color Attachments: " << value << "\n";

    glGetIntegerv(GL_MAX_DRAW_BUFFERS, &value);
    glCheckError();
    std::cout << "- Max Draw Buffers: " << value << "\n";

    // MSAA Capabilities
    glGetIntegerv(GL_MAX_SAMPLES, &value);
    glCheckError();
    std::cout << "- Max MSAA Samples: " << value << "\n";
#if false
    // Compute Shader Limits (if supported)
    glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &value);
    std::cout << "- Max Compute Work Group Invocations: " << value << "\n";

    glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_COUNT, &value);
    std::cout << "- Max Compute Work Group Count (x, y, z): (" << value << ", " << value << ", " << value << ")\n";
#endif
    // Viewport and Clipping
    GLint viewportDims[2];
    glGetIntegerv(GL_MAX_VIEWPORT_DIMS, viewportDims);
    glCheckError();
    std::cout << "- Max Viewport Dimensions: " << viewportDims[0] << "x" << viewportDims[1] << "\n";

    glGetIntegerv(GL_MAX_CLIP_DISTANCES, &value);
    glCheckError();
    std::cout << "- Max Clip Distances: " << value << "\n";

    // Extensions
    glGetIntegerv(GL_NUM_EXTENSIONS, &value);
    std::cout << "- Number of Extensions: " << value << "\n";
    if(value != 0) {
        for (GLint i = 0; i < value; i++) {
            const char* extensionName = (const char*)glGetStringi(GL_EXTENSIONS, i);
            if (extensionName) {
                std::cout << "  " << i << ": " << extensionName << std::endl;
            }
        }
    }
}

void Renderer::BeginFrame() {
    // bind and clear our frame buffer

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glCheckError();
    glViewport(0, 0, size.x, size.y);
    glCheckError();
    glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
    glCheckError();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glCheckError();
}

void Renderer::RenderOpaque(std::vector<DrawCall> drawCalls, const IBLEnvironment& environment) {
    state.SetDepthTestingEnabled(true);
    state.SetBlendingEnabled(false);

    for(const DrawCall& dc : drawCalls) {
        dc.material->Queue();
        
        // bind the IBL maps for any IBL materials
        if(environment.irradianceMap != nullptr && dc.material->usesIBL) {
            environment.irradianceMap->Bind(0U);
            environment.prefilteredEnvMap->Bind(1U);
            environment.brdfLUT->Bind(2U);
        }
        
        // set any shader uniforms
        const Shader& shader = dc.material->GetShader();
        shader.SetMat4("model", dc.transform);
        shader.SetMat4("normalMatrix", dc.normalMatrix);

        dc.mesh->Draw();
    }
}

void Renderer::RenderSkybox(Skybox* skybox, glm::mat4 cameraView, glm::mat4 cameraProjection) {
    // Change depth function so depth test passes when values are equal to
    // depth buffer's content. Do this because we're setting the cubemap depth
    // value to 1.0 in the shader.
    state.SetDepthTestingEnabled(true, GL_LEQUAL);

    skybox->Draw(cameraView, cameraProjection);

    // set depth function back to default
    state.SetDepthTestingEnabled(true, GL_LESS);
}

void Renderer::RenderTransparent(std::vector<DrawCall> drawCalls) {
    state.SetDepthTestingEnabled(true);
    state.SetBlendingEnabled(true);

    // TODO first render back faces, then front faces?
    /*
    state.SetFaceCullingEnabled(true, GL_FRONT);

    for(const DrawCall& dc : drawCalls) {
        // only render the backfaces of transparent types
        if(dc.material->renderType == RenderType::ADDITIVE) {
            continue;
        }
        
        dc.material->Queue();
        const Shader& shader = dc.material->GetShader();

        // set any shader uniforms
        shader.SetMat4("model", dc.transform);
        shader.SetMat4("normalMatrix", dc.normalMatrix);

        // set the proper gl blend mode (no-op if already set)
        state.SetBlendingEnabled(true, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        dc.mesh->Draw();
    }

    state.SetFaceCullingEnabled(true, GL_BACK);
    */

    for(const DrawCall& dc : drawCalls) {
        dc.material->Queue();
        const Shader& shader = dc.material->GetShader();

        // set any shader uniforms
        shader.SetMat4("model", dc.transform);
        shader.SetMat4("normalMatrix", dc.normalMatrix);

        // set the proper gl blend mode
        if(dc.material->renderType == RenderType::TRANSPARENT) {
            state.SetBlendingEnabled(true, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
        else if(dc.material->renderType == RenderType::ADDITIVE) {
            state.SetBlendingEnabled(true, GL_SRC_ALPHA, GL_ONE);
        }

        dc.mesh->Draw();
    }
}

void Renderer::EndGeometryPass() {
    state.SetDepthTestingEnabled(false);
    state.SetBlendingEnabled(false);

    // copy the MS buffer to the normal colorbuffer of intermediate framebuffer
    if(msaaSamples > 0) {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediateFramebuffer);
        glBlitFramebuffer(0, 0, size.x, size.y, 0, 0, size.x, size.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    }

    // unbind our framebuffer, and render the full screen quad

    glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
    glCheckError();
    glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
    glCheckError();
    glClear(GL_COLOR_BUFFER_BIT);
    glCheckError();

    screenQuad->Draw(textureColorbuffer);
}

void Renderer::EndFrame() {
    // TODO do final tonemapping and gamma correction pass here?
}

} // namespace gyo
