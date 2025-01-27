#ifndef Model_LOADER_H
#define Model_LOADER_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

class Model;
class Mesh;
class Texture2D;

class ModelLoader {
public:
    static std::string ResourceDir;

    static Model* LoadModel(const char* fileName);

private:
    static Assimp::Importer importer;

    static void ProcessNode(aiNode* node, const aiScene* scene, std::vector<Mesh*>& meshes);
    static Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene);
    static Texture2D* LoadMaterialTexture(aiMaterial* mat, aiTextureType type, const aiScene* scene);
};
  
#endif // Model_LOADER_H
