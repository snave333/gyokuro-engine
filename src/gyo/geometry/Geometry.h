#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <gyo/mesh/Vertex.h>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>

namespace gyo {

struct Geometry {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    void ComputeTangents() {
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

            Vertex& v0 = vertices[i0];
            Vertex& v1 = vertices[i1];
            Vertex& v2 = vertices[i2];

            // compute our variables needed for the change-of-basis matrix
            edge1 = v1.position - v0.position;
            edge2 = v2.position - v0.position;
            deltaUV1 = v1.texCoord - v0.texCoord;
            deltaUV2 = v2.texCoord - v0.texCoord;

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

            v0.tangent += tangent;
            v1.tangent += tangent;
            v2.tangent += tangent;
        }

        // normalize the results per vert to account for verts that are shared
        // between multiple triangles

        for(auto& vertex : vertices) {
            vertex.tangent = glm::normalize(vertex.tangent);
        }
    }
};

} // namespace gyo

#endif // GEOMETRY_H
