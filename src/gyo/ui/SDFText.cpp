
#include <gyo/ui/SDFText.h>

#include <gyo/ui/SDFFont.h>
#include <gyo/resources/Resources.h>

namespace gyo {

SDFText::SDFText(const char* fontName, const glm::ivec2& viewportSize) {
    font = Resources::GetSDFFont(fontName);

    // TODO
}

SDFText::~SDFText() {
    // TODO
}

void SDFText::UpdateViewportSize(const glm::ivec2& size) {
    // TODO
}

void SDFText::RenderText(std::string text, unsigned int x, unsigned int y, float scale, glm::vec3 color) {
    // TODO
}

} // namespace gyo
