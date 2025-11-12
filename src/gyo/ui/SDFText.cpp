
#include <gyo/ui/SDFText.h>

#include <gyo/ui/SDFFont.h>
#include <gyo/shading/Shader.h>
#include <gyo/resources/Resources.h>

#include <glm/gtc/matrix_transform.hpp>

namespace gyo {

SDFText::SDFText(const char* fontName, const glm::ivec2& viewportSize) {
    font = Resources::GetSDFFont(fontName);
    shader = Resources::GetShader("glyph.vert", "glyph.frag");

    renderQueue.reserve(16);

    // generate our vertex array and buffer
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // bind
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    // each 2D quad requires 6 vertices of 8 floats each; for now just reserve 1,
    // we'll update the vbo's memory later when rendering characters
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GlyphVertex), NULL, GL_DYNAMIC_DRAW);

    // link the vertex attribute pointers
    // screen-space position
    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GlyphVertex), (void*)0);
    // texture coord
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GlyphVertex), (void*)offsetof(GlyphVertex, texCoord));
    // vertex color
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(GlyphVertex), (void*)offsetof(GlyphVertex, color));

    // unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // set our shader uniforms
    shader->Use();
    shader->SetInt("glyphAtlas", 0);
    glm::mat4 projection = glm::ortho(0.0f, (float)viewportSize.x, 0.0f, (float)viewportSize.y);
    shader->SetMat4("projection", projection);
}

SDFText::~SDFText() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    font = nullptr;
    shader = nullptr;
}

void SDFText::UpdateViewportSize(const glm::ivec2& size) {
    glm::mat4 projection = glm::ortho(0.0f, (float)size.x, 0.0f, (float)size.y);

    shader->Use();
    shader->SetMat4("projection", projection);
}

void SDFText::QueueStringRender(std::string text, unsigned int x, unsigned int y, float scale, glm::vec4 color) {
    renderQueue.emplace_back(text, x, y, scale, color);
    pendingGlyphs += text.length();
}

void SDFText::ExecuteRender() {
    if(renderQueue.empty()) {
        return;
    }

    // activate corresponding render state
    shader->Use();
    glBindVertexArray(VAO);
    font->BindTexture();
    
    // assemble our vertex array with all our queued string renders
    std::vector<GlyphVertex> vertices;
    vertices.reserve(6 * pendingGlyphs);

    for (TextStringRender render : renderQueue) {
        unsigned int x = render.x;
        unsigned int y = render.y;
        float scale = render.scale;

        std::string::const_iterator c;

        for (c = render.text.begin(); c != render.text.end(); c++) {
            SDFCharacter ch = font->GetCharacter(*c);
            float uvLeft = ch.texCoord[0];
            float uvBottom = ch.texCoord[1];
            float uvRight = ch.texCoord[2];
            float uvTop = ch.texCoord[3];
            
            // origin is bottom left
            float xpos = x + ch.bearing.x * scale;
            float ypos = y - (ch.size.y - ch.bearing.y) * scale;

            float w = ch.size.x * scale;
            float h = ch.size.y * scale;

            // tri 1
            vertices.emplace_back(glm::vec2(xpos,     ypos + h), glm::vec2(uvLeft,  uvBottom), render.color);
            vertices.emplace_back(glm::vec2(xpos,     ypos    ), glm::vec2(uvLeft,  uvTop   ), render.color);
            vertices.emplace_back(glm::vec2(xpos + w, ypos    ), glm::vec2(uvRight, uvTop   ), render.color);
            // tri 2
            vertices.emplace_back(glm::vec2(xpos,     ypos + h), glm::vec2(uvLeft,  uvBottom), render.color);
            vertices.emplace_back(glm::vec2(xpos + w, ypos    ), glm::vec2(uvRight, uvTop   ), render.color);
            vertices.emplace_back(glm::vec2(xpos + w, ypos + w), glm::vec2(uvRight, uvBottom), render.color);

            // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            x += (ch.advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
        }
    }

    // update content of VBO memory
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), &vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // render quad
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    renderQueue.clear();
    pendingGlyphs = 0;
}

} // namespace gyo
