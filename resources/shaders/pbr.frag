#version 330 core

out vec4 FragColor;

in VS_OUT {
    vec3 fragPos;
    vec3 normal;
    vec2 texCoord;
    vec3 tangent;
} fs_in;

#include "camera.glsl"
#include "lights.glsl"
#include "pbr.glsl"

// material
uniform vec3 albedo;
uniform float metallic;
uniform float roughness;
uniform float ao;

void main()
{
    vec3 a = albedo;
    vec3 mra = vec3(metallic, roughness, ao);
    FragColor = vec4(1, 0.5, 0, 1);
}
