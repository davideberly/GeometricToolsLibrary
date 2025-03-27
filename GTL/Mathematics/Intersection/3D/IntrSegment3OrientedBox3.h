// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

// The test-intersection queries use the method of separating axes.
// https://www.geometrictools.com/Documentation/MethodOfSeparatingAxes.pdf
// The find-intersection queries use parametric clipping against the six
// faces of the box.  The find-intersection queries use Liang-Barsky
// clipping.  The queries consider the box to be a solid.  The algorithms
// are described in
// https://www.geometrictools.com/Documentation/IntersectionLineBox.pdf

#include <GTL/Mathematics/Intersection/3D/IntrSegment3AlignedBox3.h>
#include <GTL/Mathematics/Primitives/ND/OrientedBox.h>
#include <cstddef>

namespace gtl
{
    template <typename T>
    class TIQuery<T, Segment3<T>, OrientedBox3<T>>
        :
        public TIQuery<T, Segment3<T>, AlignedBox3<T>>
    {
    public:
        struct Output
            :
            public TIQuery<T, Segment3<T>, AlignedBox3<T>>::Output
        {
            // No additional information to compute.
            Output() = default;
        };

        Output operator()(Segment3<T> const& segment, OrientedBox3<T> const& box)
        {
            // Transform the segment to the oriented-box coordinate system.
            Vector3<T> tmpOrigin{}, tmpDirection{};
            T segExtent{};
            segment.GetCenteredForm(tmpOrigin, tmpDirection, segExtent);
            Vector3<T> diff = tmpOrigin - box.center;
            Vector3<T> segOrigin
            {
                Dot(diff, box.axis[0]),
                Dot(diff, box.axis[1]),
                Dot(diff, box.axis[2])
            };
            Vector3<T> segDirection
            {
                Dot(tmpDirection, box.axis[0]),
                Dot(tmpDirection, box.axis[1]),
                Dot(tmpDirection, box.axis[2])
            };

            Output output{};
            this->DoQuery(segOrigin, segDirection, segExtent, box.extent, output);
            return output;
        }

    private:
        friend class UnitTestIntrSegment3OrientedBox3;
    };

    template <typename T>
    class FIQuery<T, Segment3<T>, OrientedBox3<T>>
        :
        public FIQuery<T, Segment3<T>, AlignedBox3<T>>
    {
    public:
        struct Output
            :
            public FIQuery<T, Segment3<T>, AlignedBox3<T>>::Output
        {
            // No additional relevant information to compute.
            Output() = default;
        };

        Output operator()(Segment3<T> const& segment, OrientedBox3<T> const& box)
        {
            // Transform the segment to the oriented-box coordinate system.
            Vector3<T> tmpOrigin{}, tmpDirection{};
            T segExtent{};
            segment.GetCenteredForm(tmpOrigin, tmpDirection, segExtent);
            Vector3<T> diff = tmpOrigin - box.center;
            Vector3<T> segOrigin
            {
                Dot(diff, box.axis[0]),
                Dot(diff, box.axis[1]),
                Dot(diff, box.axis[2])
            };
            Vector3<T> segDirection
            {
                Dot(tmpDirection, box.axis[0]),
                Dot(tmpDirection, box.axis[1]),
                Dot(tmpDirection, box.axis[2])
            };

            Output output{};
            this->DoQuery(segOrigin, segDirection, segExtent, box.extent, output);
            if (output.intersect)
            {
                for (std::size_t i = 0; i < 2; ++i)
                {
                    // Compute the intersection point in the oriented-box
                    // coordinate system.
                    Vector3<T> y = segOrigin + output.parameter[i] * segDirection;

                    // Transform the intersection point to the original coordinate
                    // system.
                    output.point[i] = box.center;
                    for (std::size_t j = 0; j < 3; ++j)
                    {
                        output.point[i] += y[j] * box.axis[j];
                    }
                }
            }
            return output;
        }

    private:
        friend class UnitTestIntrSegment3OrientedBox3;
    };
}
