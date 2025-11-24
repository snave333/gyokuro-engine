layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec4 aColor;

#include "camera.glsl"

out VS_OUT {
    vec3 fragPos;
    vec3 normal;
    vec2 texCoord;
    vec3 tangent;
    vec4 color;
} vs_out;

uniform mat4 model;
uniform mat4 normalMatrix;
uniform vec4 uvTilingOffset;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    vs_out.texCoord = aTexCoord * uvTilingOffset.xy + uvTilingOffset.zw;

    // transform our position and normal into world space
    vs_out.fragPos = vec3(model * vec4(aPos, 1.0));
    vs_out.normal = vec3(normalMatrix * vec4(aNormal, 1.0));
    vs_out.tangent = vec3(normalMatrix * vec4(aTangent, 1.0));

    vs_out.color = aColor;
}
