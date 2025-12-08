
#include <gyo/resources/FontLoader.h>
#include <gyo/resources/Resources.h>
#include <gyo/shading/Texture2D.h>
#include <gyo/ui/Font.h>
#include <gyo/utilities/FileSystem.h>
#include <gyo/utilities/Log.h>

#include <map>

#include <glad/glad.h>

namespace gyo {

std::string FontLoader::ResourceDir = "";

Font FontLoader::LoadFont(const char* fontName, const float& pixelsPerEm) {
    // get texture name
    std::string textureFileName = std::string(fontName) + std::string("-Atlas.png");
    Texture2D* fontAtlas = Resources::GetTexture(textureFileName.c_str(), false, GL_CLAMP_TO_EDGE, false);
    unsigned int width = fontAtlas->width;
    unsigned int height = fontAtlas->height;
    
    // get the csv file
    std::string csvFilePath = FileSystem::CombinePath(
        ResourceDir, std::string(fontName) + std::string("-Layout.csv"));
    CSVData glyphData = Resources::GetCSV(csvFilePath.c_str());
    
    std::map<char, Character> characters;

    // parse the glyphData and create the characters
    for(std::vector<std::string> glyph : glyphData) {
        if(glyph.size() != 10) {
            LOGE("Invalid glyph data for '%s'", fontName);
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

        Character character = {
            glm::dvec2(boundsR - boundsL, boundsT - boundsB),
            glm::dvec2(boundsL, boundsB),
            static_cast<unsigned int>(std::round(advance)),
            glm::dvec4(uvL, uvB, uvR, uvT)
        };

        characters[c] = character;
    }

    return Font(fontAtlas, characters);
}

} // namespace gyo
