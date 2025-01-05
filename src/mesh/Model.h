#ifndef MODEL_NODE_H
#define MODEL_NODE_H

#include <scene/SceneNode.h>
#include <mesh/Mesh.h>

#include <glm/glm.hpp>

class Shader;
struct AABB;
enum RenderType;

class Model : public SceneNode {
public:
    Model(Mesh* mesh);
    ~Model();

    // TODO make mesh a list of meshes
    // void AddMesh(const Mesh &mesh);

    void Queue() const { mesh->Queue(); }
    void Draw() const { mesh->Draw(); }

    const Shader& GetShader() { return mesh->GetShader(); }
    const RenderType& GetRenderType() { return mesh->GetRenderType(); }

    const AABB& GetBounds();
    const std::array<glm::vec3, 8>& GetLUT() { return boundsLUT; }

protected:
    Mesh* mesh = nullptr;

private:
    AABB bounds;
    std::array<glm::vec3, 8> boundsLUT = {};

    void UpdateBounds();
};

#endif // MODEL_NODE_H
