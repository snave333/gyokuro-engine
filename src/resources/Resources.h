#ifndef RESOURCES_H
#define RESOURCES_H

#include <map>
#include <shading/Shader.h>
#include <shading/Texture2D.h>
#include <ui/Font.h>

class Shader;
class Texture2D;
class Font;

enum ResourceType {
    SHADER,
    TEXTURE,
    FONT,
};

class Resources {
public:
    static void Initialize();
    static void Dispose();

    static Shader* GetShader(const char* vertexFileName, const char* fragmentFileName);
    static Texture2D* GetTexture(const char* imageFileName, bool srgb);
    static Font* GetFont(const char* fontFileName, unsigned int fontSize);

private:
    // our cached resources
    static std::map<unsigned int, Shader> shaders;
    static std::map<unsigned int, Texture2D> textures;
    static std::map<unsigned int, Font> fonts;

    static std::map<ResourceType, std::string> resourceTypeDirMap;

    static std::string GetShaderPath(const char* fileName);
    static std::string GetTexturePath(const char* fileName);
    static std::string GetFontPath(const char* fileName);
    static std::string GetResourcesDir(ResourceType resourceType);
};
  
#endif // RESOURCES_H
