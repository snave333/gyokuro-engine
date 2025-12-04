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
    vec3 V = normalize(viewPos.xyz - fs_in.fragPos);
    vec3 N = normalize(fs_in.normal);
    vec3 P = fs_in.fragPos;

    vec3 Lo = calcRadiance(V, P, N);

#ifdef USE_IBL
    vec3 ambient = calcAmbient(V, P, N);
#else
    vec3 ambient = globalAmbient.rgb * uMaterial.albedo * uMaterial.ao;
#endif
    
    vec3 color = ambient + Lo;

    FragColor = vec4(color, 1.0);
}
