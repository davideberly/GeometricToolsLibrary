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
#include <GTL/Mathematics/Primitives/3D/Ellipsoid3.h>
#include <GTL/Mathematics/Algebra/Matrix.h>
#include <algorithm>
#include <cmath>

namespace gtl
{
    template <typename T>
    class TIQuery<T, Halfspace3<T>, Ellipsoid3<T>>
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

        Output operator()(Halfspace3<T> const& halfspace, Ellipsoid3<T> const& ellipsoid)
        {
            // Project the ellipsoid onto the normal line. The plane of the
            // halfspace occurs at the origin (zero) of the normal line.
            Output output{};
            Matrix3x3<T> MInverse{};
            ellipsoid.GetMInverse(MInverse);
            T discr = Dot(halfspace.normal, MInverse * halfspace.normal);
            T extent = std::sqrt(std::max(discr, C_<T>(0)));
            T center = Dot(halfspace.normal, ellipsoid.center) - halfspace.constant;
            T tmax = center + extent;

            // The ellipsoid and halfspace intersect when the projection
            // interval maximum is nonnegative.
            output.intersect = (tmax >= C_<T>(0));
            return output;
        }

    private:
        friend class UnitTestIntrHalfspace3Ellipsoid3;
    };
}
