// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

#include <GTL/Mathematics/Intersection/IntersectionQuery.h>
#include <GTL/Mathematics/Distance/3D/DistPoint3Frustum3.h>
#include <GTL/Mathematics/Primitives/3D/Sphere3.h>

namespace gtl
{
    template <typename T>
    class TIQuery<T, Sphere3<T>, Frustum3<T>>
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

        Output operator()(Sphere3<T> const& sphere, Frustum3<T> const& frustum)
        {
            Output output{};
            DCPQuery<T, Vector3<T>, Frustum3<T>> vfQuery{};
            T distance = vfQuery(sphere.center, frustum).distance;
            output.intersect = (distance <= sphere.radius);
            return output;
        }

    private:
        friend class UnitTestIntrSphere3Frustum3;
    };
}
