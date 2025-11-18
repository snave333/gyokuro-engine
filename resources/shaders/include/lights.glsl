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
}; // total size with std140 layout: 32 bytes

struct SpotLight {
    vec4 position;                              // .xyz: light world-space position, .w: 0 [unused]
    vec4 direction;                             // .xyz: normalized light direction, .w: 0 [unused]
    vec4 color;                                 // .rgb: light color, .a: 0 [unused]
    float cosAngle;                             // 4 bytes
    // 12 bytes padding
}; // total size with std140 layout: 64 bytes

// our uniforms

layout (std140) uniform Lights {
    vec4 globalAmbient;                         // .rgb: ambient light color, .a: 0 [unused]
    DirectionalLight dirLight;                  // 32 bytes
    PointLight pointLights[MAX_POINT_LIGHTS];   // 128 bytes (32 * 4)
    SpotLight spotLights[MAX_SPOT_LIGHTS];      // 256 bytes (64 * 4)
    int numPointLights;                         // 4 bytes
    // 4 bytes padding
    int numSpotLights;                          // 4 bytes
    // 4 bytes padding
}; // total size with std140 layout: 448 bytes
