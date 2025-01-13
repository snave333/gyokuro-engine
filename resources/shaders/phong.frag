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
    float cosAngle;
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
vec3 calcNormal(vec3 wsNormal, vec3 wsTangent, sampler2D texNormal, vec2 texCoord);
float calcDiffuse(vec3 L, vec3 N);
float calcSpecular(vec3 V, vec3 L, vec3 N, float a);
float calcAttenuation(float kc, float kl, float kq, float d);
float calcSpotCone(SpotLight light, vec3 L);
LightingResult calcDirectionalLight(DirectionalLight light, vec3 V, vec3 N);
LightingResult calcPointLight(PointLight light, vec3 V, vec3 P, vec3 N);
LightingResult calcSpotLight(SpotLight light, vec3 V, vec3 P, vec3 N);
LightingResult calcTotalLighting(vec3 V, vec3 P, vec3 N);

struct Material {
    vec3 diffuse;
    vec3 specular;
    float shininess;
    sampler2D diffuseMap;
    sampler2D specularMap;
    sampler2D normalMap;
    vec2 uvTiling;
    vec2 uvOffset;
};

in VS_OUT {
    vec3 fragPos;
    vec3 normal;
    vec2 texCoord;
    vec3 tangent;
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
uniform PointLight pointLights[2];
uniform SpotLight spotLights[2];

uniform Material material;

void main()
{
    vec2 mappedTexCoord = fs_in.texCoord * material.uvTiling + material.uvOffset;

    vec3 V = normalize(viewPos.xyz - fs_in.fragPos);
    vec3 N = calcNormal(fs_in.normal, fs_in.tangent, material.normalMap, mappedTexCoord);
    vec3 P = fs_in.fragPos;

    LightingResult totalLighting = calcTotalLighting(V, P, N);

    vec3 ambient = globalAmbient;
    vec3 diffuse = totalLighting.diffuse * material.diffuse * texture(material.diffuseMap, mappedTexCoord).xyz;
    vec3 specular = totalLighting.specular * material.specular * texture(material.specularMap, mappedTexCoord).xyz;
    vec3 result = ambient + diffuse + specular;

    FragColor = vec4(result, 1.0);
}

vec3 calcNormal(vec3 wsNormal, vec3 wsTangent, sampler2D texNormal, vec2 texCoord) {
    vec3 normal = normalize(wsNormal);
    vec3 tangent = normalize(wsTangent);

    // re-orthogonalize the tangent because averaging the results could have
    // ended up slightly non-perpindicular
    tangent = normalize(tangent - dot(tangent, normal) * normal);

    // compute the bitangent and matrix for tangent-space transformation
    vec3 bitangent = cross(tangent, normal);
    mat3 tbn = mat3(tangent, bitangent, normal);

    // obtain normal from normal map in range [0,1]
    vec3 retrievedNormal = texture(texNormal, texCoord).rgb;
    // transform normal vector to range [-1,1]
    retrievedNormal = retrievedNormal * 2.0 - 1.0;

    // transform our tangent-space sampled normal vector into world-space
    vec3 newNormal = tbn * retrievedNormal;
    newNormal = normalize(newNormal);
    return newNormal;
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

// technique taken from https://www.3dgep.com/texturing-lighting-directx-11/#Spotlight_Cone
float calcSpotCone(SpotLight light, vec3 L) {
    float minCos = light.cosAngle;
    float maxCos = (minCos + 1.0f) / 2.0f;
    float cosAngle = dot(light.direction, -L );

    return smoothstep( minCos, maxCos, cosAngle ); 
}

LightingResult calcDirectionalLight(DirectionalLight light, vec3 V, vec3 N) {
    LightingResult result = LightingResult(vec3(0.0), vec3(0.0));

    vec3 L = normalize(-light.direction);

    result.diffuse = light.color * calcDiffuse(L, N);
    result.specular = light.color * calcSpecular(V, L, N, material.shininess);

    return result;
}

LightingResult calcPointLight(PointLight light, vec3 V, vec3 P, vec3 N) {
    LightingResult result = LightingResult(vec3(0.0), vec3(0.0));
    
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
    LightingResult result = LightingResult(vec3(0.0), vec3(0.0));
    
    vec3 L = light.position - P;
    float distance = length(L);
    L = L / distance; // normalize

    // spotlight soft edges
    float spotlightFactor = calcSpotCone(light, L);
    if(spotlightFactor == 0.0) {
        // early exit if we're beyond the outer angle
        return result;
    }

    // attenuation
    float attenuation = calcAttenuation(light.constant, light.linear, light.quadratic, distance);

    result.diffuse = light.color * calcDiffuse(L, N) * attenuation * spotlightFactor;
    result.specular = light.color * calcSpecular(V, L, N, material.shininess) * attenuation * spotlightFactor;

    return result;
}

LightingResult calcTotalLighting(vec3 V, vec3 P, vec3 N) {
    LightingResult result = LightingResult(vec3(0.0), vec3(0.0));

    LightingResult directional = calcDirectionalLight(dirLight, V, N);
    result.diffuse += directional.diffuse;
    result.specular += directional.specular;

    for(int i = 0; i < pointLights.length(); i++) {
        LightingResult point = calcPointLight(pointLights[i], V, P, N);
        result.diffuse += point.diffuse;
        result.specular += point.specular;
    }

    for(int i = 0; i < spotLights.length(); i++) {
        LightingResult spot = calcSpotLight(spotLights[i], V, P, N);
        result.diffuse += spot.diffuse;
        result.specular += spot.specular;
    }

    return result;
}
