
#include <gyo/shading/Material.h>
#include <gyo/shading/Shader.h>
#include <gyo/shading/ShaderSemantics.h>
#include <gyo/utilities/Log.h>

#include <iostream>

namespace gyo {

bool Material::ValidateShaderAttributes() {
    const std::map<std::string, AttributeInfo>& shaderAttributes = shader->GetAttributes();

    for (const auto& pair : semantics) {
        const std::string name = pair.first;
        unsigned int semantic = pair.second;

        auto attributeIt = shaderAttributes.find(name);
        if(attributeIt == shaderAttributes.end()) {
            LOGE("Semantic '%s' not found in compiled shader", name.c_str());
            return false;
        }

        auto requiredTypeIt = SEMANTIC_TO_GLTYPE.find(semantic);
        if(requiredTypeIt == SEMANTIC_TO_GLTYPE.end()) {
            return false;
        }

        GLenum actualType = attributeIt->second.type;
        GLenum requiredType = requiredTypeIt->second;

        if(actualType != requiredType) {
            LOGE("Type mismatch for shader attribute '%s': expected %u but got %u", name.c_str(), requiredType, actualType);
            return false;
        }
    }

    return true;
}

} // namespace gyo
