#ifndef DRAW_CALL_H
#define DRAW_CALL_H

#include <glm/glm.hpp>

class Mesh;
class Material;

struct DrawCall {
    Mesh* mesh;
    Material* material;
    glm::mat4 transform;
    glm::mat4 normalMatrix;
    // unsigned int modelID;
};

#endif // DRAW_CALL_H