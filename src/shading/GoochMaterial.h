#ifndef GOOCH_MATERIAL_H
#define GOOCH_MATERIAL_H

#include <glm/glm.hpp>

#include <shading/Material.h>

namespace gyo {

class Texture2D;

class GoochMaterial : public Material {
public:
    GoochMaterial(
        glm::vec3 coolColor = glm::vec3(0.0f, 0.0f, 0.55f) + 0.25f,
        glm::vec3 warmColor = glm::vec3(0.3f, 0.3f, 0) + 0.25f
    );
    ~GoochMaterial() override;

    void Queue() override;

private:
    glm::vec3 coolColor;
    glm::vec3 warmColor;
};

} // namespace gyo

#endif // GOOCH_MATERIAL_H
