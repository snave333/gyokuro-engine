struct Material {
    vec3 albedo;
    float metallic;
    float roughness;
    float ao;
    sampler2D diffuseMap;
    sampler2D specularMap;
    sampler2D normalMap;
};
uniform Material material;
