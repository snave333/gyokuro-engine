#version 330 core

out vec4 FragColor;

struct Material {
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
} fs_in;

uniform Material material;

void main()
{
    FragColor = vec4(material.diffuse, 1.0);
}
