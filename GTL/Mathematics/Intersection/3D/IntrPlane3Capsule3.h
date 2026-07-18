// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.18

#pragma once

#include <GTL/Mathematics/Intersection/IntersectionQuery.h>
#include <GTL/Mathematics/Distance/ND/DistPointHyperplane.h>
#include <GTL/Mathematics/Primitives/ND/Capsule.h>
#include <cmath>

namespace gtl
{
    template <typename T>
    class TIQuery<T, Plane3<T>, Capsule3<T>>
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

        Output operator()(Plane3<T> const& plane, Capsule3<T> const& capsule)
        {
            Output output{};

            DCPQuery<T, Vector3<T>, Plane3<T>> vpQuery{};
            T sdistance0 = vpQuery(capsule.segment.p[0], plane).signedDistance;
            T sdistance1 = vpQuery(capsule.segment.p[1], plane).signedDistance;
            if (sdistance0 * sdistance1 <= C_<T>(0))
            {
                // A capsule segment endpoint is on the plane or the two
                // endpoints are on opposite sides of the plane.
                output.intersect = true;
                return output;
            }

            // The endpoints on same side of plane, but the endpoint spheres
            // might intersect the plane.
            output.intersect =
                std::fabs(sdistance0) <= capsule.radius ||
                std::fabs(sdistance1) <= capsule.radius;
            return output;
        }

    private:
        friend class UnitTestIntrPlane3Capsule3;
    };
}
