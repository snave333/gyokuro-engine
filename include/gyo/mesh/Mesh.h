#ifndef MESH_H
#define MESH_H

#include <vector>

#include <glm/glm.hpp>

#include <math/AABB.h>
#include <gyo/internal/shading/Material.h>

namespace gyo {

class Shader;
struct Geometry;

class Mesh {
public:
    // constructor
    Mesh(Geometry* geometry, Material* material, bool computeTangents = true);
    ~Mesh();

    void Draw();

    Material* GetMaterial() { return material; }
    void SetMaterial(Material* newMaterial);
    const RenderType& GetRenderType() { return material->renderType; }

    const AABB& GetBounds() { return bounds; }
    const unsigned int& GetNumTris() { return numTris; }

protected:
    Geometry* geometry = nullptr;
    Material* material = nullptr;

    AABB bounds;

    void Initialize();
    void ComputeBounds();

private:
    // render data
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    unsigned int indexCount;
    unsigned int numTris;
};

} // namespace gyo

#endif // MESH_H
