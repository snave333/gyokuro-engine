#ifndef TEXTURE2D_H
#define TEXTURE2D_H

class Texture2D {
public:
    // the texture id
    unsigned int ID;

    // constructor reads the image data and creates the gl texture
    Texture2D(const char* imagePath);
    ~Texture2D();
};

#endif // TEXTURE2D_H
