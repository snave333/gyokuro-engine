// This modified version of the Gooch lighting model was originally
// demonstrated in the book "Realtime Rendering, Fourth Edition", and this
// implementation currently only works with point lights.

out vec4 FragColor;

in VS_OUT {
    vec3 fragPos;
    vec3 normal;
    vec2 texCoord;
    vec3 tangent;
    vec4 color;
} fs_in;

vec3 lit(vec3 L, vec3 N, vec3 V);

#include "camera.glsl"
#include "lights.glsl"

uniform vec3 coolColor;
uniform vec3 warmColor;

void main()
{
    vec3 V = normalize(viewPos.xyz - fs_in.fragPos);
    vec3 N = normalize(fs_in.normal);
    vec3 P = fs_in.fragPos;

    vec3 unlit = coolColor * 0.5;

    vec4 result = vec4(unlit, 1);

    for(int i = 0; i < min(numPointLights, MAX_POINT_LIGHTS); i++) {
        vec3 L = normalize(pointLights[i].position.xyz - P);
        float NdotL = max(dot(N, L), 0.0);

        result.rgb += NdotL * pointLights[i].color.rgb * lit(L, N, V);
    }

    FragColor = result;
}

vec3 lit(vec3 L, vec3 N, vec3 V) {
    vec3 R = reflect(-L, N);
    float s = clamp(100.0 * dot(R, V) - 97.0, 0.0, 1.0);
    
    vec3 highlightColor = vec3(2, 2, 2);

    return mix(warmColor, highlightColor, s);
}
