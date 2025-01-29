#ifndef FONT_LOADER_H
#define FONT_LOADER_H

#include <string>

namespace gyo {

class Font;

class FontLoader {
public:
    static std::string ResourceDir;
    
    static Font LoadFont(const char* fontFileName, unsigned int fontSize);
};
  
} // namespace gyo

#endif // FONT_LOADER_H
