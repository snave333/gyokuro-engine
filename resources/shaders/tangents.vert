#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;

#include "camera.glsl"

out VS_OUT {
    vec3 fragPos;
    vec3 normal;
    vec3 tangent;
} vs_out;

uniform mat4 model;
uniform mat4 normalMatrix;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    // transform our position and normal into world space
    vs_out.fragPos = vec3(model * vec4(aPos, 1.0));
    vs_out.normal = vec3(normalMatrix * vec4(aNormal, 1.0));
    vs_out.tangent = vec3(normalMatrix * vec4(aTangent, 1.0));
}
