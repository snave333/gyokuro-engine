#ifndef SHADER_H
#define SHADER_H

#include <map>
#include <set>

#include <glm/glm.hpp>
#include <glad/glad.h>

namespace gyo {

struct AttributeInfo {
    GLint location;
    GLenum type;
};

struct UniformInfo {
    GLint location;
    GLenum type;
};

class Shader {
public:
    Shader() {}
    Shader(const unsigned int& shaderProgramId,
        const std::set<std::string>& defines,
        const std::map<std::string, AttributeInfo>& attributes,
        const std::map<std::string, UniformInfo>& uniforms
    );
    
    void Dispose();

    // use/activate the shader
    void Use() const;
    
    // utility uniform functions
    void SetBool(const char* name, bool value) const;  
    void SetInt(const char* name, int value) const;   
    void SetFloat(const char* name, float value) const;
    void SetVec2(const char* name, glm::vec2 value) const;
    void SetVec3(const char* name, glm::vec3 value) const;
    void SetVec4(const char* name, glm::vec4 value) const;
    void SetMat4(const char* name, glm::mat4 value) const;
    // and for uniform blocks
    void SetUniformBlockBinding(const char* name, int bindingPoint) const;

private:
    // the program ID
    unsigned int ID;

    std::set<std::string> defines;
    std::map<std::string, AttributeInfo> attributes;
    std::map<std::string, UniformInfo> uniforms;

    bool HasUniform(const char* name) const;
    GLint GetUniformLocation(const char* name) const;
};
  
} // namespace gyo

#endif // SHADER_H
