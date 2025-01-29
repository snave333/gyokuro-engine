#ifndef MATERIAL_H
#define MATERIAL_H

#include <gyo/internal/shading/Shader.h>
#include <gyo/internal/renderer/RenderType.h>

#include <glm/glm.hpp>

namespace gyo {

class Material {
public:
    virtual ~Material() {
        shader = nullptr;
    }

    virtual void Queue() = 0; // pure virtual
    
    const Shader& GetShader() const { return *shader; }

    RenderType renderType = RenderType::OPAQUE;
    bool usesDirectLighting = false; // include scene direct lighting

protected:
    Shader* shader = nullptr;
};

} // namespace gyo

#endif // MATERIAL_H
