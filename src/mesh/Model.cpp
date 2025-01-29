
#include <gyo/mesh/Model.h>
#include <math/AABB.h>

namespace gyo {

unsigned int Model::ModelCounter = 0;

Model::Model(Mesh* mesh) : Model(std::vector<Mesh*>{ mesh }) {}

Model::Model(std::vector<Mesh*> meshes) {
    this->meshes = meshes;

    ID = ++ModelCounter;
    
    ComputeBounds();
};

Model::~Model() {
    for(const auto& mesh : meshes) {
        delete mesh;
    }
    meshes.clear();
}

void Model::ComputeBounds() {
    // init min and max with extreme values
    glm::vec3 minPoint(std::numeric_limits<float>::max());
    glm::vec3 maxPoint(std::numeric_limits<float>::lowest());

    // iterate through the mesh array
    for(Mesh* mesh : meshes) {
        const AABB& meshBounds = mesh->GetBounds();

        minPoint = glm::min(minPoint, meshBounds.min);
        maxPoint = glm::max(maxPoint, meshBounds.max);
    }

    bounds = { minPoint, maxPoint };
}

} // namespace gyo
