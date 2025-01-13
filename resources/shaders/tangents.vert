#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec3 aBitangent;

layout (std140) uniform Camera {
    mat4 projection;
    mat4 view;
    vec4 viewPos; // .xyz: camera position in world space, .w = 0
};

out vec3 color;

uniform mat4 model;
uniform mat4 normalMatrix;
uniform float lineLength;

void main()
{
    // world position and directions of base vertex
    vec3 position = vec3(model * vec4(aPos, 1.0));
    vec3 offset;

    // position of final vertex
    vec3 p = vec3( 0);

    // current index of the vertex we're drawing
    int i = gl_VertexID % 6;

    // first determine our vertex position, offset in the proper direction

    switch(i) {
    case 1: // normal (+z)
        offset = normalize(vec3(normalMatrix * vec4(aNormal, 1.0)));
        p = position + offset * lineLength;
        break;
    case 3: // tangent (+x)
        offset = normalize(vec3(normalMatrix * vec4(aTangent, 1.0)));
        p = position + offset * lineLength;
        break;
    case 5: // bitangent (+y)
        offset = normalize(vec3(normalMatrix * vec4(aBitangent, 1.0)));
        p = position + offset * lineLength;
        break;
    default:
        p = position;
        break;
    }

    // now determine our line color, coded by axis

    int i2 = int(floor(i / 2.0));
    switch(i2) {
    case 0: // normal (+z)
        color = vec3(0, 0, 1);
        break;
    case 1: // tangent (+x)
        color = vec3(1, 0, 0);
        break;
    case 2: // bitangent (+y)
        color = vec3(0, 1, 0);
        break;
    }

    gl_Position = projection * view * vec4(p, 1.0);
}
