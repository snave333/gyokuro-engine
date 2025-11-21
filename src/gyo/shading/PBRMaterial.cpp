
#include <gyo/shading/PBRMaterial.h>
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
    std::set<std::string> defines = { "USE_IBL" };
    shader = Resources::GetShader("default.vert", "pbr.frag", defines);
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
