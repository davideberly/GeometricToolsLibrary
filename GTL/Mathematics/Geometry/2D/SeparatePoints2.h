// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.13

#pragma once

// Separate two point sets, if possible, by computing a line for which the
// point sets lie on opposite sides. The algorithm computes the convex hull
// of the point sets, then uses the method of separating axes to determine
// whether the two convex polygons are disjoint.
// https://www.geometrictools.com/Documentation/MethodOfSeparatingAxes.pdf

#include <GTL/Mathematics/Geometry/2D/ConvexHull2.h>
#include <GTL/Mathematics/Primitives/ND/Line.h>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace gtl
{
    template <typename T>
    class SeparatePoints2
    {
    public:
        // The return value is 'true' if and only if there is a separation.
        // If 'true', the returned line is a separating line. The code
        // assumes that each point set has at least 3 noncollinear points.
        static bool Execute(std::vector<Vector2<T>> const& points0,
            std::vector<Vector2<T>> const& points1, Line2<T>& separatingLine)
        {
            // Construct the convex hull of point set 0.
            ConvexHull2<T> ch0{};
            ch0(points0);
            if (ch0.GetDimension() != 2)
            {
                return false;
            }

            // Construct the convex hull of point set 1.
            ConvexHull2<T> ch1{};
            ch1(points1);
            if (ch1.GetDimension() != 2)
            {
                return false;
            }

            auto const& edges0 = ch0.GetHull();
            auto const& edges1 = ch1.GetHull();

            // Test the edges of hull 0 for possible separation of points.
            for (std::size_t j1 = 0, j0 = edges0.size() - 1; j1 < edges0.size(); j0 = j1++)
            {
                // Get the edge indices.
                std::size_t i0 = edges0[j0];
                std::size_t i1 = edges0[j1];

                // Compute the potential separating line.
                separatingLine.origin = points0[i0];
                separatingLine.direction = points0[i1] - points0[i0];
                Normalize(separatingLine.direction);
                Vector2<T> lineNormal = Perp(separatingLine.direction);
                GTL_RUNTIME_ASSERT(
                    !IsZero(lineNormal),
                    "Unexpected zero normal.");

                T lineConstant = Dot(lineNormal, separatingLine.origin);

                // Determine whether hull 1 is on the same side of the line.
                std::int32_t side1 = OnSameSide(lineNormal, lineConstant, edges1, points1);
                if (side1)
                {
                    // Determine on which side of the line hull 0 lies.
                    std::int32_t side0 = WhichSide(lineNormal, lineConstant, edges0, points0);
                    if (side0 * side1 <= 0)
                    {
                        // The line separates the hulls.
                        return true;
                    }
                }
            }

            // Test edges of hull 1 for possible separation of points.
            for (std::size_t j1 = 0, j0 = edges1.size() - 1; j1 < edges1.size(); j0 = j1++)
            {
                // Look up edge (assert: i0 != i1 ).
                std::size_t i0 = edges1[j0];
                std::size_t i1 = edges1[j1];

                // Compute the potential separating line.
                separatingLine.origin = points1[i0];
                separatingLine.direction = points1[i1] - points1[i0];
                Normalize(separatingLine.direction);
                Vector2<T> lineNormal = Perp(separatingLine.direction);
                GTL_RUNTIME_ASSERT(
                    !IsZero(lineNormal),
                    "Unexpected zero normal.");

                T lineConstant = Dot(lineNormal, separatingLine.origin);

                // Determine whether hull 0 is on the same side of the line.
                std::int32_t side0 = OnSameSide(lineNormal, lineConstant, edges0, points0);
                if (side0)
                {
                    // Determine on which side of the line hull 1 lies.
                    std::int32_t side1 = WhichSide(lineNormal, lineConstant, edges1, points1);
                    if (side0 * side1 <= 0)
                    {
                        // The line separates the hulls.
                        return true;
                    }
                }
            }

            return false;
        }

    private:
        static std::int32_t OnSameSide(Vector2<T> const& lineNormal, T const& lineConstant,
            std::vector<std::size_t> const& edges, std::vector<Vector2<T>> const& points)
        {
            // Test whether all points are on the same side of the line
            // Dot(N,X) = c.
            std::size_t posSide = 0, negSide = 0;
            for (std::size_t i1 = 0, i0 = edges.size() - 1; i1 < edges.size(); i0 = i1++)
            {
                T c0 = Dot(lineNormal, points[edges[i0]]);
                if (c0 > lineConstant)
                {
                    ++posSide;
                }
                else if (c0 < lineConstant)
                {
                    ++negSide;
                }

                if (posSide > 0 && negSide > 0)
                {
                    // The line splits the point set.
                    return 0;
                }

                c0 = Dot(lineNormal, points[edges[i1]]);
                if (c0 > lineConstant)
                {
                    ++posSide;
                }
                else if (c0 < lineConstant)
                {
                    ++negSide;
                }

                if (posSide > 0 && negSide > 0)
                {
                    // The line splits the point set.
                    return 0;
                }
            }

            return (posSide > 0 ? +1 : -1);
        }

        static std::int32_t WhichSide(Vector2<T> const& lineNormal, T const& lineConstant,
            std::vector<std::size_t> const& edges, std::vector<Vector2<T>> const& points)
        {
            // Establish which side of the line the hull is on.
            for (std::size_t i1 = 0, i0 = edges.size() - 1; i1 < edges.size(); i0 = i1++)
            {
                T c0 = Dot(lineNormal, points[edges[i0]]);
                if (c0 > lineConstant)
                {
                    // The hull is on the positive side.
                    return +1;
                }
                if (c0 < lineConstant)
                {
                    // The hull is on the negative side.
                    return -1;
                }

                c0 = Dot(lineNormal, points[edges[i1]]);
                if (c0 > lineConstant)
                {
                    // The hull is on the positive side.
                    return +1;
                }
                if (c0 < lineConstant)
                {
                    // The hull is on the negative side.
                    return -1;
                }
            }

            // The hull is a line segment and on the line.
            return 0;
        }

    private:
        friend class UnitTestSeparatePoints2;
    };
}
