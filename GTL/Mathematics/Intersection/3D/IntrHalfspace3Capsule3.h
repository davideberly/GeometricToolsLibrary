// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.18

#pragma once

// Queries for intersection of objects with halfspaces. These are useful for
// containment testing, object culling and clipping.

#include <GTL/Mathematics/Intersection/IntersectionQuery.h>
#include <GTL/Mathematics/Primitives/ND/Halfspace.h>
#include <GTL/Mathematics/Primitives/ND/Capsule.h>
#include <algorithm>

namespace gtl
{
    template <typename T>
    class TIQuery<T, Halfspace3<T>, Capsule3<T>>
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

        Output operator()(Halfspace3<T> const& halfspace, Capsule3<T> const& capsule)
        {
            Output output{};

            // Project the capsule onto the normal line. The plane of the
            // halfspace occurs at the origin (zero) of the normal line.
            T e0 = Dot(halfspace.normal, capsule.segment.p[0]) - halfspace.constant;
            T e1 = Dot(halfspace.normal, capsule.segment.p[1]) - halfspace.constant;

            // The capsule and halfspace intersect when the projection
            // interval maximum is nonnegative.
            output.intersect = (std::max(e0, e1) + capsule.radius >= C_<T>(0));
            return output;
        }

    private:
        friend class UnitTestIntrHalfspace3Capsule3;
    };
}
