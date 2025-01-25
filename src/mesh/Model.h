#ifndef MODEL_H
#define MODEL_H

#include <mesh/Mesh.h>

#include <glm/glm.hpp>

class Material;
struct AABB;
enum RenderType;

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

#endif // MODEL_H
