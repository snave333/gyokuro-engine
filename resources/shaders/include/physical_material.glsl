
#ifdef TEX_NORMAL
#include "normal.glsl"
#endif

// IBL
#ifdef USE_IBL
uniform samplerCube irradianceMap;
uniform samplerCube prefilteredEnvMap;
uniform sampler2D brdfLUT;
#endif

struct PhysicalMaterialUniforms {
    vec3 albedo;
    float metallic;
    float roughness;
    float ao;
    vec3 emissive;

#ifdef TEX_ALBEDO
    sampler2D albedoMap;
#endif

#ifdef TEX_NORMAL
    sampler2D normalMap;
#endif

#ifdef TEX_METALLIC_ROUGHNESS
    sampler2D metallicRoughnessMap;
#else
    #ifdef TEX_METALLIC
        sampler2D metallicMap;
    #endif

    #ifdef TEX_ROUGHNESS
        sampler2D roughnessMap;
    #endif
#endif

#ifdef TEX_AO
    sampler2D aoMap;
#endif

#ifdef TEX_EMISSIVE
    sampler2D emissiveMap;
#endif
};
uniform PhysicalMaterialUniforms uMaterial;

// our material definition used for lighting calculations

struct PhysicalMaterial {
    vec3 albedo;
    vec3 worldNormal;
    float metallic;
    float roughness;
    float ao;
    vec3 emissive;
};

// derive our material properties from our uniforms

PhysicalMaterial getMaterialProps(
    vec3 normal,
    vec2 texCoord,
    vec3 tangent
) {
    PhysicalMaterial material;

    material.albedo = uMaterial.albedo;
    material.metallic = uMaterial.metallic;
    material.roughness = uMaterial.roughness;
    material.ao = uMaterial.ao;
    material.emissive = uMaterial.emissive;

#ifdef TEX_ALBEDO
    vec3 albedoSample = texture(uMaterial.albedoMap, texCoord).rgb;
    material.albedo *= albedoSample;
#endif

#ifdef TEX_NORMAL
    material.worldNormal = calcNormal(normal, tangent, uMaterial.normalMap, texCoord);
#else
    material.worldNormal = normal;
#endif

#ifdef TEX_METALLIC_ROUGHNESS
    vec3 metallicRoughnessSample = texture(uMaterial.metallicRoughnessMap, texCoord).rgb;
    material.metallic *= metallicRoughnessSample.b;
    material.roughness *= metallicRoughnessSample.g;
#else
    #ifdef TEX_METALLIC
        float metallicSample = texture(uMaterial.metallicMap, texCoord).r;
        material.metallic *= metallicSample;
    #endif
    
    #ifdef TEX_ROUGHNESS
        float roughnessSample = texture(uMaterial.roughnessMap, texCoord).r;
        material.roughness *= roughnessSample;
    #endif
#endif

#ifdef TEX_AO
    float aoSample = texture(uMaterial.aoMap, texCoord).r;
    material.ao *= aoSample;
#endif

#ifdef TEX_EMISSIVE
    vec3 emissiveSample = texture(uMaterial.emissiveMap, texCoord).rgb;
    material.emissive *= emissiveSample;
#endif

    return material;
}
