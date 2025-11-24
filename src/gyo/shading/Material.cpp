
#include <gyo/shading/Material.h>
#include <gyo/shading/Shader.h>
#include <gyo/shading/ShaderSemantics.h>

#include <iostream>

namespace gyo {

bool Material::ValidateShaderAttributes() {
    const std::map<std::string, AttributeInfo>& shaderAttributes = shader->GetAttributes();

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

        auto requiredTypeIt = SEMANTIC_TO_GLTYPE.find(semantic);
        if(requiredTypeIt == SEMANTIC_TO_GLTYPE.end()) {
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
