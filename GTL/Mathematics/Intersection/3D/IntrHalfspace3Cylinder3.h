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
#include <GTL/Mathematics/Primitives/ND/Cylinder.h>
#include <algorithm>
#include <cmath>

namespace gtl
{
    template <typename T>
    class TIQuery<T, Halfspace3<T>, Cylinder3<T>>
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

        Output operator()(Halfspace3<T> const& halfspace, Cylinder3<T> const& cylinder)
        {
            Output output{};

            // Compute extremes of signed distance Dot(N,X)-d for points on
            // the cylinder. These are
            //   min = (Dot(N,C)-d) - r*sqrt(1-Dot(N,W)^2) - (h/2)*|Dot(N,W)|
            //   max = (Dot(N,C)-d) + r*sqrt(1-Dot(N,W)^2) + (h/2)*|Dot(N,W)|
            T center = Dot(halfspace.normal, cylinder.center) - halfspace.constant;
            T absNdW = std::fabs(Dot(halfspace.normal, cylinder.direction));
            T root = std::sqrt(std::max(C_<T>(1), C_<T>(1) - absNdW * absNdW));
            T tmax = center + cylinder.radius * root + C_<T>(1, 2) * cylinder.height * absNdW;

            // The cylinder and halfspace intersect when the projection
            // interval maximum is nonnegative.
            output.intersect = (tmax >= C_<T>(0));
            return output;
        }

    private:
        friend class UnitTestIntrHalfspace3Cylinder3;
    };
}
