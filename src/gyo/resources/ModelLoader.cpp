
#include <gyo/resources/ModelLoader.h>
#include <gyo/resources/Resources.h>
#include <gyo/resources/TextureLoader.h>
#include <gyo/mesh/Model.h>
#include <gyo/mesh/Mesh.h>
#include <gyo/geometry/Geometry.h>
#include <gyo/shading/GoochMaterial.h>
#include <gyo/shading/Material.h>
#include <gyo/shading/PBRMaterial.h>
#include <gyo/shading/PhongMaterial.h>
#include <gyo/shading/Texture2D.h>
#include <gyo/utilities/Clock.h>
#include <gyo/utilities/FileSystem.h>
#include <gyo/utilities/Log.h>

#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace gyo {

std::string ModelLoader::ResourceDir = "";

Assimp::Importer ModelLoader::importer;

Model* ModelLoader::LoadModel(const char* fileName, bool flipUVs) {
    LOGI("Importing model %s", fileName);
    CLOCK(Model_Load);

    // get the full file path
    std::string modelFilePath = FileSystem::CombinePath(ResourceDir, fileName);
    
    std::string ext = FileSystem::GetFilePathExtension(modelFilePath);
    if(ext.compare("gltf") != 0 && ext.compare("glb") != 0 && ext.compare("fbx") != 0) {
        throw std::runtime_error("Only GLTF, GLTF2, and FBX model file types are supported");
    }

    // our import flags
    unsigned int flags = aiProcess_Triangulate | aiProcess_CalcTangentSpace;
    // aiProcess_GenSmoothNormals | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph
    if(flipUVs) {
        flags |= aiProcess_FlipUVs;
    }

    // use assimp to load the model
    const aiScene* scene = ModelLoader::importer.ReadFile(modelFilePath, flags);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        LOGE("Import error: %s", ModelLoader::importer.GetErrorString());
        return nullptr;
    }

    LOGI("Importing model '%s' with %u meshes, %u materials, and %u textures",
        fileName, scene->mNumMeshes, scene->mNumMaterials, scene->mNumTextures);

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

    LOGD("Processing %u vertices", mesh->mNumVertices);

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

    LOGD("Processing %u faces", mesh->mNumFaces);

    // process indices

    for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    Geometry* geo = new Geometry( positions, normals, texCoords, tangents, indices );

    // process material 0

    Material* mat = nullptr;
    if(mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        // assemble our texture types
    
        std::unordered_set<aiTextureType> texTypes;
        for(int i = 0; i < AI_TEXTURE_TYPE_MAX; i++) {
            aiTextureType type = static_cast<aiTextureType>(i);
            if(material->GetTextureCount(type) > 0) {
                texTypes.insert(type);
            }
        }

        // determine which material type to use depending on the textures

        LOGD("Processing material with %u properties", material->mNumProperties);

        LogMaterialProperties(material);
        LogMaterialTextureTypes(material, scene);

        bool isPBR =
            texTypes.contains(aiTextureType_METALNESS) |
            texTypes.contains(aiTextureType_DIFFUSE_ROUGHNESS) |
            texTypes.contains(aiTextureType_GLTF_METALLIC_ROUGHNESS);

        if(isPBR) {
            Texture2D* albedoMap = nullptr;
            Texture2D* normalMap = nullptr;
            Texture2D* metallicMap = nullptr;
            Texture2D* roughnessMap = nullptr;
            Texture2D* metallicRoughnessMap = nullptr;
            Texture2D* aoMap = nullptr;
            Texture2D* emissiveMap = nullptr;

            albedoMap = LoadMaterialTexture(material, aiTextureType_BASE_COLOR, scene, true);
            if(!albedoMap) albedoMap = LoadMaterialTexture(material, aiTextureType_DIFFUSE, scene, true);

            normalMap = LoadMaterialTexture(material, aiTextureType_NORMALS, scene);

            metallicRoughnessMap = LoadMaterialTexture(material, aiTextureType_GLTF_METALLIC_ROUGHNESS, scene);
            if(!metallicRoughnessMap) {
                metallicMap = LoadMaterialTexture(material, aiTextureType_METALNESS, scene);
                roughnessMap = LoadMaterialTexture(material, aiTextureType_DIFFUSE_ROUGHNESS, scene);
            }

            aoMap = LoadMaterialTexture(material, aiTextureType_AMBIENT_OCCLUSION, scene);
            if(!aoMap) aoMap = LoadMaterialTexture(material, aiTextureType_LIGHTMAP, scene);

            emissiveMap = LoadMaterialTexture(material, aiTextureType_EMISSION_COLOR, scene);
            if(!emissiveMap) emissiveMap = LoadMaterialTexture(material, aiTextureType_EMISSIVE, scene);

            mat = new PBRMaterial(
                true,
                glm::vec3(1.0),
                metallicMap || metallicRoughnessMap ? 1.0 : 0.0,
                roughnessMap || metallicRoughnessMap ? 1.0 : 0.5,
                1.0,
                emissiveMap ? glm::vec3(1.0) : glm::vec3(0.0),
                albedoMap,
                normalMap,
                metallicMap,
                roughnessMap,
                metallicRoughnessMap,
                aoMap,
                emissiveMap
            );
        }
        else {
            Texture2D* diffuseMap = nullptr;
            Texture2D* specularMap = nullptr;
            Texture2D* normalMap = nullptr;
    
            diffuseMap = LoadMaterialTexture(material, aiTextureType_DIFFUSE, scene, true);
            if(!diffuseMap) diffuseMap = LoadMaterialTexture(material, aiTextureType_BASE_COLOR, scene, true);

            specularMap = LoadMaterialTexture(material, aiTextureType_SPECULAR, scene);
            
            normalMap = LoadMaterialTexture(material, aiTextureType_NORMALS, scene);

            mat = new PhongMaterial(
                glm::vec4(1),
                glm::vec4(1),
                128,
                diffuseMap, specularMap, normalMap);
        }
    }
    else {
        mat = new GoochMaterial();
    }

    return new Mesh(geo, mat);
}

Texture2D* ModelLoader::LoadMaterialTexture(aiMaterial* mat, aiTextureType type, const aiScene* scene, bool srgb) {
    Texture2D* texture = nullptr;
    
    aiString str;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiReturn result = mat->GetTexture(type, i, &str);
        if(result != AI_SUCCESS) {
            LOGW("Failed to get texture path from material");
            continue;
        }
        
        const aiTexture* aiTex = scene->GetEmbeddedTexture(str.C_Str());
        if(aiTex) {
            LOGD(" Loading embedded %s texture '%s', %ux%u - %s", TextureTypeToString(type), str.C_Str(), aiTex->mWidth, aiTex->mHeight, aiTex->achFormatHint);
            
            texture = TextureLoader::LoadEmbeddedTexture(aiTex, srgb);
            break;
        }
        else {
            LOGD(" Found referenced %s texture '%s'", TextureTypeToString(type), str.C_Str());
            
            // FIXME: this assumes referenced textures use file names (not paths),
            // and are placed in the /textures folder.
            // std::string fileName = FileSystem::GetFileName(str.C_Str());
            // texture = Resources::GetTexture(fileName.c_str(), srgb);
            break;
        }
    }

    return texture;
}

void ModelLoader::LogMaterialProperties(aiMaterial* mat) {
    for(unsigned int i = 0; i < mat->mNumProperties; i++) {
        aiMaterialProperty* prop = mat->mProperties[i];

        LOGT(" aiMaterial[%s] size: %u bytes, type: %s",
            prop->mKey.C_Str(), prop->mDataLength, PropertyTypeToString(prop->mType));
    }
}

void ModelLoader::LogMaterialTextureTypes(aiMaterial* mat, const aiScene* scene) {
    LOGT("Logging material texture types:");
    
    for(unsigned int t = aiTextureType_NONE; t <= AI_TEXTURE_TYPE_MAX; t++) {
        aiTextureType type = static_cast<aiTextureType>(t);
        unsigned int count = mat->GetTextureCount(type);

        aiString str;
        for(unsigned int i = 0; i < count; i++) {
            mat->GetTexture(type, i, &str);
            
            const aiTexture* aiTex = scene->GetEmbeddedTexture(str.C_Str());
            if(aiTex) {
                LOGT(" Embedded %s texture '%s', %ux%u - %s", TextureTypeToString(type), str.C_Str(), aiTex->mWidth, aiTex->mHeight, aiTex->achFormatHint);
            }
            else {
                LOGT(" Referenced %s texture '%s'", TextureTypeToString(type), str.C_Str());
            }
        }
    }
}

const char* ModelLoader::TextureTypeToString(aiTextureType type) {
    switch(type) {
        // legacy api materials
        case aiTextureType_NONE:                   return "NONE";
        case aiTextureType_DIFFUSE:                return "DIFFUSE";
        case aiTextureType_SPECULAR:               return "SPECULAR";
        case aiTextureType_AMBIENT:                return "AMBIENT";
        case aiTextureType_EMISSIVE:               return "EMISSIVE";
        case aiTextureType_HEIGHT:                 return "HEIGHT";
        case aiTextureType_NORMALS:                return "NORMALS";
        case aiTextureType_SHININESS:              return "SHININESS";
        case aiTextureType_OPACITY:                return "OPACITY";
        case aiTextureType_DISPLACEMENT:           return "DISPLACEMENT";
        case aiTextureType_LIGHTMAP:               return "LIGHTMAP (AO)";
        case aiTextureType_REFLECTION:             return "REFLECTION";
        // pbr materials
        case aiTextureType_BASE_COLOR:             return "BASE_COLOR";
        case aiTextureType_NORMAL_CAMERA:          return "NORMAL_CAMERA";
        case aiTextureType_EMISSION_COLOR:         return "EMISSION_COLOR";
        case aiTextureType_METALNESS:              return "METALNESS";
        case aiTextureType_DIFFUSE_ROUGHNESS:      return "DIFFUSE_ROUGHNESS";
        case aiTextureType_AMBIENT_OCCLUSION:      return "AMBIENT_OCCLUSION";

        case aiTextureType_UNKNOWN:                return "UNKNOWN";

        case aiTextureType_SHEEN:                  return "SHEEN";
        case aiTextureType_CLEARCOAT:              return "CLEARCOAT";
        case aiTextureType_TRANSMISSION:           return "TRANSMISSION";
        // Maya material declerations
        case aiTextureType_MAYA_BASE:              return "MAYA_BASE";
        case aiTextureType_MAYA_SPECULAR:          return "MAYA_SPECULAR";
        case aiTextureType_MAYA_SPECULAR_COLOR:    return "MAYA_SPECULAR_COLOR";
        case aiTextureType_MAYA_SPECULAR_ROUGHNESS:return "MAYA_SPECULAR_ROUGHNESS";

        case aiTextureType_ANISOTROPY:             return "ANISOTROPY";

        // GLTF material declerations
        case aiTextureType_GLTF_METALLIC_ROUGHNESS:return "GLTF_METALLIC_ROUGHNESS";

        default:                                   return "UNRECOGNIZED_aiTextureType";
    }
}

const char* ModelLoader::PropertyTypeToString(aiPropertyTypeInfo type) {
    switch(type) {
        case aiPTI_Float:       return "Float";
        case aiPTI_Double:      return "Double";
        case aiPTI_String:      return "String";
        case aiPTI_Integer:     return "Integer";
        case aiPTI_Buffer:
        default:                return "Buffer";
    }
}

} // namespace gyo
