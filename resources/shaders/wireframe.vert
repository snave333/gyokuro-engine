layout (location = 0) in vec3 aPos;

#include "camera.glsl"

void main()
{
    // the vertex array is already in world space, so we don't need the model
    // matrix
    gl_Position = projection * view * vec4(aPos, 1.0);
}
