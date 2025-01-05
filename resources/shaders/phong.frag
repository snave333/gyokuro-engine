#version 330 core

struct Material {
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform Material material;

void main()
{
    FragColor = vec4(material.diffuse, 1.0);
}
