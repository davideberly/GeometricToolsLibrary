// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.12

#pragma once

// Given a polygon as an order list of vertices (x[i],y[i]) for 0 <= i < N
// and a test point (xt,yt), return 'true' if (xt,yt) is in the polygon and
// 'false' if it is not. All queries require that the number of vertices
// satisfies N >= 3.

#include <GTL/Mathematics/Algebra/Vector.h>
#include <cstddef>
#include <vector>

namespace gtl
{
    template <typename T>
    class ContPolygon2
    {
    public:
        // A containment query for a simple polygons. The polygon vertices
        // can be counterclockwise or clockwise ordered.
        static bool InContainer(std::vector<Vector2<T>> const& points, Vector2<T> const& p)
        {
            GTL_ARGUMENT_ASSERT(
                points.size() >= 3,
                "The polygon must have at least 3 vertices.");

            bool inside = false;
            for (std::size_t i = 0, j = points.size() - 1; i < points.size(); j = i++)
            {
                Vector2<T> const& U0 = points[i];
                Vector2<T> const& U1 = points[j];

                if (p[1] < U1[1])  // U1 above ray
                {
                    if (U0[1] <= p[1])  // U0 on or below ray
                    {
                        T lhs = (p[1] - U0[1]) * (U1[0] - U0[0]);
                        T rhs = (p[0] - U0[0]) * (U1[1] - U0[1]);
                        if (lhs > rhs)
                        {
                            inside = !inside;
                        }
                    }
                }
                else if (p[1] < U0[1])  // U1 on or below ray, U0 above ray
                {
                    T lhs = (p[1] - U0[1]) * (U1[0] - U0[0]);
                    T rhs = (p[0] - U0[0]) * (U1[1] - U0[1]);
                    if (lhs < rhs)
                    {
                        inside = !inside;
                    }
                }
            }
            return inside;
        }

        // Algorithms for convex polygons. The input polygons must have
        // vertices in counterclockwise order.

        // O(N) algorithm (which-side-of-edge tests)
        static bool InContainerConvexOrderN(std::vector<Vector2<T>> const& points,
            Vector2<T> const& p)
        {
            GTL_ARGUMENT_ASSERT(
                points.size() >= 3,
                "The polygon must have at least 3 vertices.");

            for (std::size_t i1 = 0, i0 = points.size() - 1; i1 < points.size(); i0 = i1++)
            {
                T nx = points[i1][1] - points[i0][1];
                T ny = points[i0][0] - points[i1][0];
                T dx = p[0] - points[i0][0];
                T dy = p[1] - points[i0][1];
                if (nx * dx + ny * dy > C_<T>(0))
                {
                    return false;
                }
            }
            return true;
        }

        // O(log N) algorithm (bisection and recursion, similar to a BSP
        // tree).
        static bool InContainerConvexOrderLogN(std::vector<Vector2<T>> const& points,
            Vector2<T> const& p)
        {
            GTL_ARGUMENT_ASSERT(
                points.size() >= 3,
                "The polygon must have at least 3 vertices.");

            return SubContainsPoint(points, p, 0, 0);
        }

        // The polygon must have exactly four vertices. This method is like
        // the O(log N) and uses three which-side-of-segment test instead of
        // four which-side-of-edge tests. If the polygon does not have four
        // vertices, the function returns false.
        static bool InContainerQuadrilateral(std::vector<Vector2<T>> const& points,
            Vector2<T> const& p)
        {
            GTL_ARGUMENT_ASSERT(
                points.size() != 4,
                "The polygon must have exactly 4 vertices.");

            T nx = points[2][1] - points[0][1];
            T ny = points[0][0] - points[2][0];
            T dx = p[0] - points[0][0];
            T dy = p[1] - points[0][1];

            if (nx * dx + ny * dy > C_<T>(0))
            {
                // P potentially in <V0,V1,V2>
                nx = points[1][1] - points[0][1];
                ny = points[0][0] - points[1][0];
                if (nx * dx + ny * dy > C_<T>(0))
                {
                    return false;
                }

                nx = points[2][1] - points[1][1];
                ny = points[1][0] - points[2][0];
                dx = p[0] - points[1][0];
                dy = p[1] - points[1][1];
                if (nx * dx + ny * dy > C_<T>(0))
                {
                    return false;
                }
            }
            else
            {
                // P potentially in <V0,V2,V3>
                nx = points[0][1] - points[3][1];
                ny = points[3][0] - points[0][0];
                if (nx * dx + ny * dy > C_<T>(0))
                {
                    return false;
                }

                nx = points[3][1] - points[2][1];
                ny = points[2][0] - points[3][0];
                dx = p[0] - points[3][0];
                dy = p[1] - points[3][1];
                if (nx * dx + ny * dy > C_<T>(0))
                {
                    return false;
                }
            }
            return true;
        }

    private:
        // For recursion in ContainsConvexOrderLogN.
        static bool SubContainsPoint(std::vector<Vector2<T>> const& points,
            Vector2<T> const& p, std::size_t i0, std::size_t i1)
        {
            T nx = C_<T>(0), ny = C_<T>(0), dx = C_<T>(0), dy = C_<T>(0);

            std::size_t diff = i1 - i0;
            if (diff == 1 || (diff < 0 && diff + points.size() == 1))
            {
                nx = points[i1][1] - points[i0][1];
                ny = points[i0][0] - points[i1][0];
                dx = p[0] - points[i0][0];
                dy = p[1] - points[i0][1];
                return nx * dx + ny * dy <= C_<T>(0);
            }

            // Bisect the index range.
            std::size_t mid = 0;
            if (i0 < i1)
            {
                mid = (i0 + i1) >> 1;
            }
            else
            {
                mid = (i0 + i1 + points.size()) >> 1;
                if (mid >= points.size())
                {
                    mid -= points.size();
                }
            }

            // Determine which side of the splitting line contains the point.
            nx = points[mid][1] - points[i0][1];
            ny = points[i0][0] - points[mid][0];
            dx = p[0] - points[i0][0];
            dy = p[1] - points[i0][1];
            if (nx * dx + ny * dy > C_<T>(0))
            {
                // P potentially in <V(i0),V(i0+1),...,V(mid-1),V(mid)>
                return SubContainsPoint(points, p, i0, mid);
            }
            else
            {
                // P potentially in <V(mid),V(mid+1),...,V(i1-1),V(i1)>
                return SubContainsPoint(points, p, mid, i1);
            }
        }

    private:
        friend class UnitTestContPolygon2;
    };
}
