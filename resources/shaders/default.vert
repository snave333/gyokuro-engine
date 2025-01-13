#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;

layout (std140) uniform Camera {
    mat4 projection;
    mat4 view;
    vec4 viewPos; // .xyz: camera position in world space, .w = 0
};

out VS_OUT {
    vec3 fragPos;
    vec3 normal;
    vec2 texCoord;
    vec3 tangent;
} vs_out;

uniform mat4 model;
uniform mat4 normalMatrix;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    vs_out.texCoord = aTexCoord;
    // transform our position and normal into world space
    vs_out.fragPos = vec3(model * vec4(aPos, 1.0));
    vs_out.normal = vec3(normalMatrix * vec4(aNormal, 1.0));
    vs_out.tangent = vec3(normalMatrix * vec4(aTangent, 1.0));
}
