// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.28

#pragma once

// The queries consider the triangle to be a solid.

#include <GTL/Mathematics/Intersection/2D/IntrLine2Triangle2.h>
#include <GTL/Mathematics/Intersection/1D/IntrIntervals.h>
#include <GTL/Mathematics/Primitives/ND/Segment.h>
#include <array>
#include <cstddef>

namespace gtl
{
    template <typename T>
    class TIQuery<T, Segment2<T>, Triangle2<T>>
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

        // The segment is P0 + t * (P1 - P0) for t in [0,1].
        Output operator()(Segment2<T> const& segment, Triangle2<T> const& triangle)
        {
            Output output{};
            FIQuery<T, Segment2<T>, Triangle2<T>> stQuery{};
            output.intersect = stQuery(segment, triangle).intersect;
            return output;
        }

    private:
        friend class UnitTestIntrSegment2Triangle2;
    };

    template <typename T>
    class FIQuery <T, Segment2<T>, Triangle2<T>>
        :
        public FIQuery<T, Line2<T>, Triangle2<T>>
    {
    public:
        struct Output
            :
            public FIQuery<T, Line2<T>, Triangle2<T>>::Output
        {
            // No additional information to compute.
        };

        // The segment is P0 + t * (P1 - P0) for t in [0,1].
        Output operator()(Segment2<T> const& segment, Triangle2<T> const& triangle)
        {
            Output output{};
            Vector2<T> const& segOrigin = segment.p[0];
            Vector2<T> segDirection = segment.p[1] - segment.p[0];
            DoQuery(segOrigin, segDirection, triangle, output);
            if (output.intersect)
            {
                for (std::size_t i = 0; i < 2; ++i)
                {
                    output.point[i] = segOrigin + output.parameter[i] * segDirection;
                }
            }
            return output;
        }

    protected:
        // The caller must ensure that on entry, 'output' is default
        // constructed as if there is no intersection. If an intersection is
        // found, the 'output' values will be modified accordingly.
        void DoQuery(Vector2<T> const& origin, Vector2<T> const& direction,
            Triangle2<T> const& triangle, Output& output)
        {
            FIQuery<T, Line2<T>, Triangle2<T>>::DoQuery(
                origin, direction, triangle, output);

            if (output.intersect)
            {
                // The line containing the segment intersects the triangle;
                // the t-interval is [t0,t1]. The segment intersects the
                // triangle as long as [t0,t1] overlaps the segment t-interval
                // [0,1].
                FIQuery<T, std::array<T, 2>, std::array<T, 2>> iiQuery{};
                std::array<T, 2> segInterval{ C_<T>(0), C_<T>(1) };
                auto iiOutput = iiQuery(output.parameter, segInterval);
                if (iiOutput.intersect)
                {
                    output.numIntersections = iiOutput.numIntersections;
                    output.parameter = iiOutput.overlap;
                }
                else
                {
                    // The line containing the segment does not intersect the
                    // triangle.
                    output = Output{};
                }
            }
        }

    private:
        friend class UnitTestIntrSegment2Triangle2;
    };
}
