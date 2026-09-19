// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.09.19

#pragma once

// Compute the distance between a segment and a solid aligned box in 2D.
// 
// The segment is P0 + t * (P1 - P0) for 0 <= t <= 1. The direction D = P1-P0
// is generally not unit length.
// 
// The aligned box has minimum corner A and maximum corner B. A box point is X
// where A <= X <= B; the comparisons are componentwise.
// 
// The closest point on the segment is stored in closest[0] with parameter t.
// The closest point on the box is stored in closest[1]. When there are
// infinitely many choices for the pair of closest points, only one of them is
// returned.

#include <GTL/Mathematics/Distance/2D/DistLine2AlignedBox2.h>
#include <GTL/Mathematics/Distance/ND/DistPointAlignedBox.h>
#include <GTL/Mathematics/Primitives/ND/Segment.h>

namespace gtl
{
    template <typename T>
    class DCPQuery<T, Segment2<T>, AlignedBox2<T>>
    {
    public:
        using AlignedQuery = DCPQuery<T, Line2<T>, AlignedBox2<T>>;
        using Output = typename AlignedQuery::Output;

        Output operator()(Segment2<T> const& segment, AlignedBox2<T> const& box)
        {
            Output output{};

            Vector2<T> direction = segment.p[1] - segment.p[0];
            Line2<T> line(segment.p[0], direction);
            DCPQuery<T, Line2<T>, AlignedBox2<T>> lbQuery{};
            auto lbOutput = lbQuery(line, box);
            if (lbOutput.parameter >= C_<T>(0))
            {
                if (lbOutput.parameter <= C_<T>(1))
                {
                    output = lbOutput;
                }
                else
                {
                    DCPQuery<T, Vector2<T>, AlignedBox2<T>> pbQuery{};
                    auto pbOutput = pbQuery(segment.p[1], box);
                    output.sqrDistance = pbOutput.sqrDistance;
                    output.distance = pbOutput.distance;
                    output.parameter = C_<T>(1);
                    output.closest[0] = segment.p[1];
                    output.closest[1] = pbOutput.closest[1];
                }
            }
            else
            {
                DCPQuery<T, Vector2<T>, AlignedBox2<T>> pbQuery{};
                auto pbOutput = pbQuery(segment.p[0], box);
                output.distance = pbOutput.distance;
                output.sqrDistance = pbOutput.sqrDistance;
                output.parameter = C_<T>(0);
                output.closest[0] = segment.p[0];
                output.closest[1] = pbOutput.closest[1];
            }

            return output;
        }

    private:
        friend class UnitTestDistSegment2AlignedBox2;
    };
}
