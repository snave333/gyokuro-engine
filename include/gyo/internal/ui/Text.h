#ifndef TEXT_H
#define TEXT_H

#include <string>
#include <glm/glm.hpp>

namespace gyo {

class Font;
class Shader;

class Text {
public:
    Text(const char* fontName, const glm::ivec2& viewportSize, unsigned int fontSize = 16);
    ~Text();

    void UpdateViewportSize(const glm::ivec2& size);
    void RenderText(std::string text, unsigned int x, unsigned int y,
        float scale = 1.0f, glm::vec3 color = glm::vec3(1));

    const unsigned int& GetFontSize() { return fontSize; }

private:
    Font* font = nullptr;
    Shader* shader = nullptr;

    unsigned int fontSize;

    unsigned int VAO;
    unsigned int VBO;
};
  
} // namespace gyo

#endif // TEXT_H
