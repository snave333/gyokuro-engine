
#include <gyo/internal/shading/TextureCube.h>

#include <glad/glad.h>

namespace gyo {

void TextureCube::Dispose() {
    glDeleteTextures(1, &ID);
}

void TextureCube::Bind(unsigned int textureUnit) {
    glActiveTexture(GL_TEXTURE0 + textureUnit); // activate texture unit 0 before binding
    glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
}

} // namespace gyo
