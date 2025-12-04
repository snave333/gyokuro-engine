// IBL
#ifdef USE_IBL
uniform samplerCube irradianceMap;
uniform samplerCube prefilteredEnvMap;
uniform sampler2D brdfLUT;
#endif

struct Material {
    vec3 albedo;
    float metallic;
    float roughness;
    float ao;

#ifdef TEX_ALBEDO
    sampler2D albedoMap;
#endif
#ifdef TEX_NORMAL
    sampler2D normalMap;
#endif
#ifdef TEX_METALLIC
    sampler2D metallicMap;
#endif
#ifdef TEX_ROUGHNESS
    sampler2D roughnessMap;
#endif
#ifdef TEX_METALLIC_ROUGHNESS
    sampler2D metallicRoughnessMap;
#endif
#ifdef TEX_AO
    sampler2D aoMap;
#endif
#ifdef TEX_EMISSIVE
    sampler2D emissiveMap;
#endif
};
uniform Material uMaterial;
