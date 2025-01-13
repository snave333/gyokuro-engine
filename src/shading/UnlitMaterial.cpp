
#include <shading/UnlitMaterial.h>
#include <shading/Shader.h>
#include <shading/Texture2D.h>
#include <resources/Resources.h>

UnlitMaterial::UnlitMaterial(
    glm::vec3 color,
    Texture2D* texture,
    glm::vec2 uvTiling,
    glm::vec2 uvOffset
) {
    this->color = color;
    this->texture = texture;
    this->uvTiling = uvTiling;
    this->uvOffset = uvOffset;

    if(texture == nullptr) {
        shader = Resources::GetShader("default.vert", "solidColor.frag");
    }
    else {
        shader = Resources::GetShader("default.vert", "unlitColorTexture.frag");

        shader->Use();
        shader->SetInt("tex", 0);
    }
}

UnlitMaterial::~UnlitMaterial() {
    texture = nullptr;
}

void UnlitMaterial::Queue() {
    shader->Use();

    shader->SetVec3("color", color);
    shader->SetVec2("uvTiling", this->uvTiling);
    shader->SetVec2("uvOffset", this->uvOffset);

    if(texture != nullptr) {
        texture->Bind(0);
    }
}
