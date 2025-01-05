#ifndef UNLIT_MATERIAL_H
#define UNLIT_MATERIAL_H

#include <glm/glm.hpp>

#include <shading/Material.h>

class Texture2D;

class UnlitMaterial : public Material {
public:
    UnlitMaterial(glm::vec3 color = glm::vec3(1), const char* imagePath = nullptr);
    ~UnlitMaterial() override;

    void Queue() override;

private:
    glm::vec3 color = glm::vec3(1);
    Texture2D* texture = nullptr;
};

#endif // UNLIT_MATERIAL_H
