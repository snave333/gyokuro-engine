#ifndef SHADER_LOADER_H
#define SHADER_LOADER_H

#include <string>
#include <unordered_set>
#include <set>
#include <map>

namespace gyo {

class Shader;

class ShaderLoader {
public:
    static std::string ResourceDir;
    static std::string IncludesDir;
    static std::string VersionString;

    static Shader LoadShader(
        const char* vertFileName,
        const char* fragFileName,
        const std::set<std::string>& defines
    );
    static Shader LoadShader(
        const char* vertFileName,
        const char* geomFileName,
        const char* fragFileName,
        const std::set<std::string>& defines
    );

private:
    static std::string ResolveIncludes(
        const std::string& source,
        std::unordered_set<std::string>& includedFiles);
    
    static std::string ReadFile(std::string fileName, bool isInclude);
    static std::string ReadFilePath(std::string filePath);

    static void GetUniformLocations(unsigned int id, std::map<std::string, int>& uniforms);
};
  
} // namespace gyo

#endif // SHADER_LOADER_H
