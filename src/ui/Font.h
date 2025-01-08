#ifndef FONT_H
#define FONT_H

#include <map>
#include <glm/glm.hpp>

struct Character {
    unsigned int textureID;  // ID handle of the glyph texture
    glm::ivec2   size;       // size of glyph
    glm::ivec2   bearing;    // offset from baseline to left/top of glyph
    unsigned int advance;    // offset to advance to next glyph
};

class Font {
public:
    Font() {}
    Font(std::map<char, Character> characters) : characters(characters) {}
    void Dispose();

    const Character& GetCharacter(char c) { return characters[c]; }

private:
    std::map<char, Character> characters;
};
  
#endif // FONT_H
