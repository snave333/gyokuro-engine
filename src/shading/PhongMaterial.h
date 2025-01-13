#ifndef PHONG_MATERIAL_H
#define PHONG_MATERIAL_H

#include <glm/glm.hpp>

#include <shading/Material.h>

class Texture2D;

class PhongMaterial : public Material {
public:
    PhongMaterial(
        glm::vec3 diffuse = glm::vec3(0.5f),
        glm::vec3 specular = glm::vec3(1),
        float shininess = 128,
        Texture2D* diffuseMap = nullptr,
        Texture2D* specularMap = nullptr,
        Texture2D* normalMap = nullptr,
        glm::vec2 uvTiling = glm::vec2(1),
        glm::vec2 uvOffset = glm::vec2(0)
    );
    ~PhongMaterial() override;

    void Queue() override;

private:
    glm::vec3 diffuse;;
    glm::vec3 specular;
    float shininess;
    Texture2D* diffuseMap = nullptr;
    Texture2D* specularMap = nullptr;
    Texture2D* normalMap = nullptr;
    glm::vec2 uvTiling;
    glm::vec2 uvOffset;
};

#endif // PHONG_MATERIAL_H
