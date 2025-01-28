// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.28

#pragma once

// The test-intersection queries use the method of separating axes.
// https://www.geometrictools.com/Documentation/MethodOfSeparatingAxes.pdf
// The find-intersection queries use parametric clipping against the six
// faces of the box.  The find-intersection queries use Liang-Barsky
// clipping.  The queries consider the box to be a solid.  The algorithms
// are described in
// https://www.geometrictools.com/Documentation/IntersectionLineBox.pdf

#include <GTL/Mathematics/Intersection/1D/IntrIntervals.h>
#include <GTL/Mathematics/Intersection/3D/IntrLine3AlignedBox3.h>
#include <GTL/Mathematics/Primitives/ND/Segment.h>
#include <array>
#include <cmath>
#include <cstddef>

namespace gtl
{
    template <typename T>
    class TIQuery<T, Segment3<T>, AlignedBox3<T>>
        :
        public TIQuery<T, Line3<T>, AlignedBox3<T>>
    {
    public:
        struct Output
            :
            public TIQuery<T, Line3<T>, AlignedBox3<T>>::Output
        {
            // No additional information to compute.
            Output() = default;
        };

        Output operator()(Segment3<T> const& segment, AlignedBox3<T> const& box)
        {
            // Get the centered form of the aligned box. The axes are
            // implicitly axis[d] = Vector3<T>::Unit(d).
            Vector3<T> boxCenter{}, boxExtent{};
            box.GetCenteredForm(boxCenter, boxExtent);

            // Transform the segment to a centered form in the aligned-box
            // coordinate system.
            Vector3<T> transformedP0 = segment.p[0] - boxCenter;
            Vector3<T> transformedP1 = segment.p[1] - boxCenter;
            Segment3<T> transformedSegment(transformedP0, transformedP1);
            Vector3<T> segOrigin{}, segDirection{};
            T segExtent{};
            transformedSegment.GetCenteredForm(segOrigin, segDirection, segExtent);

            Output output{};
            DoQuery(segOrigin, segDirection, segExtent, boxExtent, output);
            return output;
        }

    protected:
        // The caller must ensure that on entry, 'output' is default
        // constructed as if there is no intersection. If an intersection is
        // found, the 'output' values will be modified accordingly.
        void DoQuery(Vector3<T> const& segOrigin, Vector3<T> const& segDirection,
            T segExtent, Vector3<T> const& boxExtent, Output& output)
        {
            for (std::size_t i = 0; i < 3; ++i)
            {
                if (std::fabs(segOrigin[i]) > boxExtent[i] + segExtent * std::fabs(segDirection[i]))
                {
                    output.intersect = false;
                    return;
                }
            }

            TIQuery<T, Line3<T>, AlignedBox3<T>>::DoQuery(
                segOrigin, segDirection, boxExtent, output);
        }

    private:
        friend class UnitTestIntrSegment3AlignedBox3;
    };

    template <typename T>
    class FIQuery<T, Segment3<T>, AlignedBox3<T>>
        :
        public FIQuery<T, Line3<T>, AlignedBox3<T>>
    {
    public:
        struct Output
            :
            public FIQuery<T, Line3<T>, AlignedBox3<T>>::Output
        {
            // No additional information to compute.
            Output() = default;
        };

        Output operator()(Segment3<T> const& segment, AlignedBox3<T> const& box)
        {
            // Get the centered form of the aligned box. The axes are
            // implicitly axis[d] = Vector3<T>::Unit(d).
            Vector3<T> boxCenter{}, boxExtent{};
            box.GetCenteredForm(boxCenter, boxExtent);

            // Transform the segment to a centered form in the aligned-box
            // coordinate system.
            Vector3<T> transformedP0 = segment.p[0] - boxCenter;
            Vector3<T> transformedP1 = segment.p[1] - boxCenter;
            Segment3<T> transformedSegment(transformedP0, transformedP1);
            Vector3<T> segOrigin{}, segDirection{};
            T segExtent{};
            transformedSegment.GetCenteredForm(segOrigin, segDirection, segExtent);

            Output output{};
            DoQuery(segOrigin, segDirection, segExtent, boxExtent, output);
            if (output.intersect)
            {
                // The segment origin is in aligned-box coordinates. Transform
                // it back to the original space.
                segOrigin += boxCenter;
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
        void DoQuery(Vector3<T> const& segOrigin, Vector3<T> const& segDirection,
            T const& segExtent, Vector3<T> const& boxExtent, Output& output)
        {
            FIQuery<T, Line3<T>, AlignedBox3<T>>::DoQuery(
                segOrigin, segDirection, boxExtent, output);

            if (output.intersect)
            {
                // The line containing the segment intersects the box; the
                // t-interval is [t0,t1]. The segment intersects the box as
                // long as [t0,t1] overlaps the segment t-interval
                // [-segExtent,+segExtent].
                FIQuery<T, std::array<T, 2>, std::array<T, 2>> iiQuery{};

                std::array<T, 2> interval0{ output.parameter[0], output.parameter[1]  };
                std::array<T, 2> interval1{ -segExtent, segExtent };
                auto iiOutput = iiQuery(interval0, interval1);
                if (iiOutput.numIntersections > 0)
                {
                    output.numIntersections = iiOutput.numIntersections;
                    output.parameter = iiOutput.overlap;
                }
                else
                {
                    // The line containing the segment does not intersect
                    // the box.
                    output = Output{};
                }
            }
        }

    private:
        friend class UnitTestIntrSegment3AlignedBox3;
    };
}
