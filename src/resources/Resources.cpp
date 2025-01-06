
#include <resources/Resources.h>
#include <utilities/FileSystem.h>

std::map<ResourceType, std::string> Resources::resourceTypeDirMap = {
    { Shader, "shaders" },
    { Texture, "textures" },
    { Font, "fonts" },
    { Model, "models" },
};

std::string Resources::GetShaderPath(const char* fileName) {
    return FileSystem::CombinePath(GetResourcesDir(Shader), std::string(fileName));
}

std::string Resources::GetTexturePath(const char* fileName) {
    return FileSystem::CombinePath(GetResourcesDir(Texture), std::string(fileName));
}

std::string Resources::GetFontPath(const char* fileName) {
    return FileSystem::CombinePath(GetResourcesDir(Font), std::string(fileName));
}

std::string Resources::GetResourcesDir(ResourceType resourceType) {
    std::string cwd = FileSystem::GetCurrentWorkingDirectory();
    std::string subDirName = Resources::resourceTypeDirMap[resourceType];

    return FileSystem::CombinePath(cwd, FileSystem::CombinePath("resources", subDirName));
}
