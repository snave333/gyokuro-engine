#version 330 core

out vec4 FragColor;

in VS_OUT {
    vec3 fragPos;
    vec3 normal;
    vec2 texCoord;
    vec3 tangent;
} fs_in;

#include "camera.glsl"
#include "physical_lighting.glsl"

void main()
{
    vec3 V = normalize(viewPos.xyz - fs_in.fragPos);
    vec3 N = normalize(fs_in.normal);
    vec3 P = fs_in.fragPos;

    vec3 Lo = calcIrradiance(V, P, N);

    vec3 ambient = globalAmbient.rgb * albedo * ao;
    vec3 color = ambient + Lo;

    // gamma correct?
    // color = color / (color + vec3(1.0));
    // color = pow(color, vec3(1.0 / 2.2));

    FragColor = vec4(color, 1.0);
}
