#ifndef TEXT_H
#define TEXT_H

#include <glm/glm.hpp>

class Font;
class Shader;

class Text {
public:
    Text(const char* fontName, const glm::ivec2& viewportSize, unsigned int fontSize = 16);
    ~Text();

    void UpdateViewportSize(const glm::ivec2& size);
    void RenderText(std::string text, unsigned int x, unsigned int y, float scale, glm::vec3 color);

private:
    Font* font;
    Shader* shader;

    unsigned int VAO;
    unsigned int VBO;
};
  
#endif // TEXT_H
