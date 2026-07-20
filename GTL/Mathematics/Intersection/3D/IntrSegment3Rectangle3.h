// David Eberly, Geometric Tools, Redmond WA 98052
// Copyright (c) 1998-2026
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt
// https://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 0.0.2026.07.19

#pragma once

// Compute the intersection between a segment and a solid rectangle in 3D.
// 
// The segment is P0 + t * (P1 - P0) for 0 <= t <= 1. The direction D = P1-P0
// is generally not unit length.
// 
// The rectangle has center C, unit-length axis directions W[0] and W[1], and
// extents e[0] and e[1]. A rectangle point is X = C + sum_{i=0}^2 s[i] * W[i]
// where |s[i]| <= e[i] for all i.
// 
// The intersection point, if any, is stored in output.point. The
// corresponding segment parameter t is stored in output.parameter. The
// corresponding rectangle parameters s[] are stored in output.rectCoord[].
// When the segment is in the plane of the rectangle and intersects the
// rectangle, the queries state that there are no intersections.
//
// TODO: Modify to support non-unit-length W[]. Return the point or
// segment of intersection when the segment is in the plane of the rectangle
// and intersects the rectangle.

#include <GTL/Mathematics/Intersection/3D/IntrLine3Rectangle3.h>
#include <GTL/Mathematics/Primitives/ND/Segment.h>
#include <array>

namespace gtl
{
    template <typename T>
    class TIQuery<T, Segment3<T>, Rectangle3<T>>
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

        Output operator()(Segment3<T> const& segment, Rectangle3<T> const& rectangle)
        {
            Output output{};

            T const zero = C_<T>(0);
            T const one = C_<T>(1);
            FIQuery<T, Line3<T>, Rectangle3<T>> lrQuery{};
            Line3<T> line(segment.p[0], segment.p[1] - segment.p[0]);
            auto lrOutput = lrQuery(line, rectangle);
            if (lrOutput.intersect)
            {
                if (zero <= lrOutput.parameter && lrOutput.parameter <= one)
                {
                    // The line-rectangle intersection is on the segment.
                    output.intersect = true;
                    return output;
                }
            }

            output.intersect = false;
            return output;
        }
    };

    template <typename T>
    class FIQuery<T, Segment3<T>, Rectangle3<T>>
    {
    public:
        struct Output
        {
            Output()
                :
                intersect(false),
                parameter(C_<T>(0)),
                rectCoord{ C_<T>(0), C_<T>(0), C_<T>(0) },
                point{ C_<T>(0), C_<T>(0), C_<T>(0) }
            {
            }

            bool intersect;
            T parameter;
            std::array<T, 3> rectCoord;
            Vector3<T> point;
        };

        Output operator()(Segment3<T> const& segment, Rectangle3<T> const& rectangle)
        {
            Output output{};

            T const zero = C_<T>(0);
            T const one = C_<T>(1);
            FIQuery<T, Line3<T>, Rectangle3<T>> lrQuery{};
            Line3<T> line(segment.p[0], segment.p[1] - segment.p[0]);
            auto lrOutput = lrQuery(line, rectangle);
            if (lrOutput.intersect)
            {
                if (zero <= lrOutput.parameter && lrOutput.parameter <= one)
                {
                    // The line-rectangle intersection is on the segment.
                    output.intersect = true;
                    output.parameter = lrOutput.parameter;
                    output.rectCoord = lrOutput.rectCoord;
                    output.point = lrOutput.point;
                    return output;
                }
            }

            output.intersect = false;
            return output;
        }

        friend class UnitTestSegment3Rectangle3;
    };
}
