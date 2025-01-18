#ifndef TEXTURE2D_H
#define TEXTURE2D_H

class Texture2D {
public:
    Texture2D() {}
    Texture2D(const unsigned int& textureId) : ID(textureId) {}
    
    void Dispose();

    void Bind(unsigned int textureUnit = 0);

private:
    // the texture id
    unsigned int ID;
};

#endif // TEXTURE2D_H
