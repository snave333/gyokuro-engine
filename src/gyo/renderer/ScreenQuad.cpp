
#include <gyo/renderer/ScreenQuad.h>
#include <gyo/geometry/Quad.h>
#include <gyo/mesh/Mesh.h>
#include <gyo/shading/ShaderMaterial.h>
#include <gyo/shading/ShaderSemantics.h>
#include <gyo/resources/Resources.h>
#include <gyo/utilities/GetError.h>

namespace gyo {

ScreenQuad::ScreenQuad() {
    mesh = new Mesh(new Quad(), new ShaderMaterial(
        Resources::GetShader("screen.vert", "screen.frag"),
        {
            { "aPos", SEMANTIC_POSITION },
            { "aTexCoord", SEMANTIC_TEXCOORD0 }
        }
    ));

    const Shader& shader = mesh->GetMaterial()->GetShader();
    shader.Use();
    shader.SetInt("screenTexture", 0);
}

ScreenQuad::~ScreenQuad() {
    delete mesh;
    mesh = nullptr;
}

void ScreenQuad::Draw(unsigned int textureColorbuffer) {
    mesh->GetMaterial()->Queue();

    glActiveTexture(GL_TEXTURE0);
    glCheckError();
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glCheckError();

    mesh->Draw();
}

} // namespace gyo
