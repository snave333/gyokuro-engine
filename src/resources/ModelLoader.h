#ifndef Model_LOADER_H
#define Model_LOADER_H

#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/material.h>

struct aiScene;
struct aiNode;
struct aiMesh;
struct aiMaterial;

namespace gyo {

class Model;
class Mesh;
class Texture2D;

class ModelLoader {
public:
    static std::string ResourceDir;

    static Model* LoadModel(const char* fileName, bool flipUVs);

private:
    static Assimp::Importer importer;

    static void ProcessNode(aiNode* node, const aiScene* scene, std::vector<Mesh*>& meshes);
    static Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene);
    static Texture2D* LoadMaterialTexture(aiMaterial* mat, aiTextureType type, const aiScene* scene);

    static void LogMaterialProperties(aiMaterial* mat);
    static void LogMaterialTextureTypes(aiMaterial* mat, const aiScene* scene);
};
  
} // namespace gyo

#endif // Model_LOADER_H
