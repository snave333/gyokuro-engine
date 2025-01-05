#version 330 core

out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
} fs_in;

uniform vec3 color;
uniform sampler2D tex;

void main()
{
    FragColor = vec4(texture(tex, fs_in.TexCoord).xyz * color, 1);
}
