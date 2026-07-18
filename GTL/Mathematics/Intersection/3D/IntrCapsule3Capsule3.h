// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.18

#pragma once

#include <GTL/Mathematics/Intersection/IntersectionQuery.h>
#include <GTL/Mathematics/Distance/ND/DistSegmentSegment.h>
#include <GTL/Mathematics/Primitives/ND/Capsule.h>

namespace gtl
{
    template <typename T>
    class TIQuery<T, Capsule3<T>, Capsule3<T>>
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

        Output operator()(Capsule3<T> const& capsule0, Capsule3<T> const& capsule1)
        {
            Output output{};
            DCPQuery<T, Segment3<T>, Segment3<T>> ssQuery{};
            auto ssOutput = ssQuery(capsule0.segment, capsule1.segment);
            T rSum = capsule0.radius + capsule1.radius;
            output.intersect = (ssOutput.distance <= rSum);
            return output;
        }

    private:
        friend class UnitTestIntrCapsule3Capsule3;
    };
}
