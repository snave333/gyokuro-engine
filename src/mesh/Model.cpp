
#include <mesh/Model.h>

#include <math/AABB.h>

Model::~Model() {
    delete mesh;
    mesh = nullptr;
}

AABB Model::GetBounds() {
    const AABB& meshBounds = mesh->GetBounds();
    const glm::mat4& transform = GetTransform();

    // extract the 8 corners of the original AABB
    std::vector<glm::vec3> corners = {
        { meshBounds.min.x, meshBounds.min.y, meshBounds.min.z },
        { meshBounds.min.x, meshBounds.min.y, meshBounds.max.z },
        { meshBounds.min.x, meshBounds.max.y, meshBounds.min.z },
        { meshBounds.min.x, meshBounds.max.y, meshBounds.max.z },
        { meshBounds.max.x, meshBounds.min.y, meshBounds.min.z },
        { meshBounds.max.x, meshBounds.min.y, meshBounds.max.z },
        { meshBounds.max.x, meshBounds.max.y, meshBounds.min.z },
        { meshBounds.max.x, meshBounds.max.y, meshBounds.max.z }
    };

    // transform each corner and find new min and max
    glm::vec3 newMin(std::numeric_limits<float>::max());
    glm::vec3 newMax(std::numeric_limits<float>::lowest());
    
    for (const auto& corner : corners) {
        glm::vec3 transformedCorner = glm::vec3(transform * glm::vec4(corner, 1.0f));
        newMin = glm::min(newMin, transformedCorner);
        newMax = glm::max(newMax, transformedCorner);
    }

    return { newMin, newMax };
}
