#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H

#include <string>

class Texture2D;

class TextureLoader {
public:
    static Texture2D LoadTexture(std::string imageFileName, bool srgb);
    static Texture2D GenerateTexture2D(int width, int height, unsigned int format, const unsigned char* pixels);
};
  
#endif // TEXTURE_LOADER_H
