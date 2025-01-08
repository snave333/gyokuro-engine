#ifndef FONT_LOADER_H
#define FONT_LOADER_H

#include <string>

class Font;

class FontLoader {
public:
    static Font LoadFont(std::string fontFilePath, unsigned int fontSize);
};
  
#endif // FONT_LOADER_H
