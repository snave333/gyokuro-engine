
#include <renderer/Shader.h>

#include <glad/glad.h>

#include <fstream>
#include <sstream>
#include <iostream>
#ifdef __APPLE__
#include <mach-o/dyld.h>
#else
#include <windows.h>
#endif

std::string GetCurrentWorkingDirectory() {
#ifdef __APPLE__
    char buffer[PATH_MAX];
    uint32_t size = PATH_MAX;
    if (_NSGetExecutablePath(buffer, &size) != 0) {
        throw std::runtime_error("Failed to retrieve executable path");
    }
    std::string fullPath(buffer);
    return fullPath.substr(0, fullPath.find_last_of("/")); // Remove executable name
#else
    char buffer[MAX_PATH];
    GetModuleFileName(NULL, buffer, MAX_PATH);
    std::string fullPath(buffer);
    return fullPath.substr(0, fullPath.find_last_of("\\/")); // Remove executable name
#endif
}

#ifdef __APPLE__
    const char PATH_SEPARATOR = '/';
#else
    const char PATH_SEPARATOR = '\\';
#endif

std::string CombinePath(const std::string& path1, const std::string& path2) {
    return path1 + PATH_SEPARATOR + path2;
}

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    // retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        // construct our full paths to the shader files
        std::string cwd = GetCurrentWorkingDirectory();
        std::string shaderDir = CombinePath(cwd, CombinePath("resources", "shaders"));
        std::string fullVertexPath = CombinePath(shaderDir, std::string(vertexPath));
        std::string fullFragmentPath = CombinePath(shaderDir, std::string(fragmentPath));

        // open files
        vShaderFile.open(fullVertexPath.c_str());
        fShaderFile.open(fullFragmentPath.c_str());
        std::stringstream vShaderStream, fShaderStream;

        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        // close file handlers
        vShaderFile.close();
        fShaderFile.close();

        // convert stream into string
        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();		
    }
    catch(std::ifstream::failure e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // compile shaders
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];
    
    // vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);

    // print compile errors if any
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    };
    
    // similiar for Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);

    // print compile errors if any
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    };
    
    // shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    // print linking errors if any
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    
    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader() {
    glDeleteProgram(ID);
}

void Shader::Use() {
    glUseProgram(ID);
}

void Shader::SetBool(const std::string &name, bool value) const {
    int location = glGetUniformLocation(ID, name.c_str());
    if(location == -1) {
        std::cout << "ERROR::SHADER::UNIFORM_NOT_FOUND: " << name << std::endl;
        return;
    }
    glUniform1i(location, (int)value);
}

void Shader::SetInt(const std::string &name, int value) const {
    int location = glGetUniformLocation(ID, name.c_str());
    if(location == -1) {
        std::cout << "ERROR::SHADER::UNIFORM_NOT_FOUND: " << name << std::endl;
        return;
    }
    glUniform1i(location, value);
}

void Shader::SetFloat(const std::string &name, float value) const {
    int location = glGetUniformLocation(ID, name.c_str());
    if(location == -1) {
        std::cout << "ERROR::SHADER::UNIFORM_NOT_FOUND: " << name << std::endl;
        return;
    }
    glUniform1f(location, value);
}

void Shader::SetFloat2(const std::string &name, float value1, float value2) const {
    int location = glGetUniformLocation(ID, name.c_str());
    if(location == -1) {
        std::cout << "ERROR::SHADER::UNIFORM_NOT_FOUND: " << name << std::endl;
        return;
    }
    glUniform2f(location, value1, value2);
}

void Shader::SetFloat3(const std::string &name, float value1, float value2, float value3) const {
    int location = glGetUniformLocation(ID, name.c_str());
    if(location == -1) {
        std::cout << "ERROR::SHADER::UNIFORM_NOT_FOUND: " << name << std::endl;
        return;
    }
    glUniform3f(location, value1, value2, value3);
}

void Shader::SetFloat4(const std::string &name, float value1, float value2, float value3, float value4) const {
    int location = glGetUniformLocation(ID, name.c_str());
    if(location == -1) {
        std::cout << "ERROR::SHADER::UNIFORM_NOT_FOUND: " << name << std::endl;
        return;
    }
    glUniform4f(location, value1, value2, value3, value4);
}
