// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.28

#pragma once

// The queries consider the triangle to be a solid. The algorithms are based
// on determining on which side of the line the vertices lie. The test uses
// the sign of the projections of the vertices onto a normal line that is
// perpendicular to the specified line. The table of possibilities is listed
// next with n = numNegative, p = numPositive and z = numZero.
//
//   n p z  intersection
//   ------------------------------------
//   0 3 0  none
//   0 2 1  vertex
//   0 1 2  edge
//   0 0 3  none (degenerate triangle)
//   1 2 0  segment (2 edges clipped)
//   1 1 1  segment (1 edge clipped)
//   1 0 2  edge
//   2 1 0  segment (2 edges clipped)
//   2 0 1  vertex
//   3 0 0  none
//
// The case (n,p,z) = (0,0,3) is treated as a no-intersection because the
// triangle is degenerate.

#include <GTL/Mathematics/Intersection/IntersectionQuery.h>
#include <GTL/Mathematics/Primitives/ND/Line.h>
#include <GTL/Mathematics/Primitives/ND/Triangle.h>
#include <algorithm>
#include <array>
#include <cstddef>

namespace gtl
{
    template <typename T>
    class TIQuery<T, Line2<T>, Triangle2<T>>
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

        Output operator()(Line2<T> const& line, Triangle2<T> const& triangle)
        {
            Output output{};
            std::size_t numPositive = 0, numNegative = 0, numZero = 0;
            for (std::size_t i = 0; i < 3; ++i)
            {
                Vector2<T> diff = triangle.v[i] - line.origin;
                T s = DotPerp(line.direction, diff);
                if (s > C_<T>(0))
                {
                    ++numPositive;
                }
                else if (s < C_<T>(0))
                {
                    ++numNegative;
                }
                else
                {
                    ++numZero;
                }
            }

            output.intersect =
                (numZero == 0 && numPositive > 0 && numNegative > 0) ||
                (numZero == 1) ||
                (numZero == 2);

            return output;
        }

    private:
        friend class UnitTestIntrLine2Triangle2;
    };

    template <typename T>
    class FIQuery<T, Line2<T>, Triangle2<T>>
    {
    public:
        struct Output
        {
            Output()
                :
                intersect(false),
                numIntersections(0),
                parameter{ C_<T>(0), C_<T>(0) },
                point{
                    Vector2<T>{ C_<T>(0), C_<T>(0) },
                    Vector2<T>{ C_<T>(0), C_<T>(0) }}
            {
            }

            bool intersect;
            std::size_t numIntersections;
            std::array<T, 2> parameter;
            std::array<Vector2<T>, 2> point;
        };

        Output operator()(Line2<T> const& line, Triangle2<T> const& triangle)
        {
            Output output{};
            DoQuery(line.origin, line.direction, triangle, output);
            if (output.intersect)
            {
                for (std::size_t i = 0; i < 2; ++i)
                {
                    output.point[i] = line.origin + output.parameter[i] * line.direction;
                }
            }
            return output;
        }

    protected:
        // The caller must ensure that on entry, 'output' is default
        // constructed as if there is no intersection. If an intersection is
        // found, the 'output' values will be modified accordingly.
        void DoQuery(Vector2<T> const& origin, Vector2<T> const& direction,
            Triangle2<T> const& triangle, Output& output)
        {
            std::array<T, 3> s{ C_<T>(0), C_<T>(0), C_<T>(0) };
            std::size_t numPositive = 0, numNegative = 0, numZero = 0;
            for (std::size_t i = 0; i < 3; ++i)
            {
                Vector2<T> diff = triangle.v[i] - origin;
                s[i] = DotPerp(direction, diff);
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
                // (n,p,z) is (1,2,0) or (2,1,0).
                output.intersect = true;
                output.numIntersections = 2;

                // sign is +1 when (n,p) is (2,1) or -1 when (n,p) is (1,2).
                T sign = (3 > numPositive * 2 ? C_<T>(1) : -C_<T>(1));
                for (std::size_t i0 = 1, i1 = 2, i2 = 0; i2 < 3; i0 = i1, i1 = i2++)
                {
                    if (sign * s[i2] > C_<T>(0))
                    {
                        Vector2<T> diffVi0P0 = triangle.v[i0] - origin;
                        Vector2<T> diffVi2Vi0 = triangle.v[i2] - triangle.v[i0];
                        T lambda0 = s[i0] / (s[i0] - s[i2]);
                        Vector2<T> q0 = diffVi0P0 + lambda0 * diffVi2Vi0;
                        output.parameter[0] = Dot(direction, q0);
                        Vector2<T> diffVi1P0 = triangle.v[i1] - origin;
                        Vector2<T> diffVi2Vi1 = triangle.v[i2] - triangle.v[i1];
                        T lambda1 = s[i1] / (s[i1] - s[i2]);
                        Vector2<T> q1 = diffVi1P0 + lambda1 * diffVi2Vi1;
                        output.parameter[1] = Dot(direction, q1);
                        break;
                    }
                }
            }
            else if (numZero == 1)
            {
                // (n,p,z) is (1,1,1), (2,0,1) or (0,2,1).
                output.intersect = true;
                for (std::size_t i0 = 1, i1 = 2, i2 = 0; i2 < 3; i0 = i1, i1 = i2++)
                {
                    if (s[i2] == C_<T>(0))
                    {
                        Vector2<T> diffVi2P0 = triangle.v[i2] - origin;
                        output.parameter[0] = Dot(direction, diffVi2P0);
                        if (numPositive == 2 || numNegative == 2)
                        {
                            // (n,p,z) is (2,0,1) or (0,2,1).
                            output.numIntersections = 1;
                            output.parameter[1] = output.parameter[0];
                        }
                        else
                        {
                            // (n,p,z) is (1,1,1).
                            output.numIntersections = 2;
                            Vector2<T> diffVi0P0 = triangle.v[i0] - origin;
                            Vector2<T> diffVi1Vi0 = triangle.v[i1] - triangle.v[i0];
                            T lambda0 = s[i0] / (s[i0] - s[i1]);
                            Vector2<T> q = diffVi0P0 + lambda0 * diffVi1Vi0;
                            output.parameter[1] = Dot(direction, q);
                        }
                        break;
                    }
                }
            }
            else if (numZero == 2)
            {
                // (n,p,z) is (1,0,2) or (0,1,2).
                output.intersect = true;
                output.numIntersections = 2;
                for (std::size_t i0 = 1, i1 = 2, i2 = 0; i2 < 3; i0 = i1, i1 = i2++)
                {
                    if (s[i2] != C_<T>(0))
                    {
                        Vector2<T> diffVi0P0 = triangle.v[i0] - origin;
                        output.parameter[0] = Dot(direction, diffVi0P0);
                        Vector2<T> diffVi1P0 = triangle.v[i1] - origin;
                        output.parameter[1] = Dot(direction, diffVi1P0);
                        break;
                    }
                }
            }
            // else: (n,p,z) is (3,0,0), (0,3,0) or (0,0,3). The constructor
            // for Output initializes all members to zero, so no additional
            // assignments are needed for 'output'.

            if (output.intersect)
            {
                T directionSqrLength = Dot(direction, direction);
                output.parameter[0] /= directionSqrLength;
                output.parameter[1] /= directionSqrLength;
                if (output.parameter[0] > output.parameter[1])
                {
                    std::swap(output.parameter[0], output.parameter[1]);
                }
            }
        }

    private:
        friend class UnitTestIntrLine2Triangle2;
    };
}
