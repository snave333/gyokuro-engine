
#include <gyo/shading/GoochMaterial.h>
#include <gyo/shading/Shader.h>
#include <gyo/shading/ShaderSemantics.h>
#include <gyo/resources/Resources.h>

namespace gyo {

GoochMaterial::GoochMaterial(glm::vec3 coolColor, glm::vec3 warmColor) {
    this->coolColor = coolColor;
    this->warmColor = warmColor;

    usesDirectLighting = true;

    shader = Resources::GetShader("default.vert", "gooch.frag");
    semantics = {
        { "aPos", SEMANTIC_POSITION },
        { "aNormal", SEMANTIC_NORMAL }
    };
}

GoochMaterial::~GoochMaterial() {
}

void GoochMaterial::Queue() {
    shader->Use();

    shader->SetVec3("coolColor", coolColor);
    shader->SetVec3("warmColor", warmColor);
}

} // namespace gyo
