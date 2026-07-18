// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.18

#pragma once

// Queries for intersection of objects with halfspaces.  These are useful for
// containment testing, object culling, and clipping.

#include <GTL/Mathematics/Intersection/IntersectionQuery.h>
#include <GTL/Mathematics/Primitives/ND/Halfspace.h>
#include <GTL/Mathematics/Primitives/ND/Triangle.h>
#include <GTL/Mathematics/Algebra/Vector.h>
#include <algorithm>
#include <array>
#include <cstddef>

namespace gtl
{
    template <typename T>
    class TIQuery<T, Halfspace3<T>, Triangle3<T>>
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

        Output operator()(Halfspace3<T> const& halfspace, Triangle3<T> const& triangle)
        {
            Output output{};

            // Project the triangle vertices onto the normal line.  The plane
            // of the halfspace occurs at the origin (zero) of the normal
            // line.
            std::array<T, 3> s{};
            for (std::size_t i = 0; i < 3; ++i)
            {
                s[i] = Dot(halfspace.normal, triangle.v[i]) - halfspace.constant;
            }

            // The triangle and halfspace intersect when the projection
            // interval maximum is nonnegative.
            output.intersect = (std::max(std::max(s[0], s[1]), s[2]) >= C_<T>(0));
            return output;
        }

    private:
        friend class UnitTestIntrHalfspace3Triangle3;
    };

    template <typename T>
    class FIQuery<T, Halfspace3<T>, Triangle3<T>>
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

            // The triangle is clipped against the plane defining the
            // halfspace. The 'numPoints' is either 0 (no intersection),
            // 1 (point), 2 (segment), 3 (triangle) or 4 (quadrilateral).
            std::size_t numPoints;
            std::array<Vector3<T>, 4> point;
        };

        Output operator()(Halfspace3<T> const& halfspace, Triangle3<T> const& triangle)
        {
            Output output{};

            // Determine on which side of the plane the vertices lie.  The
            // table of possibilities is listed next with n = numNegative,
            // p = numPositive, and z = numZero.
            //
            //   n p z  intersection
            //   ---------------------------------
            //   0 3 0  triangle (original)
            //   0 2 1  triangle (original)
            //   0 1 2  triangle (original)
            //   0 0 3  triangle (original)
            //   1 2 0  quad (2 edges clipped)
            //   1 1 1  triangle (1 edge clipped)
            //   1 0 2  edge
            //   2 1 0  triangle (2 edges clipped)
            //   2 0 1  vertex
            //   3 0 0  none

            std::array<T, 3> s{};
            std::size_t numPositive = 0, numNegative = 0;
            for (std::size_t i = 0; i < 3; ++i)
            {
                s[i] = Dot(halfspace.normal, triangle.v[i]) - halfspace.constant;
                if (s[i] > C_<T>(0))
                {
                    ++numPositive;
                }
                else if (s[i] < C_<T>(0))
                {
                    ++numNegative;
                }
            }

            if (numNegative == 0)
            {
                // The triangle is in the halfspace.
                output.intersect = true;
                output.numPoints = 3;
                output.point[0] = triangle.v[0];
                output.point[1] = triangle.v[1];
                output.point[2] = triangle.v[2];
            }
            else if (numNegative == 1)
            {
                output.intersect = true;
                if (numPositive == 2)
                {
                    // The portion of the triangle in the halfspace is a
                    // quadrilateral.
                    output.numPoints = 4;
                    for (std::size_t i0 = 0; i0 < 3; ++i0)
                    {
                        if (s[i0] < C_<T>(0))
                        {
                            std::size_t i1 = (i0 + 1) % 3, i2 = (i0 + 2) % 3;
                            output.point[0] = triangle.v[i1];
                            output.point[1] = triangle.v[i2];
                            T t2 = s[i2] / (s[i2] - s[i0]);
                            T t0 = s[i0] / (s[i0] - s[i1]);
                            output.point[2] = triangle.v[i2] + t2 *
                                (triangle.v[i0] - triangle.v[i2]);
                            output.point[3] = triangle.v[i0] + t0 *
                                (triangle.v[i1] - triangle.v[i0]);
                            break;
                        }
                    }
                }
                else if (numPositive == 1)
                {
                    // The portion of the triangle in the halfspace is a
                    // triangle with one vertex on the plane.
                    output.numPoints = 3;
                    for (std::size_t i0 = 0; i0 < 3; ++i0)
                    {
                        if (s[i0] == C_<T>(0))
                        {
                            std::size_t i1 = (i0 + 1) % 3, i2 = (i0 + 2) % 3;
                            output.point[0] = triangle.v[i0];
                            T t1 = s[i1] / (s[i1] - s[i2]);
                            Vector3<T> p = triangle.v[i1] + t1 *
                                (triangle.v[i2] - triangle.v[i1]);
                            if (s[i1] > C_<T>(0))
                            {
                                output.point[1] = triangle.v[i1];
                                output.point[2] = p;
                            }
                            else
                            {
                                output.point[1] = p;
                                output.point[2] = triangle.v[i2];
                            }
                            break;
                        }
                    }
                }
                else
                {
                    // Only an edge of the triangle is in the halfspace.
                    output.numPoints = 0;
                    for (std::size_t i = 0; i < 3; ++i)
                    {
                        if (s[i] == C_<T>(0))
                        {
                            output.point[output.numPoints++] = triangle.v[i];
                        }
                    }
                }
            }
            else if (numNegative == 2)
            {
                output.intersect = true;
                if (numPositive == 1)
                {
                    // The portion of the triangle in the halfspace is a
                    // triangle.
                    output.numPoints = 3;
                    for (std::size_t i0 = 0; i0 < 3; ++i0)
                    {
                        if (s[i0] > C_<T>(0))
                        {
                            std::size_t i1 = (i0 + 1) % 3, i2 = (i0 + 2) % 3;
                            output.point[0] = triangle.v[i0];
                            T t0 = s[i0] / (s[i0] - s[i1]);
                            T t2 = s[i2] / (s[i2] - s[i0]);
                            output.point[1] = triangle.v[i0] + t0 *
                                (triangle.v[i1] - triangle.v[i0]);
                            output.point[2] = triangle.v[i2] + t2 *
                                (triangle.v[i0] - triangle.v[i2]);
                            break;
                        }
                    }
                }
                else
                {
                    // Only a vertex of the triangle is in the halfspace.
                    output.numPoints = 1;
                    for (std::size_t i = 0; i < 3; ++i)
                    {
                        if (s[i] == C_<T>(0))
                        {
                            output.point[0] = triangle.v[i];
                            break;
                        }
                    }
                }
            }
            else  // numNegative == 3
            {
                // The triangle is outside the halfspace. (numNegative == 3)
                output.intersect = false;
                output.numPoints = 0;
            }

            return output;
        }

    private:
        friend class UnitTestIntrHalfspace3Triangle3;
    };
}
