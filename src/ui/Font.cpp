
#include <ui/Font.h>
#include <resources/Resources.h>

#include <iostream>

#include <glad/glad.h>

#include <ft2build.h>
#include <freetype/freetype.h>

Font::Font(const char* fontFileName, unsigned int fontSize) {
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return;
    }

    std::string fullFontPath = Resources::GetFontPath(fontFileName);

    FT_Face face;
    if (FT_New_Face(ft, fullFontPath.c_str(), 0, &face)) {
        std::cout << "ERROR::FREETYPE: Failed to load font " << fontFileName << std::endl;  
        return;
    }

    // set font size (0 width for auto)
    FT_Set_Pixel_Sizes(face, 0, fontSize);

    // disable byte-alignment restriction since we're using the GL_RED format
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // cache the 128 ascii characters
    for (unsigned char c = 0; c < 128; c++) {
        // load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph " << c << std::endl;
            continue;
        }

        // generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // now store character for later use
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            (unsigned int)face->glyph->advance.x
        };

        characters.insert(std::pair<char, Character>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    // clear freetype resources
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

Font::~Font() {
    for (const auto& character : characters) {
        glDeleteTextures(1, &character.second.textureID);        
    }
    characters.clear();
}
