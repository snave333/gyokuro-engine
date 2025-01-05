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
        float shininess = 32);
    ~PhongMaterial() override;

    void Queue() override;

private:
    glm::vec3 diffuse;;
    glm::vec3 specular;
    float shininess;
};

#endif // PHONG_MATERIAL_H
