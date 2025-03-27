// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

// Compute the distance between a segment and a solid canonical box in 3D.
// 
// The segment is P0 + t * (P1 - P0) for 0 <= t <= 1. The direction D = P1-P0
// is generally not unit length.
// 
// The canonical box has center at the origin and is aligned with the
// coordinate axes. The extents are E = (e[0],e[1],e[2]). A box point is
// Y = (y[0],y[1],y[2]) with |y[i]| <= e[i] for all i.
// 
// The closest point on the segment is stored in closest[0] with parameter t.
// The closest point on the box is stored in closest[1]. When there are
// infinitely many choices for the pair of closest points, only one of them is
// returned.

#include <GTL/Mathematics/Distance/3D/DistLine3CanonicalBox3.h>
#include <GTL/Mathematics/Distance/ND/DistPointCanonicalBox.h>
#include <GTL/Mathematics/Primitives/ND/Segment.h>

namespace gtl
{
    template <typename T>
    class DCPQuery<T, Segment3<T>, CanonicalBox3<T>>
    {
    public:
        using LBQuery = DCPQuery<T, Line3<T>, CanonicalBox3<T>>;
        using Output = typename LBQuery::Output;

        Output operator()(Segment3<T> const& segment, CanonicalBox3<T> const& box)
        {
            Output output{};

            Vector3<T> segDirection = segment.p[1] - segment.p[0];
            Line3<T> line(segment.p[0], segDirection);
            LBQuery lbQuery{};
            auto lbOutput = lbQuery(line, box);
            if (lbOutput.parameter >= C_<T>(0))
            {
                if (lbOutput.parameter <= C_<T>(1))
                {
                    output = lbOutput;
                }
                else
                {
                    DCPQuery<T, Vector3<T>, CanonicalBox3<T>> pbQuery{};
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
                DCPQuery<T, Vector3<T>, CanonicalBox3<T>> pbQuery{};
                auto pbOutput = pbQuery(segment.p[0], box);
                output.sqrDistance = pbOutput.sqrDistance;
                output.distance = pbOutput.distance;
                output.parameter = C_<T>(0);
                output.closest[0] = segment.p[0];
                output.closest[1] = pbOutput.closest[1];
            }
            return output;
        }

    private:
        friend class UnitTestDistSegment3CanonicalBox3;
    };
}
