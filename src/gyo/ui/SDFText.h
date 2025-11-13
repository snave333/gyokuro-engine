#ifndef SDF_TEXT_H
#define SDF_TEXT_H

#include <string>
#include <glm/glm.hpp>

namespace gyo {

class SDFFont;
class Shader;

struct TextStringRender {
    std::string text;
    int x;
    int y;
    unsigned int fontSize;
    glm::vec4 color;

    TextStringRender(std::string text, int x, int y, unsigned int fontSize, glm::vec4 color) :
        text(text), x(x), y(y), fontSize(fontSize), color(color) {}
};

struct GlyphVertex {
    glm::vec2 position;
    glm::vec2 texCoord;
    glm::vec4 color;

    GlyphVertex(glm::vec2 position, glm::vec2 texCoord, glm::vec4 color) :
        position(position), texCoord(texCoord), color(color) {}
};

class SDFText {
public:
    SDFText(const char* fontName, const glm::ivec2& viewportSize, const float& pixelsPerEm, const float& pixelRange);
    ~SDFText();

    void UpdateViewportSize(const glm::ivec2& size);
    void QueueStringRender(
        std::string text,
        int x,
        int y,
        unsigned int fontSize = 20,
        glm::vec4 color = glm::vec4(1)
    );
    void ExecuteRender();

private:
    float pixelsPerEm;

    SDFFont* font = nullptr;
    Shader* shader = nullptr;

    std::vector<TextStringRender> renderQueue = {};
    unsigned int pendingGlyphs = 0;

    unsigned int VAO;
    unsigned int VBO;
};
  
} // namespace gyo

#endif // SDF_TEXT_H
