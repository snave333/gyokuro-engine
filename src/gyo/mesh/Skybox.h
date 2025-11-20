#ifndef SKYBOX_H
#define SKYBOX_H

#include <glm/glm.hpp>

namespace gyo {

class Shader;
class TextureCube;
class Texture2D;

class Skybox {
public:
    Skybox(TextureCube* cubeMap);
    Skybox(Texture2D* hdrTexture);
    ~Skybox();

    void Draw(glm::mat4 view, glm::mat4 projection);

private:
    void CreateDefaultResources();
    TextureCube* RenderTexCube(const Shader* shader, unsigned int size, std::function<void()> setUniforms);

    // render data
    unsigned int VAO;
    unsigned int VBO;

    // framebuffer for cubemap rendering
    unsigned int captureFBO;
    unsigned int captureRBO;
    glm::mat4 captureProjection;
    glm::mat4 captureViews[6];

    TextureCube* cubeMap = nullptr;
    TextureCube* irradianceMap = nullptr;
    Shader* shader = nullptr;

    // normally, disposal is handled by Resources, unless we generated from a
    // hdr texture
    bool shouldDisposeCubemap = false;
};

} // namespace gyo

#endif // SKYBOX_H
