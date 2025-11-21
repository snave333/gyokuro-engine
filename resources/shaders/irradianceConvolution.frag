// defined in ShaderLoader: SAMPLE_DELTA 0.025

out vec4 FragColor;

// world vector acts as the normal
in vec3 worldPos;

#include "common.glsl"

uniform samplerCube environmentMap;

void main()
{
    vec3 N = normalize(worldPos);

    vec3 up    = vec3(0.0, 1.0, 0.0);
    vec3 right = normalize(cross(up, N));
    up         = normalize(cross(N, right));
    
    // calculation the approximation of all incoming radiance of the environment

    vec3 irradiance = vec3(0.0);

    int numSamples = 0;
    for(float phi = 0.0; phi < 2.0 * PI; phi += SAMPLE_DELTA) {
        for(float theta = 0.0; theta < 0.5 * PI; theta += SAMPLE_DELTA) {
            // spherical to cartesian (in tangent space)
            vec3 tangentSample = vec3(
                sin(theta) * cos(phi),
                sin(theta) * sin(phi),
                cos(theta));
            
            // tangent space to world
            vec3 sampleVec = 
                tangentSample.x * right +
                tangentSample.y * up +
                tangentSample.z * N; 

            // scale the sampled color value due to the light being weaker at larger angles
            // and to account for the smaller sample areas in the higher hemisphere areas
            irradiance += texture(environmentMap, sampleVec).rgb * cos(theta) * sin(theta);
            numSamples++;
        }
    }
    irradiance = PI * irradiance * (1.0 / float(numSamples));

    FragColor = vec4(irradiance, 1.0);
}
