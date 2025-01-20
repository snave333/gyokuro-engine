#ifndef FONT_LOADER_H
#define FONT_LOADER_H

#include <string>

class Font;

class FontLoader {
public:
    static std::string ResourceDir;
    
    static Font LoadFont(const char* fontFileName, unsigned int fontSize);
};
  
#endif // FONT_LOADER_H
