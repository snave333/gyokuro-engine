layout (std140) uniform Camera {
    mat4 projection;
    mat4 view;
    vec4 viewPos; // .xyz: camera position in world space, .w = 0
};
