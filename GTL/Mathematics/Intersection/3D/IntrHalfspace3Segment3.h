// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.18

#pragma once

// Queries for intersection of objects with halfspaces. These are useful for
// containment testing, object culling and clipping.

#include <GTL/Mathematics/Intersection/IntersectionQuery.h>
#include <GTL/Mathematics/Primitives/ND/Halfspace.h>
#include <GTL/Mathematics/Primitives/ND/Segment.h>
#include <GTL/Mathematics/Algebra/Vector.h>
#include <algorithm>
#include <array>
#include <cstddef>

namespace gtl
{
    template <typename T>
    class TIQuery<T, Halfspace3<T>, Segment3<T>>
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

        Output operator()(Halfspace3<T> const& halfspace, Segment3<T> const& segment)
        {
            Output output{};

            // Project the segment endpoints onto the normal line. The plane
            // of the halfspace occurs at the origin (zero) of the normal
            // line.
            std::array<T, 2> s{};
            for (std::size_t i = 0; i < 2; ++i)
            {
                s[i] = Dot(halfspace.normal, segment.p[i]) - halfspace.constant;
            }

            // The segment and halfspace intersect when the projection
            // interval maximum is nonnegative.
            output.intersect = (std::max(s[0], s[1]) >= C_<T>(0));
            return output;
        }

    private:
        friend class UnitTestIntrHalfspace3Segment3;
    };

    template <typename T>
    class FIQuery<T, Halfspace3<T>, Segment3<T>>
    {
    public:
        struct Output
        {
            Output()
                :
                intersect(false),
                numPoints(0),
                point{}
            {
            }

            bool intersect;

            // The segment is clipped against the plane defining the 
            // halfspace. The 'numPoints' is either 0 (no intersection),
            // 1 (point) or 2 (segment).
            std::size_t numPoints;
            std::array<Vector3<T>, 2> point;
        };

        Output operator()(Halfspace3<T> const& halfspace, Segment3<T> const& segment)
        {
            // Determine on which side of the plane the endpoints lie. The
            // table of possibilities is listed next with n = numNegative,
            // p = numPositive, and z = numZero.
            //
            //   n p z  intersection
            //   -------------------------
            //   0 2 0  segment (original)
            //   0 1 1  segment (original)
            //   0 0 2  segment (original)
            //   1 1 0  segment (clipped)
            //   1 0 1  point (endpoint)
            //   2 0 0  none

            std::array<T, 2> s{};
            std::size_t numPositive = 0, numNegative = 0;
            for (std::size_t i = 0; i < 2; ++i)
            {
                s[i] = Dot(halfspace.normal, segment.p[i]) - halfspace.constant;
                if (s[i] > C_<T>(0))
                {
                    ++numPositive;
                }
                else if (s[i] < C_<T>(0))
                {
                    ++numNegative;
                }
            }

            Output output{};

            if (numNegative == 0)
            {
                // The segment is in the halfspace.
                output.intersect = true;
                output.numPoints = 2;
                output.point[0] = segment.p[0];
                output.point[1] = segment.p[1];
            }
            else if (numNegative == 1)
            {
                output.intersect = true;
                output.numPoints = 1;
                if (numPositive == 1)
                {
                    // The segment is intersected at an interior point.
                    output.point[0] = segment.p[0] +
                        (s[0] / (s[0] - s[1])) * (segment.p[1] - segment.p[0]);
                }
                else  // numZero = 1
                {
                    // One segment endpoint is on the plane.
                    if (s[0] == C_<T>(0))
                    {
                        output.point[0] = segment.p[0];
                    }
                    else
                    {
                        output.point[0] = segment.p[1];
                    }
                }
            }
            else  // numNegative == 2
            {
                // The segment is outside the halfspace. (numNegative == 2)
                output.intersect = false;
                output.numPoints = 0;
            }

            return output;
        }

    private:
        friend class UnitTestIntrHalfspace3Segment3;
    };
}
