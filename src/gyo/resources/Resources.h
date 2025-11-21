#ifndef RESOURCES_H
#define RESOURCES_H

#include <map>
#include <set>

#include <glm/glm.hpp>
#include <glad/glad.h>

namespace gyo {

class Model;
class Shader;
class Texture2D;
class TextureCube;
class Font;

typedef std::vector<std::vector<std::string>> CSVData;

class Resources {
public:
    static void Initialize();
    static void Dispose();

    static Model* GetModel(const char* fileName, bool flipUVs);
    static Shader* GetShader(const char* vertFileName, const char* fragFileName, const std::set<std::string>& defines = {});
    static Shader* GetShader(const char* vertFileName, const char* geomFileName, const char* fragFileName, const std::set<std::string>& defines = { });
    static Texture2D* GetTexture(const char* imageFileName, bool srgb, int wrapMode = GL_REPEAT, bool useMipmaps = true);
    static Texture2D* GetHDRTexture(const char* imageFileName);
    static TextureCube* GetTextureCube(std::vector<const char*> faceFileNames, bool srgb);
    static Font* GetFont(const char* fontName, const float& pixelsPerEm, const float& pixelRange);
    static CSVData GetCSV(const char* filePath);

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
