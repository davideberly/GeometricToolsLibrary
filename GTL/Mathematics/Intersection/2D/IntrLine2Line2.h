// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.28

#pragma once

// Test-intersection and find-intersection queries for two lines. The line
// directions are nonzero but not required to be unit length.

#include <GTL/Mathematics/Intersection/IntersectionQuery.h>
#include <GTL/Mathematics/Primitives/ND/Line.h>
#include <array>
#include <cmath>
#include <cstddef>
#include <limits>

namespace gtl
{
    template <typename T>
    class TIQuery<T, Line2<T>, Line2<T>>
    {
    public:
        struct Output
        {
            Output()
                :
                intersect(false),
                numIntersections(0)
            {
            }

            // If the lines do not intersect,
            //   intersect = false
            //   numIntersections = 0
            //
            // If the lines intersect in a single point,
            //   intersect = true
            //   numIntersections = 1
            //
            // If the lines are the same,
            //   intersect = true
            //   numIntersections = std::numeric_limits<std::size_t>::max()
            bool intersect;
            std::size_t numIntersections;
        };

        Output operator()(Line2<T> const& line0, Line2<T> const& line1)
        {
            Output output{};

            // The intersection of two lines is a solution to P0 + s0 * D0 =
            // P1 + s1 * D1. Rewrite this as s0*D0 - s1*D1 = P1 - P0 = Q. If
            // DotPerp(D0, D1)) = 0, the lines are parallel. Additionally, if
            // DotPerp(Q, D1)) = 0, the lines are the same. If
            // DotPerp(D0, D1)) is not zero, then the lines intersect in a
            // single point where
            //   s0 = DotPerp(Q, D1))/DotPerp(D0, D1))
            //   s1 = DotPerp(Q, D0))/DotPerp(D0, D1))

            T dotD0PerpD1 = DotPerp(line0.direction, line1.direction);
            if (dotD0PerpD1 != C_<T>(0))
            {
                // The lines are not parallel.
                output.intersect = true;
                output.numIntersections = 1;
            }
            else
            {
                // The lines are parallel.
                Vector2<T> Q = line1.origin - line0.origin;
                T dotQDotPerpD1 = DotPerp(Q, line1.direction);
                if (dotQDotPerpD1 != C_<T>(0))
                {
                    // The lines are parallel but distinct.
                    output.intersect = false;
                    output.numIntersections = 0;
                }
                else
                {
                    // The lines are the same.
                    output.intersect = true;
                    output.numIntersections = std::numeric_limits<std::size_t>::max();
                }
            }

            return output;
        }

    private:
        friend class UnitTestIntrLine2Line2;
    };

    template <typename T>
    class FIQuery<T, Line2<T>, Line2<T>>
    {
    public:
        struct Output
        {
            Output()
                :
                intersect(false),
                numIntersections(0),
                line0Parameter{ C_<T>(0), C_<T>(0) },
                line1Parameter{ C_<T>(0), C_<T>(0) },
                point{}
            {
            }

            // If the lines do not intersect,
            //   intersect = false
            //   numIntersections = 0
            //   line0Parameter[] = { 0, 0 }  // invalid
            //   line1Parameter[] = { 0, 0 }  // invalid
            //   point = { 0, 0 }  // invalid
            //
            // If the lines intersect in a single point, the parameter for
            // line0 is s0 and the parameter for line1 is s1,
            //   intersect = true
            //   numIntersections = 1
            //   line0Parameter = { s0, s0 }
            //   line1Parameter = { s1, s1 }
            //   point = line0.origin + s0 * line0.direction
            //         = line1.origin + s1 * line1.direction
            //
            // If the lines are the same, let
            // maxT = std::numeric_limits<T>::max(),
            //   intersect = true
            //   numIntersections = std::numeric_limits<std::size_t>::max()
            //   line0Parameter = { -maxT, +maxT }
            //   line1Parameter = { -maxT, +maxT }
            //   point = { 0, 0 }  // invalid
            bool intersect;
            std::size_t numIntersections;
            std::array<T, 2> line0Parameter, line1Parameter;
            Vector2<T> point;
        };

        Output operator()(Line2<T> const& line0, Line2<T> const& line1)
        {
            Output output{};

            // The intersection of two lines is a solution to P0 + s0 * D0 =
            // P1 + s1 * D1. Rewrite this as s0*D0 - s1*D1 = P1 - P0 = Q. If
            // DotPerp(D0, D1)) = 0, the lines are parallel. Additionally, if
            // DotPerp(Q, D1)) = 0, the lines are the same. If
            // DotPerp(D0, D1)) is not zero, then the lines intersect in a
            // single point where
            //   s0 = DotPerp(Q, D1))/DotPerp(D0, D1))
            //   s1 = DotPerp(Q, D0))/DotPerp(D0, D1))

            Vector2<T> Q = line1.origin - line0.origin;
            T dotD0PerpD1 = DotPerp(line0.direction, line1.direction);
            if (dotD0PerpD1 != C_<T>(0))
            {
                // The lines are not parallel.
                output.intersect = true;
                output.numIntersections = 1;
                T dotQPerpD0 = DotPerp(Q, line0.direction);
                T dotQPerpD1 = DotPerp(Q, line1.direction);
                T s0 = dotQPerpD1 / dotD0PerpD1;
                T s1 = dotQPerpD0 / dotD0PerpD1;
                output.line0Parameter = { s0, s0 };
                output.line1Parameter = { s1, s1 };
                output.point = line0.origin + s0 * line0.direction;
            }
            else
            {
                // The lines are parallel.
                T dotQPerpD1 = DotPerp(Q, line1.direction);
                if (std::fabs(dotQPerpD1) != C_<T>(0))
                {
                    // The lines are parallel but distinct.
                    output.intersect = false;
                    output.numIntersections = 0;
                }
                else
                {
                    // The lines are the same.
                    output.intersect = true;
                    output.numIntersections = std::numeric_limits<std::size_t>::max();
                    T const maxT = std::numeric_limits<T>::max();
                    output.line0Parameter = { -maxT, maxT };
                    output.line1Parameter = { -maxT, maxT };
                }
            }

            return output;
        }

    private:
        friend class UnitTestIntrLine2Line2;
    };
}
