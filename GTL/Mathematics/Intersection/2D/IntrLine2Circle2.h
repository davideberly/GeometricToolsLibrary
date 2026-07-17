// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.17

#pragma once

// The queries consider the circle to be a solid disk.

#include <GTL/Mathematics/Intersection/IntersectionQuery.h>
#include <GTL/Mathematics/Distance/ND/DistPointLine.h>
#include <GTL/Mathematics/Primitives/2D/Circle2.h>
#include <GTL/Mathematics/Algebra/Vector.h>
#include <array>
#include <cmath>
#include <cstddef>

namespace gtl
{
    template <typename T>
    class TIQuery<T, Line2<T>, Circle2<T>>
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

        Output operator()(Line2<T> const& line, Circle2<T> const& circle)
        {
            Output output{};
            DCPQuery<T, Vector2<T>, Line2<T>> plQuery{};
            auto plOutput = plQuery(circle.center, line);
            output.intersect = (plOutput.distance <= circle.radius);
            return output;
        }

    private:
        friend class UnitTestIntrLine2Circle2;
    };

    template <typename T>
    class FIQuery<T, Line2<T>, Circle2<T>>
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
            std::array<Vector2<T>, 2> point;
        };

        Output operator()(Line2<T> const& line, Circle2<T> const& circle)
        {
            Output output{};
            DoQuery(line.origin, line.direction, circle, output);
            for (std::size_t i = 0; i < output.numIntersections; ++i)
            {
                output.point[i] = line.origin + output.parameter[i] * line.direction;
            }
            return output;
        }

    protected:
        void DoQuery(Vector2<T> const& lineOrigin,
            Vector2<T> const& lineDirection, Circle2<T> const& circle,
            Output& output)
        {
            // Intersection of a the line P+t*D and the circle |X-C| = R.
            // The line direction is unit length. The t-value is a
            // real-valued root to the quadratic equation
            //   0 = |t*D+P-C|^2 - R^2
            //     = t^2 + 2*Dot(D,P-C)*t + |P-C|^2-R^2
            //     = t^2 + 2*a1*t + a0
            // If there are two distinct roots, the order is t0 < t1.
            Vector2<T> diff = lineOrigin - circle.center;
            T a0 = Dot(diff, diff) - circle.radius * circle.radius;
            T a1 = Dot(lineDirection, diff);
            T discr = a1 * a1 - a0;
            if (discr > C_<T>(0))
            {
                T root = std::sqrt(discr);
                output.intersect = true;
                output.numIntersections = 2;
                output.parameter[0] = -a1 - root;
                output.parameter[1] = -a1 + root;
            }
            else if (discr < C_<T>(0))
            {
                output.intersect = false;
                output.numIntersections = 0;
            }
            else  // discr == 0
            {
                // The line is tangent to the circle. Set the parameters to
                // the same number because other queries involving linear
                // components and circular components use interval-interval
                // intersection tests which consume both parameters.
                output.intersect = true;
                output.numIntersections = 1;
                output.parameter[0] = -a1;
                output.parameter[1] = -a1;
            }
        }

    private:
        friend class UnitTestIntrLine2Circle2;
    };
}
