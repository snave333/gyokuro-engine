#ifndef PBR_MATERIAL_H
#define PBR_MATERIAL_H

#include <glm/glm.hpp>

#include <gyo/shading/Material.h>

namespace gyo {

class Texture2D;

class PBRMaterial : public Material {
public:
    PBRMaterial(
        bool useIBL = true,
        glm::vec3 albedo = glm::vec3(1),
        float metallic = 0,
        float roughness = 0.5f,
        float ao = 1,
        glm::vec3 emissive = glm::vec3(0),
        Texture2D* albedoMap = nullptr,
        Texture2D* normalMap = nullptr,
        Texture2D* metallicMap = nullptr,
        Texture2D* roughnessMap = nullptr,
        Texture2D* metallicRoughnessMap = nullptr,
        Texture2D* aoMap = nullptr,
        Texture2D* emissiveMap = nullptr,
        glm::vec2 uvTiling = glm::vec2(1),
        glm::vec2 uvOffset = glm::vec2(0)
    );
    ~PBRMaterial() override;

    void Queue() override;

private:
    glm::vec3 albedo;
    float metallic;
    float roughness;
    float ao;
    glm::vec3 emissive;

    bool hasTextures = false;

    Texture2D* albedoMap = nullptr;
    Texture2D* normalMap = nullptr;
    Texture2D* metallicMap = nullptr;
    Texture2D* roughnessMap = nullptr;
    Texture2D* metallicRoughnessMap = nullptr;
    Texture2D* aoMap = nullptr;
    Texture2D* emissiveMap = nullptr;
    glm::vec2 uvTiling;
    glm::vec2 uvOffset;
};

} // namespace gyo

#endif // PBR_MATERIAL_H
