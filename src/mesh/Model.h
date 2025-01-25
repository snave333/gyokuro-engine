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

    Mesh* GetMesh() { return meshes[0]; }
    Material* GetMaterial() { return meshes[0]->GetMaterial(); }
    const RenderType& GetRenderType() { return meshes[0]->GetRenderType(); }

    const AABB& GetBounds() { return bounds; }

private:
    std::vector<Mesh*> meshes = {};
    AABB bounds;

    void ComputeBounds();
};

#endif // MODEL_H
