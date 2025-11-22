
#include <gyo/resources/ModelLoader.h>
#include <gyo/resources/Resources.h>
#include <gyo/resources/TextureLoader.h>
#include <gyo/utilities/FileSystem.h>
#include <gyo/mesh/Model.h>
#include <gyo/mesh/Mesh.h>
#include <gyo/geometry/Geometry.h>
#include <gyo/shading/Material.h>
#include <gyo/shading/PhongMaterial.h>
#include <gyo/shading/Texture2D.h>

#include <iostream>

#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace gyo {

std::string ModelLoader::ResourceDir = "";

Assimp::Importer ModelLoader::importer;

Model* ModelLoader::LoadModel(const char* fileName, bool flipUVs) {
    // get the full file path
    std::string modelFilePath = FileSystem::CombinePath(ResourceDir, fileName);

    // our import flags
    unsigned int flags = aiProcess_Triangulate | aiProcess_CalcTangentSpace;
    // aiProcess_GenSmoothNormals | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph
    if(flipUVs) {
        flags |= aiProcess_FlipUVs;
    }

    // use assimp to load the model
    const aiScene* scene = ModelLoader::importer.ReadFile(modelFilePath, flags);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP::" << ModelLoader::importer.GetErrorString() << std::endl;
        throw std::runtime_error("ERROR::ASSIMP::" + std::string(ModelLoader::importer.GetErrorString()));
    }

    std::cout << "assimp successfully imported model with " <<
        std::to_string(scene->mNumMeshes) << " meshes, " <<
        std::to_string(scene->mNumMaterials) << " materials, and " <<
        std::to_string(scene->mNumTextures) << " textures " <<
        "for model " << fileName << std::endl;

    // assemble the meshes which makeup the model
    std::vector<Mesh*> meshes;
    ProcessNode(scene->mRootNode, scene, meshes);

    return new Model(meshes);
}

void ModelLoader::ProcessNode(aiNode* node, const aiScene* scene, std::vector<Mesh*>& meshes) {
    // process all node meshes
    for(unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(ProcessMesh(mesh, scene));
    }

    // now process node's children
    for(unsigned int i = 0; i < node->mNumChildren; i++) {
        ProcessNode(node->mChildren[i], scene, meshes);
    }
}

Mesh* ModelLoader::ProcessMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> tangents;
    std::vector<unsigned int> indices;

    std::cout << "- processing " << std::to_string(mesh->mNumVertices) << " vertices" << std::endl;

    positions.reserve(mesh->mNumVertices);
    normals.reserve(mesh->mNumVertices);
    texCoords.reserve(mesh->mNumVertices);
    tangents.reserve(mesh->mNumVertices);

    // process vertices
    for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
        positions.emplace_back(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        normals.emplace_back(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        tangents.emplace_back(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);

        // NOTE we're only using the first set of tex coords for now
        if(mesh->mTextureCoords[0]) {
            texCoords.emplace_back(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        }
        else {
            texCoords.push_back({ 0.0, 0.0 });
        }
    }

    std::cout << "- processing " << std::to_string(mesh->mNumFaces) << " faces" << std::endl;

    // process indices
    for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    // process material
    Texture2D* diffMap = nullptr;
    Texture2D* specMap = nullptr;
    Texture2D* nrmMap = nullptr;
    if(mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        std::cout << "- processing material with " << std::to_string(material->mNumProperties) << " properties" << std::endl;
        // LogMaterialProperties(material);
        // LogMaterialTextureTypes(material, scene);

        diffMap = LoadMaterialTexture(material, aiTextureType_DIFFUSE, scene);
        specMap = LoadMaterialTexture(material, aiTextureType_SPECULAR, scene);
        nrmMap = LoadMaterialTexture(material, aiTextureType_NORMALS, scene);
    }

    return new Mesh(
        new Geometry{ positions, normals, texCoords, tangents, indices },
        new PhongMaterial(glm::vec4(1), glm::vec4(1), 128, diffMap, specMap, nrmMap),
        false);
}

Texture2D* ModelLoader::LoadMaterialTexture(aiMaterial* mat, aiTextureType type, const aiScene* scene) {
    std::cout << "- found " << std::to_string(mat->GetTextureCount(type)) << " textures of type " << std::to_string(type) << std::endl;

    Texture2D* texture = nullptr;
    
    // TODO return an array of textures of type

    aiString str;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiReturn result = mat->GetTexture(type, i, &str);
        if(result != AI_SUCCESS) {
            std::cerr << "Failed to get texture path from material" << std::endl;
            continue;
        }
        
        const aiTexture* aiTex = scene->GetEmbeddedTexture(str.C_Str());
        if(aiTex) {
            std::cout << "- embedded texture '" << str.C_Str() << "', " <<
                std::to_string(aiTex->mWidth) << " x " << std::to_string(aiTex->mHeight) << " - " <<
                aiTex->achFormatHint << std::endl;
            
            texture = TextureLoader::LoadEmbeddedTexture(aiTex, type == aiTextureType_DIFFUSE);
        }
        else {
            std::cout << "- referenced texture '" << str.C_Str() << "'" << std::endl;

            // NOTE: this assumes referenced textures use file names (not paths),
            // and are placed in the /textures folder.
            std::string fileName = FileSystem::GetFileName(str.C_Str());
            texture = Resources::GetTexture(fileName.c_str(), type == aiTextureType_DIFFUSE);
        }
    }

    return texture;
}

void ModelLoader::LogMaterialProperties(aiMaterial* mat) {
    for(unsigned int i = 0; i < mat->mNumProperties; i++) {
        aiMaterialProperty* prop = mat->mProperties[i];
        std::string data = std::string(prop->mData, prop->mDataLength);
        std::cout << "aiMaterial[" << prop->mKey.C_Str() << "] = " << data << std::endl;
    }
}

void ModelLoader::LogMaterialTextureTypes(aiMaterial* mat, const aiScene* scene) {
    for(unsigned int t = aiTextureType_NONE; t <= AI_TEXTURE_TYPE_MAX; t++) {
        aiTextureType type = static_cast<aiTextureType>(t);
        unsigned int count = mat->GetTextureCount(type);

        if(count > 0) {
            std::cout << "found " << std::to_string(count) << " textures of type " << std::to_string(type) << std::endl;
        }
        
        aiString str;
        for(unsigned int i = 0; i < count; i++) {
            mat->GetTexture(type, i, &str);
            
            const aiTexture* aiTex = scene->GetEmbeddedTexture(str.C_Str());
            if(aiTex) {
                std::cout << "\tembedded texture '" << str.C_Str() << "', " <<
                    std::to_string(aiTex->mWidth) << " x " << std::to_string(aiTex->mHeight) << " - " <<
                    aiTex->achFormatHint << std::endl;
            }
            else {
                std::cout << "\treferenced texture '" << str.C_Str() << "'" << std::endl;
            }
        }
    }
}

} // namespace gyo
