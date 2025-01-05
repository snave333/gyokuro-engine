#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform vec3 color;
uniform sampler2D tex;

void main()
{
    FragColor = vec4(texture(tex, TexCoord).xyz * color, 1);
}
