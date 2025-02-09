
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

    shader = Resources::GetShader("default.vert", "pbr.frag");
}

PBRMaterial::~PBRMaterial() {
}

void PBRMaterial::Queue() {
    shader->Use();

    shader->SetVec3("albedo", albedo);
    shader->SetFloat("metallic", metallic);
    shader->SetFloat("roughness", roughness);
    shader->SetFloat("ao", ao);
}

} // namespace gyo
