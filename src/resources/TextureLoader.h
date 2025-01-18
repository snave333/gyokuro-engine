#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H

#include <string>

class Texture2D;
class TextureCube;

class TextureLoader {
public:
    static Texture2D LoadTexture(std::string imageFilePath, bool srgb);
    static TextureCube LoadTextureCube(std::vector<std::string> faceFilePaths, bool srgb);
    static Texture2D GenerateTexture2D(int width, int height, unsigned int format, const unsigned char* pixels);

private:
    static void GetTextureFormat(const bool& srgb, const int& numChannels, unsigned int* format, unsigned int* internalFormat);
};
  
#endif // TEXTURE_LOADER_H
