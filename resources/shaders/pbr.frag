out vec4 FragColor;

in VS_OUT {
    vec3 fragPos;
    vec3 normal;
    vec2 texCoord;
    vec3 tangent;
    vec4 color;
} fs_in;

#include "camera.glsl"
#include "physical_lighting.glsl"

void main()
{
    PhysicalMaterial material = getMaterialProps(
        fs_in.normal,
        fs_in.texCoord,
        fs_in.tangent
    );

    vec3 V = normalize(viewPos.xyz - fs_in.fragPos);
    vec3 N = normalize(material.worldNormal);
    vec3 P = fs_in.fragPos;

    vec3 Lo = calcRadiance(V, P, N, material);

#ifdef USE_IBL
    vec3 ambient = calcAmbient(V, P, N, material);
#else
    vec3 ambient = globalAmbient.rgb * material.albedo * material.ao;
#endif
    
    vec3 color = ambient + Lo + material.emissive;

    FragColor = vec4(color, 1.0);
}
