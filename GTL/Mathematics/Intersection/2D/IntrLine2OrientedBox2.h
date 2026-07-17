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

#include <GTL/Mathematics/Intersection/2D/IntrLine2AlignedBox2.h>
#include <GTL/Mathematics/Primitives/ND/OrientedBox.h>

namespace gtl
{
    template <typename T>
    class TIQuery<T, Line2<T>, OrientedBox2<T>>
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

            // No additional relevant information to compute.
        };

        Output operator()(Line2<T> const& line, OrientedBox2<T> const& box)
        {
            // Transform the line to the oriented-box coordinate system.
            Vector2<T> diff = line.origin - box.center;
            Vector2<T> lineOrigin
            {
                Dot(diff, box.axis[0]),
                Dot(diff, box.axis[1])
            };
            Vector2<T> lineDirection
            {
                Dot(line.direction, box.axis[0]),
                Dot(line.direction, box.axis[1])
            };

            Output output{};
            this->DoQuery(lineOrigin, lineDirection, box.extent, output);
            return output;
        }

    private:
        friend class UnitTestIntrLine2OrientedBox2;
    };

    template <typename T>
    class FIQuery<T, Line2<T>, OrientedBox2<T>>
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
                FIQuery<T, Line2<T>, AlignedBox2<T>>::Output{}
            {
            }

            // No additional relevant information to compute.
        };

        Output operator()(Line2<T> const& line, OrientedBox2<T> const& box)
        {
            // Transform the line to the oriented-box coordinate system.
            Vector2<T> diff = line.origin - box.center;
            Vector2<T> lineOrigin
            {
                Dot(diff, box.axis[0]),
                Dot(diff, box.axis[1])
            };
            Vector2<T> lineDirection
            {
                Dot(line.direction, box.axis[0]),
                Dot(line.direction, box.axis[1])
            };

            Output output{};
            this->DoQuery(lineOrigin, lineDirection, box.extent, output);
            for (std::size_t i = 0; i < output.numIntersections; ++i)
            {
                output.point[i] = line.origin + output.parameter[i] * line.direction;
            }
            return output;
        }

    private:
        friend class UnitTestIntrLine2OrientedBox2;
    };
}
