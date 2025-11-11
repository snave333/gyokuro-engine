#ifndef SDF_TEXT_H
#define SDF_TEXT_H

#include <string>
#include <glm/glm.hpp>

namespace gyo {

class SDFFont;
class Shader;

class SDFText {
public:
    SDFText(const char* fontName, const glm::ivec2& viewportSize);
    ~SDFText();

    void UpdateViewportSize(const glm::ivec2& size);
    void RenderText(std::string text, unsigned int x, unsigned int y,
        float scale = 1.0f, glm::vec3 color = glm::vec3(1));

    // TODO remove me; only here while transitioning to SDF text rendering
    const unsigned int GetFontSize() { return 16; }

private:
    SDFFont* font = nullptr;
    Shader* shader = nullptr;

    unsigned int VAO;
    unsigned int VBO;
};
  
} // namespace gyo

#endif // SDF_TEXT_H
