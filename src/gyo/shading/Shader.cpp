
#include <gyo/shading/Shader.h>
#include <gyo/utilities/GetError.h>
#include <gyo/utilities/Log.h>

#include <iostream>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace gyo {

Shader::Shader(const unsigned int& shaderProgramId,
    const std::set<std::string>& defines,
    const std::map<std::string, AttributeInfo>& attributes,
    const std::map<std::string, UniformInfo>& uniforms
) {
    ID = shaderProgramId;
    this->defines = defines;
    this->attributes = attributes;
    this->uniforms = uniforms;
}

void Shader::Dispose() {
    glDeleteProgram(ID);
    glCheckError();
}

void Shader::Use() const {
    glUseProgram(ID);
    glCheckError();
}

void Shader::SetBool(const char* name, bool value) const {
    int location = GetUniformLocation(name);
    if(location == -1) {
        LOGW("Shader uniform '%s' not found", name);
        return;
    }
    glUniform1i(location, (int)value);
    glCheckError();
}

void Shader::SetInt(const char* name, int value) const {
    int location = GetUniformLocation(name);
    if(location == -1) {
        LOGW("Shader uniform '%s' not found", name);
        return;
    }
    glUniform1i(location, value);
    glCheckError();
}

void Shader::SetFloat(const char* name, float value) const {
    int location = GetUniformLocation(name);
    if(location == -1) {
        LOGW("Shader uniform '%s' not found", name);
        return;
    }
    glUniform1f(location, value);
    glCheckError();
}

void Shader::SetVec2(const char* name, glm::vec2 value) const {
    int location = GetUniformLocation(name);
    if(location == -1) {
        LOGW("Shader uniform '%s' not found", name);
        return;
    }
    glUniform2f(location, value.x, value.y);
    glCheckError();
}

void Shader::SetVec3(const char* name, glm::vec3 value) const {
    int location = GetUniformLocation(name);
    if(location == -1) {
        LOGW("Shader uniform '%s' not found", name);
        return;
    }
    glUniform3f(location, value.x, value.y, value.z);
    glCheckError();
}

void Shader::SetVec4(const char* name, glm::vec4 value) const {
    int location = GetUniformLocation(name);
    if(location == -1) {
        LOGW("Shader uniform '%s' not found", name);
        return;
    }
    glUniform4f(location, value.x, value.y, value.z, value.w);
    glCheckError();
}

void Shader::SetMat4(const char* name, glm::mat4 value) const {
    int location = GetUniformLocation(name);
    if(location == -1) {
        LOGW("Shader uniform '%s' not found", name);
        return;
    }
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    glCheckError();
}

void Shader::SetUniformBlockBinding(const char* name, int bindingPoint) const {
    int index = glGetUniformBlockIndex(ID, name);
    if(index == -1) {
        LOGW("Shader uniform block '%s' not found", name);
        return;
    }
    glUniformBlockBinding(ID, index, bindingPoint);
    glCheckError();
}

bool Shader::HasUniform(const char* name) const {
    return uniforms.find(name) != uniforms.end();
}

GLint Shader::GetUniformLocation(const char* name) const {
    if(uniforms.find(name) == uniforms.end()) {
        return -1;
    }

    return uniforms.at(name).location;
}

} // namespace gyo
