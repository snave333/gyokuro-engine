#ifndef FONT_H
#define FONT_H

#include <map>
#include <glm/glm.hpp>

namespace gyo {

class Texture2D;

struct Character {
    glm::dvec2   size;      // size of glyph
    glm::dvec2   bearing;   // offset from baseline to left/top of glyph
    unsigned int advance;   // offset to advance to next glyph
    glm::dvec4   texCoord;  // glyph's bounds in the atlas in normalized coords [0, 1] – left, bottom, right, & top
};

class Font {
public:
    Font() {}
    Font(
        Texture2D* textureAtlas,
        std::map<char, Character> characters
    ) : textureAtlas(textureAtlas), characters(characters) {}
    
    void Dispose();

    void BindTexture();
    const Character& GetCharacter(char c) { return characters[c]; }

private:
    Texture2D* textureAtlas = nullptr;
    std::map<char, Character> characters = {};
};
  
} // namespace gyo

#endif // FONT_H
