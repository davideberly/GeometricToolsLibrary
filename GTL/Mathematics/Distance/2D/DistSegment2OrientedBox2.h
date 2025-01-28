// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.28

#pragma once

// Compute the distance between a segment and a solid oriented box in 2D.
// 
// The segment is P0 + t * (P1 - P0) for 0 <= t <= 1. The direction D = P1-P0
// is generally not unit length.
// 
// The oriented box has center C, unit-length axis directions U[i] and extents
// e[i] for 0 <= i < N. A box point is X = C + sum_i y[i] * U[i], where
// |y[i]| <= e[i] for all i.
// 
// The closest point on the segment is stored in closest[0] with parameter t.
// The closest point on the box is stored in closest[1]. When there are
// infinitely many choices for the pair of closest points, only one of them is
// returned.

#include <GTL/Mathematics/Distance/2D/DistLine2OrientedBox2.h>
#include <GTL/Mathematics/Distance/ND/DistPointOrientedBox.h>
#include <GTL/Mathematics/Primitives/ND/Segment.h>

namespace gtl
{
    template <typename T>
    class DCPQuery<T, Segment2<T>, OrientedBox2<T>>
    {
    public:
        using OrientedQuery = DCPQuery<T, Line2<T>, OrientedBox2<T>>;
        using Output = typename OrientedQuery::Output;

        Output operator()(Segment2<T> const& segment, OrientedBox2<T> const& box)
        {
            Output output{};

            Vector2<T> direction = segment.p[1] - segment.p[0];
            Line2<T> line(segment.p[0], direction);
            DCPQuery<T, Line2<T>, OrientedBox2<T>> lbQuery{};
            auto lbOutput = lbQuery(line, box);
            if (lbOutput.parameter >= C_<T>(0))
            {
                if (lbOutput.parameter <= C_<T>(1))
                {
                    output = lbOutput;
                }
                else
                {
                    DCPQuery<T, Vector2<T>, OrientedBox2<T>> pbQuery{};
                    auto pbResult = pbQuery(segment.p[1], box);
                    output.distance = pbResult.distance;
                    output.sqrDistance = pbResult.sqrDistance;
                    output.parameter = C_<T>(1);
                    output.closest[0] = segment.p[1];
                    output.closest[1] = pbResult.closest[1];
                }
            }
            else
            {
                DCPQuery<T, Vector2<T>, OrientedBox2<T>> pbQuery{};
                auto pbResult = pbQuery(segment.p[0], box);
                output.distance = pbResult.distance;
                output.sqrDistance = pbResult.sqrDistance;
                output.parameter = C_<T>(0);
                output.closest[0] = segment.p[0];
                output.closest[1] = pbResult.closest[1];
            }

            return output;
        }

    private:
        friend class UnitTestDistSegment2OrientedBox2;
    };
}
