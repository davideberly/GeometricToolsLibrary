// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.28

#pragma once

// The queries consider the cylinder to be a solid.

#include <GTL/Mathematics/Intersection/1D/IntrIntervals.h>
#include <GTL/Mathematics/Intersection/3D/IntrLine3Cylinder3.h>
#include <GTL/Mathematics/Primitives/ND/Segment.h>
#include <array>
#include <cstddef>

namespace gtl
{
    template <typename T>
    class FIQuery<T, Segment3<T>, Cylinder3<T>>
        :
        public FIQuery<T, Line3<T>, Cylinder3<T>>
    {
    public:
        struct Output
            :
            public FIQuery<T, Line3<T>, Cylinder3<T>>::Output
        {
            // No additional information to compute.
            Output() = default;
        };

        Output operator()(Segment3<T> const& segment, Cylinder3<T> const& cylinder)
        {
            Vector3<T> segOrigin{};     // P
            Vector3<T> segDirection{};  // D
            T segExtent{};              // e
            segment.GetCenteredForm(segOrigin, segDirection, segExtent);

            Output output{};
            DoQuery(segOrigin, segDirection, segExtent, cylinder, output);
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
        void DoQuery(Vector3<T> const& segOrigin,
            Vector3<T> const& segDirection, T segExtent,
            Cylinder3<T> const& cylinder, Output& output)
        {
            FIQuery<T, Line3<T>, Cylinder3<T>>::DoQuery(
                segOrigin, segDirection, cylinder, output);

            if (output.intersect)
            {
                // The line containing the segment intersects the cylinder;
                // the t-interval is [t0,t1]. The segment intersects the
                // cylinder as long as [t0,t1] overlaps the segment t-interval
                // [-segExtent,+segExtent].
                std::array<T, 2> segInterval = { -segExtent, segExtent };
                FIQuery<T, std::array<T, 2>, std::array<T, 2>> iiQuery{};
                auto iiOutput = iiQuery(output.parameter, segInterval);
                if (iiOutput.intersect)
                {
                    output.numIntersections = iiOutput.numIntersections;
                    output.parameter = iiOutput.overlap;
                }
                else
                {
                    // The line containing the segment does not intersect
                    // the cylinder.
                    output = Output{};
                }
            }
        }

    private:
        friend class UnitTestIntrSegment3Cylinder3;
    };
}
