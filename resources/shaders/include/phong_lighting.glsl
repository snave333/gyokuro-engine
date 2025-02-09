#include "phong_material.glsl"
#include "lights.glsl"

struct LightingResult {
    vec3 diffuse;
    vec3 specular;
};

/*
  Direct lighting function definitions:

  P - the fragment position
  N - the surface normal
  L - the normalized direction from P to the light source
  V - the normalized direction from P to the eye (camera position)
*/

float calcDiffuse(vec3 L, vec3 N) {
    float NdotL = max(dot(N, L), 0.0);
    return NdotL;
}

float calcSpecular(vec3 V, vec3 L, vec3 N, float a) {
    // specular (Phong)
    vec3 R = reflect(-L, N);
    float RdotV = max(dot(R, V), 0.0);

    // specular (Blinn-Phong)
    vec3 H = normalize(L + V);
    float NdotH = max(dot(N, H), 0.0);

    return pow(NdotH, a);
}

// technique taken from https://www.3dgep.com/texturing-lighting-directx-11/#Spotlight_Cone
float calcSpotCone(SpotLight light, vec3 L) {
    float minCos = light.cosAngle;
    float maxCos = (minCos + 1.0f) / 2.0f;
    float cosAngle = dot(light.direction.xyz, -L );

    return smoothstep( minCos, maxCos, cosAngle ); 
}

LightingResult calcDirectionalLight(DirectionalLight light, vec3 V, vec3 N) {
    LightingResult result = LightingResult(vec3(0.0), vec3(0.0));

    vec3 L = normalize(-light.direction.xyz);

    result.diffuse = light.color.rgb * calcDiffuse(L, N);
    result.specular = light.color.rgb * calcSpecular(V, L, N, material.shininess);

    return result;
}

LightingResult calcPointLight(PointLight light, vec3 V, vec3 P, vec3 N) {
    LightingResult result = LightingResult(vec3(0.0), vec3(0.0));
    
    vec3 L = light.position.xyz - P;
    float distance = length(L);
    L = L / distance; // normalize

    float attenuation = 1.0 / (distance * distance);

    result.diffuse = light.color.rgb * calcDiffuse(L, N) * attenuation;
    result.specular = light.color.rgb * calcSpecular(V, L, N, material.shininess) * attenuation;

    return result;
}

LightingResult calcSpotLight(SpotLight light, vec3 V, vec3 P, vec3 N) {
    LightingResult result = LightingResult(vec3(0.0), vec3(0.0));
    
    vec3 L = light.position.xyz - P;
    float distance = length(L);
    L = L / distance; // normalize

    // spotlight soft edges
    float spotlightFactor = calcSpotCone(light, L);
    if(spotlightFactor == 0.0) {
        // early exit if we're beyond the outer angle
        return result;
    }

    float attenuation = 1.0 / (distance * distance);

    result.diffuse = light.color.rgb * calcDiffuse(L, N) * attenuation * spotlightFactor;
    result.specular = light.color.rgb * calcSpecular(V, L, N, material.shininess) * attenuation * spotlightFactor;

    return result;
}

LightingResult calcTotalLighting(vec3 V, vec3 P, vec3 N) {
    LightingResult result = LightingResult(vec3(0.0), vec3(0.0));

    LightingResult directional = calcDirectionalLight(dirLight, V, N);
    result.diffuse += directional.diffuse;
    result.specular += directional.specular;

    for(int i = 0; i < min(numPointLights, MAX_POINT_LIGHTS); i++) {
        LightingResult point = calcPointLight(pointLights[i], V, P, N);
        result.diffuse += point.diffuse;
        result.specular += point.specular;
    }

    for(int i = 0; i < min(numSpotLights, MAX_SPOT_LIGHTS); i++) {
        LightingResult spot = calcSpotLight(spotLights[i], V, P, N);
        result.diffuse += spot.diffuse;
        result.specular += spot.specular;
    }

    return result;
}
