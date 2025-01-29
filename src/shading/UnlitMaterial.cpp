
#include <gyo/shading/UnlitMaterial.h>
#include <gyo/internal/shading/Shader.h>
#include <gyo/internal/shading/Texture2D.h>
#include <gyo/resources/Resources.h>

namespace gyo {

UnlitMaterial::UnlitMaterial(
    glm::vec4 color,
    bool additive,
    Texture2D* texture,
    glm::vec2 uvTiling,
    glm::vec2 uvOffset
) {
    this->color = color;
    this->texture = texture;
    this->uvTiling = uvTiling;
    this->uvOffset = uvOffset;

    bool hasAlpha = false;
    hasAlpha = hasAlpha || color.a < 1.0f;

    // select our shader depending on whether or not a texture was passed
    if(texture == nullptr) {
        shader = Resources::GetShader("default.vert", "solidColor.frag");
    }
    else {
        shader = Resources::GetShader("default.vert", "unlitColorTexture.frag");

        shader->Use();
        shader->SetInt("tex", 0);

        hasAlpha = hasAlpha || texture->hasAlpha;
    }

    if(additive) {
        renderType = RenderType::ADDITIVE;
    }
    else if(hasAlpha) {
        renderType = RenderType::TRANSPARENT;
    }
}

UnlitMaterial::~UnlitMaterial() {
    texture = nullptr;
}

void UnlitMaterial::Queue() {
    shader->Use();

    shader->SetVec4("color", color);

    if(texture != nullptr) {
        texture->Bind(0);

        shader->SetVec4("uvTilingOffset",
            glm::vec4(uvTiling.x, uvTiling.y, uvOffset.x, uvOffset.y));
    }
}

} // namespace gyo
