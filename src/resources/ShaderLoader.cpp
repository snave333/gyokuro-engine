
#include <resources/ShaderLoader.h>
#include <shading/Shader.h>
#include <utilities/FileSystem.h>

#include <fstream>
#include <sstream>
#include <iostream>
#include <regex>

#include <glad/glad.h>

std::string ShaderLoader::ResourceDir = "";
std::string ShaderLoader::IncludesDir = "";

Shader ShaderLoader::LoadShader(const char* vertexFileName, const char* fragmentFileName) {
    // read the base file contents
    std::string vShaderCodeStr = ReadFile(vertexFileName, false);
    std::string fShaderCodeStr = ReadFile(fragmentFileName, false);

    // recursively resolve any #include instances, avoiding circular includes
    std::unordered_set<std::string> includedFiles;
    vShaderCodeStr = ResolveIncludes(vShaderCodeStr, includedFiles);
    includedFiles.clear();
    fShaderCodeStr = ResolveIncludes(fShaderCodeStr, includedFiles);

    // c strings for glad shader compilation
    const char* vShaderCode = vShaderCodeStr.c_str();
    const char* fShaderCode = fShaderCodeStr.c_str();

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
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED " << vertexFileName << "\n" << infoLog << std::endl;
    };
    
    // similiar for Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);

    // print compile errors if any
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED " << fragmentFileName << "\n" << infoLog << std::endl;
    };
    
    // shader Program
    unsigned int id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);

    // print linking errors if any
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(id, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    
    // now save the uniforms for reduced gl calls later
    std::map<std::string, int> uniforms;
    GetUniformLocations(id, uniforms);

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return Shader(id, uniforms);
}

std::string ShaderLoader::ResolveIncludes(
    const std::string& source,
    std::unordered_set<std::string>& includedFiles)
{
    std::stringstream result;

    // look for all instances of #include
    std::regex includeRegex(R"(#include\s*\"(.+?)\")");
    std::sregex_iterator it(source.begin(), source.end(), includeRegex);
    std::sregex_iterator end;

    std::size_t lastPos = 0;
    for (; it != end; ++it) {
        std::smatch match = *it;
        std::string includeName = match[1].str();

        // append the code before the include directive
        result << source.substr(lastPos, match.position() - lastPos);

        if (includedFiles.find(includeName) == includedFiles.end()) {
            includedFiles.insert(includeName);

            try {
                // read the included file using ReadFile
                std::string includeContent = ReadFile(includeName, true);

                // Recursively resolve includes in the included file
                result << ResolveIncludes(includeContent, includedFiles);
            } catch (const std::exception& e) {
                std::cerr << "ERROR::SHADER::INCLUDE_FILE_NOT_FOUND: " << includeName << "\n" << e.what() << std::endl;
            }
        } else {
            std::cerr << "WARNING::SHADER::CIRCULAR_INCLUDE_DETECTED: " << includeName << std::endl;
        }

        lastPos = match.position() + match.length();
    }

    // append the remaining code after the last include directive
    result << source.substr(lastPos);
    
    return result.str();
}

std::string ShaderLoader::ReadFile(std::string fileName, bool isInclude) {
    if(isInclude) {
        return ReadFilePath(FileSystem::CombinePath(IncludesDir, fileName));
    }
    else {
        return ReadFilePath(FileSystem::CombinePath(ResourceDir, fileName));
    }
}

std::string ShaderLoader::ReadFilePath(std::string filePath) {
    std::string result;
    std::ifstream fileStream;

    // ensure ifstream objects can throw exceptions:
    fileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        // open file
        fileStream.open(filePath.c_str());
        std::stringstream stringStream;

        // read file's buffer contents into stream
        stringStream << fileStream.rdbuf();

        // close file handler
        fileStream.close();

        // convert stream into string
        result = stringStream.str();
    }
    catch(std::ifstream::failure e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    return result;
}

void ShaderLoader::GetUniformLocations(unsigned int id, std::map<std::string, int>& uniforms) {
    int numUniforms;
    glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &numUniforms);

    int maxUniformNameLength;
    glGetProgramiv(id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformNameLength);

    char* nameBuffer = new char[maxUniformNameLength];

    for(int i = 0; i < numUniforms; i++) {
        int length;
        int size;
        unsigned int type;

        glGetActiveUniform(id, i, maxUniformNameLength, &length, &size, &type, nameBuffer);

        int location = glGetUniformLocation(id, nameBuffer);

        uniforms[std::string(nameBuffer, length)] = location;
    }

    delete[] nameBuffer;
}