#version 330 core

out vec4 FragColor;

in VS_OUT {
    vec3 fragPos;
    vec3 normal;
    vec2 texCoord;
    vec3 tangent;
} fs_in;

vec3 calcNormal(vec3 wsNormal, vec3 wsTangent, sampler2D texNormal, vec2 texCoord);

#include "lighting.glsl"
#include "camera.glsl"

void main()
{
    vec2 mappedTexCoord = fs_in.texCoord;

    vec3 V = normalize(viewPos.xyz - fs_in.fragPos);
    vec3 N = calcNormal(fs_in.normal, fs_in.tangent, material.normalMap, mappedTexCoord);
    vec3 P = fs_in.fragPos;

    LightingResult totalLighting = calcTotalLighting(V, P, N);

    vec3 ambient = globalAmbient;
    vec4 diffuse = vec4(totalLighting.diffuse, 1) * material.diffuse * texture(material.diffuseMap, mappedTexCoord);
    vec3 specular = totalLighting.specular * material.specular * texture(material.specularMap, mappedTexCoord).rgb;
    vec3 result = ambient + diffuse.rgb + specular;

    FragColor = vec4(result, diffuse.a);
}

vec3 calcNormal(vec3 wsNormal, vec3 wsTangent, sampler2D texNormal, vec2 texCoord) {
    vec3 normal = normalize(wsNormal);
    vec3 tangent = normalize(wsTangent);

    // re-orthogonalize the tangent because averaging the results could have
    // ended up slightly non-perpindicular
    tangent = normalize(tangent - dot(tangent, normal) * normal);

    // compute the bitangent and matrix for tangent-space transformation
    vec3 bitangent = cross(tangent, normal);
    mat3 tbn = mat3(tangent, bitangent, normal);

    // obtain normal from normal map in range [0,1]
    vec3 retrievedNormal = texture(texNormal, texCoord).rgb;
    // transform normal vector to range [-1,1]
    retrievedNormal = retrievedNormal * 2.0 - 1.0;

    // transform our tangent-space sampled normal vector into world-space
    vec3 newNormal = tbn * retrievedNormal;
    newNormal = normalize(newNormal);
    return newNormal;
}
