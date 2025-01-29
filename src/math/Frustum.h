#ifndef FRUSTUM_H
#define FRUSTUM_H

#include <glm/glm.hpp>
#include <iostream>
#include <array>

#include <math/AABB.h>
#include <math/Plane.h>

enum class FrustumTestResult {
    OUTSIDE,
    INSIDE,
    INTERSECTING
};

struct Frustum {
    std::array<Plane, 6> planes;

    /**
     * Used for more efficient AABB frustum culling
     */
    std::array<std::pair<int, int>, 6> ComputeAABBTestLUT() const {
        std::array<std::pair<int, int>, 6> LUT = {};

        for(int i = 0; i < 6; i++) {
            // compute bitfield for the plane normal
            int bitfield = 0;
            if (planes[i].normal.x > 0) bitfield |= 1;
            if (planes[i].normal.y > 0) bitfield |= 2;
            if (planes[i].normal.z > 0) bitfield |= 4;

            // the indices of our n-vertex and p-vertex
            LUT[i] = { bitfield, bitfield ^ 7 };
        }

        return LUT;
    }

#if 0
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
                return FrustumTestResult::OUTSIDE;
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

        return intersects ? FrustumTestResult::INTERSECTING : FrustumTestResult::INSIDE;
    }

    /**
     * Same as above, except optimized with bitfields
     */
    FrustumTestResult TestAABBIntersection(
        const AABB& bounds,
        const std::array<glm::vec3, 8>& boundsLUT,
        const std::array<std::pair<int, int>, 6>& frustumLUT) const
    {
        bool intersects = false;

        for(int i = 0; i < 6; i++) {
            const Plane& plane = planes[i];

            // determine the n-vertex relative to the plane normal
            const glm::vec3& vn = boundsLUT[frustumLUT[i].first];

            // test the n-vertex
            float a = glm::dot(plane.normal, vn) + plane.distance;
            if (a < 0) {
                // if the n-vertex is outside, the box is outside
                return FrustumTestResult::OUTSIDE;
            }

            // determine the p-vertex relative to the plane normal
            const glm::vec3& vp = boundsLUT[frustumLUT[i].second];

            // test the p-vertex
            float b = glm::dot(plane.normal, vp) + plane.distance;
            if (b < 0) {
                // if p-vertex is inside, we might be intersecting
                intersects = true;
            }
        }

        return intersects ? FrustumTestResult::INTERSECTING : FrustumTestResult::INSIDE;
    }
#endif

    /**
     * Same as above, except optimized with plane-coherency
     */
    FrustumTestResult TestAABBIntersection(
        const AABB& bounds,
        const std::array<glm::vec3, 8>& boundsLUT,
        const std::array<std::pair<int, int>, 6>& frustumLUT,
        int* planeFailIdx) const
    {
        bool intersects = false;

        for(int d = 0; d < 6; d++) {
            // start testing at the last frustum plane this AABB failed at
            int i = (d + *planeFailIdx) % 6;
            
            const Plane& plane = planes[i];

            // determine the n-vertex relative to the plane normal
            const glm::vec3& vn = boundsLUT[frustumLUT[i].first];

            // test the n-vertex
            float a = glm::dot(plane.normal, vn) + plane.distance;
            if (a < 0) {
                // update our last failed frustum plane for faster rejection
                *planeFailIdx = d;

                // if the n-vertex is outside, the box is outside
                return FrustumTestResult::OUTSIDE;
            }

            // determine the p-vertex relative to the plane normal
            const glm::vec3& vp = boundsLUT[frustumLUT[i].second];

            // test the p-vertex
            float b = glm::dot(plane.normal, vp) + plane.distance;
            if (b < 0) {
                // if p-vertex is inside, we might be intersecting
                intersects = true;
            }
        }

        return intersects ? FrustumTestResult::INTERSECTING : FrustumTestResult::INSIDE;
    }
};

#endif // FRUSTUM_H
