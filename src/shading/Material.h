#ifndef MATERIAL_H
#define MATERIAL_H

#include <shading/Shader.h>
#include <renderer/RenderType.h>

#include <glm/glm.hpp>

class Material {
public:
    virtual ~Material() {
        delete shader;
        shader = nullptr;
    }

    virtual void Queue() = 0; // pure virtual
    
    const Shader& GetShader() const { return *shader; }

    RenderType renderType = Opaque;
    bool usesDirectLighting = false; // include scene direct lighting

protected:
    Shader* shader = nullptr;
};

#endif // MATERIAL_H