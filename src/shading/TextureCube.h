#ifndef TEXTURE_CUBE_H
#define TEXTURE_CUBE_H

class TextureCube {
public:
    TextureCube() {}
    TextureCube(const unsigned int& textureId) : ID(textureId) {}
    
    void Dispose();

    void Bind(unsigned int textureUnit = 0);

private:
    // the texture id
    unsigned int ID;
};

#endif // TEXTURE_CUBE_H
