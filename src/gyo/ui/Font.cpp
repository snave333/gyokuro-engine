
#include <gyo/ui/Font.h>

#include <iostream>

#include <gyo/shading/Texture2D.h>

namespace gyo {

void Font::Dispose() {
    // no need to delete the texture; it is handled in Resources
    textureAtlas = nullptr;
    
    characters.clear();
}

void Font::BindTexture() {
    if(textureAtlas == nullptr) {
        std::cerr << "SDF Font missing texture atlas" << std::endl;
        return;
    }

    textureAtlas->Bind();
}

} // namespace gyo
