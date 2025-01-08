
#include <shading/PhongMaterial.h>
#include <resources/Resources.h>

PhongMaterial::PhongMaterial(
    glm::vec3 diffuse,
    glm::vec3 specular,
    float shininess
) {
    this->diffuse = diffuse;
    this->specular = specular;
    this->shininess = shininess;

    usesDirectLighting = true;

    shader = Resources::GetShader("default.vert", "phong.frag");
}

PhongMaterial::~PhongMaterial() {
}

void PhongMaterial::Queue() {
    shader->Use();

    shader->SetVec3("material.diffuse", diffuse);
    shader->SetVec3("material.specular", specular);
    shader->SetFloat("material.shininess", shininess);
}
