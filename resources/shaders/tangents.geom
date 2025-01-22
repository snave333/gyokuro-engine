#version 330 core

layout (points) in;
layout (line_strip, max_vertices = 6) out;

#include "camera.glsl"

in VS_OUT {
    vec3 fragPos;
    vec3 normal;
    vec3 tangent;
} gs_in[];

out vec3 color; // pass color to the fragment shader

uniform float lineLength;

void main()
{
    vec3 position = gs_in[0].fragPos;
    vec3 normal = gs_in[0].normal;
    vec3 tangent = gs_in[0].tangent;
    vec3 bitangent = cross(normal, tangent);

    vec4 clipSpacePosition = projection * view * vec4(position, 1.0);

    // normal line (blue)

    gl_Position = clipSpacePosition;
    color = vec3(0.0, 0.0, 1.0);
    EmitVertex();

    vec4 normalEnd = projection * view * vec4(position + normal * lineLength, 1.0);
    gl_Position = normalEnd;
    color = vec3(0.0, 0.0, 1.0);
    EmitVertex();
    EndPrimitive();

    // tangent line (red)

    gl_Position = clipSpacePosition;
    color = vec3(1.0, 0.0, 0.0);
    EmitVertex();

    vec4 tangentEnd = projection * view * vec4(position + tangent * lineLength, 1.0);
    gl_Position = tangentEnd;
    color = vec3(1.0, 0.0, 0.0);
    EmitVertex();
    EndPrimitive();

    // bitangent line (green)

    gl_Position = clipSpacePosition;
    color = vec3(0.0, 1.0, 0.0);
    EmitVertex();

    vec4 bitangentEnd = projection * view * vec4(position + bitangent * lineLength, 1.0);
    gl_Position = bitangentEnd;
    color = vec3(0.0, 1.0, 0.0);
    EmitVertex();
    EndPrimitive();
}
