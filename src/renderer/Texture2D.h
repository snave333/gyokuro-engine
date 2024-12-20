#ifndef TEXTURE2D_H
#define TEXTURE2D_H

class Texture2D {
public:
    // the texture id
    unsigned int ID;

    // constructor reads the image data and creates the gl texture
    Texture2D(const char* imagePath, bool transparency = false);
    ~Texture2D();

    void Bind(unsigned int textureUnit = 0);
};

#endif // TEXTURE2D_H
