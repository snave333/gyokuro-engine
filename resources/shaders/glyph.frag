out vec4 FragColor;

in VS_OUT {
    vec2 texCoord;
    vec4 color;
} fs_in;

uniform sampler2D glyphAtlas;

void main()
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(glyphAtlas, fs_in.texCoord).r);
    FragColor = fs_in.color * sampled;
}
