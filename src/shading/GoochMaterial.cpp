
#include <shading/GoochMaterial.h>
#include <shading/Shader.h>
#include <resources/Resources.h>

namespace gyo {

GoochMaterial::GoochMaterial(glm::vec3 coolColor, glm::vec3 warmColor) {
    this->coolColor = coolColor;
    this->warmColor = warmColor;

    usesDirectLighting = true;

    shader = Resources::GetShader("default.vert", "gooch.frag");
}

GoochMaterial::~GoochMaterial() {
}

void GoochMaterial::Queue() {
    shader->Use();

    shader->SetVec3("coolColor", coolColor);
    shader->SetVec3("warmColor", warmColor);
}

} // namespace gyo
