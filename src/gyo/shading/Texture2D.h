#ifndef TEXTURE2D_H
#define TEXTURE2D_H

namespace gyo {

class Texture2D {
public:
    static void UnbindTextureSlot(int textureUnit = 0);
    
public:
    Texture2D();
    Texture2D(const unsigned int& textureId, unsigned int width, unsigned int height, bool hasAlpha);
    
    void Dispose();

    void Bind(unsigned int textureUnit = 0) const;

    unsigned int width;
    unsigned int height;
    bool hasAlpha = false;

private:
    // the texture id
    unsigned int ID;
};

} // namespace gyo

#endif // TEXTURE2D_H
