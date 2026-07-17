// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.17

#pragma once

#include <GTL/Mathematics/Intersection/2D/IntrCircle2Circle2.h>
#include <GTL/Mathematics/Primitives/2D/Arc2.h>
#include <array>
#include <cstddef>
#include <limits>

namespace gtl
{
    template <typename T>
    class FIQuery<T, Circle2<T>, Arc2<T>>
    {
    public:
        struct Output
        {
            Output()
                :
                intersect(false),
                numIntersections(0),
                point{},
                arc{}
            {
            }

            bool intersect;

            // The number of intersections is 0, 1, 2 or maxSizeT =
            // std::numeric_limits<std::size_t>::max(). When 1, the arc and circle
            // intersect in a single point. When 2, the arc is not on the
            // circle and they intersect in two points. When maxInt, the
            // arc is on the circle.
            std::size_t numIntersections;

            // Valid only when numIntersections = 1 or 2.
            std::array<Vector2<T>, 2> point;

            // Valid only when numIntersections = maxInt.
            Arc2<T> arc;
        };

        // See Arc2.h for a description of epsilon >= 0.
        Output operator()(Circle2<T> const& circle, Arc2<T> const& arc, T const& epsilon)
        {
            Output output{};

            Circle2<T> circleOfArc(arc.center, arc.radius);
            FIQuery<T, Circle2<T>, Circle2<T>> ccQuery{};
            auto ccOutput = ccQuery(circle, circleOfArc);
            if (!ccOutput.intersect)
            {
                output.intersect = false;
                output.numIntersections = 0;
                return output;
            }

            if (ccOutput.numIntersections == std::numeric_limits<std::size_t>::max())
            {
                // The arc is on the circle.
                output.intersect = true;
                output.numIntersections = std::numeric_limits<std::size_t>::max();
                output.arc = arc;
                return output;
            }

#if defined(GTL_USE_MSWINDOWS)
#pragma warning(disable : 28020)
            // The code analysis tool complained:
            // warning C28020: The expression '0<=_Param_(1)&&_Param_(1)<=2-1'
            // is not true at this call.: Lines: 51, 53, 54, 55, 56, 63, 73,
            // 74, 76, 78, 79, 74, 76, 78. Before inserting this comment, line
            // 74 is the for-loop below. The code analysis tool seems to
            // believe that result.numIntersections is 2 and that this number
            // is out-of-range. It is not because ccResult sets the value of
            // numIntersections to 0, 1, 2 or maxInt. The test for maxInt
            // occurs in the if-statement above. Inferring that the number of
            // intersections at this point being 0, 1, or 2 is probably
            // difficult for a code analysis tool.
#endif
            // Test whether circle-circle intersection points are on the arc.
            output.numIntersections = 0;
            for (std::size_t i = 0; i < ccOutput.numIntersections; ++i)
            {
                if (arc.Contains(ccOutput.point[i], epsilon))
                {
                    output.point[output.numIntersections++] = ccOutput.point[i];
                    output.intersect = true;
                }
            }
#if defined(GTL_USE_MSWINDOWS)
#pragma warning(default : 28020)
#endif
            return output;
        }

    private:
        friend class UnitTestIntrCircle2Arc2;
    };
}
