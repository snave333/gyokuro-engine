#version 330 core

out vec4 FragColor;

in VS_OUT {
    vec3 fragPos;
    vec3 normal;
    vec2 texCoord;
    vec3 tangent;
} fs_in;

#include "normal.glsl"
#include "camera.glsl"
#include "phong_lighting.glsl"

void main()
{
    vec3 V = normalize(viewPos.xyz - fs_in.fragPos);
    vec3 N = calcNormal(fs_in.normal, fs_in.tangent, material.normalMap, fs_in.texCoord);
    vec3 P = fs_in.fragPos;

    LightingResult totalLighting = calcTotalLighting(V, P, N);

    vec3 ambient = globalAmbient.rgb;
    vec4 diffuse = vec4(totalLighting.diffuse, 1) * material.diffuse * texture(material.diffuseMap, fs_in.texCoord);
    vec3 specular = totalLighting.specular * material.specular * texture(material.specularMap, fs_in.texCoord).rgb;
    vec3 result = ambient + diffuse.rgb + specular;

    FragColor = vec4(result, diffuse.a);
}
