#ifndef SHADER_LOADER_H
#define SHADER_LOADER_H

#include <string>
#include <unordered_set>

class Shader;

class ShaderLoader {
public:
    static std::string ResourceDir;
    static std::string IncludesDir;

    static Shader LoadShader(const char* vertexFileName, const char* fragmentFileName);

private:
    static std::string ResolveIncludes(
        const std::string& source,
        std::unordered_set<std::string>& includedFiles);
    static std::string ReadFile(std::string fileName, bool isInclude);
    static std::string ReadFilePath(std::string filePath);
};
  
#endif // SHADER_LOADER_H
