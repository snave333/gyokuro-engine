#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform bool applyGammaCorrection;

void main()
{
    vec4 color = texture(screenTexture, TexCoords);

    if(applyGammaCorrection) {
        float gamma = 2.2;
        FragColor = vec4(pow(color.rgb, vec3(1.0 / gamma)), color.a);
    }
    else {
        FragColor = color;
    }
}
