#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
} vs_out;

layout (std140) uniform Camera {
    mat4 projection;
    mat4 view;
};

uniform mat4 model;
uniform mat4 normalMatrix;

void main()
{
    mat4 mvp = projection * view * model;
    gl_Position = mvp * vec4(aPos, 1.0);

    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
    vs_out.Normal = vec3(normalMatrix * vec4(aNormal, 1.0));
    vs_out.TexCoord = aTexCoord;
}
