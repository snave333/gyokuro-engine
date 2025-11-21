
#include <gyo/shading/Texture2D.h>

#include <gyo/utilities/GetError.h>

#include <glad/glad.h>

namespace gyo {

Texture2D::Texture2D() {
}

Texture2D::Texture2D(const unsigned int& textureId, unsigned int width, unsigned int height, bool hasAlpha)
    : ID(textureId), width(width), height(height), hasAlpha(hasAlpha)
{}

void Texture2D::Dispose() {
    glDeleteTextures(1, &ID);
    glCheckError();
}

void Texture2D::Bind(unsigned int textureUnit) const {
    glActiveTexture(GL_TEXTURE0 + textureUnit); // activate texture unit 0 before binding
    glCheckError();
    glBindTexture(GL_TEXTURE_2D, ID);
    glCheckError();
}

void Texture2D::UnbindTextureSlot(int textureUnit) {
    glActiveTexture(GL_TEXTURE0 + textureUnit); // activate texture unit 0 before binding
    glCheckError();
    glBindTexture(GL_TEXTURE_2D, 0);
    glCheckError();
}

} // namespace gyo
