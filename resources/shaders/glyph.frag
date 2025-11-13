out vec4 FragColor;

in VS_OUT {
    vec2 texCoord;
    vec4 color;
} fs_in;

uniform sampler2D glyphAtlas;
uniform float pixelRange;

float median(float r, float g, float b)
{
    return max(min(r, g), min(max(r, g), b));
}

float screenPxRange()
{
    vec2 unitRange = vec2(pixelRange) / vec2(textureSize(glyphAtlas, 0));
    vec2 screenTexSize = vec2(1.0) / fwidth(fs_in.texCoord);
    return max(0.5 * dot(unitRange, screenTexSize), 1.0);
}

void main()
{
    vec4 bgColor = vec4(0.0);
    vec4 fgColor = vec4(1.0);

    vec3 msd = texture(glyphAtlas, fs_in.texCoord).rgb;
    float dist = median(msd.r, msd.g, msd.b);
    float screenPxDistance = screenPxRange() * (dist - 0.5);
    float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);

    FragColor = vec4(fs_in.color.rgb, opacity * fs_in.color.a);
}
