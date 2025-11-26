
#include <gyo/shading/PBRMaterial.h>
#include <gyo/shading/ShaderSemantics.h>
#include <gyo/resources/Resources.h>

namespace gyo {

PBRMaterial::PBRMaterial(
    glm::vec3 albedo,
    float metallic,
    float roughness,
    float ao
) {
    this->albedo = albedo;
    this->metallic = metallic;
    this->roughness = roughness;
    this->ao = ao;

    usesDirectLighting = true;
    usesIBL = true;

    // compile a IBL version which includes irradiance sampler
    std::set<std::string> defines;
    if(usesIBL) {
        defines.insert("USE_IBL");
    }
    shader = Resources::GetShader("default.vert", "pbr.frag", defines);
    semantics = {
        { "aPos", SEMANTIC_POSITION },
        { "aNormal", SEMANTIC_NORMAL }
    };

    if(usesIBL) {
        shader->Use();
        shader->SetInt("irradianceMap", 0);
        shader->SetInt("prefilteredEnvMap", 1);
        shader->SetInt("brdfLUT", 2);
    }
}

PBRMaterial::~PBRMaterial() {
}

void PBRMaterial::Queue() {
    shader->Use();

    shader->SetVec3("material.albedo", albedo);
    shader->SetFloat("material.metallic", metallic);
    shader->SetFloat("material.roughness", roughness);
    shader->SetFloat("material.ao", ao);
}

} // namespace gyo
