#ifndef SDF_FONT_H
#define SDF_FONT_H

#include <map>
#include <glm/glm.hpp>

namespace gyo {

class Texture2D;

struct SDFCharacter {
    glm::dvec2   size;      // size of glyph
    glm::dvec2   bearing;   // offset from baseline to left/top of glyph
    unsigned int advance;   // offset to advance to next glyph
    glm::dvec4   texCoord;  // glyph's bounds in the atlas in normalized coords [0, 1] – left, bottom, right, & top
};

class SDFFont {
public:
    SDFFont() {}
    SDFFont(
        Texture2D* textureAtlas,
        std::map<char, SDFCharacter> characters
    ) : textureAtlas(textureAtlas), characters(characters) {}
    
    void Dispose();

    void BindTexture();
    const SDFCharacter& GetCharacter(char c) { return characters[c]; }

private:
    Texture2D* textureAtlas = nullptr;
    std::map<char, SDFCharacter> characters = {};
};
  
} // namespace gyo

#endif // SDF_FONT_H
