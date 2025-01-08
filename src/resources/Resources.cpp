
#include <resources/Resources.h>
#include <resources/ShaderLoader.h>
#include <resources/TextureLoader.h>
#include <resources/FontLoader.h>
#include <utilities/FileSystem.h>
#include <utilities/Hash.h>

#include <glad/glad.h>

#include <iostream>

std::map<ResourceType, std::string> Resources::resourceTypeDirMap = {
    { SHADER, "shaders" },
    { TEXTURE, "textures" },
    { FONT, "fonts" },
};

std::map<unsigned int, Shader> Resources::shaders = {};
std::map<unsigned int, Texture2D> Resources::textures = {};
std::map<unsigned int, Font> Resources::fonts = {};

void Resources::Initialize() {
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

    for (auto& font : Resources::fonts) {
        font.second.Dispose();
    }
    Resources::fonts.clear();
}

Shader* Resources::GetShader(const char* vertexFileName, const char* fragmentFileName) {
    std::string hashKey = std::string(vertexFileName) + "|" + std::string(fragmentFileName);

    unsigned int id = HASH(hashKey);

    if(Resources::shaders.find(id) != Resources::shaders.end()) {
        std::cout << "Loading shaders " << hashKey << " from cache" << std::endl;
        return &Resources::shaders[id];
    }

    std::string vertexFilePath = Resources::GetShaderPath(vertexFileName);
    std::string fragmentFilePath = Resources::GetShaderPath(fragmentFileName);

    Shader shader = ShaderLoader::LoadShader(vertexFilePath, fragmentFilePath);

    Resources::shaders[id] = shader;

    return &Resources::shaders[id];
}

Texture2D* Resources::GetTexture(const char* imageFileName) {
    unsigned int id = HASH(imageFileName);

    if (Resources::textures.find(id) != Resources::textures.end()) {
        std::cout << "Loading texture " << imageFileName << " from cache" << std::endl;
        return &Resources::textures[id];
    }

    std::string fullImagePath = Resources::GetTexturePath(imageFileName);

    Texture2D texture = TextureLoader::LoadTexture(fullImagePath);

    Resources::textures[id] = texture;

    return &Resources::textures[id];
}

Font* Resources::GetFont(const char* fontFileName, unsigned int fontSize) {
    unsigned int id = HASH(fontFileName);

    if(Resources::fonts.find(id) != Resources::fonts.end()) {
        std::cout << "Loading font " << fontFileName << " from cache" << std::endl;
        return &Resources::fonts[id];
    }

    std::string fontFilePath = Resources::GetFontPath(fontFileName);

    Font font = FontLoader::LoadFont(fontFilePath, fontSize);

    Resources::fonts[id] = font;

    return &Resources::fonts[id];
}

std::string Resources::GetShaderPath(const char* fileName) {
    return FileSystem::CombinePath(GetResourcesDir(SHADER), std::string(fileName));
}

std::string Resources::GetTexturePath(const char* fileName) {
    return FileSystem::CombinePath(GetResourcesDir(TEXTURE), std::string(fileName));
}

std::string Resources::GetFontPath(const char* fileName) {
    return FileSystem::CombinePath(GetResourcesDir(FONT), std::string(fileName));
}

std::string Resources::GetResourcesDir(ResourceType resourceType) {
    std::string cwd = FileSystem::GetCurrentWorkingDirectory();
    std::string subDirName = Resources::resourceTypeDirMap[resourceType];

    return FileSystem::CombinePath(cwd, FileSystem::CombinePath("resources", subDirName));
}
