#ifndef FONT_LOADER_H
#define FONT_LOADER_H

#include <string>

namespace gyo {

class Font;

class FontLoader {
public:
    static std::string ResourceDir;
    
    static Font LoadFont(const char* fontName, const float& pixelsPerEm);
};
  
} // namespace gyo

#endif // FONT_LOADER_H
