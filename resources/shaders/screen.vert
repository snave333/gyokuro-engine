#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 texCoord;

void main()
{
    gl_Position = vec4(aPos.xyz, 1);
    texCoord = aTexCoords;
}
