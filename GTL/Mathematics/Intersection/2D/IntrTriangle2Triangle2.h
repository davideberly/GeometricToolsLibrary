// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

// The test-intersection queries are based on the document
// https://www.geometrictools.com/Documentation/MethodOfSeparatingAxes.pdf
// The find-intersection query for stationary triangles is based on clipping
// one triangle against the edges of the other to compute the intersection
// set (if it exists). The find-intersection query for moving triangles is
// based on the previously mentioned document about the method of separating
// axes.

#include <GTL/Mathematics/Intersection/ND/IntrConvexPolygonHyperplane.h>
#include <GTL/Mathematics/Primitives/ND/Triangle.h>
#include <cstddef>
#include <cstdint>
#include <utility>
#include <vector>

namespace gtl
{
    // Test whether two triangles intersect using the method of separating
    // axes. The set of intersection, if it exists, is not computed. The
    // input triangles' vertices must be counterclockwise ordered.
    template <typename T>
    class TIQuery<T, Triangle2<T>, Triangle2<T>>
    {
    public:
        struct Output
        {
            Output()
                :
                intersect(false)
            {
            }

            bool intersect;
        };

        Output operator()(Triangle2<T> const& triangle0, Triangle2<T> const& triangle1)
        {
            Output output{};
            output.intersect =
                !Separated(triangle0, triangle1) &&
                !Separated(triangle1, triangle0);
            return output;
        }

    protected:
        // The triangle vertices are projected to t-values for the line P+t*D.
        // The D-vector is nonzero but does not have to be unit length. The
        // return value is +1 if all t >= 0, -1 if all t <= 0, but 0 otherwise
        // in which case the line splits the triangle into two subtriangles,
        // each of positive area.
        std::int32_t WhichSide(Triangle2<T> const& triangle, Vector2<T> const& P,
            Vector2<T> const& D) const
        {
            std::size_t positive = 0, negative = 0;
            for (std::size_t i = 0; i < 3; ++i)
            {
                T t = Dot(D, triangle.v[i] - P);
                if (t > C_<T>(0))
                {
                    ++positive;
                }
                else if (t < C_<T>(0))
                {
                    --negative;
                }

                if (positive && negative)
                {
                    // The triangle has vertices strictly on both sides of
                    // the line, so the line splits the triangle into two
                    // subtriangles each of positive area.
                    return 0;
                }
            }

            // Either positive > 0 or negative > 0 but not both are positive.
            return (positive > 0 ? +1 : -1);
        }

        bool Separated(Triangle2<T> const& triangle0, Triangle2<T> const& triangle1) const
        {
            // Test edges of triangle0 for separation. Because of the
            // counterclockwise ordering, the projection interval for
            // triangle0 is [T,0] for some T < 0. Determine whether
            // triangle1 is on the positive side of the line; if it is, the
            // triangles are separated.
            for (std::size_t i0 = 2, i1 = 0; i1 < 3; i0 = i1++)
            {
                // The potential separating axis is P + t * D.
                Vector2<T> P = triangle0.v[i0];
                Vector2<T> D = Perp(triangle0.v[i1] - triangle0.v[i0]);
                if (WhichSide(triangle1, P, D) > 0)
                {
                    // The triangle1 projection interval is [a,b] where a > 0,
                    // so the triangles are separated.
                    return true;
                }
            }
            return false;
        }

    private:
        friend class UnitTestIntrTriangle2Triangle2;
    };

    // Find the convex polygon, segment or point of intersection of two
    // triangles. The input triangles' vertices must be counterclockwise
    // ordered.
    template <typename T>
    class FIQuery<T, Triangle2<T>, Triangle2<T>>
    {
    public:
        struct Output
        {
            Output()
                :
                intersect(false),
                intersection{}
            {
            }

            bool intersect;
            std::vector<Vector2<T>> intersection;
        };

        Output operator()(Triangle2<T> const& triangle0, Triangle2<T> const& triangle1)
        {
            Output output{};

            // Start with triangle1 and clip against the edges of triangle0.
            std::vector<Vector2<T>> polygon =
            {
                triangle1.v[0], triangle1.v[1], triangle1.v[2]
            };

            FIQuery<T, std::vector<Vector2<T>>, Hyperplane<T, 2>> ppQuery{};
            for (std::size_t i1 = 2, i0 = 0; i0 < 3; i1 = i0++)
            {
                // Create the clipping line for the current edge. The edge
                // normal N points inside the triangle.
                Vector2<T> P = triangle0.v[i0];
                Vector2<T> N = Perp(triangle0.v[i1] - triangle0.v[i0]);
                Hyperplane<T, 2> clippingLine(N, Dot(N, P));

                // Do the clipping operation.
                auto ppOutput = ppQuery(polygon, clippingLine);
                if (ppOutput.positivePolygon.size() == 0)
                {
                    // The current clipped polygon is outside triangle0.
                    output.intersect = false;
                    return output;
                }
                polygon = std::move(ppOutput.positivePolygon);
            }

            output.intersect = true;
            output.intersection = polygon;
            return output;
        }

    private:
        friend class UnitTestIntrTriangle2Triangle2;
    };
}
