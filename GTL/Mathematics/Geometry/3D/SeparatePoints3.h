// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.17

#pragma once

// Separate two point sets, if possible, by computing a plane for which the
// point sets lie on opposite sides. The algorithm computes the convex hull
// of the point sets, then uses the method of separating axes to determine
// whether the two convex polyhedra are disjoint.
// https://www.geometrictools.com/Documentation/MethodOfSeparatingAxes.pdf

#include <GTL/Mathematics/Geometry/3D/ConvexHull3.h>
#include <GTL/Mathematics/Primitives/3D/Plane3.h>
#include <GTL/Mathematics/Primitives/ND/Line.h>
#include <cstddef>
#include <cstdint>
#include <set>
#include <utility>
#include <vector>

namespace gtl
{
    template <typename T>
    class SeparatePoints3
    {
    public:
        // The return value is 'true' if and only if there is a separation.
        // If 'true', the returned plane is a separating plane. The code
        // assumes that each point set has at least 4 noncoplanar points.
        static bool Execute(std::vector<Vector3<T>> const& points0,
            std::vector<Vector3<T>> const& points1, std::size_t lgNumThreads,
            Plane3<T>& separatingPlane)
        {
            // Construct the convex hull of point set 0.
            ConvexHull3<T> ch0{};
            ch0(points0, lgNumThreads);
            if (ch0.GetDimension() != 3)
            {
                return false;
            }

            // Construct the convex hull of point set 1.
            ConvexHull3<T> ch1{};
            ch1(points1, lgNumThreads);
            if (ch1.GetDimension() != 3)
            {
                return false;
            }

            auto const& hull0 = ch0.GetHull();
            auto const& hull1 = ch1.GetHull();
            std::size_t const numTriangles0 = hull0.size() / 3;
            std::size_t const numTriangles1 = hull1.size() / 3;

            // Test faces of hull 0 for possible separation of points.
            for (std::size_t i = 0; i < numTriangles0; ++i)
            {
                // Get the triangle indices.
                std::size_t i0 = hull0[3 * i];
                std::size_t i1 = hull0[3 * i + 1];
                std::size_t i2 = hull0[3 * i + 2];

                // Compute the potential separating plane.
                separatingPlane = Plane3<T>({ points0[i0], points0[i1], points0[i2] });
                GTL_RUNTIME_ASSERT(
                    !IsZero(separatingPlane.normal),
                    "Unexpected zero normal.");

                // Determine whether hull 1 is on the same side of the plane.
                std::int32_t side1 = OnSameSide(separatingPlane, hull1, points1);
                if (side1)
                {
                    // Determine on which side of the plane hull 0 lies.
                    std::int32_t side0 = WhichSide(separatingPlane, hull0, points0);
                    if (side0 * side1 <= 0)
                    {
                        // The plane separates the hulls.
                        return true;
                    }
                }
            }

            // Test faces of hull 1 for possible separation of points.
            for (std::size_t i = 0; i < numTriangles1; ++i)
            {
                // Get the triangle indices.
                std::size_t i0 = hull1[3 * i];
                std::size_t i1 = hull1[3 * i + 1];
                std::size_t i2 = hull1[3 * i + 2];

                // Compute the potential separating plane.
                separatingPlane = Plane3<T>({ points1[i0], points1[i1], points1[i2] });

                // Determine whether hull 0 is on the same side of the plane.
                std::int32_t side0 = OnSameSide(separatingPlane, hull0, points0);
                if (side0)
                {
                    // Determine on which side of the plane hull 1 lies.
                    std::int32_t side1 = WhichSide(separatingPlane, hull1, points1);
                    if (side0 * side1 <= 0)
                    {
                        // The plane separates the hulls.
                        return true;
                    }
                }
            }

            // Build the edge set for hull 0.
            std::set<std::pair<std::size_t, std::size_t>> edgeSet0{};
            for (std::size_t i = 0; i < numTriangles0; ++i)
            {
                // Get the triangle indices.
                std::size_t i0 = hull0[3 * i];
                std::size_t i1 = hull0[3 * i + 1];
                std::size_t i2 = hull0[3 * i + 2];

                // Store the edges.
                edgeSet0.insert(std::make_pair(i0, i1));
                edgeSet0.insert(std::make_pair(i0, i2));
                edgeSet0.insert(std::make_pair(i1, i2));
            }

            // Build the edge set for hull 1.
            std::set<std::pair<std::size_t, std::size_t>> edgeSet1{};
            for (std::size_t i = 0; i < numTriangles1; ++i)
            {
                // Get the triangle indices.
                std::size_t i0 = hull1[3 * i];
                std::size_t i1 = hull1[3 * i + 1];
                std::size_t i2 = hull1[3 * i + 2];

                // Store the edges.
                edgeSet1.insert(std::make_pair(i0, i1));
                edgeSet1.insert(std::make_pair(i0, i2));
                edgeSet1.insert(std::make_pair(i1, i2));
            }

            // Test planes whose normals are cross products of two edges, one
            // from each hull.
            for (auto const& e0 : edgeSet0)
            {
                // Get edge.
                Vector3<T> diff0 = points0[e0.second] - points0[e0.first];

                for (auto const& e1 : edgeSet1)
                {
                    Vector3<T> diff1 = points1[e1.second] - points1[e1.first];

                    // Compute potential separating plane.
                    separatingPlane.normal = UnitCross(diff0, diff1);
                    separatingPlane.constant = Dot(separatingPlane.normal, points0[e0.first]);
                    separatingPlane.origin = separatingPlane.constant * separatingPlane.normal;

                    // Determine whether hull 0 is on the same side of the
                    // plane.
                    std::int32_t side0 = OnSameSide(separatingPlane, hull0, points0);
                    std::int32_t side1 = OnSameSide(separatingPlane, hull1, points1);
                    if (side0 * side1 < 0)
                    {
                        // The plane separates the hulls.
                        return true;
                    }
                }
            }

            return false;
        }

    private:
        static std::int32_t OnSameSide(Plane3<T> const& plane,
            std::vector<std::size_t> const& hull, std::vector<Vector3<T>> const& points)
        {
            // Test whether all points are on the same side of the plane
            // Dot(N,X) = c.
            std::size_t const numTriangles = hull.size() / 3;
            std::size_t posSide = 0, negSide = 0;
            for (std::size_t t = 0; t < numTriangles; ++t)
            {
                for (std::size_t i = 0; i < 3; ++i)
                {
                    std::size_t v = hull[3 * t + i];
                    T c0 = Dot(plane.normal, points[v]);
                    if (c0 > plane.constant)
                    {
                        ++posSide;
                    }
                    else if (c0 < plane.constant)
                    {
                        ++negSide;
                    }

                    if (posSide > 0 && negSide > 0)
                    {
                        // The plane splits the point set.
                        return 0;
                    }
                }
            }

            return (posSide > 0 ? +1 : -1);
        }

        static std::int32_t WhichSide(Plane3<T> const& plane,
            std::vector<std::size_t> const& hull, std::vector<Vector3<T>> const& points)
        {
            // Establish which side of plane hull is on.
            std::size_t const numTriangles = hull.size() / 3;
            for (std::size_t t = 0; t < numTriangles; ++t)
            {
                for (std::size_t i = 0; i < 3; ++i)
                {
                    std::size_t v = hull[3 * t + i];
                    T c0 = Dot(plane.normal, points[v]);
                    if (c0 > plane.constant)
                    {
                        // The hull is on the positive side.
                        return +1;
                    }
                    if (c0 < plane.constant)
                    {
                        // The hull is on the negative side.
                        return -1;
                    }
                }
            }

            // The hull is a line segment and on the line.
            return 0;
        }

    private:
        friend class UnitTestSeparatePoints3;
    };
}
