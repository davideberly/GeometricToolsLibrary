// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

// Compute the distance between a segment and a solid triangle in 3D.
// 
// The segment is P0 + t * (P1 - P0) for 0 <= t <= 1. The direction D = P1-P0
// is generally not unit length.
// 
// The triangle has vertices <V[0],V[1],V[2]>. A triangle point is
// X = sum_{i=0}^2 b[i] * V[i], where 0 <= b[i] <= 1 for all i and
// sum_{i=0}^2 b[i] = 1.
// 
// The closest point on the segment is stored in closest[0] with parameter t.
// The closest point on the triangle is closest[1] with barycentric
// coordinates (b[0],b[1],b[2]). When there are infinitely many choices for the pair of closest
// points, only one of them is returned.

#include <GTL/Mathematics/Distance/2D/DistLine2Triangle2.h>
#include <GTL/Mathematics/Distance/ND/DistPointTriangle.h>
#include <GTL/Mathematics/Primitives/ND/Segment.h>

namespace gtl
{
    template <typename T>
    class DCPQuery<T, Segment2<T>, Triangle2<T>>
    {
    public:
        using LTQuery = DCPQuery<T, Line2<T>, Triangle2<T>>;
        using Output = typename LTQuery::Output;

        Output operator()(Segment2<T> const& segment, Triangle2<T> const& triangle)
        {
            Output output{};

            T const zero = static_cast<T>(0);
            T const one = static_cast<T>(1);
            Vector2<T> segDirection = segment.p[1] - segment.p[0];
            Line2<T> line(segment.p[0], segDirection);
            LTQuery ltQuery{};
            auto ltResult = ltQuery(line, triangle);
            if (ltResult.parameter >= zero)
            {
                if (ltResult.parameter <= one)
                {
                    output = ltResult;
                }
                else
                {
                    DCPQuery<T, Vector2<T>, Triangle2<T>> ptQuery{};
                    auto ptResult = ptQuery(segment.p[1], triangle);
                    output.distance = ptResult.distance;
                    output.sqrDistance = ptResult.sqrDistance;
                    output.parameter = one;
                    output.barycentric = ptResult.barycentric;
                    output.closest[0] = segment.p[1];
                    output.closest[1] = ptResult.closest[1];
                }
            }
            else
            {
                DCPQuery<T, Vector2<T>, Triangle2<T>> ptQuery{};
                auto ptResult = ptQuery(segment.p[0], triangle);
                output.distance = ptResult.distance;
                output.sqrDistance = ptResult.sqrDistance;
                output.parameter = zero;
                output.barycentric = ptResult.barycentric;
                output.closest[0] = segment.p[0];
                output.closest[1] = ptResult.closest[1];
            }
            return output;
        }
    };
}
