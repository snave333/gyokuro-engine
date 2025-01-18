
#include <mesh/Model.h>
#include <math/AABB.h>

Model::Model(Mesh* mesh) : SceneNode() {
    this->mesh = mesh;
    
    UpdateBounds();
};

Model::~Model() {
    delete mesh;
    mesh = nullptr;
}

const AABB& Model::GetBounds() {
    if(isDirty) {
        UpdateBounds();
    }

    return bounds;
}

void Model::UpdateBounds() {
    const AABB& meshBounds = mesh->GetBounds();
    const glm::mat4& transform = GetTransform(); // updates the transform, if necessary

    // extract the 8 corners of the original AABB, ordered like the LUT
    std::array<glm::vec3, 8> corners = {
        glm::vec3(meshBounds.min.x, meshBounds.min.y, meshBounds.min.z),
        glm::vec3(meshBounds.max.x, meshBounds.min.y, meshBounds.min.z),
        glm::vec3(meshBounds.min.x, meshBounds.max.y, meshBounds.min.z),
        glm::vec3(meshBounds.max.x, meshBounds.max.y, meshBounds.min.z),
        glm::vec3(meshBounds.min.x, meshBounds.min.y, meshBounds.max.z),
        glm::vec3(meshBounds.max.x, meshBounds.min.y, meshBounds.max.z),
        glm::vec3(meshBounds.min.x, meshBounds.max.y, meshBounds.max.z),
        glm::vec3(meshBounds.max.x, meshBounds.max.y, meshBounds.max.z),
    };

    // transform each corner and find new min and max
    glm::vec3 newMin(std::numeric_limits<float>::max());
    glm::vec3 newMax(std::numeric_limits<float>::lowest());

    for(int i = 0; i < 8; i++) {
        glm::vec3 transformedCorner = glm::vec3(transform * glm::vec4(corners[i], 1.0f));

        newMin = glm::min(newMin, transformedCorner);
        newMax = glm::max(newMax, transformedCorner);
    }

    bounds = { newMin, newMax };

    UpdateBoundsLUT();
}

void Model::UpdateBoundsLUT() {
    // precomputing the LUT for more efficient AABB frustum culling
    boundsLUT[0] = { bounds.min.x, bounds.min.y, bounds.min.z };  // 000
    boundsLUT[1] = { bounds.max.x, bounds.min.y, bounds.min.z };  // 001
    boundsLUT[2] = { bounds.min.x, bounds.max.y, bounds.min.z };  // 010
    boundsLUT[3] = { bounds.max.x, bounds.max.y, bounds.min.z };  // 011
    boundsLUT[4] = { bounds.min.x, bounds.min.y, bounds.max.z };  // 100
    boundsLUT[5] = { bounds.max.x, bounds.min.y, bounds.max.z };  // 101
    boundsLUT[6] = { bounds.min.x, bounds.max.y, bounds.max.z };  // 110
    boundsLUT[7] = { bounds.max.x, bounds.max.y, bounds.max.z };  // 111
}

