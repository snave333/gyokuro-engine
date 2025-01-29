
#include <gyo/internal/ui/Text.h>

#include <gyo/internal/ui/Font.h>
#include <gyo/internal/shading/Shader.h>
#include <gyo/resources/Resources.h>

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

namespace gyo {

Text::Text(const char* fontName, const glm::ivec2& viewportSize, unsigned int fontSize) {
    font = Resources::GetFont(fontName, fontSize);
    shader = Resources::GetShader("glyph.vert", "glyph.frag");
    this->fontSize = fontSize;

    // generate our vertex array and buffer
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // bind
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    // each 2D quad requires 6 vertices of 4 floats each; for now just reserve 1,
    // we'll update the vbo's memory later when rendering characters
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

    // link the vertex attribute pointers
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

    // unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // set our shader uniforms
    shader->Use();
    shader->SetInt("bitmap", 0);
    glm::mat4 projection = glm::ortho(0.0f, (float)viewportSize.x, 0.0f, (float)viewportSize.y);
    shader->SetMat4("projection", projection);
}

Text::~Text() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    font = nullptr;
    shader = nullptr;
}

void Text::UpdateViewportSize(const glm::ivec2& size) {
    glm::mat4 projection = glm::ortho(0.0f, (float)size.x, 0.0f, (float)size.y);

    shader->Use();
    shader->SetMat4("projection", projection);
}

void Text::RenderText(std::string text, unsigned int x, unsigned int y, float scale, glm::vec3 color) {
    if(text.empty()) {
        return;
    }

    // activate corresponding render state
    shader->Use();
    shader->SetVec3("color", color);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) {
        Character ch = font->GetCharacter(*c);

        float xpos = x + ch.bearing.x * scale;
        float ypos = y - (ch.size.y - ch.bearing.y) * scale;

        float w = ch.size.x * scale;
        float h = ch.size.y * scale;

        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },            
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }           
        };

        // skip the space character because it causes a texture error
        if(*c == 32) {
            x += (ch.advance >> 6) * scale;
            continue;
        }

        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.textureID);

        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

} // namespace gyo
