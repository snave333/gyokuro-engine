#version 330 core

out vec4 FragColor;

in VS_OUT {
    vec3 fragPos;
    vec3 normal;
    vec2 texCoord;
} fs_in;

uniform vec4 color;
uniform sampler2D tex;
uniform vec2 uvTiling;
uniform vec2 uvOffset;

void main()
{
    FragColor = texture(tex, fs_in.texCoord) * color;
}
