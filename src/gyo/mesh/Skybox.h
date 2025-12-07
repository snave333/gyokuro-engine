#ifndef SKYBOX_H
#define SKYBOX_H

#include <glm/glm.hpp>

namespace gyo {

class Mesh;
class TextureCube;

class Skybox {
public:
    Skybox(TextureCube* cubeMap);
    ~Skybox();

    void Draw(glm::mat4 view, glm::mat4 projection) const;

private:
    TextureCube* cubeMap = nullptr;

    Mesh* mesh;
};

} // namespace gyo

#endif // SKYBOX_H
