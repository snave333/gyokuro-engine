
#include <gyo/ui/Font.h>

#include <glad/glad.h>

namespace gyo {

void Font::Dispose() {
    for (const auto& character : characters) {
        glDeleteTextures(1, &character.second.textureID);
    }
    characters.clear();
}

} // namespace gyo
