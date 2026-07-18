// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

// The queries consider the spheres to be solids.

#include <GTL/Mathematics/Intersection/IntersectionQuery.h>
#include <GTL/Mathematics/Primitives/3D/Sphere3.h>
#include <GTL/Mathematics/Primitives/3D/Circle3.h>
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <limits>

namespace gtl
{
    template <typename T>
    class TIQuery<T, Sphere3<T>, Sphere3<T>>
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

        Output operator()(Sphere3<T> const& sphere0, Sphere3<T> const& sphere1)
        {
            Output output{};
            Vector3<T> diff = sphere1.center - sphere0.center;
            T rSum = sphere0.radius + sphere1.radius;
            output.intersect = (Dot(diff, diff) <= rSum * rSum);
            return output;
        }

    private:
        friend class UnitTestIntrSphere3Sphere3;
    };

    template <typename T>
    class FIQuery<T, Sphere3<T>, Sphere3<T>>
    {
    public:
        struct Output
        {
            Output()
                :
                intersect(false),
                type(std::numeric_limits<std::size_t>::max()),
                point{},
                circle{}
            {
            }

            // The type of intersection.
            //   0: spheres are disjoint and separated
            //   1: spheres touch at point, each sphere outside the other
            //   2: spheres intersect in a circle
            //   3: sphere0 strictly contained in sphere1
            //   4: sphere0 contained in sphere1, share common point
            //   5: sphere1 strictly contained in sphere0
            //   6: sphere1 contained in sphere0, share common point
            bool intersect;
            std::size_t type;
            Vector3<T> point;    // types 1, 4, 6
            Circle3<T> circle;   // type 2
        };

        Output operator()(Sphere3<T> const& sphere0, Sphere3<T> const& sphere1)
        {
            Output output{};

            // The plane of intersection must have C1-C0 as its normal
            // direction.
            Vector3<T> C1mC0 = sphere1.center - sphere0.center;
            T sqrLen = Dot(C1mC0, C1mC0);
            T r0 = sphere0.radius, r1 = sphere1.radius;
            T rSum = r0 + r1;
            T rSumSqr = rSum * rSum;

            if (sqrLen > rSumSqr)
            {
                // The spheres are disjoint/separated.
                output.intersect = false;
                output.type = 0;
                return output;
            }

            if (sqrLen == rSumSqr)
            {
                // The spheres are just touching with each sphere outside the
                // other.
                Normalize(C1mC0);
                output.intersect = true;
                output.type = 1;
                output.point = sphere0.center + r0 * C1mC0;
                return output;
            }

            T rDif = r0 - r1;
            T rDifSqr = rDif * rDif;
            if (sqrLen < rDifSqr)
            {
                // One sphere is strictly contained in the other.  Compute a
                // point in the intersection set.
                output.intersect = true;
                output.type = (rDif <= C_<T>(0) ? 3 : 5);
                output.point = C_<T>(1, 2) * (sphere0.center + sphere1.center);
                return output;
            }
            if (sqrLen == rDifSqr)
            {
                // One sphere is contained in the other sphere but with a
                // single point of contact.
                Normalize(C1mC0);
                output.intersect = true;
                if (rDif <= C_<T>(0))
                {
                    output.type = 4;
                    output.point = sphere1.center + r1 * C1mC0;
                }
                else
                {
                    output.type = 6;
                    output.point = sphere0.center + r0 * C1mC0;
                }
                return output;
            }

            // Compute t for which the circle of intersection has center
            // K = C0 + t*(C1 - C0).
            T t = C_<T>(1, 2) * (C_<T>(1) + rDif * rSum / sqrLen);

            // Compute the center and radius of the circle of intersection.
            output.circle.center = sphere0.center + t * C1mC0;
            output.circle.radius = std::sqrt(std::max(r0 * r0 - t * t * sqrLen, C_<T>(0)));

            // Compute the normal for the plane of the circle.
            Normalize(C1mC0);
            output.circle.normal = C1mC0;

            // The intersection is a circle.
            output.intersect = true;
            output.type = 2;
            return output;
        }

    private:
        friend class UnitTestIntrSphere3Sphere3;
    };
}
