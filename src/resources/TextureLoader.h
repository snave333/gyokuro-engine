#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H

#include <string>

class Texture2D;
class TextureCube;
struct aiTexture;
struct aiTexel;

class TextureLoader {
public:
    static std::string ResourceDir;
    
    static Texture2D LoadTexture(const char* imageFileName, bool srgb);
    static Texture2D* LoadEmbeddedTexture(const aiTexture* texture, bool srgb);
    static TextureCube LoadTextureCube(std::vector<const char*> faceFileNames, bool srgb);
    static Texture2D GenerateTexture2D(int width, int height, unsigned int format, const unsigned char* pixels);

private:
    static void DecompressJpegData(
        const aiTexel* pcData, const unsigned int& pcDataSize,
        int* width, int* height, int* numChannels,
        unsigned char** imageData);
    static void GetTextureFormat(const bool& srgb, const int& numChannels, unsigned int* format, unsigned int* internalFormat);
};
  
#endif // TEXTURE_LOADER_H
