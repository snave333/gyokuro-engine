#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H

#include <string>

class Texture2D;

class TextureLoader {
public:
    static Texture2D LoadTexture(std::string imageFileName);
    static Texture2D GenerateTexture();
};
  
#endif // TEXTURE_LOADER_H
