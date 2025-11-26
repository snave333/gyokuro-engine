
const float PI = 3.14159265359;

float luminance(vec3 linearRGB) {
    const vec3 lumCoeffs = vec3(0.2126, 0.7152, 0.0722);
    return dot(linearRGB, lumCoeffs);
}
