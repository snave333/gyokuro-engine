#ifndef RESOURCES_H
#define RESOURCES_H

#include <map>

#include <glm/glm.hpp>

class Shader;
class Texture2D;
class TextureCube;
class Font;

class Resources {
public:
    static void Initialize();
    static void Dispose();

    static Shader* GetShader(const char* vertFileName, const char* fragFileName);
    static Shader* GetShader(const char* vertFileName, const char* geomFileName, const char* fragFileName);
    static Texture2D* GetTexture(const char* imageFileName, bool srgb);
    static TextureCube* GetTextureCube(std::vector<const char*> faceFileNames, bool srgb);
    static Font* GetFont(const char* fontFileName, unsigned int fontSize);

private:
    // our cached resources
    static std::map<long, Shader> shaders;
    static std::map<long, Texture2D> textures;
    static std::map<long, TextureCube> cubeMaps;
    static std::map<long, Font> fonts;

    static Texture2D GenerateBuiltInTexture(glm::vec4 color);
};
  
#endif // RESOURCES_H
