
#include <shading/UnlitMaterial.h>
#include <shading/Shader.h>
#include <shading/Texture2D.h>

UnlitMaterial::UnlitMaterial(glm::vec3 color, const char* imagePath) {
    this->color = color;

    if(imagePath == nullptr) {
        shader = new Shader("default.vert", "unlitColor.frag");
    }
    else {
        texture = new Texture2D(imagePath);
        shader = new Shader("default.vert", "unlitColorTexture.frag");

        shader->Use();
        shader->SetInt("tex", 0);
    }
}

UnlitMaterial::~UnlitMaterial() {
    delete texture;
    texture = nullptr;
}

void UnlitMaterial::Queue() {
    shader->Use();

    shader->SetVec3("color", color);

    if(texture != nullptr) {
        texture->Bind(0);
    }
}
