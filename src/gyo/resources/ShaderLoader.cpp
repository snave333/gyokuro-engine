
#include <gyo/resources/ShaderLoader.h>
#include <gyo/shading/Shader.h>
#include <gyo/utilities/FileSystem.h>

#include <fstream>
#include <sstream>
#include <iostream>
#include <regex>

#include <glad/glad.h>

namespace gyo {

std::string ShaderLoader::ResourceDir = "";
std::string ShaderLoader::IncludesDir = "";
std::string ShaderLoader::VersionString = "#version 330 core\n";

Shader ShaderLoader::LoadShader(
    const char* vertFileName,
    const char* fragFileName,
    const std::set<std::string>& defines)
{
    std::cout << "Compiling shaders " << vertFileName << " & " << fragFileName << std::endl;

    // read the base file contents
    std::string vShaderCodeStr = ReadFile(vertFileName, false);
    std::string fShaderCodeStr = ReadFile(fragFileName, false);

    // ensure there is no match for '#version', followed by number and optional profile
    std::regex versionPattern(R"(#version\s+\d+\s+\w*)");
    if(std::regex_search(vShaderCodeStr, versionPattern) ||
        std::regex_search(fShaderCodeStr, versionPattern))
    {
        std::cout << "ERROR: '#version ...' should not be included in your glsl code" << std::endl;
    }

    // prepend the defines
    if(!defines.empty()) {
        std::string definesString = "";
        for(const std::string& define : defines) {
            definesString = definesString + std::string("#define ") + define + "\n";
        }
        
        vShaderCodeStr = definesString + vShaderCodeStr;
        fShaderCodeStr = definesString + fShaderCodeStr;
    }

    // prepend the #version
    vShaderCodeStr = VersionString + vShaderCodeStr;
    fShaderCodeStr = VersionString + fShaderCodeStr;

    // recursively resolve any #include instances, avoiding circular includes
    std::unordered_set<std::string> includedFiles;
    vShaderCodeStr = ResolveIncludes(vShaderCodeStr, includedFiles);
    includedFiles.clear();
    fShaderCodeStr = ResolveIncludes(fShaderCodeStr, includedFiles);

    // c strings for glad shader compilation
    const char* vShaderCode = vShaderCodeStr.c_str();
    const char* fShaderCode = fShaderCodeStr.c_str();

    // std::cout << "Final vertex shader code:\n" << vShaderCode << std::endl;
    // std::cout << "Final fragment shader code:\n" << fShaderCode << std::endl;

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
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED " << vertFileName << "\n" << infoLog << std::endl;
    };
    
    // similiar for Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);

    // print compile errors if any
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED " << fragFileName << "\n" << infoLog << std::endl;
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

Shader ShaderLoader::LoadShader(
    const char* vertFileName,
    const char* geomFileName,
    const char* fragFileName,
    const std::set<std::string>& defines)
{
    std::cout << "Compiling shaders " << vertFileName << ", " << geomFileName << ", & " << fragFileName << std::endl;
    
    // read the base file contents
    std::string vShaderCodeStr = ReadFile(vertFileName, false);
    std::string gShaderCodeStr = ReadFile(geomFileName, false);
    std::string fShaderCodeStr = ReadFile(fragFileName, false);

    // ensure there is no match for '#version', followed by number and optional profile
    std::regex versionPattern(R"(#version\s+\d+\s+\w*)");
    if(std::regex_search(vShaderCodeStr, versionPattern) ||
        std::regex_search(gShaderCodeStr, versionPattern) ||
        std::regex_search(fShaderCodeStr, versionPattern))
    {
        std::cout << "ERROR: '#version ...' should not be included in your glsl code" << std::endl;
    }

    // prepend the defines
    if(!defines.empty()) {
        std::string definesString = "";
        for(const std::string& define : defines) {
            definesString = definesString + std::string("#define ") + define + "\n";
        }
        
        vShaderCodeStr = definesString + vShaderCodeStr;
        gShaderCodeStr = definesString + gShaderCodeStr;
        fShaderCodeStr = definesString + fShaderCodeStr;
    }

    // prepend the #version
    vShaderCodeStr = VersionString + vShaderCodeStr;
    gShaderCodeStr = VersionString + gShaderCodeStr;
    fShaderCodeStr = VersionString + fShaderCodeStr;

    // recursively resolve any #include instances, avoiding circular includes
    std::unordered_set<std::string> includedFiles;
    vShaderCodeStr = ResolveIncludes(vShaderCodeStr, includedFiles);
    includedFiles.clear();
    gShaderCodeStr = ResolveIncludes(gShaderCodeStr, includedFiles);
    includedFiles.clear();
    fShaderCodeStr = ResolveIncludes(fShaderCodeStr, includedFiles);

    // c strings for glad shader compilation
    const char* vShaderCode = vShaderCodeStr.c_str();
    const char* gShaderCode = gShaderCodeStr.c_str();
    const char* fShaderCode = fShaderCodeStr.c_str();

    // std::cout << "Final vertex shader code:\n" << vShaderCode << std::endl;
    // std::cout << "Final geometry shader code:\n" << gShaderCode << std::endl;
    // std::cout << "Final fragment shader code:\n" << fShaderCode << std::endl;

    // compile shaders
    unsigned int vertex, geometry, fragment;
    int success;
    char infoLog[512];
    
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);

    // print compile errors if any
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED " << vertFileName << "\n" << infoLog << std::endl;
    };
    
    // geometry shader
    geometry = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geometry, 1, &gShaderCode, NULL);
    glCompileShader(geometry);

    // print compile errors if any
    glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(geometry, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED " << geomFileName << "\n" << infoLog << std::endl;
    };
    
    // finally, the fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);

    // print compile errors if any
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED " << fragFileName << "\n" << infoLog << std::endl;
    };
    
    // shader Program
    unsigned int id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, geometry);
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
    glDeleteShader(geometry);
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

    std::cout << "Found " << std::to_string(numUniforms) << " uniforms:" << std::endl;

    int maxUniformNameLength;
    glGetProgramiv(id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformNameLength);

    char* nameBuffer = new char[maxUniformNameLength];

    for(int i = 0; i < numUniforms; i++) {
        int length;
        int size;
        unsigned int type;

        glGetActiveUniform(id, i, maxUniformNameLength, &length, &size, &type, nameBuffer);

        int location = glGetUniformLocation(id, nameBuffer);

        std::cout << "- " << std::string(nameBuffer, length) << std::endl;

        uniforms[std::string(nameBuffer, length)] = location;
    }

    delete[] nameBuffer;
}

} // namespace gyo
