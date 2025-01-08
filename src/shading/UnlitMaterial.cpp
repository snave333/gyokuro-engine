
#include <shading/UnlitMaterial.h>
#include <shading/Shader.h>
#include <shading/Texture2D.h>
#include <resources/Resources.h>

UnlitMaterial::UnlitMaterial(glm::vec3 color, Texture2D* texture) {
    this->color = color;
    this->texture = texture;

    if(texture == nullptr) {
        shader = Resources::GetShader("default.vert", "unlitColor.frag");
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

    if(texture != nullptr) {
        texture->Bind(0);
    }
}
