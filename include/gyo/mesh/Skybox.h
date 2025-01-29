#ifndef SKYBOX_H
#define SKYBOX_H

#include <glm/glm.hpp>

namespace gyo {

class Shader;
class TextureCube;

class Skybox {
public:
    Skybox(TextureCube* cubeMap = nullptr);
    ~Skybox();

    void Draw(glm::mat4 view, glm::mat4 projection);

private:
    // render data
    unsigned int VAO;
    unsigned int VBO;

    TextureCube* cubeMap = nullptr;
    Shader* shader = nullptr;
};

} // namespace gyo

#endif // SKYBOX_H
