#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>

namespace gyo {

struct Geometry {
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> tangents;
    std::vector<unsigned int> indices;

    void ComputeTangents() {
        tangents.resize(positions.size());

        // variables for calculating tangent/bitangent
        unsigned int i0, i1, i2;
        glm::vec3 edge1, edge2;
        glm::vec2 deltaUV1, deltaUV2;
        glm::vec3 tangent, bitangent;

        // accumulate per-vertex the tangent and bitangent of each associated tri

        int size = indices.size();
        for(int i = 0; i < size; i += 3) {
            i0 = indices[i];
            i1 = indices[i+1];
            i2 = indices[i+2];

            glm::vec3 p0 = positions[i0];
            glm::vec3 p1 = positions[i1];
            glm::vec3 p2 = positions[i2];

            glm::vec2 uv0 = texCoords[i0];
            glm::vec2 uv1 = texCoords[i1];
            glm::vec2 uv2 = texCoords[i2];

            // compute our variables needed for the change-of-basis matrix
            edge1 = p1 - p0;
            edge2 = p2 - p0;
            deltaUV1 = uv1 - uv0;
            deltaUV2 = uv2 - uv0;

            // fractional part
            float det = deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x;
            if (glm::epsilonEqual(det, 0.0f, 1e-6f)) {
                continue;
            }
            float invDet = 1.0f / det;

            // now do matrix multiplication to generate the tangent and bitangent
            tangent =   invDet * ( deltaUV2.y * edge1 - deltaUV1.y * edge2);
            bitangent = invDet * (-deltaUV2.x * edge1 + deltaUV1.x * edge2);

            // accumulate our results

            tangents[i0] += tangent;
            tangents[i1] += tangent;
            tangents[i2] += tangent;
        }

        // normalize the results per vert to account for verts that are shared
        // between multiple triangles

        for(int i = 0; i < tangents.size(); i++) {
            tangents[i] = glm::normalize(tangents[i]);
        }
    }
};

} // namespace gyo

#endif // GEOMETRY_H
