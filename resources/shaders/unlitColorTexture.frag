#version 330 core

out vec4 FragColor;

in VS_OUT {
    vec3 fragPos;
    vec3 normal;
    vec2 texCoord;
} fs_in;

uniform vec3 color;
uniform sampler2D tex;

void main()
{
    FragColor = vec4(texture(tex, fs_in.texCoord).xyz * color, 1);
}
