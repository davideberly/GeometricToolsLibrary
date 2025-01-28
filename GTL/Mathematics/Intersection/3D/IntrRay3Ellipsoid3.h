// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.28

#pragma once

// The queries consider the ellipsoid to be a solid.
//
// The ellipsoid is (X-C)^T*M*(X-C)-1 = 0 and the ray is X = P+t*D for t >= 0.
// Substitute the ray equation into the ellipsoid equation to obtain a
// quadratic equation Q(t) = a2*t^2 + 2*a1*t + a0 = 0, where a2 = D^T*M*D,
// a1 = D^T*M*(P-C) and a0 = (P-C)^T*M*(P-C)-1. The algorithm involves an
// analysis of the real-valued roots of Q(t) for t >= 0.

#include <GTL/Mathematics/Intersection/1D/IntrIntervals.h>
#include <GTL/Mathematics/Intersection/3D/IntrLine3Ellipsoid3.h>
#include <GTL/Mathematics/Primitives/ND/Ray.h>
#include <array>
#include <cstddef>

namespace gtl
{
    template <typename T>
    class TIQuery<T, Ray3<T>, Ellipsoid3<T>>
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

        Output operator()(Ray3<T> const& ray, Ellipsoid3<T> const& ellipsoid)
        {
            Output output{};

            Matrix3x3<T> M{};
            ellipsoid.GetM(M);
            Vector3<T> diff = ray.origin - ellipsoid.center;
            Vector3<T> matDir = M * ray.direction;
            Vector3<T> matDiff = M * diff;
            T a0 = Dot(diff, matDiff) - C_<T>(1);
            if (a0 <= C_<T>(0))
            {
                // P is inside the ellipsoid.
                output.intersect = true;
                return output;
            }
            // else: P is outside the ellipsoid

            T a1 = Dot(ray.direction, matDiff);
            if (a1 >= C_<T>(0))
            {
                // Q(t) >= a0 > 0 for t >= 0, so Q(t) cannot be zero for
                // t in [0,+infinity) and the ray does not intersect the
                // ellipsoid.
                output.intersect = false;
                return output;
            }

            // The minimum of Q(t) occurs for some t in (0,+infinity). An
            // intersection occurs when Q(t) has real roots.
            T a2 = Dot(ray.direction, matDir);
            T discr = a1 * a1 - a0 * a2;
            output.intersect = (discr >= C_<T>(0));
            return output;
        }

    private:
        friend class UnitTestIntrRay3Ellipsoid3;
    };

    template <typename T>
    class FIQuery<T, Ray3<T>, Ellipsoid3<T>>
        :
        public FIQuery<T, Line3<T>, Ellipsoid3<T>>
    {
    public:
        struct Output
            :
            public FIQuery<T, Line3<T>, Ellipsoid3<T>>::Output
        {
            // No additional information to compute.
            Output() = default;
        };

        Output operator()(Ray3<T> const& ray, Ellipsoid3<T> const& ellipsoid)
        {
            Output output{};
            DoQuery(ray.origin, ray.direction, ellipsoid, output);
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
            Vector3<T> const& rayDirection, Ellipsoid3<T> const& ellipsoid,
            Output& output)
        {
            FIQuery<T, Line3<T>, Ellipsoid3<T>>::DoQuery(rayOrigin,
                rayDirection, ellipsoid, output);

            if (output.intersect)
            {
                // The line containing the ray intersects the ellipsoid; the
                // t-interval is [t0,t1]. The ray intersects the capsule as
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
                    // ellipsoid.
                    output = Output{};
                }
            }
        }

    private:
        friend class UnitTestIntrRay3Ellipsoid3;
    };
}
