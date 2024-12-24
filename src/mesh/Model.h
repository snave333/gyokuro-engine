#ifndef MODEL_NODE_H
#define MODEL_NODE_H

#include <scene/SceneNode.h>
#include <mesh/Mesh.h>

#include <glm/glm.hpp>

class Model : public SceneNode {
public:
    Model(Mesh* mesh) : SceneNode() { this->mesh = mesh; };
    ~Model();

    // TODO make mesh a list of meshes
    // void AddMesh(const Mesh &mesh);

    void Draw(Shader &shader) const { mesh->Draw(shader); }

protected:
    Mesh* mesh = nullptr;
};

#endif // MODEL_NODE_H
