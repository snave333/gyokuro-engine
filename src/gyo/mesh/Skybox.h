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

    // render data
    unsigned int VAO;
    unsigned int VBO;

    TextureCube* cubeMap = nullptr;
    Shader* shader = nullptr;

    // normally, disposal is handled by Resources, unless we generated from a
    // hdr texture
    bool shouldDisposeCubemap = false;
};

} // namespace gyo

#endif // SKYBOX_H
