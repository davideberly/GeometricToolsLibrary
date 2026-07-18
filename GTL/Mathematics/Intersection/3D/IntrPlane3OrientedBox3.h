// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.18

#pragma once

#include <GTL/Mathematics/Intersection/IntersectionQuery.h>
#include <GTL/Mathematics/Distance/ND/DistPointHyperplane.h>
#include <GTL/Mathematics/Primitives/ND/OrientedBox.h>
#include <cmath>

namespace gtl
{
    template <typename T>
    class TIQuery<T, Plane3<T>, OrientedBox3<T>>
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

        Output operator()(Plane3<T> const& plane, OrientedBox3<T> const& box)
        {
            Output output{};

            T radius =
                std::fabs(box.extent[0] * Dot(plane.normal, box.axis[0])) +
                std::fabs(box.extent[1] * Dot(plane.normal, box.axis[1])) +
                std::fabs(box.extent[2] * Dot(plane.normal, box.axis[2]));

            DCPQuery<T, Vector3<T>, Plane3<T>> ppQuery{};
            auto ppResult = ppQuery(box.center, plane);
            output.intersect = (ppResult.distance <= radius);
            return output;
        }

    private:
        friend class UnitTestIntrPlane3OrientedBox3;
    };
}
