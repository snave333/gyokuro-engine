
#include <shading/Texture2D.h>

#include <glad/glad.h>

namespace gyo {

Texture2D::Texture2D() {
}

Texture2D::Texture2D(const unsigned int& textureId, bool hasAlpha)
    : ID(textureId), hasAlpha(hasAlpha)
{}

void Texture2D::Dispose() {
    glDeleteTextures(1, &ID);
}

void Texture2D::Bind(unsigned int textureUnit) {
    glActiveTexture(GL_TEXTURE0 + textureUnit); // activate texture unit 0 before binding
    glBindTexture(GL_TEXTURE_2D, ID);
}

} // namespace gyo
