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
#include <GTL/Mathematics/Primitives/ND/OrientedBox.h>
#include <cmath>

namespace gtl
{
    template <typename T>
    class TIQuery<T, Halfspace3<T>, OrientedBox3<T>>
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

        Output operator()(Halfspace3<T> const& halfspace, OrientedBox3<T> const& box)
        {
            Output output{};

            // Project the box center onto the normal line. The plane of the
            // halfspace occurs at the origin (zero) of the normal line.
            T center = Dot(halfspace.normal, box.center) - halfspace.constant;

            // Compute the radius of the interval of projection.
            T radius =
                std::fabs(box.extent[0] * Dot(halfspace.normal, box.axis[0])) +
                std::fabs(box.extent[1] * Dot(halfspace.normal, box.axis[1])) +
                std::fabs(box.extent[2] * Dot(halfspace.normal, box.axis[2]));

            // The box and halfspace intersect when the projection interval
            // maximum is nonnegative.
            output.intersect = (center + radius >= C_<T>(0));
            return output;
        }

    private:
        friend class UnitTestIntrHalfspace3OrientedBox3;
    };
}
