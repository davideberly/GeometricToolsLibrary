// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.17

#pragma once

// The queries consider the box to be a solid. The test-intersection queries
// use the method of separating axes.
// https://www.geometrictools.com/Documentation/MethodOfSeparatingAxes.pdf
// The find-intersection queries use parametric clipping against the four
// edges of the box.

#include <GTL/Mathematics/Intersection/2D/IntrSegment2AlignedBox2.h>
#include <GTL/Mathematics/Primitives/ND/OrientedBox.h>
#include <array>
#include <cstddef>

namespace gtl
{
    template <typename T>
    class TIQuery<T, Segment2<T>, OrientedBox2<T>>
        :
        public TIQuery<T, Segment2<T>, AlignedBox2<T>>
    {
    public:
        struct Output
            :
            public TIQuery<T, Segment2<T>, AlignedBox2<T>>::Output
        {
            Output()
                :
                TIQuery<T, Segment2<T>, AlignedBox2<T>>::Output{}
            {
            }

            // No additional information to compute.
        };

        Output operator()(Segment2<T> const& segment, OrientedBox2<T> const& box)
        {
            // Transform the segment to the oriented-box coordinate system.
            Vector2<T> tmpOrigin{}, tmpDirection{};
            T segExtent{};
            segment.GetCenteredForm(tmpOrigin, tmpDirection, segExtent);
            Vector2<T> diff = tmpOrigin - box.center;
            Vector2<T> segOrigin
            {
                Dot(diff, box.axis[0]),
                Dot(diff, box.axis[1])
            };
            Vector2<T> segDirection
            {
                Dot(tmpDirection, box.axis[0]),
                Dot(tmpDirection, box.axis[1])
            };

            Output output{};
            this->DoQuery(segOrigin, segDirection, segExtent, box.extent, output);
            return output;
        }

    private:
        friend class UnitTestIntrSegment2OrientedBox2;
    };

    template <typename T>
    class FIQuery<T, Segment2<T>, OrientedBox2<T>>
        :
        public FIQuery<T, Segment2<T>, AlignedBox2<T>>
    {
    public:
        struct Output
            :
            public FIQuery<T, Segment2<T>, AlignedBox2<T>>::Output
        {
            Output()
                :
                FIQuery<T, Segment2<T>, AlignedBox2<T>>::Output{},
                cdeParameter{ C_<T>(0), C_<T>(0) }
            {
            }

            // The base class parameter[] values are t-values for the
            // segment parameterization (1-t)*p[0] + t*p[1], where t in [0,1].
            // The values in this class are s-values for the centered form
            // C + s * D, where s in [-e,e] and e is the extent of the
            // segment.
            std::array<T, 2> cdeParameter;
        };

        Output operator()(Segment2<T> const& segment, OrientedBox2<T> const& box)
        {
            // Transform the segment to the oriented-box coordinate system.
            Vector2<T> tmpOrigin{}, tmpDirection{};
            T segExtent{};
            segment.GetCenteredForm(tmpOrigin, tmpDirection, segExtent);
            Vector2<T> diff = tmpOrigin - box.center;
            Vector2<T> segOrigin
            {
                Dot(diff, box.axis[0]),
                Dot(diff, box.axis[1])
            };
            Vector2<T> segDirection
            {
                Dot(tmpDirection, box.axis[0]),
                Dot(tmpDirection, box.axis[1])
            };

            Output output{};
            this->DoQuery(segOrigin, segDirection, segExtent, box.extent, output);
            for (std::size_t i = 0; i < output.numIntersections; ++i)
            {
                // Compute the segment in the aligned-box coordinate system
                // and then translate it back to the original coordinates
                // using the box cener.
                output.point[i] = box.center + (segOrigin + output.parameter[i] * segDirection);
                output.cdeParameter[i] = output.parameter[i];

                // Convert the parameters from the centered form to the
                // endpoint form.
                output.parameter[i] = (output.parameter[i] / segExtent + C_<T>(1)) * C_<T>(1, 2);
            }
            return output;
        }

    private:
        friend class UnitTestIntrSegment2OrientedBox2;
    };
}
