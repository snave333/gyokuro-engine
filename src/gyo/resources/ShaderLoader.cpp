
#include <gyo/resources/ShaderLoader.h>
#include <gyo/shading/Shader.h>
#include <gyo/utilities/Clock.h>
#include <gyo/utilities/FileSystem.h>
#include <gyo/utilities/GetError.h>
#include <gyo/utilities/Log.h>

#include <fstream>
#include <sstream>
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
    LOGI("Compiling shaders %s & %s", vertFileName, fragFileName);
    CLOCK(Shader_Compilation);

    // read the base file contents
    std::string vShaderCodeStr = ReadFile(vertFileName, false);
    std::string fShaderCodeStr = ReadFile(fragFileName, false);

    // ensure there is no match for '#version', followed by number and optional profile
    std::regex versionPattern(R"(#version\s+\d+\s+\w*)");
    if( std::regex_search(vShaderCodeStr, versionPattern) ||
        std::regex_search(fShaderCodeStr, versionPattern)
    ) {
        LOGE("#version ...' should not be included in your glsl code");
    }

    // prepend the defines
    if(!defines.empty()) {
        std::string definesString = "";
        for(const std::string& define : defines) {
            definesString = definesString + std::string("#define ") + define + "\n";
            LOGD("Setting define '%s'", define.c_str());
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

    // LOGT("Final vert shader code:\n```\n%s\n```", vShaderCode);
    // LOGT("Final frag shader code:\n```\n%s\n```", fShaderCode);

    // compile shaders
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];
    
    // vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glCheckError();
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCheckError();
    glCompileShader(vertex);
    glCheckError();

    // print compile errors if any
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    glCheckError();
    if(!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        glCheckError();
        LOGE("Vertex shader '%s' compilation failed: %s", vertFileName, infoLog);
    };
    
    // similiar for Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glCheckError();
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCheckError();
    glCompileShader(fragment);
    glCheckError();

    // print compile errors if any
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    glCheckError();
    if(!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        glCheckError();
        LOGE("Fragment shader '%s' compilation failed: %s", fragFileName, infoLog);
    };
    
    // shader Program
    unsigned int id = glCreateProgram();
    glCheckError();
    glAttachShader(id, vertex);
    glCheckError();
    glAttachShader(id, fragment);
    glCheckError();
    glLinkProgram(id);
    glCheckError();

    // print linking errors if any
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    glCheckError();
    if(!success) {
        glGetProgramInfoLog(id, 512, NULL, infoLog);
        glCheckError();
        LOGE("Shader program linking failed: %s", infoLog);
    }
    
    // now save the uniforms for reduced gl calls later
    std::map<std::string, AttributeInfo> attributes;
    std::map<std::string, UniformInfo> uniforms;
    QueryShaderInfo(id, attributes, uniforms);
    PrintShaderInfo(attributes, uniforms);

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glCheckError();
    glDeleteShader(fragment);
    glCheckError();

    return Shader(id, defines, attributes, uniforms);
}

Shader ShaderLoader::LoadShader(
    const char* vertFileName,
    const char* geomFileName,
    const char* fragFileName,
    const std::set<std::string>& defines)
{
    LOGI("Compiling shaders %s, %s, & %s", vertFileName, geomFileName, fragFileName);
    
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
        LOGE("#version ...' should not be included in your glsl code");
    }

    // prepend the defines
    if(!defines.empty()) {
        std::string definesString = "";
        for(const std::string& define : defines) {
            definesString = definesString + std::string("#define ") + define + "\n";
            LOGD("Setting define '%s'", define.c_str());
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

    // LOGT("Final vert shader code:\n```\n%s\n```", vShaderCode);
    // LOGT("Final geom shader code:\n```\n%s\n```", gShaderCode);
    // LOGT("Final frag shader code:\n```\n%s\n```", fShaderCode);

    // compile shaders
    unsigned int vertex, geometry, fragment;
    int success;
    char infoLog[512];
    
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glCheckError();
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCheckError();
    glCompileShader(vertex);
    glCheckError();

    // print compile errors if any
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    glCheckError();
    if(!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        glCheckError();
        LOGE("Vertex shader '%s' compilation failed: %s", vertFileName, infoLog);
    };
    
    // geometry shader
    geometry = glCreateShader(GL_GEOMETRY_SHADER);
    glCheckError();
    glShaderSource(geometry, 1, &gShaderCode, NULL);
    glCheckError();
    glCompileShader(geometry);
    glCheckError();

    // print compile errors if any
    glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
    glCheckError();
    if(!success) {
        glGetShaderInfoLog(geometry, 512, NULL, infoLog);
        glCheckError();
        LOGE("Geometry shader '%s' compilation failed: %s", geomFileName, infoLog);
    };
    
    // finally, the fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glCheckError();
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCheckError();
    glCompileShader(fragment);
    glCheckError();

    // print compile errors if any
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    glCheckError();
    if(!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        glCheckError();
        LOGE("Fragment shader '%s' compilation failed: %s", fragFileName, infoLog);
    };
    
    // shader Program
    unsigned int id = glCreateProgram();
    glCheckError();
    glAttachShader(id, vertex);
    glCheckError();
    glAttachShader(id, geometry);
    glCheckError();
    glAttachShader(id, fragment);
    glCheckError();
    glLinkProgram(id);
    glCheckError();

    // print linking errors if any
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    glCheckError();
    if(!success) {
        glGetProgramInfoLog(id, 512, NULL, infoLog);
        glCheckError();
        LOGE("Shader program linking failed: %s", infoLog);
    }
    
    // now save the uniforms for reduced gl calls later
    std::map<std::string, AttributeInfo> attributes;
    std::map<std::string, UniformInfo> uniforms;
    QueryShaderInfo(id, attributes, uniforms);
    PrintShaderInfo(attributes, uniforms);

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glCheckError();
    glDeleteShader(geometry);
    glCheckError();
    glDeleteShader(fragment);
    glCheckError();

    return Shader(id, defines, attributes, uniforms);
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
                LOGE("Include file '%s' not found: %s", includeName.c_str(), e.what());
            }
        } else {
            LOGW("Circular include detected: %s", includeName.c_str());
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
        LOGE("Shader '%s' not successfully read: %s", filePath.c_str(), e.what());
    }

    return result;
}

void ShaderLoader::QueryShaderInfo(
    unsigned int id,
    std::map<std::string, AttributeInfo>& attributes,
    std::map<std::string, UniformInfo>& uniforms
) {
    // query our attributes

    GLint numAttributes;
    glGetProgramiv(id, GL_ACTIVE_ATTRIBUTES, &numAttributes);
    glCheckError();

    LOGD("Found %d attributes", numAttributes);

    GLint maxAttributeNameLength;
    glGetProgramiv(id, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxAttributeNameLength);
    glCheckError();

    char* nameBuffer = new char[maxAttributeNameLength];

    for (GLint i = 0; i < numAttributes; i++) {
        GLsizei length = 0;
        GLint size = 0;
        GLenum type = 0;

        glGetActiveAttrib(id, i, maxAttributeNameLength, &length, &size, &type, nameBuffer);
        glCheckError();

        std::string name(nameBuffer, length);

        GLint location = glGetAttribLocation(id, name.c_str());
        glCheckError();

        // LOGT("- %s, length: %d, size: %d, type: 0x%X", name.c_str(), length, size, type);

        attributes[name] = { location, type };
    }

    delete[] nameBuffer;

    // query our uniforms

    GLint numUniforms;
    glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &numUniforms);
    glCheckError();

    LOGD("Found %d uniforms", numUniforms);

    GLint maxUniformNameLength;
    glGetProgramiv(id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformNameLength);
    glCheckError();

    nameBuffer = new char[maxUniformNameLength];

    for(GLint i = 0; i < numUniforms; i++) {
        GLsizei length;
        GLint size;
        GLenum type;

        glGetActiveUniform(id, i, maxUniformNameLength, &length, &size, &type, nameBuffer);
        glCheckError();

        std::string name(nameBuffer, length);

        GLint location = glGetUniformLocation(id, name.c_str());
        glCheckError();

        // LOGT("- %s, length: %d, size: %d, type: 0x%X", name.c_str(), length, size, type);

        uniforms[name] = { location, type };
    }

    delete[] nameBuffer;
}

void ShaderLoader::PrintShaderInfo(
    const std::map<std::string, AttributeInfo>& attributes,
    const std::map<std::string, UniformInfo>& uniforms
) {
    LOGD("Attributes:");
    for (const auto& pair : attributes) {
        LOGD("- '%s', location: %d, type: 0x%X", pair.first.c_str(), pair.second.location, pair.second.type);
    }

    LOGD("Uniforms:");
    for (const auto& pair : uniforms) {
        std::ostringstream oss;
        oss << "- \'" << pair.first << "\'"
            << ", location: " << pair.second.location
            << ", type: 0x" << std::hex << pair.second.type << std::dec;
        LOGD("- '%s', location: %d, type: 0x%X", pair.first.c_str(), pair.second.location, pair.second.type);
    }
}


} // namespace gyo
