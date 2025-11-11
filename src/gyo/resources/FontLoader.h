#ifndef FONT_LOADER_H
#define FONT_LOADER_H

#include <string>

namespace gyo {

class Font;
class SDFFont;

class FontLoader {
public:
    static std::string ResourceDir;
    
    static Font LoadFont(const char* fontFileName, unsigned int fontSize);
    static SDFFont LoadSDFFont(const char* fontName);
};
  
} // namespace gyo

#endif // FONT_LOADER_H
