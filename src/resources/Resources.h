#ifndef RESOURCES_H
#define RESOURCES_H

#include <map>

enum ResourceType {
    Shader,
    Texture,
    Font,
    Model
};

class Resources {
public:
    static std::string GetShaderPath(const char* fileName);
    static std::string GetTexturePath(const char* fileName);
    static std::string GetFontPath(const char* fileName);

    /**
     * TODO provide static loading functions for multiple resource types
     * - Shader
     * - Texture2D
     * - Font
     * - Model
     * 
     * Cache loaded resources in maps, for easy reuse
     */

private:
    static std::string GetResourcesDir(ResourceType resourceType);

    static std::map<ResourceType, std::string> resourceTypeDirMap;
};
  
#endif // RESOURCES_H
