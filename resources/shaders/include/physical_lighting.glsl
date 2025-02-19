#include "physical_material.glsl"
#include "lights.glsl"
#include "brdf.glsl"

/*
    Analytic lighting function definitions:

    P - the fragment position
    N - the surface normal
    L - the normalized direction from P to the light source
    V - the normalized direction from P to the eye (camera position)
    H - halfway vector (between L and V)
    lR - individual light's radiance
*/

// compute the total reflectance for this light source, using our BRDF
vec3 calcReflectance(vec3 lR, vec3 V, vec3 N, vec3 L, vec3 H) {
    // pre-computed reflection coefficient
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, material.albedo, material.metallic);

    // our Cook-Torrance specular BRDF term

    float D = DistributionGGX(N, H, material.roughness);
    float G = GeometrySmith(N, V, L, material.roughness);
    vec3 F = fresnelSchlick(max(0.0, dot(H, V)), F0);

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - material.metallic;

    vec3 numerator = D * G * F;
    float denominator = 4.0 * max(0.0, dot(N, V)) * max(0.0, dot(N, L)) + 0.0001; // prevent divide by 0
    vec3 specular = numerator / denominator;

    float NdotL = max(0.0, dot(N, L));

    return (kD * material.albedo / PI + specular) * lR * NdotL;
}

vec3 calcDirectionalLight(DirectionalLight light, vec3 V, vec3 N) {
    vec3 L = normalize(-light.direction.xyz);
    vec3 H = normalize(L + V);

    // the light's individual radiance
    vec3 lR = light.color.rgb;

    return calcReflectance(lR, V, N, L, H);
}

vec3 calcPointLight(PointLight light, vec3 V, vec3 P, vec3 N) {
    vec3 L = light.position.xyz - P;
    float distance = length(L);
    L = L / distance; // normalize

    vec3 H = normalize(L + V);
    float attenuation = 1.0 / (distance * distance);

    // the light's individual radiance
    vec3 lR = light.color.rgb * attenuation;

    return calcReflectance(lR, V, N, L, H);
}

// technique taken from https://www.3dgep.com/texturing-lighting-directx-11/#Spotlight_Cone
float calcSpotCone(SpotLight light, vec3 L) {
    float minCos = light.cosAngle;
    float maxCos = (minCos + 1.0f) / 2.0f;
    float cosAngle = max(0.0, dot(light.direction.xyz, -L));

    return smoothstep(minCos, maxCos, cosAngle); 
}

vec3 calcSpotLight(SpotLight light, vec3 V, vec3 P, vec3 N) {
    vec3 L = light.position.xyz - P;
    float distance = length(L);
    L = L / distance; // normalize

    vec3 H = normalize(L + V);
    float attenuation = 1.0 / (distance * distance);

    // spotlight soft edges
    float spotlightFactor = calcSpotCone(light, L);
    if(spotlightFactor == 0.0) {
        // early exit if we're beyond the outer angle
        return vec3(0, 0, 0);
    }

    // the light's individual radiance
    vec3 lR = light.color.rgb * attenuation * spotlightFactor;

    return calcReflectance(lR, V, N, L, H);
}

// compute the irradiance, or total scene lighting
vec3 calcIrradiance(vec3 V, vec3 P, vec3 N) {
    vec3 Lo = vec3(0.0);

    Lo += calcDirectionalLight(dirLight, V, N);

    for(int i = 0; i < min(numPointLights, MAX_POINT_LIGHTS); i++) {
        Lo += calcPointLight(pointLights[i], V, P, N);
    }

    for(int i = 0; i < min(numSpotLights, MAX_SPOT_LIGHTS); i++) {
        Lo += calcSpotLight(spotLights[i], V, P, N);
    }

    return Lo;
}
