
#include <shading/PhongMaterial.h>
#include <resources/Resources.h>
#include <shading/Texture2D.h>

PhongMaterial::PhongMaterial(
    glm::vec4 diffuse,
    glm::vec3 specular,
    float shininess,
    Texture2D* diffuseMap,
    Texture2D* specularMap,
    Texture2D* normalMap,
    glm::vec2 uvTiling,
    glm::vec2 uvOffset
) {
    this->diffuse = diffuse;
    this->specular = specular;
    this->shininess = shininess;
    this->diffuseMap = diffuseMap;
    this->specularMap = specularMap;
    this->normalMap = normalMap;
    this->uvTiling = uvTiling;
    this->uvOffset = uvOffset;

    usesDirectLighting = true;

    bool hasAlpha = false;
    hasAlpha = hasAlpha || diffuse.a < 1.0f;

    shader = Resources::GetShader("default.vert", "phong.frag");

    // fallback to the built-in white textures from Resources
    if(diffuseMap == nullptr) {
        this->diffuseMap = Resources::GetTexture("BUILTIN_white", true);
    }
    if(specularMap == nullptr) {
        this->specularMap = Resources::GetTexture("BUILTIN_white", false);
    }
    if(normalMap == nullptr) {
        this->normalMap = Resources::GetTexture("BUILTIN_normal", false);
    }

    shader->Use();
    shader->SetInt("material.diffuseMap", 0);
    shader->SetInt("material.specularMap", 1);
    shader->SetInt("material.normalMap", 2);

    hasAlpha = hasAlpha || this->diffuseMap->hasAlpha;
    if(hasAlpha) {
        renderType = RenderType::TRANSPARENT;
    }
}

PhongMaterial::~PhongMaterial() {
    diffuseMap = nullptr;
    specularMap = nullptr;
    normalMap = nullptr;
}

void PhongMaterial::Queue() {
    shader->Use();

    diffuseMap->Bind(0);
    specularMap->Bind(1);
    normalMap->Bind(2);

    shader->SetVec4("material.diffuse", diffuse);
    shader->SetVec3("material.specular", specular);
    shader->SetFloat("material.shininess", shininess);
    shader->SetVec4("uvTilingOffset",
        glm::vec4(uvTiling.x, uvTiling.y, uvOffset.x, uvOffset.y));
}
