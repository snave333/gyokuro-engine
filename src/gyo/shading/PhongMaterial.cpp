
#include <gyo/shading/PhongMaterial.h>
#include <gyo/resources/Resources.h>
#include <gyo/shading/Texture2D.h>

namespace gyo {

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

    hasTextures = diffuseMap != nullptr ||
        specularMap != nullptr ||
        normalMap != nullptr;

    if(hasTextures) {
        std::set<std::string> defines = { "MATERIAL_TEXTURES" };
        shader = Resources::GetShader("default.vert", "phong.frag", defines);

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
        
        hasAlpha = hasAlpha || this->diffuseMap->hasAlpha;
    
        shader->Use();
        shader->SetInt("material.diffuseMap", 0);
        shader->SetInt("material.specularMap", 1);
        shader->SetInt("material.normalMap", 2);
    }
    else {
        shader = Resources::GetShader("default.vert", "phong.frag");
    }

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

    shader->SetVec4("material.diffuse", diffuse);
    shader->SetVec3("material.specular", specular);
    shader->SetFloat("material.shininess", shininess);
    
    if(hasTextures) {
        diffuseMap->Bind(0);
        specularMap->Bind(1);
        normalMap->Bind(2);
    
        shader->SetVec4("uvTilingOffset",
            glm::vec4(uvTiling.x, uvTiling.y, uvOffset.x, uvOffset.y));
    }
    else {
        Texture2D::UnbindTextureSlot(0);
        Texture2D::UnbindTextureSlot(1);
        Texture2D::UnbindTextureSlot(2);
    }
}

} // namespace gyo
