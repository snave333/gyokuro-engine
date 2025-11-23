
#include <gyo/shading/Material.h>
#include <gyo/shading/Shader.h>
#include <gyo/shading/ShaderSemantics.h>

#include <iostream>

namespace gyo {

bool Material::ValidateShaderAttributes() {
    const std::map<std::string, AttributeInfo>& shaderAttributes = shader->GetAttributes();

    const std::unordered_map<unsigned int, GLenum> requiredTypes = {
        { SEMANTIC_POSITION,  GL_FLOAT_VEC3 },
        { SEMANTIC_NORMAL,    GL_FLOAT_VEC3 },
        { SEMANTIC_TEXCOORD0, GL_FLOAT_VEC2 },
        { SEMANTIC_TANGENT,   GL_FLOAT_VEC3 },
        { SEMANTIC_COLOR,     GL_FLOAT_VEC4 }
    };
    
    for (const auto& pair : semantics) {
        const std::string name = pair.first;
        unsigned int semantic = pair.second;

        auto attributeIt = shaderAttributes.find(name);
        if(attributeIt == shaderAttributes.end()) {
            std::cerr << "Error: semantic '" << name
                      << "' not found in compiled shader"
                      << std::endl;
            return false;
        }

        auto requiredTypeIt = requiredTypes.find(semantic);
        if(requiredTypeIt == requiredTypes.end()) {
            return false;
        }

        GLenum actualType = attributeIt->second.type;
        GLenum requiredType = requiredTypeIt->second;

        if(actualType != requiredType) {
            std::cerr << "Error: type mismatch for shader attribute '" << name
                      << "': expected " << requiredType
                      << ", got " << actualType
                      << std::endl;
            return false;
        }
    }

    return true;
}

} // namespace gyo
