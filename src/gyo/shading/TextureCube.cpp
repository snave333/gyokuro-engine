
#include <gyo/shading/TextureCube.h>

#include <gyo/utilities/GetError.h>

#include <glad/glad.h>

namespace gyo {

TextureCube::TextureCube(const unsigned int& textureId, unsigned int width, unsigned int height)
    : ID(textureId), width(width), height(height)
{}

void TextureCube::Dispose() {
    glDeleteTextures(1, &ID);
    glCheckError();
}

void TextureCube::Bind(unsigned int textureUnit) const {
    glActiveTexture(GL_TEXTURE0 + textureUnit); // activate texture unit 0 before binding
    glCheckError();
    glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
    glCheckError();
}

} // namespace gyo
