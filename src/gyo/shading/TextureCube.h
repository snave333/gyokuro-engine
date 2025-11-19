#ifndef TEXTURE_CUBE_H
#define TEXTURE_CUBE_H

namespace gyo {

class TextureCube {
public:
    TextureCube() {}
    TextureCube(const unsigned int& textureId, unsigned int width, unsigned int height);
    
    void Dispose();

    void Bind(unsigned int textureUnit = 0);

    unsigned int width;
    unsigned int height;

private:
    // the texture id
    unsigned int ID;
};

} // namespace gyo

#endif // TEXTURE_CUBE_H
