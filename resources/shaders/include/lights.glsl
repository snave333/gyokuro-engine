// direct lighting defines

#define MAX_POINT_LIGHTS 4
#define MAX_SPOT_LIGHTS 4

// direct lighting types

struct DirectionalLight {
    vec4 direction;                             // .xyz: normalized light direction, .w: 0 [unused]
    vec4 color;                                 // .rgb: light color, .a: 0 [unused]
}; // total size with std140 layout: 32 bytes

struct PointLight {
    vec4 position;                              // .xyz: light world-space position, .w: 0 [unused]
    vec4 color;                                 // .rgb: light color, .a: 0 [unused]
    vec4 attenuation;                           // .r: constant, .g: linear, .b: quadratic, .a: 0 [unused]
}; // total size with std140 layout: 48 bytes

struct SpotLight {
    vec4 position;                              // .xyz: light world-space position, .w: 0 [unused]
    vec4 direction;                             // .xyz: normalized light direction, .w: 0 [unused]
    vec4 color;                                 // .rgb: light color, .a: 0 [unused]
    vec4 attenuation;                           // .r: constant, .g: linear, .b: quadratic, .a: 0 [unused]
    float cosAngle;
}; // total size with std140 layout: 80 bytes

// our uniforms

layout (std140) uniform Lights {
    vec4 globalAmbient;                         // .rgb: ambient light color, .a: 0 [unused]
    DirectionalLight dirLight;                  // 32 bytes
    PointLight pointLights[MAX_POINT_LIGHTS];   // 192 bytes (48 * 4)
    SpotLight spotLights[MAX_SPOT_LIGHTS];      // 320 bytes (80 * 4)
    int numPointLights;                         // 4 bytes
    int numSpotLights;                          // 4 bytes
    // 8 bytes of padding
}; // total size with std140 layout: 576 bytes
