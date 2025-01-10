
#include <shading/PhongMaterial.h>
#include <resources/Resources.h>
#include <shading/Texture2D.h>

PhongMaterial::PhongMaterial(
    glm::vec3 diffuse,
    glm::vec3 specular,
    float shininess,
    Texture2D* diffuseMap,
    Texture2D* specularMap
) {
    this->diffuse = diffuse;
    this->specular = specular;
    this->shininess = shininess;
    this->diffuseMap = diffuseMap;
    this->specularMap = specularMap;

    usesDirectLighting = true;

    shader = Resources::GetShader("default.vert", "phong.frag");

    // fallback to the built-in white textures from Resources
    if(diffuseMap == nullptr) {
        this->diffuseMap = Resources::GetTexture("BUILTIN_white", true);
    }
    if(specularMap == nullptr) {
        this->specularMap = Resources::GetTexture("BUILTIN_white", true);
    }

    shader->Use();
    shader->SetInt("material.diffuseMap", 0);
    shader->SetInt("material.specularMap", 1);
}

PhongMaterial::~PhongMaterial() {
}

void PhongMaterial::Queue() {
    shader->Use();

    diffuseMap->Bind(0);
    specularMap->Bind(1);

    shader->SetVec3("material.diffuse", diffuse);
    shader->SetVec3("material.specular", specular);
    shader->SetFloat("material.shininess", shininess);
}
