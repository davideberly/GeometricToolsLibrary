// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

// The queries consider the sphere to be a solid.
//
// The sphere is (X-C)^T*(X-C)-r^2 = 0 and the ray is X = P+t*D for t >= 0.
// Substitute the ray equation into the sphere equation to obtain a quadratic
// equation Q(t) = t^2 + 2*a1*t + a0 = 0, where a1 = D^T*(P-C) and
// a0 = (P-C)^T*(P-C)-r^2. The algorithm involves an analysis of the
// real-valued roots of Q(t) for t >= 0.

#include <GTL/Mathematics/Intersection/1D/IntrIntervals.h>
#include <GTL/Mathematics/Intersection/3D/IntrLine3Sphere3.h>
#include <GTL/Mathematics/Primitives/ND/Ray.h>
#include <array>
#include <cstddef>

namespace gtl
{
    template <typename T>
    class TIQuery<T, Ray3<T>, Sphere3<T>>
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

        Output operator()(Ray3<T> const& ray, Sphere3<T> const& sphere)
        {
            Output output{};

            Vector3<T> diff = ray.origin - sphere.center;
            T a0 = Dot(diff, diff) - sphere.radius * sphere.radius;
            if (a0 <= C_<T>(0))
            {
                // P is inside the sphere.
                output.intersect = true;
                return output;
            }
            // else: P is outside the sphere

            T a1 = Dot(ray.direction, diff);
            if (a1 >= C_<T>(0))
            {
                // Q(t) >= a0 > 0 for t >= 0, so Q(t) cannot be zero for
                // t in [0,+infinity) and the ray does not intersect the
                // sphere.
                output.intersect = false;
                return output;
            }

            // The minimum of Q(t) occurs for some t in (0,+infinity). An
            // intersection occurs when Q(t) has real roots.
            T discr = a1 * a1 - a0;
            output.intersect = (discr >= C_<T>(0));
            return output;
        }

    private:
        friend class UnitTestIntrRay3Sphere3;
    };

    template <typename T>
    class FIQuery<T, Ray3<T>, Sphere3<T>>
        :
        public FIQuery<T, Line3<T>, Sphere3<T>>
    {
    public:
        struct Output
            :
            public FIQuery<T, Line3<T>, Sphere3<T>>::Output
        {
            // No additional information to compute.
            Output() = default;
        };

        Output operator()(Ray3<T> const& ray, Sphere3<T> const& sphere)
        {
            Output output{};
            DoQuery(ray.origin, ray.direction, sphere, output);
            if (output.intersect)
            {
                for (std::size_t i = 0; i < 2; ++i)
                {
                    output.point[i] = ray.origin + output.parameter[i] * ray.direction;
                }
            }
            return output;
        }

    protected:
        // The caller must ensure that on entry, 'output' is default
        // constructed as if there is no intersection. If an intersection is
        // found, the 'output' values will be modified accordingly.
        void DoQuery(Vector3<T> const& rayOrigin,
            Vector3<T> const& rayDirection, Sphere3<T> const& sphere,
            Output& output)
        {
            FIQuery<T, Line3<T>, Sphere3<T>>::DoQuery(
                rayOrigin, rayDirection, sphere, output);

            if (output.intersect)
            {
                // The line containing the ray intersects the sphere; the
                // t-interval is [t0,t1]. The ray intersects the sphere as
                // long as [t0,t1] overlaps the ray t-interval [0,+infinity).
                FIQuery<T, std::array<T, 2>, std::array<T, 2>> iiQuery{};
                auto iiOutput = iiQuery(output.parameter, C_<T>(0), true);
                if (iiOutput.intersect)
                {
                    output.numIntersections = iiOutput.numIntersections;
                    output.parameter = iiOutput.overlap;
                }
                else
                {
                    // The line containing the ray does not intersect the
                    // sphere.
                    output = Output{};
                }
            }
        }

    private:
        friend class UnitTestIntrRay3Sphere3;
    };
}
