#version 330 core

layout (location = 0) in vec3 aPos;

layout (std140) uniform Camera {
    mat4 projection;
    mat4 view;
    vec4 viewPos; // .xyz: camera position in world space, .w = 0
};

void main()
{
    // the vertex array is already in world space, so we don't need the model
    // matrix
    gl_Position = projection * view * vec4(aPos, 1.0);
}
