#include "physical_material.glsl"
#include "lights.glsl"
#include "brdf.glsl"

vec3 calcIrradiance(vec3 V, vec3 P, vec3 N) {
    // pre-computed reflection coefficient
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    vec3 Lo = vec3(0.0);

    // TODO

    vec3 a = albedo;
    vec3 mra = vec3(metallic, roughness, ao);

    return Lo;
}
