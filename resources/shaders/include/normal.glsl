
vec3 calcNormal(vec3 wsNormal, vec3 wsTangent, sampler2D texNormal, vec2 texCoord) {
    vec3 normal = normalize(wsNormal);
    vec3 tangent = normalize(wsTangent);

    // re-orthogonalize the tangent because averaging the results could have
    // ended up slightly non-perpindicular
    tangent = normalize(tangent - dot(tangent, normal) * normal);

    // compute the bitangent and matrix for tangent-space transformation
    vec3 bitangent = cross(tangent, normal);
    mat3 tbn = mat3(tangent, bitangent, normal);

    // obtain normal from normal map in range [0,1]
    vec3 retrievedNormal = texture(texNormal, texCoord).rgb;
    // transform normal vector to range [-1,1]
    retrievedNormal = retrievedNormal * 2.0 - 1.0;

    // transform our tangent-space sampled normal vector into world-space
    vec3 newNormal = tbn * retrievedNormal;
    newNormal = normalize(newNormal);
    return newNormal;
}
