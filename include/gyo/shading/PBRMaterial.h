#ifndef PBR_MATERIAL_H
#define PBR_MATERIAL_H

#include <glm/glm.hpp>

#include <gyo/internal/shading/Material.h>

namespace gyo {

class PBRMaterial : public Material {
public:
    PBRMaterial(
        glm::vec3 albedo = glm::vec3(1),
        float metallic = 0,
        float roughness = 0.5f,
        float ao = 1
    );
    ~PBRMaterial() override;

    void Queue() override;

private:
    glm::vec3 albedo;
    float metallic;
    float roughness;
    float ao;
};

} // namespace gyo

#endif // PBR_MATERIAL_H
