#ifndef MODEL_NODE_H
#define MODEL_NODE_H

#include <scene/SceneNode.h>
#include <mesh/Mesh.h>

#include <glm/glm.hpp>

struct AABB;

class Model : public SceneNode {
public:
    Model(Mesh* mesh);
    ~Model();

    // TODO make mesh a list of meshes
    // void AddMesh(const Mesh &mesh);

    void Draw(Shader &shader) const { mesh->Draw(shader); }

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
