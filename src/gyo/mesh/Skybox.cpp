
#include <gyo/mesh/Skybox.h>
#include <gyo/geometry/InvertedCube.h>
#include <gyo/mesh/Mesh.h>
#include <gyo/resources/Resources.h>
#include <gyo/shading/ShaderMaterial.h>
#include <gyo/shading/ShaderSemantics.h>

namespace gyo {

Skybox::Skybox(TextureCube* cubeMap) {
    this->cubeMap = cubeMap;

    mesh = new Mesh(new InvertedCube(), new ShaderMaterial(
        Resources::GetShader("skybox.vert", "skybox.frag"),
        { { "aPos", SEMANTIC_POSITION } }
    ));
}

Skybox::~Skybox() {
    delete mesh;
    mesh = nullptr;
}

void Skybox::Draw(glm::mat4 view, glm::mat4 projection) const {
    // remove translation from the view matrix
    glm::mat4 viewNoTranslation = glm::mat4(glm::mat3(view));

    mesh->GetMaterial()->Queue();
    const Shader& shader = mesh->GetMaterial()->GetShader();

    shader.SetMat4("view", viewNoTranslation);
    shader.SetMat4("projection", projection);

    cubeMap->Bind();

    mesh->Draw();
}

} // namespace gyo
