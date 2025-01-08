#ifndef SHADER_LOADER_H
#define SHADER_LOADER_H

#include <string>

class Shader;

class ShaderLoader {
public:
    static Shader LoadShader(std::string vertexFilePath, std::string fragmentFilePath);
};
  
#endif // SHADER_LOADER_H
