#ifndef MESH_H
#define MESH_H

#include <vector>

#include <glm/glm.hpp>

#include <math/AABB.h>
#include <shading/Material.h>

class Shader;
struct Geometry;
enum RenderType;

class Mesh {
public:
    // constructor
    Mesh(Geometry* geometry, Material* material);
    ~Mesh();

    void Draw();

    Material* GetMaterial() { return material; }
    const RenderType& GetRenderType() { return material->renderType; }

    const AABB& GetBounds() { return bounds; }

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
};

#endif // MESH_H
