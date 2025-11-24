#ifndef SHADER_MATERIAL_H
#define SHADER_MATERIAL_H

#include <glm/glm.hpp>

#include <gyo/shading/Material.h>

namespace gyo {

class ShaderMaterial : public Material {
public:
    ShaderMaterial(Shader* shader, const std::map<std::string, unsigned int>& semantics);
    ~ShaderMaterial() override;

    void Queue() override;
};

} // namespace gyo

#endif // SHADER_MATERIAL_H
