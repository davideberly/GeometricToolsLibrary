// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.18

#pragma once

#include <GTL/Mathematics/Intersection/IntersectionQuery.h>
#include <GTL/Mathematics/Distance/ND/DistPointHyperplane.h>
#include <GTL/Mathematics/Primitives/3D/Ellipsoid3.h>
#include <GTL/Mathematics/Algebra/Matrix.h>
#include <algorithm>
#include <cmath>

namespace gtl
{
    template <typename T>
    class TIQuery<T, Plane3<T>, Ellipsoid3<T>>
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

        Output operator()(Plane3<T> const& plane, Ellipsoid3<T> const& ellipsoid)
        {
            Output output{};
            Matrix3x3<T> MInverse{};
            ellipsoid.GetMInverse(MInverse);
            T discr = Dot(plane.normal, MInverse * plane.normal);
            T root = std::sqrt(std::max(discr, C_<T>(0)));
            DCPQuery<T, Vector3<T>, Plane3<T>> vpQuery{};
            T distance = vpQuery(ellipsoid.center, plane).distance;
            output.intersect = (distance <= root);
            return output;
        }

    private:
        friend class UnitTestIntrPlane3Ellipsoid3;
    };
}
