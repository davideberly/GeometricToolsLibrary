// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.19

#pragma once

// Compute the intersection between a line and a solid rectangle in 3D.
// 
// The line is P + t * D, where D is not required to be unit length.
// 
// The rectangle has center C, unit-length axis directions W[0] and W[1], and
// extents e[0] and e[1]. A rectangle point is X = C + sum_{i=0}^2 s[i] * W[i]
// where |s[i]| <= e[i] for all i.
// 
// The intersection point, if any, is stored in output.point. The
// corresponding line parameter t is stored in output.parameter. The
// corresponding rectangle parameters s[] are stored in output.rectCoord[].
// When the line is in the plane of the rectangle and intersects the
// rectangle, the queries state that there are no intersections.
//
// TODO: Modify to support non-unit-length W[]. Return the point or
// segment of intersection when the line is in the plane of the rectangle
// and intersects the rectangle.

#include <GTL/Mathematics/Intersection/IntersectionQuery.h>
#include <GTL/Mathematics/Primitives/ND/Line.h>
#include <GTL/Mathematics/Primitives/ND/Rectangle.h>
#include <GTL/Mathematics/Algebra/Vector.h>
#include <array>
#include <cmath>
#include <cstddef>

namespace gtl
{
    template <typename T>
    class TIQuery<T, Line3<T>, Rectangle3<T>>
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

        Output operator()(Line3<T> const& line, Rectangle3<T> const& rectangle)
        {
            Output output{};

            // Compute the offset origin and rectangle normal.
            Vector3<T> diff = line.origin - rectangle.center;
            Vector3<T> normal = Cross(rectangle.axis[0], rectangle.axis[1]);

            // Solve Q + t*D = s0*W0 + s1*W1 (Q = diff, D = line direction,
            // W0 = edge 0 direction, W1 = edge 1 direction, N = Cross(W0,W1))
            // by
            //   s0 = Dot(W1,Cross(D,Q)) / Dot(D,N)
            //   s1 = -Dot(W0,Cross(D,Q)) / Dot(D,N)
            //   t = -Dot(Q,N) / Dot(D,N)
            T DdN = Dot(line.direction, normal);
            if (DdN == C_<T>(0))
            {
                // Line and triangle are parallel, call it a "no intersection"
                // even if the line and triangle are coplanar and
                // intersecting.
                output.intersect = false;
                return output;
            }

            T absDdN = std::fabs(DdN);
            Vector3<T> DxQ = Cross(line.direction, diff);
            T W1dDxQ = Dot(rectangle.axis[1], DxQ);
            if (std::fabs(W1dDxQ) <= rectangle.extent[0] * absDdN)
            {
                T W0dDxQ = Dot(rectangle.axis[0], DxQ);
                if (std::fabs(W0dDxQ) <= rectangle.extent[1] * absDdN)
                {
                    output.intersect = true;
                    return output;
                }
            }
            output.intersect = false;
            return output;
        }
    };

    template <typename T>
    class FIQuery<T, Line3<T>, Rectangle3<T>>
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

        Output operator()(Line3<T> const& line, Rectangle3<T> const& rectangle)
        {
            Output output{};

            // Compute the offset origin and rectangle normal.
            Vector3<T> diff = line.origin - rectangle.center;
            Vector3<T> normal = Cross(rectangle.axis[0], rectangle.axis[1]);

            // Solve Q + t*D = s0*W0 + s1*W1 (Q = diff, D = line direction,
            // W0 = edge 0 direction, W1 = edge 1 direction, N = Cross(W0,W1))
            // by
            //   s0 = Dot(W1,Cross(D,Q)) / Dot(D,N)
            //   s1 = -Dot(W0,Cross(D,Q)) / Dot(D,N)
            //   t = -Dot(Q,N) / Dot(D,N)
            T DdN = Dot(line.direction, normal);
            if (DdN == C_<T>(0))
            {
                // Line and triangle are parallel, call it a "no intersection"
                // even if the line and triangle are coplanar and
                // intersecting.
                output.intersect = false;
                return output;
            }

            T absDdN = std::fabs(DdN);
            Vector3<T> DxQ = Cross(line.direction, diff);
            T W1dDxQ = Dot(rectangle.axis[1], DxQ);
            if (std::fabs(W1dDxQ) <= rectangle.extent[0] * absDdN)
            {
                T W0dDxQ = Dot(rectangle.axis[0], DxQ);
                if (std::fabs(W0dDxQ) <= rectangle.extent[1] * absDdN)
                {
                    output.intersect = true;
                    output.parameter = -Dot(diff, normal) / DdN;
                    output.rectCoord[0] = W1dDxQ / DdN;
                    output.rectCoord[1] = -W0dDxQ / DdN;
                    output.point = line.origin + output.parameter * line.direction;
                    return output;
                }
            }
            output.intersect = false;
            return output;
        }

        friend class UnitTestLine3Rectangle3;
    };
}
