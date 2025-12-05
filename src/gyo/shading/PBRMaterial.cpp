
#include <gyo/shading/PBRMaterial.h>
#include <gyo/shading/ShaderSemantics.h>
#include <gyo/shading/TextureDefines.h>
#include <gyo/resources/Resources.h>
#include <gyo/resources/IBLEnvironmentLoader.h>
// #include <gyo/utilities/Log.h>

namespace gyo {

PBRMaterial::PBRMaterial(
    bool useIBL,
    glm::vec3 albedo,
    float metallic,
    float roughness,
    float ao,
    glm::vec3 emissive,
    Texture2D* albedoMap,
    Texture2D* normalMap,
    Texture2D* metallicMap,
    Texture2D* roughnessMap,
    Texture2D* metallicRoughnessMap,
    Texture2D* aoMap,
    Texture2D* emissiveMap,
    glm::vec2 uvTiling,
    glm::vec2 uvOffset
) {
    this->albedo = albedo;
    this->metallic = metallic;
    this->roughness = roughness;
    this->ao = ao;
    this->emissive = emissive;

    usesDirectLighting = true;
    this->usesIBL = useIBL;

    // assemble our shader information

    std::set<std::string> defines = {};
    if(usesIBL) {
        defines.insert("USE_IBL");

        unsigned int maxMipLevels = IBLEnvironmentLoader::PrefilterMipLevels - 1U;
        defines.insert(
            "MAX_REFLECTION_LOD " + std::to_string(maxMipLevels) + ".0"
        );
    }

    semantics = {
        { "aPos", SEMANTIC_POSITION },
        { "aNormal", SEMANTIC_NORMAL }
    };

    // textures

    hasTextures = albedoMap != nullptr ||
        normalMap != nullptr ||
        metallicMap != nullptr ||
        roughnessMap != nullptr ||
        metallicRoughnessMap != nullptr ||
        aoMap != nullptr ||
        emissiveMap != nullptr;
    if(hasTextures) {
        semantics["aTexCoord"] = SEMANTIC_TEXCOORD0;
    }

    if(albedoMap != nullptr) {
        defines.insert(TEX_ALBEDO);
        this->albedoMap = albedoMap;
    }
    if(normalMap != nullptr) {
        defines.insert(TEX_NORMAL);
        this->normalMap = normalMap;

        semantics["aTangent"] = SEMANTIC_TANGENT;
    }
    if(metallicMap != nullptr) {
        defines.insert(TEX_METALLIC);
        this->metallicMap = metallicMap;
    }
    if(roughnessMap != nullptr) {
        defines.insert(TEX_ROUGHNESS);
        this->roughnessMap = roughnessMap;
    }
    if(metallicRoughnessMap != nullptr) {
        defines.insert(TEX_METALLIC_ROUGHNESS);
        this->metallicRoughnessMap = metallicRoughnessMap;
    }
    if(aoMap != nullptr) {
        defines.insert(TEX_AO);
        this->aoMap = aoMap;
    }
    if(emissiveMap != nullptr) {
        defines.insert(TEX_EMISSIVE);
        this->emissiveMap = emissiveMap;
    }

    // create our shader now that we've assembled our defines

    shader = Resources::GetShader("default.vert", "pbr.frag", defines);

    // set our texture slots

    shader->Use();
    unsigned int texSlot = 0U;
    if(usesIBL) {
        shader->SetInt("irradianceMap", texSlot++);
        shader->SetInt("prefilteredEnvMap", texSlot++);
        shader->SetInt("brdfLUT", texSlot++);
    }
    if(albedoMap)               shader->SetInt("uMaterial.albedoMap", texSlot++);
    if(normalMap)               shader->SetInt("uMaterial.normalMap", texSlot++);
    if(metallicMap)             shader->SetInt("uMaterial.metallicMap", texSlot++);
    if(roughnessMap)            shader->SetInt("uMaterial.roughnessMap", texSlot++);
    if(metallicRoughnessMap)    shader->SetInt("uMaterial.metallicRoughnessMap", texSlot++);
    if(aoMap)                   shader->SetInt("uMaterial.aoMap", texSlot++);
    if(emissiveMap)             shader->SetInt("uMaterial.emissiveMap", texSlot++);
}

PBRMaterial::~PBRMaterial() {
}

void PBRMaterial::Queue() {
    shader->Use();

    shader->SetVec3("uMaterial.albedo", albedo);
    shader->SetFloat("uMaterial.metallic", metallic);
    shader->SetFloat("uMaterial.roughness", roughness);
    shader->SetFloat("uMaterial.ao", ao);
    // TODO emissive

    if(hasTextures) {
        unsigned int texSlot = 0U;
        if(usesIBL) {
            // irradiance, prefiltered diffuse, and brdf maps set in Renderer::RenderOpaque
            texSlot += 3U;
        }
        if(albedoMap)               albedoMap->Bind(texSlot++);
        if(normalMap)               normalMap->Bind(texSlot++);
        if(metallicMap)             metallicMap->Bind(texSlot++);
        if(roughnessMap)            roughnessMap->Bind(texSlot++);
        if(metallicRoughnessMap)    metallicRoughnessMap->Bind(texSlot++);
        if(aoMap)                   aoMap->Bind(texSlot++);
        if(emissiveMap)             emissiveMap->Bind(texSlot++);

        shader->SetVec4("uvTilingOffset",
            glm::vec4(uvTiling.x, uvTiling.y, uvOffset.x, uvOffset.y));
    }
    else {
        for (unsigned int i = 0u; i < 10u; ++i) {
            Texture2D::UnbindTextureSlot(i);
        }
    }
}

} // namespace gyo
