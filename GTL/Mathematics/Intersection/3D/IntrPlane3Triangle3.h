// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.18

#pragma once

#include <GTL/Mathematics/Intersection/IntersectionQuery.h>
#include <GTL/Mathematics/Primitives/3D/Plane3.h>
#include <GTL/Mathematics/Primitives/ND/Triangle.h>
#include <GTL/Mathematics/Algebra/Vector.h>
#include <array>
#include <cstddef>

namespace gtl
{
    template <typename T>
    class TIQuery<T, Plane3<T>, Triangle3<T>>
    {
    public:
        struct Output
        {
            Output()
                :
                intersect(false),
                numIntersections(0),
                isInterior(false)
            {
            }

            bool intersect;

            // The number is 0 (no intersection), 1 (plane and triangle
            // intersect at a single point [vertex]), 2 (plane and triangle
            // intersect in a segment) or 3 (triangle is in the plane).
            // When the number is 2, the segment is either interior to the
            // triangle or is an edge of the triangle, the distinction stored
            // in 'isInterior'.
            std::size_t numIntersections;
            bool isInterior;
        };

        Output operator()(Plane3<T> const& plane, Triangle3<T> const& triangle)
        {
            Output output{};

            // Determine on which side of the plane the vertices lie.  The
            // table of possibilities is listed next with n = numNegative,
            // p = numPositive and z = numZero.
            //
            //   n p z  intersection
            //   ------------------------------------
            //   0 3 0  none
            //   0 2 1  vertex
            //   0 1 2  edge
            //   0 0 3  triangle in the plane
            //   1 2 0  segment (2 edges clipped)
            //   1 1 1  segment (1 edge clipped)
            //   1 0 2  edge
            //   2 1 0  segment (2 edges clipped)
            //   2 0 1  vertex
            //   3 0 0  none

            std::array<T, 3> s{};
            std::size_t numPositive = 0, numNegative = 0, numZero = 0;
            for (std::size_t i = 0; i < 3; ++i)
            {
                s[i] = Dot(plane.normal, triangle.v[i]) - plane.constant;
                if (s[i] > C_<T>(0))
                {
                    ++numPositive;
                }
                else if (s[i] < C_<T>(0))
                {
                    ++numNegative;
                }
                else
                {
                    ++numZero;
                }
            }

            if (numZero == 0 && numPositive > 0 && numNegative > 0)
            {
                output.intersect = true;
                output.numIntersections = 2;
                output.isInterior = true;
                return output;
            }

            if (numZero == 1)
            {
                output.intersect = true;
                for (std::size_t i = 0; i < 3; ++i)
                {
                    if (s[i] == C_<T>(0))
                    {
                        if (numPositive == 2 || numNegative == 2)
                        {
                            output.numIntersections = 1;
                        }
                        else
                        {
                            output.numIntersections = 2;
                            output.isInterior = true;
                        }
                        break;
                    }
                }
                return output;
            }

            if (numZero == 2)
            {
                output.intersect = true;
                output.numIntersections = 2;
                output.isInterior = false;
                return output;
            }

            if (numZero == 3)
            {
                output.intersect = true;
                output.numIntersections = 3;
            }
            else
            {
                output.intersect = false;
                output.numIntersections = 0;

            }
            return output;
        }

    private:
        friend class UnitTestIntrPlane3Triangle3;
    };

    template <typename T>
    class FIQuery<T, Plane3<T>, Triangle3<T>>
    {
    public:
        struct Output
        {
            Output()
                :
                intersect(false),
                numIntersections(0),
                isInterior(false),
                point{}
            {
            }

            bool intersect;

            // The number is 0 (no intersection), 1 (plane and triangle
            // intersect at a single point [vertex]), 2 (plane and triangle
            // intersect in a segment) or 3 (triangle is in the plane).
            // When the number is 2, the segment is either interior to the
            // triangle or is an edge of the triangle, the distinction stored
            // in 'isInterior'.
            std::size_t numIntersections;
            bool isInterior;
            std::array<Vector3<T>, 3> point;
        };

        Output operator()(Plane3<T> const& plane, Triangle3<T> const& triangle)
        {
            Output output{};

            // Determine on which side of the plane the vertices lie.  The
            // table of possibilities is listed next with n = numNegative,
            // p = numPositive and z = numZero.
            //
            //   n p z  intersection
            //   ------------------------------------
            //   0 3 0  none
            //   0 2 1  vertex
            //   0 1 2  edge
            //   0 0 3  triangle in the plane
            //   1 2 0  segment (2 edges clipped)
            //   1 1 1  segment (1 edge clipped)
            //   1 0 2  edge
            //   2 1 0  segment (2 edges clipped)
            //   2 0 1  vertex
            //   3 0 0  none

            std::array<T, 3> s{};
            std::size_t numPositive = 0, numNegative = 0, numZero = 0;
            for (std::size_t i = 0; i < 3; ++i)
            {
                s[i] = Dot(plane.normal, triangle.v[i]) - plane.constant;
                if (s[i] > C_<T>(0))
                {
                    ++numPositive;
                }
                else if (s[i] < C_<T>(0))
                {
                    ++numNegative;
                }
                else
                {
                    ++numZero;
                }
            }

            if (numZero == 0 && numPositive > 0 && numNegative > 0)
            {
                output.intersect = true;
                output.numIntersections = 2;
                output.isInterior = true;
                T sign = C_<T>(3) - C_<T>(2) * static_cast<T>(numPositive);
                for (std::size_t i0 = 0; i0 < 3; ++i0)
                {
                    if (sign * s[i0] > C_<T>(0))
                    {
                        std::size_t i1 = (i0 + 1) % 3, i2 = (i0 + 2) % 3;
                        T t1 = s[i1] / (s[i1] - s[i0]);
                        T t2 = s[i2] / (s[i2] - s[i0]);
                        output.point[0] = triangle.v[i1] + t1 *
                            (triangle.v[i0] - triangle.v[i1]);
                        output.point[1] = triangle.v[i2] + t2 *
                            (triangle.v[i0] - triangle.v[i2]);
                        break;
                    }
                }
                return output;
            }

            if (numZero == 1)
            {
                output.intersect = true;
                for (std::size_t i0 = 0; i0 < 3; ++i0)
                {
                    if (s[i0] == C_<T>(0))
                    {
                        std::size_t i1 = (i0 + 1) % 3, i2 = (i0 + 2) % 3;
                        output.point[0] = triangle.v[i0];
                        if (numPositive == 2 || numNegative == 2)
                        {
                            output.numIntersections = 1;
                        }
                        else
                        {
                            output.numIntersections = 2;
                            output.isInterior = true;
                            T t = s[i1] / (s[i1] - s[i2]);
                            output.point[1] = triangle.v[i1] + t *
                                (triangle.v[i2] - triangle.v[i1]);
                        }
                        break;
                    }
                }
                return output;
            }

            if (numZero == 2)
            {
                output.intersect = true;
                output.numIntersections = 2;
                output.isInterior = false;
                for (std::size_t i0 = 0; i0 < 3; ++i0)
                {
                    if (s[i0] != C_<T>(0))
                    {
                        std::size_t i1 = (i0 + 1) % 3, i2 = (i0 + 2) % 3;
                        output.point[0] = triangle.v[i1];
                        output.point[1] = triangle.v[i2];
                        break;
                    }
                }
                return output;
            }

            if (numZero == 3)
            {
                output.intersect = true;
                output.numIntersections = 3;
                output.point[0] = triangle.v[0];
                output.point[1] = triangle.v[1];
                output.point[2] = triangle.v[2];
            }
            else
            {
                output.intersect = false;
                output.numIntersections = 0;

            }
            return output;
        }

    private:
        friend class UnitTestIntrPlane3Triangle3;
    };
}
