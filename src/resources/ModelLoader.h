#ifndef Model_LOADER_H
#define Model_LOADER_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

class Model;
class Mesh;
class Texture2D;

class ModelLoader {
public:
    static Model* LoadModel(const char* fileName);

private:
    static Assimp::Importer importer;

    static void ProcessNode(aiNode* node, const aiScene* scene, std::vector<Mesh*>& meshes);
    static Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene);
    static std::vector<Texture2D> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};
  
#endif // Model_LOADER_H
