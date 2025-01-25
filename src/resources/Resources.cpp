
#include <resources/Resources.h>
#include <resources/ShaderLoader.h>
#include <resources/TextureLoader.h>
#include <resources/FontLoader.h>
#include <utilities/FileSystem.h>
#include <utilities/Hash.h>

#include <shading/Shader.h>
#include <shading/Texture2D.h>
#include <shading/TextureCube.h>
#include <ui/Font.h>

#include <glad/glad.h>

std::map<long, Shader> Resources::shaders = {};
std::map<long, Texture2D> Resources::textures = {};
std::map<long, TextureCube> Resources::cubeMaps = {};
std::map<long, Font> Resources::fonts = {};

void Resources::Initialize() {
    // set the directory paths of our resource loaders

    std::string cwd = FileSystem::GetCurrentWorkingDirectory();

    ShaderLoader::ResourceDir = FileSystem::CombinePath(cwd, "resources", "shaders");
    ShaderLoader::IncludesDir = FileSystem::CombinePath(cwd, "resources", "shaders", "include");
    TextureLoader::ResourceDir = FileSystem::CombinePath(cwd, "resources", "textures");
    FontLoader::ResourceDir = FileSystem::CombinePath(cwd, "resources", "fonts");

    // generate default textures

    long id;
    glm::vec4 color;

    // a 1 pixel white texture
    id = HASH("BUILTIN_white");
    color = { 1, 1, 1, 1 };
    Resources::textures[id] = Resources::GenerateBuiltInTexture(color);

    // default normal texture
    id = HASH("BUILTIN_normal");
    color = { 0.5f, 0.5f, 1, 1 };
    Resources::textures[id] = Resources::GenerateBuiltInTexture(color);
}

Texture2D Resources::GenerateBuiltInTexture(glm::vec4 color) {
    unsigned char pixels[4] = {
        static_cast<unsigned char>(color.r * 255.0f),
        static_cast<unsigned char>(color.g * 255.0f),
        static_cast<unsigned char>(color.b * 255.0f),
        static_cast<unsigned char>(color.a * 255.0f),
    };
    return TextureLoader::GenerateTexture2D(1, 1, GL_RGBA, pixels);
}

void Resources::Dispose() {
    for (auto& shader : Resources::shaders) {
        shader.second.Dispose();
    }
    Resources::shaders.clear();

    for (auto& texture : Resources::textures) {
        texture.second.Dispose();
    }
    Resources::textures.clear();

    for (auto& texture : Resources::cubeMaps) {
        texture.second.Dispose();
    }
    Resources::cubeMaps.clear();

    for (auto& font : Resources::fonts) {
        font.second.Dispose();
    }
    Resources::fonts.clear();
}

Shader* Resources::GetShader(const char* vertFileName, const char* fragFileName) {
    std::string hashKey = std::string(vertFileName) + "|" + std::string(fragFileName);

    long id = HASH(hashKey);

    if(Resources::shaders.find(id) != Resources::shaders.end()) {
        return &Resources::shaders[id];
    }

    Shader shader = ShaderLoader::LoadShader(vertFileName, fragFileName);

    Resources::shaders[id] = shader;

    return &Resources::shaders[id];
}

Shader* Resources::GetShader(const char* vertFileName, const char* geomFileName, const char* fragFileName) {
    std::string hashKey = 
        std::string(vertFileName) + "|" +
        std::string(geomFileName) + "|" +
        std::string(fragFileName);

    long id = HASH(hashKey);

    if(Resources::shaders.find(id) != Resources::shaders.end()) {
        return &Resources::shaders[id];
    }

    Shader shader = ShaderLoader::LoadShader(vertFileName, geomFileName, fragFileName);

    Resources::shaders[id] = shader;

    return &Resources::shaders[id];
}

Texture2D* Resources::GetTexture(const char* imageFileName, bool srgb) {
    long id = HASH(imageFileName);

    if (Resources::textures.find(id) != Resources::textures.end()) {
        return &Resources::textures[id];
    }

    Texture2D texture = TextureLoader::LoadTexture(imageFileName, srgb);

    Resources::textures[id] = texture;

    return &Resources::textures[id];
}

TextureCube* Resources::GetTextureCube(std::vector<const char*> faceFileNames, bool srgb) {
    if(faceFileNames.size() != 6) {
        throw std::runtime_error("Cannot load cubmap without 6 faces");
    }

    long id = HASH(faceFileNames[0]);
    
    if (Resources::cubeMaps.find(id) != Resources::cubeMaps.end()) {
        return &Resources::cubeMaps[id];
    }

    TextureCube texture = TextureLoader::LoadTextureCube(faceFileNames, srgb);

    Resources::cubeMaps[id] = texture;

    return &Resources::cubeMaps[id];
}

Font* Resources::GetFont(const char* fontFileName, unsigned int fontSize) {
    long id = HASH(fontFileName);

    if(Resources::fonts.find(id) != Resources::fonts.end()) {
        return &Resources::fonts[id];
    }

    Font font = FontLoader::LoadFont(fontFileName, fontSize);

    Resources::fonts[id] = font;

    return &Resources::fonts[id];
}
