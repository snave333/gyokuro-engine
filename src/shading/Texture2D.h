#ifndef TEXTURE2D_H
#define TEXTURE2D_H

class Texture2D {
public:
    // constructor reads the image data and creates the gl texture
    Texture2D(const char* imageFileName, bool transparency = false);
    ~Texture2D();

    void Bind(unsigned int textureUnit = 0);

private:
    // the texture id
    unsigned int ID;
};

#endif // TEXTURE2D_H
