
#include <gyo/shading/ShaderMaterial.h>
#include <gyo/shading/Shader.h>

namespace gyo {

ShaderMaterial::ShaderMaterial(Shader* shader, const std::map<std::string, unsigned int>& semantics) {
    this->shader = shader;
    this->semantics = semantics;

    // TODO make these ctor parameters?
    usesDirectLighting = false;
    usesIBL = false;
}

ShaderMaterial::~ShaderMaterial() {
}

void ShaderMaterial::Queue() {
    shader->Use();
}

} // namespace gyo
