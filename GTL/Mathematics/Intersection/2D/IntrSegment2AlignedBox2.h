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

#include <GTL/Mathematics/Intersection/1D/IntrIntervals.h>
#include <GTL/Mathematics/Intersection/2D/IntrLine2AlignedBox2.h>
#include <GTL/Mathematics/Primitives/ND/Segment.h>
#include <GTL/Mathematics/Algebra/Vector.h>
#include <array>
#include <cmath>
#include <cstddef>

namespace gtl
{
    template <typename T>
    class TIQuery<T, Segment2<T>, AlignedBox2<T>>
        :
        public TIQuery<T, Line2<T>, AlignedBox2<T>>
    {
    public:
        struct Output
            :
            public TIQuery<T, Line2<T>, AlignedBox2<T>>::Output
        {
            Output()
                :
                TIQuery<T, Line2<T>, AlignedBox2<T>>::Output{}
            {
            }

            // No additional information to compute.
        };

        Output operator()(Segment2<T> const& segment, AlignedBox2<T> const& box)
        {
            // Get the centered form of the aligned box. The axes are
            // implicitly Axis[d] = Vector2<T>::Unit(d).
            Vector2<T> boxCenter{}, boxExtent{};
            box.GetCenteredForm(boxCenter, boxExtent);

            // Transform the segment to a centered form in the aligned-box
            // coordinate system.
            Vector2<T> transformedP0 = segment.p[0] - boxCenter;
            Vector2<T> transformedP1 = segment.p[1] - boxCenter;
            Segment2<T> transformedSegment(transformedP0, transformedP1);
            Vector2<T> segOrigin{}, segDirection{};
            T segExtent{};
            transformedSegment.GetCenteredForm(segOrigin, segDirection, segExtent);

            Output output{};
            DoQuery(segOrigin, segDirection, segExtent, boxExtent, output);
            return output;
        }

    protected:
        void DoQuery(Vector2<T> const& segOrigin,
            Vector2<T> const& segDirection, T segExtent,
            Vector2<T> const& boxExtent, Output& output)
        {
            for (std::size_t i = 0; i < 2; ++i)
            {
                T lhs = std::fabs(segOrigin[i]);
                T rhs = boxExtent[i] + segExtent * std::fabs(segDirection[i]);
                if (lhs > rhs)
                {
                    output.intersect = false;
                    return;
                }
            }

            TIQuery<T, Line2<T>, AlignedBox2<T>>::DoQuery(segOrigin,
                segDirection, boxExtent, output);
        }

    private:
        friend class UnitTestIntrSegment2AlignedBox2;
    };

    template <typename T>
    class FIQuery<T, Segment2<T>, AlignedBox2<T>>
        :
        public FIQuery<T, Line2<T>, AlignedBox2<T>>
    {
    public:
        struct Output
            :
            public FIQuery<T, Line2<T>, AlignedBox2<T>>::Output
        {
            Output()
                :
                FIQuery<T, Line2<T>, AlignedBox2<T>>::Output{},
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

        Output operator()(Segment2<T> const& segment, AlignedBox2<T> const& box)
        {
            // Get the centered form of the aligned box.  The axes are
            // implicitly Axis[d] = Vector2<T>::Unit(d).
            Vector2<T> boxCenter{}, boxExtent{};
            box.GetCenteredForm(boxCenter, boxExtent);

            // Transform the segment to a centered form in the aligned-box
            // coordinate system.
            Vector2<T> transformedP0 = segment.p[0] - boxCenter;
            Vector2<T> transformedP1 = segment.p[1] - boxCenter;
            Segment2<T> transformedSegment(transformedP0, transformedP1);
            Vector2<T> segOrigin{}, segDirection{};
            T segExtent{};
            transformedSegment.GetCenteredForm(segOrigin, segDirection, segExtent);

            Output output{};
            DoQuery(segOrigin, segDirection, segExtent, boxExtent, output);
            for (std::size_t i = 0; i < output.numIntersections; ++i)
            {
                // Compute the segment in the aligned-box coordinate system
                // and then translate it back to the original coordinates
                // using the box cener.
                output.point[i] = boxCenter + (segOrigin + output.parameter[i] * segDirection);
                output.cdeParameter[i] = output.parameter[i];

                // Convert the parameters from the centered form to the
                // endpoint form.
                output.parameter[i] = (output.parameter[i] / segExtent + C_<T>(1)) * C_<T>(1, 2);
            }
            return output;
        }

    protected:
        void DoQuery(Vector2<T> const& segOrigin,
            Vector2<T> const& segDirection, T segExtent,
            Vector2<T> const& boxExtent, Output& output)
        {
            FIQuery<T, Line2<T>, AlignedBox2<T>>::DoQuery(segOrigin,
                segDirection, boxExtent, output);

            if (output.intersect)
            {
                // The line containing the segment intersects the box; the
                // t-interval is [t0,t1]. The segment intersects the box as
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
        friend class UnitTestIntrSegment2AlignedBox2;
    };
}
