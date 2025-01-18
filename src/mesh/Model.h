#ifndef MODEL_H
#define MODEL_H

#include <scene/SceneNode.h>
#include <mesh/Mesh.h>

#include <glm/glm.hpp>

class Shader;
class Material;
struct AABB;
enum RenderType;

class Model : public SceneNode {
public:
    int boundsLastFailedFrustumPlane = 0; // plane-coherency

    Model(Mesh* mesh);
    ~Model();

    // TODO make mesh a list of meshes
    // void AddMesh(const Mesh &mesh);

    Mesh* GetMesh() { return mesh; }
    Material* GetMaterial() { return mesh->GetMaterial(); }
    const RenderType& GetRenderType() { return mesh->GetRenderType(); }

    const AABB& GetBounds();
    const std::array<glm::vec3, 8>& GetLUT() { return boundsLUT; }

protected:
    Mesh* mesh = nullptr;

private:
    AABB bounds;
    std::array<glm::vec3, 8> boundsLUT = {};

    void UpdateBounds();
    void UpdateBoundsLUT();
};

#endif // MODEL_H
