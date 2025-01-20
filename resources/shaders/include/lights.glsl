// direct lighting defines

#define MAX_POINT_LIGHTS 4
#define MAX_SPOT_LIGHTS 4

// direct lighting types

struct DirectionalLight {
    vec3 direction;
    vec3 color;
}; // total size with std140 layout: 32 bytes

struct PointLight {
    vec3 position;
    vec3 color;
    vec3 attenuation;       // .r: constant, .g: linear, .b: quadratic, .a: 0
}; // total size with std140 layout: 48 bytes

struct SpotLight {
    vec3 position;
    vec3 direction;
    vec3 color;
    vec3 attenuation;       // .r: constant, .g: linear, .b: quadratic, .a: 0
    // spotlight
    float cosAngle;
}; // total size with std140 layout: 80 bytes

// our uniforms

// layout (std140) uniform Lighting {
// };
uniform vec3 globalAmbient;
uniform DirectionalLight dirLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];
uniform int numPointLights;
uniform int numSpotLights;
