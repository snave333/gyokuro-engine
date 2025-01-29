#ifndef UNLIT_MATERIAL_H
#define UNLIT_MATERIAL_H

#include <glm/glm.hpp>

#include <shading/Material.h>

namespace gyo {

class Texture2D;

class UnlitMaterial : public Material {
public:
    UnlitMaterial(
        glm::vec4 color = glm::vec4(1),
        bool additive = false,
        Texture2D* texture = nullptr,
        glm::vec2 uvTiling = glm::vec2(1),
        glm::vec2 uvOffset = glm::vec2(0)
    );
    ~UnlitMaterial() override;

    void Queue() override;

private:
    glm::vec4 color = glm::vec4(1);
    Texture2D* texture = nullptr;
    glm::vec2 uvTiling;
    glm::vec2 uvOffset;
};

} // namespace gyo

#endif // UNLIT_MATERIAL_H
