#version 330 core

out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D bitmap;
uniform vec3 color;

void main()
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(bitmap, texCoord).r);
    FragColor = vec4(color, 1.0) * sampled;
}
