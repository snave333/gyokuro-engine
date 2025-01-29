#ifndef AABB_H
#define AABB_H

#include <glm/glm.hpp>

namespace gyo {

struct AABB {
    glm::vec3 min;
    glm::vec3 max;
};

} // namespace gyo

#endif // AABB_H
