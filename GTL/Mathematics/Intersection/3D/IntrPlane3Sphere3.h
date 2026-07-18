// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.18

#pragma once

#include <GTL/Mathematics/Intersection/IntersectionQuery.h>
#include <GTL/Mathematics/Distance/ND/DistPointHyperplane.h>
#include <GTL/Mathematics/Primitives/3D/Sphere3.h>
#include <GTL/Mathematics/Primitives/3D/Circle3.h>
#include <cmath>

namespace gtl
{
    template <typename T>
    class TIQuery<T, Plane3<T>, Sphere3<T>>
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

        Output operator()(Plane3<T> const& plane, Sphere3<T> const& sphere)
        {
            Output output{};
            DCPQuery<T, Vector3<T>, Plane3<T>> ppQuery{};
            auto ppOutput = ppQuery(sphere.center, plane);
            output.intersect = (ppOutput.distance <= sphere.radius);
            return output;
        }

    private:
        friend class UnitTestIntrPlane3Sphere3;
    };

    template <typename T>
    class FIQuery<T, Plane3<T>, Sphere3<T>>
    {
    public:
        struct Output
        {
            Output()
                :
                intersect(false),
                isCircle(false),
                circle{},
                point{}
            {
            }

            bool intersect;

            // If 'intersect' is true, the intersection is either a point or a
            // circle.  When 'isCircle' is true, 'circle' is valid. When
            // 'isCircle' is false, 'point' is valid.
            bool isCircle;
            Circle3<T> circle;
            Vector3<T> point;
        };

        Output operator()(Plane3<T> const& plane, Sphere3<T> const& sphere)
        {
            Output output{};
            DCPQuery<T, Vector3<T>, Plane3<T>> ppQuery{};
            auto ppOutput = ppQuery(sphere.center, plane);
            if (ppOutput.distance < sphere.radius)
            {
                output.intersect = true;
                output.isCircle = true;
                output.circle.center = sphere.center - ppOutput.signedDistance * plane.normal;
                output.circle.normal = plane.normal;

                // The sum and diff are both positive numbers.
                T sum = sphere.radius + ppOutput.distance;
                T dif = sphere.radius - ppOutput.distance;

                // arg = sqr(sphere.radius) - sqr(ppOutput.distance)
                T arg = sum * dif;

                output.circle.radius = std::sqrt(arg);
                return output;
            }
            else if (ppOutput.distance == sphere.radius)
            {
                output.intersect = true;
                output.isCircle = false;
                output.point = sphere.center - ppOutput.signedDistance * plane.normal;
                return output;
            }
            else
            {
                output.intersect = false;
                return output;
            }
        }

    private:
        friend class UnitTestIntrPlane3Sphere3;
    };
}
