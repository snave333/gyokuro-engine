#ifndef MODEL_H
#define MODEL_H

#include <gyo/mesh/Mesh.h>

#include <glm/glm.hpp>

namespace gyo {

class Material;
struct AABB;

class Model {
private:
    static unsigned int ModelCounter;

public:
    unsigned int ID;

    Model(Mesh* mesh);
    Model(std::vector<Mesh*> meshes);
    ~Model();

    const std::vector<Mesh*>& GetMeshes() const { return meshes; }
    const AABB& GetBounds() const { return bounds; }

private:
    std::vector<Mesh*> meshes = {};
    AABB bounds;

    void ComputeBounds();
};

} // namespace gyo

#endif // MODEL_H
