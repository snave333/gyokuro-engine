
#include <ui/Font.h>

#include <iostream>

#include <glad/glad.h>

#include <ft2build.h>
#include <freetype/freetype.h>

void Font::Dispose() {
    for (const auto& character : characters) {
        glDeleteTextures(1, &character.second.textureID);
    }
    characters.clear();
}
