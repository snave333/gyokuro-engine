
#include "common.glsl"

// The Cook-Torrance specular BRDF term – the same functions used by UE4, which
// were concluded to produce an acceptable level of realism, balancing the
// computational expense of the BRDF algorithm.

// Trowbridge-Reitz GGX normal distribution function to solve for D, which
// statistically approximates the relative surface area of microfacets exactly
// aligned to the (halfway) vector h
float DistributionGGX(vec3 N, vec3 H, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;
	
    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}

// Schlick-Beckmann geometry function to approximate the amount of light that
// is shadowed or masked by microfacets on a rough surface
float GeometrySchlickGGX(float NdotV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}

// Smith's method with Schlick-Beckmann GGX as Gsub to solve for G, taking into
// account both the view direction (geometry obstruction) and light direction
// vector (geometry shadowing)
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

// Fresnel-Schlick approximation of reflectivity at an angle, using pre-computed
// F0 reflection coefficient
vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
