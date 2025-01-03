#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>

#include <vector>
#include <math/AABB.h>

class Shader;

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
};

class Mesh {
public:
    unsigned int VAO;
    
    // constructor
    Mesh();
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
    ~Mesh();

    void Draw(Shader &shader);

    const AABB& GetBounds() { return bounds; }

protected:
    // mesh Data
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    AABB bounds;

    void Initialize();
    void ComputeBounds();

private:
    // render data
    unsigned int VBO;
    unsigned int EBO;
};

#endif // MESH_H
