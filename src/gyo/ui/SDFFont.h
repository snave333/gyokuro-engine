#ifndef SDF_FONT_H
#define SDF_FONT_H

#include <map>
#include <glm/glm.hpp>

namespace gyo {

class Texture2D;

struct SDFCharacter {
    glm::dvec4   bounds;     // glyph quad's bounds in em's relative to the baseline and cursor – left, bottom, right, & top
    // glm::ivec2   bearing;    // offset from baseline to left/top of glyph
    glm::dvec4   texCoords;  // glyph's bounds in the atlas in normalized coords [0, 1] – left, bottom, right, & top
    unsigned int advance;    // offset to advance to next glyph
};

class SDFFont {
public:
    SDFFont() {}
    SDFFont(
        Texture2D* textureAtlas,
        std::map<char, SDFCharacter> characters
    ) : textureAtlas(textureAtlas), characters(characters) {}
    
    void Dispose();

    const SDFCharacter& GetCharacter(char c) { return characters[c]; }

private:
    Texture2D* textureAtlas = nullptr;
    std::map<char, SDFCharacter> characters = {};
};
  
} // namespace gyo

#endif // SDF_FONT_H
