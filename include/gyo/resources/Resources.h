#ifndef RESOURCES_H
#define RESOURCES_H

#include <map>
#include <unordered_set>

#include <glm/glm.hpp>

namespace gyo {

class Model;
class Shader;
class Texture2D;
class TextureCube;
class Font;

class Resources {
public:
    static void Initialize();
    static void Dispose();

    static Model* GetModel(const char* fileName, bool flipUVs);
    static Shader* GetShader(const char* vertFileName, const char* fragFileName, const std::unordered_set<std::string>& defines = {});
    static Shader* GetShader(const char* vertFileName, const char* geomFileName, const char* fragFileName, const std::unordered_set<std::string>& defines = { });
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
  
} // namespace gyo

#endif // RESOURCES_H
