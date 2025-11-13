
#include <gyo/resources/FontLoader.h>
#include <gyo/resources/Resources.h>
#include <gyo/shading/Texture2D.h>
#include <gyo/ui/Font.h>
#include <gyo/ui/SDFFont.h>
#include <gyo/utilities/FileSystem.h>

#include <iostream>
#include <map>

#include <glad/glad.h>

#include <ft2build.h>
#include <freetype/freetype.h>

namespace gyo {

std::string FontLoader::ResourceDir = "";

// Uses Freetype to load a .ttf font file, creating separate textures for each glyph
Font FontLoader::LoadFont(const char* fontFileName, unsigned int fontSize) {
    std::string fontFilePath = FileSystem::CombinePath(ResourceDir, fontFileName);
    
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        throw std::runtime_error("Failed to load font");
    }

    FT_Face face;
    if (FT_New_Face(ft, fontFilePath.c_str(), 0, &face)) {
        std::cout << "ERROR::FREETYPE: Failed to load font " << fontFilePath << std::endl;  
        throw std::runtime_error("Failed to load font");
    }

    // set font size (0 width for auto)
    FT_Set_Pixel_Sizes(face, 0, fontSize);

    // disable byte-alignment restriction since we're using the GL_RED format
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    std::map<char, Character> characters;

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

        characters[c] = character;
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    // clear freetype resources
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    return Font(characters);
}

// loads a SDF texture atlas and glyph data csv file
SDFFont FontLoader::LoadSDFFont(const char* fontName, const float& pixelsPerEm) {
    // get texture name
    std::string textureFileName = std::string(fontName) + std::string("-Atlas.png");
    Texture2D* fontAtlas = Resources::GetTexture(textureFileName.c_str(), false, GL_CLAMP_TO_EDGE, false);
    unsigned int width = fontAtlas->width;
    unsigned int height = fontAtlas->height;
    
    // get the csv file
    std::string csvFilePath = FileSystem::CombinePath(
        ResourceDir, std::string(fontName) + std::string("-Layout.csv"));
    CSVData glyphData = Resources::GetCSV(csvFilePath.c_str());
    
    std::map<char, SDFCharacter> characters;

    // parse the glyphData and create the characters
    for(std::vector<std::string> glyph : glyphData) {
        if(glyph.size() != 10) {
            std::cerr << "Invalid glyph data for " << fontName << std::endl;
            continue;
        }

        // character Unicode value
        int unicodeValue = std::stoi(glyph[0]);

        // horizontal advance in em's
        double advance = std::stod(glyph[1]) * pixelsPerEm;

        // the glyph quad's bounds in em's relative to the baseline and cursor - left, bottom, right, top
        double boundsL = std::stod(glyph[2]) * pixelsPerEm;
        double boundsB = std::stod(glyph[3]) * pixelsPerEm;
        double boundsR = std::stod(glyph[4]) * pixelsPerEm;
        double boundsT = std::stod(glyph[5]) * pixelsPerEm;

        // the glyph's bounds in the atlas in pixels – left, bottom, right, top
        double uvL = std::stod(glyph[6]) / width;
        double uvB = std::stod(glyph[7]) / height;
        double uvR = std::stod(glyph[8]) / width;
        double uvT = std::stod(glyph[9]) / height;

        char c = static_cast<char>(unicodeValue);

        SDFCharacter character = {
            glm::dvec2(boundsR - boundsL, boundsT - boundsB),
            glm::dvec2(boundsL, boundsB),
            static_cast<unsigned int>(std::round(advance)),
            glm::dvec4(uvL, uvB, uvR, uvT)
        };

        characters[c] = character;
    }

    return SDFFont(fontAtlas, characters);
}

} // namespace gyo
