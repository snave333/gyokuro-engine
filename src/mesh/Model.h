#ifndef MODEL_NODE_H
#define MODEL_NODE_H

#include <scene/SceneNode.h>
#include <mesh/Mesh.h>

#include <glm/glm.hpp>

class Shader;
class Material;
class AABBWireframe;
struct AABB;
enum RenderType;

class Model : public SceneNode {
public:
    Model(Mesh* mesh, bool renderBounds = true);
    ~Model();

    // TODO make mesh a list of meshes
    // void AddMesh(const Mesh &mesh);

    void Queue() const { mesh->Queue(); }
    void Draw();

    const Material& GetMaterial() { return mesh->GetMaterial(); }
    const RenderType& GetRenderType() { return mesh->GetRenderType(); }

    const AABB& GetBounds();
    const std::array<glm::vec3, 8>& GetLUT() { return boundsLUT; }

protected:
    Mesh* mesh = nullptr;

private:
    AABB bounds;
    std::array<glm::vec3, 8> boundsLUT = {};
    AABBWireframe* boundsRenderer = nullptr;

    void UpdateBounds();
};

#endif // MODEL_NODE_H
