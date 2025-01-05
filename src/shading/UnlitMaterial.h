#ifndef UNLIT_MATERIAL_H
#define UNLIT_MATERIAL_H

#include <glm/glm.hpp>

#include <shading/Material.h>

class Texture2D;

class UnlitMaterial : public Material {
public:
    UnlitMaterial(glm::vec4 color = glm::vec4(1), const char* imagePath = nullptr);
    ~UnlitMaterial() override;

    void Queue() override;

private:
    glm::vec4 color = glm::vec4(1);
    Texture2D* texture = nullptr;
};

#endif // UNLIT_MATERIAL_H
