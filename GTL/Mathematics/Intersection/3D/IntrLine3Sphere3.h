// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

// The queries consider the sphere to be a solid.
//
// The sphere is (X-C)^T*(X-C)-r^2 = 0 and the line is X = P+t*D. Substitute
// the line equation into the sphere equation to obtain a quadratic equation
// Q(t) = t^2 + 2*a1*t + a0 = 0, where a1 = D^T*(P-C) and
// a0 = (P-C)^T*(P-C)-r^2. The algorithm involves an analysis of the
// real-valued roots of Q(t) for all real t.

#include <GTL/Mathematics/Intersection/IntersectionQuery.h>
#include <GTL/Mathematics/Primitives/3D/Sphere3.h>
#include <GTL/Mathematics/Primitives/ND/Line.h>
#include <array>
#include <cmath>
#include <cstddef>

namespace gtl
{
    template <typename T>
    class TIQuery<T, Line3<T>, Sphere3<T>>
    {
    public:
        struct Output
        {
            Output()
                :
                intersect(false)
            {
            };

            bool intersect;
        };

        Output operator()(Line3<T> const& line, Sphere3<T> const& sphere)
        {
            Output output{};

            Vector3<T> diff = line.origin - sphere.center;
            T a0 = Dot(diff, diff) - sphere.radius * sphere.radius;
            T a1 = Dot(line.direction, diff);

            // An intersection occurs when Q(t) has real roots.
            T discr = a1 * a1 - a0;
            output.intersect = (discr >= C_<T>(0));
            return output;
        }

    private:
        friend class UnitTestIntrLine3Sphere3;
    };

    template <typename T>
    class FIQuery<T, Line3<T>, Sphere3<T>>
    {
    public:
        struct Output
        {
            Output()
                :
                intersect(false),
                numIntersections(0),
                parameter{ C_<T>(0), C_<T>(0) },
                point{}
            {
            }

            bool intersect;
            std::size_t numIntersections;
            std::array<T, 2> parameter;
            std::array<Vector3<T>, 2> point;
        };

        Output operator()(Line3<T> const& line, Sphere3<T> const& sphere)
        {
            Output output{};
            DoQuery(line.origin, line.direction, sphere, output);
            if (output.intersect)
            {
                for (std::size_t i = 0; i < 2; ++i)
                {
                    output.point[i] = line.origin + output.parameter[i] * line.direction;
                }
            }
            return output;
        }

    protected:
        // The caller must ensure that on entry, 'output' is default
        // constructed as if there is no intersection. If an intersection is
        // found, the 'output' values will be modified accordingly.
        void DoQuery(Vector3<T> const& lineOrigin,
            Vector3<T> const& lineDirection, Sphere3<T> const& sphere,
            Output& output)
        {
            Vector3<T> diff = lineOrigin - sphere.center;
            T a0 = Dot(diff, diff) - sphere.radius * sphere.radius;
            T a1 = Dot(lineDirection, diff);

            // An intersection occurs when Q(t) has real roots.
            T discr = a1 * a1 - a0;
            if (discr > C_<T>(0))
            {
                // The line intersects the sphere in 2 distinct points.
                output.intersect = true;
                output.numIntersections = 2;
                T root = std::sqrt(discr);
                output.parameter[0] = -a1 - root;
                output.parameter[1] = -a1 + root;
            }
            else if (discr == C_<T>(0))
            {
                // The line is tangent to the sphere, so the intersection is
                // a single point. The parameter[1] value is set, because
                // callers will access the degenerate interval [-a1,-a1].
                output.intersect = true;
                output.numIntersections = 1;
                output.parameter[0] = -a1;
                output.parameter[1] = output.parameter[0];
            }
            // else:  The line is outside the sphere, no intersection.
        }

    private:
        friend class UnitTestIntrLine3Sphere3;
    };
}
