#ifndef DRAW_CALL_H
#define DRAW_CALL_H

#include <glm/glm.hpp>

namespace gyo {

class Mesh;
class Material;

struct DrawCall {
    Mesh* mesh;
    Material* material;
    glm::mat4 transform;
    glm::mat4 normalMatrix;
    // unsigned int modelID;
};

} // namespace gyo

#endif // DRAW_CALL_H
