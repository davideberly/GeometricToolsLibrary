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

#include <GTL/Mathematics/Intersection/IntersectionQuery.h>
#include <GTL/Mathematics/Primitives/ND/Line.h>
#include <GTL/Mathematics/Primitives/ND/AlignedBox.h>
#include <GTL/Mathematics/Algebra/Vector.h>
#include <array>
#include <cmath>
#include <cstddef>
#include <limits>

namespace gtl
{
    template <typename T>
    class TIQuery<T, Line2<T>, AlignedBox2<T>>
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

        Output operator()(Line2<T> const& line, AlignedBox2<T> const& box)
        {
            // Get the centered form of the aligned box. The axes are
            // implicitly Axis[d] = Vector2<T>::Unit(d).
            Vector2<T> boxCenter{}, boxExtent{};
            box.GetCenteredForm(boxCenter, boxExtent);

            // Transform the line to the aligned-box coordinate system.
            Vector2<T> lineOrigin = line.origin - boxCenter;

            Output output{};
            DoQuery(lineOrigin, line.direction, boxExtent, output);
            return output;
        }

    protected:
        void DoQuery(Vector2<T> const& lineOrigin,
            Vector2<T> const& lineDirection, Vector2<T> const& boxExtent,
            Output& output)
        {
            T LHS = std::fabs(DotPerp(lineDirection, lineOrigin));
            T RHS =
                boxExtent[0] * std::fabs(lineDirection[1]) +
                boxExtent[1] * std::fabs(lineDirection[0]);
            output.intersect = (LHS <= RHS);
        }

    private:
        friend class UnitTestIntrLine2AlignedBox2;
    };

    template <typename T>
    class FIQuery<T, Line2<T>, AlignedBox2<T>>
    {
    public:
        struct Output
        {
            Output()
                :
                intersect(false),
                numIntersections(0),
                parameter{ C_<T>(0), C_<T>(0) },
                point{}
            {
            }

            bool intersect;
            std::size_t numIntersections;
            std::array<T, 2> parameter;
            std::array<Vector2<T>, 2> point;
        };

        Output operator()(Line2<T> const& line, AlignedBox2<T> const& box)
        {
            // Get the centered form of the aligned box. The axes are
            // implicitly Axis[d] = Vector2<T>::Unit(d).
            Vector2<T> boxCenter, boxExtent;
            box.GetCenteredForm(boxCenter, boxExtent);

            // Transform the line to the aligned-box coordinate system.
            Vector2<T> lineOrigin = line.origin - boxCenter;

            Output output{};
            DoQuery(lineOrigin, line.direction, boxExtent, output);
            for (std::size_t i = 0; i < output.numIntersections; ++i)
            {
                output.point[i] = line.origin + output.parameter[i] * line.direction;
            }
            return output;
        }

    protected:
        void DoQuery(Vector2<T> const& lineOrigin,
            Vector2<T> const& lineDirection, Vector2<T> const& boxExtent,
            Output& output)
        {
            // The line t-values are in the interval (-infinity,+infinity).
            // Clip the line against all four planes of an aligned box in
            // centered form. The output.numPoints is
            //  0, no intersection
            //  1, intersect in a single point (t0 is line parameter of point)
            //  2, intersect in a segment (line parameter interval is [t0,t1])
            T t0 = -std::numeric_limits<T>::max();
            T t1 = std::numeric_limits<T>::max();
            if (Clip(+lineDirection[0], -lineOrigin[0] - boxExtent[0], t0, t1) &&
                Clip(-lineDirection[0], +lineOrigin[0] - boxExtent[0], t0, t1) &&
                Clip(+lineDirection[1], -lineOrigin[1] - boxExtent[1], t0, t1) &&
                Clip(-lineDirection[1], +lineOrigin[1] - boxExtent[1], t0, t1))
            {
                output.intersect = true;
                if (t1 > t0)
                {
                    output.numIntersections = 2;
                    output.parameter[0] = t0;
                    output.parameter[1] = t1;
                }
                else
                {
                    output.numIntersections = 1;
                    output.parameter[0] = t0;
                    output.parameter[1] = t0;  // Used by derived classes.
                }
                return;
            }

            output.intersect = false;
            output.numIntersections = 0;
        }

    private:
        // Test whether the current clipped segment intersects the current
        // test plane. If the return value is 'true', the segment does
        // intersect the plane and is clipped; otherwise, the segment is
        // culled (no intersection with box).
        static bool Clip(T const& denom, T const& numer, T& t0, T& t1)
        {
            if (denom > C_<T>(0))
            {
                if (numer > denom * t1)
                {
                    return false;
                }
                if (numer > denom * t0)
                {
                    t0 = numer / denom;
                }
                return true;
            }
            else if (denom < C_<T>(0))
            {
                if (numer > denom * t0)
                {
                    return false;
                }
                if (numer > denom * t1)
                {
                    t1 = numer / denom;
                }
                return true;
            }
            else
            {
                return numer <= C_<T>(0);
            }
        }

    private:
        friend class UnitTestIntrLine2AlignedBox2;
    };
}
