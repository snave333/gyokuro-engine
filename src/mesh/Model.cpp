
#include <mesh/Model.h>

Model::~Model() {
    delete mesh;
    mesh = nullptr;
}
