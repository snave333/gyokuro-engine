// #defined in IBLEnvironmentLoader: ENV_MAP_RESOLUTION 512.0

out vec4 FragColor;

in vec3 worldPos;

#include "brdf.glsl"

// small deterministic hash / RNG for jitter
float hash12(vec2 p) {
    // cheap 2D->1D hash (deterministic)
    return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453123);
}

uniform samplerCube environmentMap;
uniform float roughness;
uniform int mipLevel;

const uint MIN_SAMPLES = 256u;      // minimum samples at low roughness
const uint MAX_SAMPLES = 8192u;     // maximum samples at high roughness

void main()
{
    vec3 N = normalize(worldPos);    
    vec3 V = N;

    // adapt sample count: few samples at low mip, many at high
    uint SAMPLE_COUNT = MIN_SAMPLES;
    for(int i = 0; i < mipLevel; i++) {
        SAMPLE_COUNT = min(SAMPLE_COUNT * 2u, MAX_SAMPLES);
    }

    float totalWeight = 0.0;
    vec3 prefilteredColor = vec3(0.0);

    // per-fragment jitter seed to decorrelate between fragments / roughness levels
    vec2 jitterSeed = vec2(hash12(N.xy + roughness), hash12(N.zy + roughness * 1.37));

    for(uint i = 0u; i < SAMPLE_COUNT; ++i) {
        vec2 Xi = Hammersley(i, SAMPLE_COUNT);

        // small stratified jitter to reduce structured artifacts
        // jitter magnitude scaled by 1 / samples so it doesn't break low-discrepancy property
        vec2 jitter = vec2(hash12(vec2(float(i), jitterSeed.x)), hash12(vec2(float(i) * 1.71, jitterSeed.y)));
        Xi += (jitter - 0.5) / float(SAMPLE_COUNT); 
        Xi = clamp(Xi, vec2(0.0), vec2(1.0));

        vec3 H  = ImportanceSampleGGX(Xi, N, roughness);
        vec3 L  = normalize(2.0 * dot(V, H) * H - V);

        float NdotL = max(dot(N, L), 0.0);
        if(NdotL > 0.0) {
            // sample from the environment's mip level based on roughness/pdf
            float D   = DistributionGGX(N, H, roughness);
            float NdotH = max(dot(N, H), 0.0);
            float HdotV = max(dot(H, V), 0.0);
            float pdf = D * NdotH / (4.0 * HdotV) + 1e-6;

            float resolution = ENV_MAP_RESOLUTION; // resolution of source cubemap (per face)
            float saTexel  = 4.0 * PI / (6.0 * resolution * resolution);
            float saSample = 1.0 / (float(SAMPLE_COUNT) * pdf + 1e-6);

            float mipLOD = roughness == 0.0 ? 0.0 : 0.5 * log2(saSample / saTexel);
            mipLOD = max(mipLOD, 0.0);

            vec3 sampleColor = textureLod(environmentMap, L, mipLOD).rgb;

            prefilteredColor += sampleColor * NdotL;
            totalWeight      += NdotL;
        }
    }

    // avoid divide-by-zero and fallback to something reasonable
    if (totalWeight > 1e-6) {
        prefilteredColor = prefilteredColor / totalWeight;
    } else {
        prefilteredColor = vec3(0.0);
    }

    FragColor = vec4(prefilteredColor, 1.0);
}
