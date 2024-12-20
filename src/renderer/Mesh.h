#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>

#include <vector>

class Shader;

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

class Mesh {
public:
    // mesh Data
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    unsigned int VAO;
    
    // constructor
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
    ~Mesh();

    void Draw(Shader &shader);

private:
    // render data
    unsigned int VBO;
    unsigned int EBO;
};

#endif // MESH_H
