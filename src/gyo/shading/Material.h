#ifndef MATERIAL_H
#define MATERIAL_H

#include <gyo/shading/Shader.h>
#include <gyo/renderer/RenderType.h>

#include <glm/glm.hpp>

namespace gyo {

class Material {
public:
    virtual ~Material() {
        shader = nullptr;
    }

    virtual void Queue() = 0; // pure virtual

    bool ValidateShaderAttributes();
    
    const Shader& GetShader() const { return *shader; }
    const std::map<std::string, unsigned int>& GetShaderSemantics() const { return semantics; }

    RenderType renderType = RenderType::OPAQUE;
    bool usesDirectLighting = false; // include scene direct lighting
    bool usesIBL = false; // include diffuse irradiance map

protected:
    Shader* shader = nullptr;
    std::map<std::string, unsigned int> semantics;
};

} // namespace gyo

#endif // MATERIAL_H
