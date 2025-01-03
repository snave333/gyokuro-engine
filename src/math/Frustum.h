#ifndef FRUSTUM_H
#define FRUSTUM_H

#include <glm/glm.hpp>
#include <iostream>
#include <array>

#include <math/AABB.h>
#include <math/Plane.h>

enum FrustumTestResult {
    Outside,
    Inside,
    Intersecting
};

struct Frustum {
public:
    std::array<Plane, 6> planes;

    /**
     * Adapted from https://www.cse.chalmers.se/~uffe/vfc_bbox.pdf
     */
    FrustumTestResult TestAABBIntersection(const AABB& box) const {
        bool intersects = false;

        for (const auto& plane : planes) {
            // determine the n-vertex relative to the plane normal
            glm::vec3 vn(
                (plane.normal.x > 0) ? box.max.x : box.min.x,
                (plane.normal.y > 0) ? box.max.y : box.min.y,
                (plane.normal.z > 0) ? box.max.z : box.min.z
            );

            // test the n-vertex
            float a = glm::dot(plane.normal, vn) + plane.distance;
            if (a < 0) {
                // if the n-vertex is outside, the box is outside
                return Outside;
            }

            // determine the p-vertex relative to the plane normal
            glm::vec3 vp(
                (plane.normal.x > 0) ? box.min.x : box.max.x,
                (plane.normal.y > 0) ? box.min.y : box.max.y,
                (plane.normal.z > 0) ? box.min.z : box.max.z
            );

            // test the p-vertex
            float b = glm::dot(plane.normal, vp) + plane.distance;
            if (b < 0) {
                // if p-vertex is inside, we might be intersecting
                intersects = true;
            }
        }

        return intersects ? Intersecting : Inside;
    }
};

#endif // FRUSTUM_H
