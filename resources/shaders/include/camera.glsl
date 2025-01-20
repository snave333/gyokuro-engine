layout (std140) uniform Camera {
    mat4 projection;
    mat4 view;
    vec4 viewPos;       // .xyz: camera world-space position, .w = 0 [unused]
}; // total size with std140 layout: 144 bytes
