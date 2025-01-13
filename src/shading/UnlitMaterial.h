#ifndef UNLIT_MATERIAL_H
#define UNLIT_MATERIAL_H

#include <glm/glm.hpp>

#include <shading/Material.h>

class Texture2D;

class UnlitMaterial : public Material {
public:
    UnlitMaterial(
        glm::vec3 color = glm::vec3(1),
        Texture2D* texture = nullptr,
        glm::vec2 uvTiling = glm::vec2(1),
        glm::vec2 uvOffset = glm::vec2(0)
    );
    ~UnlitMaterial() override;

    void Queue() override;

private:
    glm::vec3 color = glm::vec3(1);
    Texture2D* texture = nullptr;
    glm::vec2 uvTiling;
    glm::vec2 uvOffset;
};

#endif // UNLIT_MATERIAL_H
