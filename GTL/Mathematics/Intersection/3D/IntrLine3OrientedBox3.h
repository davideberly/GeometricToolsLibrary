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
// faces of the box. The find-intersection queries use Liang-Barsky
// clipping.  The queries consider the box to be a solid. The algorithms
// are described in
// https://www.geometrictools.com/Documentation/IntersectionLineBox.pdf

#include <GTL/Mathematics/Intersection/3D/IntrLine3AlignedBox3.h>
#include <GTL/Mathematics/Primitives/ND/OrientedBox.h>
#include <cstddef>

namespace gtl
{
    template <typename T>
    class TIQuery<T, Line3<T>, OrientedBox3<T>>
        :
        public TIQuery<T, Line3<T>, AlignedBox3<T>>
    {
    public:
        struct Output
            :
            public TIQuery<T, Line3<T>, AlignedBox3<T>>::Output
        {
            // No additional relevant information to compute.
            Output() = default;
        };

        Output operator()(Line3<T> const& line, OrientedBox3<T> const& box)
        {
            // Transform the line to the oriented-box coordinate system.
            Vector3<T> diff = line.origin - box.center;
            Vector3<T> lineOrigin
            {
                Dot(diff, box.axis[0]),
                Dot(diff, box.axis[1]),
                Dot(diff, box.axis[2])
            };
            Vector3<T> lineDirection
            {
                Dot(line.direction, box.axis[0]),
                Dot(line.direction, box.axis[1]),
                Dot(line.direction, box.axis[2])
            };

            Output output{};
            this->DoQuery(lineOrigin, lineDirection, box.extent, output);
            return output;
        }

    private:
        friend class UnitTestIntrLine3OrientedBox3;
    };

    template <typename T>
    class FIQuery<T, Line3<T>, OrientedBox3<T>>
        :
        public FIQuery<T, Line3<T>, AlignedBox3<T>>
    {
    public:
        struct Output
            :
            public FIQuery<T, Line3<T>, AlignedBox3<T>>::Output
        {
            // No additional relevant information to compute.
            Output() = default;
        };

        Output operator()(Line3<T> const& line, OrientedBox3<T> const& box)
        {
            // Transform the line to the oriented-box coordinate system.
            Vector3<T> diff = line.origin - box.center;
            Vector3<T> lineOrigin
            {
                Dot(diff, box.axis[0]),
                Dot(diff, box.axis[1]),
                Dot(diff, box.axis[2])
            };
            Vector3<T> lineDirection
            {
                Dot(line.direction, box.axis[0]),
                Dot(line.direction, box.axis[1]),
                Dot(line.direction, box.axis[2])
            };

            Output output{};
            this->DoQuery(lineOrigin, lineDirection, box.extent, output);
            if (output.intersect)
            {
                for (std::size_t i = 0; i < 2; ++i)
                {
                    output.point[i] = line.origin + output.parameter[i] * line.direction;
                }
            }
            return output;
        }

    private:
        friend class UnitTestIntrLine3OrientedBox3;
    };
}
