layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec4 color;

out VS_OUT {
    vec2 texCoord;
    vec4 color;
} vs_out;

uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(aPos, 0.0, 1.0);
    
    vs_out.texCoord = aTexCoord;
    vs_out.color = color;
}
