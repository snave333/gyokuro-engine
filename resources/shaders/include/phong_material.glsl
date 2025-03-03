struct Material {
    vec4 diffuse;
    vec3 specular;
    float shininess;

#ifdef MATERIAL_TEXTURES
    sampler2D diffuseMap;
    sampler2D specularMap;
    sampler2D normalMap;
#endif
};
uniform Material material;
