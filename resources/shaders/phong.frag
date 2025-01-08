#version 330 core

out vec4 FragColor;

// direct lighting defines

#define MAX_NUM_LIGHTS 8
#define LIGHT_TYPE_DIRECTIONAL 0
#define LIGHT_TYPE_POINT 1
#define LIGHT_TYPE_SPOT 2

// direct lighting types

struct DirectionalLight {
    vec3 direction;
    vec3 color;
};

struct PointLight {
    vec3 position;
    vec3 color;
    // attenuation
    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    vec3 color;
    // spotlight
    float cutOff;
    float outerCutOff;
    // attenuation
    float constant;
    float linear;
    float quadratic;
};

struct LightingResult {
    vec3 diffuse;
    vec3 specular;
};

// direct lighting function definitions

/*
  P - the fragment position
  N - the surface normal
  L - the normalized direction from P to the light source
  V - the normalized direction from P to the eye (camera position)
*/
float calcDiffuse(vec3 L, vec3 N);
float calcSpecular(vec3 V, vec3 L, vec3 N, float a);
float calcAttenuation(float kc, float kl, float kq, float d);
float calcSpotCone();
LightingResult calcDirectionalLight(DirectionalLight light, vec3 V, vec3 N);
LightingResult calcPointLight(PointLight light, vec3 V, vec3 P, vec3 N);
LightingResult calcSpotLight(SpotLight light, vec3 V, vec3 P, vec3 N);

struct Material {
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

in VS_OUT {
    vec3 fragPos;
    vec3 normal;
    vec2 texCoord;
} fs_in;

layout (std140) uniform Camera {
    mat4 projection;
    mat4 view;
    vec4 viewPos; // .xyz: camera position in world space, .w = 0
};

// uniform Light lights[MAX_NUM_LIGHTS];
// layout (std140) uniform Lights {};
uniform vec3 globalAmbient;
uniform DirectionalLight dirLight;
uniform PointLight pointLight[2];
uniform SpotLight spotLight[2];

uniform Material material;

void main()
{
    vec3 V = normalize(viewPos.xyz - fs_in.fragPos);
    vec3 N = normalize(fs_in.normal);
    vec3 P = fs_in.fragPos;

    LightingResult totalLighting;
    LightingResult dLighting = calcDirectionalLight(dirLight, V, N);
    LightingResult p1Lighting = calcPointLight(pointLight[0], V, P, N);
    LightingResult p2Lighting = calcPointLight(pointLight[1], V, P, N);
    LightingResult s1Lighting = calcSpotLight(spotLight[0], V, P, N);
    LightingResult s2Lighting = calcSpotLight(spotLight[1], V, P, N);

    totalLighting.diffuse = dLighting.diffuse + p1Lighting.diffuse + p2Lighting.diffuse + s1Lighting.diffuse + s2Lighting.diffuse;
    totalLighting.specular = dLighting.specular + p1Lighting.specular + p2Lighting.specular + s1Lighting.specular + s2Lighting.specular;

    vec3 ambient = globalAmbient;
    vec3 diffuse = totalLighting.diffuse * material.diffuse;
    vec3 specular = totalLighting.specular * material.specular;
    vec3 result = ambient + diffuse + specular;

    FragColor = vec4(result, 1.0);
}

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

float calcAttenuation(float kc, float kl, float kq, float d) {
    return 1.0 / (
        kc +
        kl * d +
        kq * (d * d));
}

LightingResult calcDirectionalLight(DirectionalLight light, vec3 V, vec3 N) {
    LightingResult result;

    vec3 L = normalize(-light.direction);

    result.diffuse = light.color * calcDiffuse(L, N);
    result.specular = light.color * calcSpecular(V, L, N, material.shininess);

    return result;
}

LightingResult calcPointLight(PointLight light, vec3 V, vec3 P, vec3 N) {
    LightingResult result;
    
    vec3 L = light.position - P;
    float distance = length(L);
    L = L / distance; // normalize

    // attenuation
    float attenuation = calcAttenuation(light.constant, light.linear, light.quadratic, distance);

    result.diffuse = light.color * calcDiffuse(L, N) * attenuation;
    result.specular = light.color * calcSpecular(V, L, N, material.shininess) * attenuation;

    return result;
}

LightingResult calcSpotLight(SpotLight light, vec3 V, vec3 P, vec3 N) {
    LightingResult result;
    
    vec3 L = light.position - P;
    float distance = length(L);
    L = L / distance; // normalize

    // spotlight angles
    float theta = dot(L, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;

    // spotlight soft edges
    float spotlightFactor = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    if(spotlightFactor == 0.0) {
        // early exit if we're beyond the outer angle
        result.diffuse = vec3(0);
        result.specular = vec3(0);
        return result;
    }

    // attenuation
    float attenuation = calcAttenuation(light.constant, light.linear, light.quadratic, distance);

    result.diffuse = light.color * calcDiffuse(L, N) * attenuation * spotlightFactor;
    result.specular = light.color * calcSpecular(V, L, N, material.shininess) * attenuation * spotlightFactor;

    return result;
}
