
#include <resources/ModelLoader.h>
#include <resources/Resources.h>
#include <mesh/Model.h>
#include <mesh/Mesh.h>
#include <mesh/Geometry.h>
#include <shading/Material.h>
#include <shading/PhongMaterial.h>
#include <shading/Texture2D.h>

#include <iostream>

#include <assimp/scene.h>
#include <assimp/postprocess.h>

Assimp::Importer ModelLoader::importer;

Model* ModelLoader::LoadModel(const char* fileName) {
    const aiScene* scene = ModelLoader::importer.ReadFile(
        fileName,
        aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace
    ); // aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP::" << ModelLoader::importer.GetErrorString() << std::endl;
        return nullptr;
    }

    std::cout << "assimp successfully imported model with " <<
        std::to_string(scene->mNumMeshes) << " meshes, " <<
        std::to_string(scene->mNumMaterials) << " materials, and " <<
        std::to_string(scene->mNumTextures) << " textures" << std::endl;

    // TODO
    std::vector<Mesh*> meshes;
    ProcessNode(scene->mRootNode, scene, meshes);

    ModelLoader::importer.FreeScene();

    return new Model(meshes);
}

void ModelLoader::ProcessNode(aiNode* node, const aiScene* scene, std::vector<Mesh*>& meshes) {
    for(unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(ProcessMesh(mesh, scene));
    }

    for(unsigned int i = 0; i < node->mNumChildren; i++) {
        ProcessNode(node->mChildren[i], scene, meshes);
    }
}

Mesh* ModelLoader::ProcessMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture2D> textures;

    // process vertices
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
    glm::vec3 tangent;
    for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
        position.x = mesh->mVertices[i].x;
        position.y = mesh->mVertices[i].y;
        position.z = mesh->mVertices[i].z;

        normal.x = mesh->mNormals[i].x;
        normal.y = mesh->mNormals[i].y;
        normal.z = mesh->mNormals[i].z;

        tangent.x = mesh->mTangents[i].x;
        tangent.y = mesh->mTangents[i].y;
        tangent.z = mesh->mTangents[i].z;

        if(mesh->mTextureCoords[0]) {
            texCoord.x = mesh->mTextureCoords[0][i].x;
            texCoord.y = mesh->mTextureCoords[0][i].y;
        }
        else {
            texCoord = glm::vec2(0);
        }

        vertices.push_back(Vertex( position, normal, texCoord, tangent ));
    }

    // process indices
    for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    // process material
    if(mesh->mMaterialIndex >= 0) {

    }

    return new Mesh(new Geometry{ vertices, indices }, new PhongMaterial(), false);
}

std::vector<Texture2D> ModelLoader::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {
    return {};
}
