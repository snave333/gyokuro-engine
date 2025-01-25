#ifndef MODEL_NODE_H
#define MODEL_NODE_H

#include <scene/SceneNode.h>
#include <mesh/Model.h>

#include <glm/glm.hpp>

class Mesh;
class Material;
struct AABB;
enum RenderType;

class ModelNode : public SceneNode {
public:
    int boundsLastFailedFrustumPlane = 0; // plane-coherency

    ModelNode(Model* model);
    ~ModelNode();

    Mesh* GetMesh() { return model->GetMesh(); }
    Material* GetMaterial() { return model->GetMesh()->GetMaterial(); }
    const RenderType& GetRenderType() { return model->GetMesh()->GetRenderType(); }

    const AABB& GetBounds();
    const std::array<glm::vec3, 8>& GetLUT() { return boundsLUT; }

protected:
    Model* model = nullptr;

    void SetDirty() override;

private:
    AABB bounds;
    std::array<glm::vec3, 8> boundsLUT = {};
    bool isBoundsDirty = true;

    void UpdateBounds();
    void UpdateBoundsLUT();
};

#endif // MODEL_NODE_H
