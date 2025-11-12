
#include <gyo/ui/SDFFont.h>

#include <iostream>

#include <gyo/shading/Texture2D.h>

namespace gyo {

void SDFFont::Dispose() {
    // no need to delete the texture; it is handled in Resources
    
    characters.clear();
}

void SDFFont::BindTexture() {
    if(textureAtlas == nullptr) {
        std::cerr << "SDF Font missing texture atlas" << std::endl;
        return;
    }

    textureAtlas->Bind();
}

} // namespace gyo
