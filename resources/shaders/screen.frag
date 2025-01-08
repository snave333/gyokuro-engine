#version 330 core

out vec4 FragColor;
  
in vec2 texCoord;

uniform sampler2D screenTexture;

void main()
{
    float gamma = 2.2;
    vec3 hdrColor = texture(screenTexture, texCoord).rgb;

    // reinhard tone mapping
    vec3 mapped = hdrColor / (hdrColor + vec3(1.0));
    // gamma correction
    mapped = pow(mapped, vec3(1.0 / gamma));

    FragColor = vec4(mapped, 1.0);
}
