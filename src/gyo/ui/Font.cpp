
#include <gyo/ui/Font.h>
#include <gyo/shading/Texture2D.h>
#include <gyo/utilities/Log.h>

namespace gyo {

void Font::Dispose() {
    // no need to delete the texture; it is handled in Resources
    textureAtlas = nullptr;
    
    characters.clear();
}

void Font::BindTexture() {
    if(textureAtlas == nullptr) {
        LOGE("SDF Font missing texture atlas");
        return;
    }

    textureAtlas->Bind();
}

} // namespace gyo
