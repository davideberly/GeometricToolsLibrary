// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.17

#pragma once

// The queries consider the circle to be a solid (disk).

#include <GTL/Mathematics/Intersection/1D/IntrIntervals.h>
#include <GTL/Mathematics/Intersection/2D/IntrLine2Circle2.h>
#include <GTL/Mathematics/Primitives/ND/Segment.h>
#include <array>
#include <cstddef>

namespace gtl
{
    template <typename T>
    class TIQuery<T, Segment2<T>, Circle2<T>>
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

        Output operator()(Segment2<T> const& segment, Circle2<T> const& circle)
        {
            Output output{};
            FIQuery<T, Segment2<T>, Circle2<T>> scQuery{};
            output.intersect = scQuery(segment, circle).intersect;
            return output;
        }

    private:
        friend class UnitTestIntrSegment2Circle2;
    };

    template <typename T>
    class FIQuery<T, Segment2<T>, Circle2<T>>
        :
        public FIQuery<T, Line2<T>, Circle2<T>>
    {
    public:
        struct Output
            :
            public FIQuery<T, Line2<T>, Circle2<T>>::Output
        {
            Output()
                :
                FIQuery<T, Line2<T>, Circle2<T>>::Output{}
            {
            }

            // No additional information to compute.
        };

        Output operator()(Segment2<T> const& segment, Circle2<T> const& circle)
        {
            Vector2<T> segOrigin{}, segDirection{};
            T segExtent{};
            segment.GetCenteredForm(segOrigin, segDirection, segExtent);

            Output output{};
            DoQuery(segOrigin, segDirection, segExtent, circle, output);
            for (std::size_t i = 0; i < output.numIntersections; ++i)
            {
                output.point[i] = segOrigin + output.parameter[i] * segDirection;
            }
            return output;
        }

    protected:
        void DoQuery(Vector2<T> const& segOrigin,
            Vector2<T> const& segDirection, T segExtent,
            Circle2<T> const& circle, Output& output)
        {
            FIQuery<T, Line2<T>, Circle2<T>>::DoQuery(segOrigin,
                segDirection, circle, output);

            if (output.intersect)
            {
                // The line containing the segment intersects the disk; the
                // t-interval is [t0,t1].  The segment intersects the disk as
                // long as [t0,t1] overlaps the segment t-interval
                // [-segExtent,+segExtent].
                std::array<T, 2> segInterval = { -segExtent, segExtent };
                FIQuery<T, std::array<T, 2>, std::array<T, 2>> iiQuery{};
                auto iiOutput = iiQuery(output.parameter, segInterval);
                output.intersect = iiOutput.intersect;
                output.numIntersections = iiOutput.numIntersections;
                output.parameter = iiOutput.overlap;
            }
        }

    private:
        friend class UnitTestIntrSegment2Circle2;
    };
}
