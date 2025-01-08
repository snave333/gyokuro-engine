
#include <shading/Shader.h>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

void Shader::Dispose() {
    glDeleteProgram(ID);
}

void Shader::Use() const {
    glUseProgram(ID);
}

void Shader::SetBool(const char* name, bool value) const {
    int location = glGetUniformLocation(ID, name);
    if(location == -1) {
        std::cout << "ERROR::SHADER::UNIFORM_NOT_FOUND: " << name << std::endl;
        return;
    }
    glUniform1i(location, (int)value);
}

void Shader::SetInt(const char* name, int value) const {
    int location = glGetUniformLocation(ID, name);
    if(location == -1) {
        std::cout << "ERROR::SHADER::UNIFORM_NOT_FOUND: " << name << std::endl;
        return;
    }
    glUniform1i(location, value);
}

void Shader::SetFloat(const char* name, float value) const {
    int location = glGetUniformLocation(ID, name);
    if(location == -1) {
        std::cout << "ERROR::SHADER::UNIFORM_NOT_FOUND: " << name << std::endl;
        return;
    }
    glUniform1f(location, value);
}

void Shader::SetVec2(const char* name, glm::vec2 value) const {
    int location = glGetUniformLocation(ID, name);
    if(location == -1) {
        std::cout << "ERROR::SHADER::UNIFORM_NOT_FOUND: " << name << std::endl;
        return;
    }
    glUniform2f(location, value.x, value.y);
}

void Shader::SetVec3(const char* name, glm::vec3 value) const {
    int location = glGetUniformLocation(ID, name);
    if(location == -1) {
        std::cout << "ERROR::SHADER::UNIFORM_NOT_FOUND: " << name << std::endl;
        return;
    }
    glUniform3f(location, value.x, value.y, value.z);
}

void Shader::SetVec4(const char* name, glm::vec4 value) const {
    int location = glGetUniformLocation(ID, name);
    if(location == -1) {
        std::cout << "ERROR::SHADER::UNIFORM_NOT_FOUND: " << name << std::endl;
        return;
    }
    glUniform4f(location, value.x, value.y, value.z, value.w);
}

void Shader::SetMat4(const char* name, glm::mat4 value) const {
    int location = glGetUniformLocation(ID, name);
    if(location == -1) {
        std::cout << "ERROR::SHADER::UNIFORM_NOT_FOUND: " << name << std::endl;
        return;
    }
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetUniformBlockBinding(const char* name, int bindingPoint) const {
    int index = glGetUniformBlockIndex(ID, name);
    if(index == -1) {
        std::cout << "ERROR::SHADER::UNIFORM_BLOCK_NOT_FOUND: " << name << std::endl;
        return;
    }
    glUniformBlockBinding(ID, index, bindingPoint);
}
