// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.17

#pragma once

// The two circles are |X-C0| = R0 and |X-C1| = R1. Define U = C1 - C0 and
// V = Perp(U) where Perp(x,y) = (y,-x). Note that Dot(U,V) = 0 and |V|^2
// = |U|^2. The intersection points X can be written in the form X =
// C0+s*U+t*V and X = C1+(s-1)*U+t*V. Squaring the circle equations and
// substituting these formulas into them yields
//   R0^2 = (s^2 + t^2)*|U|^2
//   R1^2 = ((s-1)^2 + t^2)*|U|^2.
// Subtracting and solving for s yields
//   s = ((R0^2-R1^2)/|U|^2 + 1)/2
// Then replace in the first equation and solve for t^2
//   t^2 = (R0^2/|U|^2) - s^2.
// In order for there to be solutions, the right-hand side must be
// nonnegative. Some algebra leads to the condition for existence of
// solutions,
//   (|U|^2 - (R0+R1)^2)*(|U|^2 - (R0-R1)^2) <= 0.
// This reduces to
//   |R0-R1| <= |U| <= |R0+R1|.
// If |U| = |R0-R1|, then the circles are side-by-side and just tangent. If
// |U| = |R0+R1|, then the circles are nested and just tangent. If
// |R0-R1| < |U| < |R0+R1|, then the two circles intersect in two points.

#include <GTL/Mathematics/Intersection/IntersectionQuery.h>
#include <GTL/Mathematics/Primitives/2D/Circle2.h>
#include <GTL/Mathematics/Algebra/Vector.h>
#include <array>
#include <cmath>
#include <cstddef>
#include <limits>

namespace gtl
{
    template <typename T>
    class TIQuery<T, Circle2<T>, Circle2<T>>
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

        Output operator()(Circle2<T> const& circle0, Circle2<T> const& circle1)
        {
            Output output{};
            Vector2<T> diff = circle0.center - circle1.center;
            output.intersect = (Length(diff) <= circle0.radius + circle1.radius);
            return output;
        }

    private:
        friend class UnitTestIntrCircle2Circle2;
    };

    template <typename T>
    class FIQuery<T, Circle2<T>, Circle2<T>>
    {
    public:
        struct Output
        {
            Output()
                :
                intersect(false),
                numIntersections(0),
                point{},
                circle{}
            {
            }

            bool intersect;

            // The number of intersections is 0, 1, 2 or maxSizeT =
            // std::numeric_limits<std::size_t>::max(). When 1, the circles are
            // tangent and intersect in a single point. When 2, circles have
            // two transverse intersection points. When maxSizeT, the circles
            // are the same.
            std::size_t numIntersections;

            // Valid only when numIntersections = 1 or 2.
            std::array<Vector2<T>, 2> point;

            // Valid only when numIntersections = maxInt.
            Circle2<T> circle;
        };

        Output operator()(Circle2<T> const& circle0, Circle2<T> const& circle1)
        {
            Output output{};

            Vector2<T> U = circle1.center - circle0.center;
            T USqrLen = Dot(U, U);
            T R0 = circle0.radius, R1 = circle1.radius;
            T R0mR1 = R0 - R1;
            if (USqrLen == C_<T>(0) && R0mR1 == C_<T>(0))
            {
                // Circles are the same.
                output.intersect = true;
                output.numIntersections = std::numeric_limits<std::size_t>::max();
                output.circle = circle0;
                return output;
            }

            T R0mR1Sqr = R0mR1 * R0mR1;
            if (USqrLen < R0mR1Sqr)
            {
                // The circles do not intersect.
                output.intersect = false;
                output.numIntersections = 0;
                return output;
            }

            T R0pR1 = R0 + R1;
            T R0pR1Sqr = R0pR1 * R0pR1;
            if (USqrLen > R0pR1Sqr)
            {
                // The circles do not intersect.
                output.intersect = false;
                output.numIntersections = 0;
                return output;
            }

            if (USqrLen < R0pR1Sqr)
            {
                if (R0mR1Sqr < USqrLen)
                {
                    T invUSqrLen = C_<T>(1) / USqrLen;
                    T s = C_<T>(1, 2) * ((R0 * R0 - R1 * R1) * invUSqrLen + C_<T>(1));
                    Vector2<T> tmp = circle0.center + s * U;

                    // In theory, discr is nonnegative.  However, numerical round-off
                    // errors can make it slightly negative.  Clamp it to zero.
                    T discr = R0 * R0 * invUSqrLen - s * s;
                    if (discr < C_<T>(0))
                    {
                        discr = C_<T>(0);
                    }
                    T t = std::sqrt(discr);
                    Vector2<T> V{ U[1], -U[0] };
                    output.point[0] = tmp - t * V;
                    output.point[1] = tmp + t * V;
                    output.numIntersections = (t > C_<T>(0) ? 2 : 1);
                }
                else
                {
                    // |U| = |R0-R1|, circles are tangent.
                    output.numIntersections = 1;
                    output.point[0] = circle0.center + (R0 / R0mR1) * U;
                }
            }
            else
            {
                // |U| = |R0+R1|, circles are tangent.
                output.numIntersections = 1;
                output.point[0] = circle0.center + (R0 / R0pR1) * U;
            }

            // The circles intersect in 1 or 2 points.
            output.intersect = true;
            return output;
        }

    private:
        friend class UnitTestIntrCircle2Circle2;
    };
}
