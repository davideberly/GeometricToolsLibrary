// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.17

#pragma once

// The queries consider the box to be a solid and the polygon to be a
// convex solid.

#include <GTL/Mathematics/Intersection/IntersectionQuery.h>
#include <GTL/Mathematics/Primitives/ND/Halfspace.h>
#include <GTL/Mathematics/Primitives/ND/OrientedBox.h>
#include <GTL/Mathematics/Algebra/Vector.h>
#include <cstddef>
#include <limits>
#include <vector>

namespace gtl
{
    template <typename T>
    class FIQuery<T, Halfspace<T, 2>, std::vector<Vector2<T>>>
    {
    public:
        struct Output
        {
            Output()
                :
                intersect(false),
                polygon{}
            {
            }

            bool intersect;

            // If 'intersect' is true, the halfspace and polygon intersect
            // in a convex polygon.
            std::vector<Vector2<T>> polygon;
        };

        Output operator()(Halfspace<T, 2> const& halfspace,
            std::vector<Vector2<T>> const& polygon)
        {
            Output output{};

            // Determine whether the polygon vertices are outside the
            // halfspace, inside the halfspace, or on the halfspace boundary.
            std::size_t const numVertices = polygon.size();
            std::vector<T> distance(numVertices);
            std::size_t positive = 0, negative = 0;
            std::size_t positiveIndex = std::numeric_limits<std::size_t>::max();
            for (std::size_t i = 0; i < numVertices; ++i)
            {
                distance[i] = Dot(halfspace.normal, polygon[i]) - halfspace.constant;
                if (distance[i] > C_<T>(0))
                {
                    ++positive;
                    if (positiveIndex == std::numeric_limits<std::size_t>::max())
                    {
                        positiveIndex = i;
                    }
                }
                else if (distance[i] < C_<T>(0))
                {
                    ++negative;
                }
            }

            if (positive == 0)
            {
                // The polygon is strictly outside the halfspace.
                output.intersect = false;
                return output;
            }

            if (negative == 0)
            {
                // The polygon is contained in the closed halfspace, so it is
                // fully visible and no clipping is necessary.
                output.intersect = true;
                return output;
            }

            // The line transversely intersects the polygon. Clip the polygon.
            Vector2<T> vertex{};
            std::size_t curr{}, prev{};
            T t{};

            if (positiveIndex > 0)
            {
                // Compute the first clip vertex on the line.
                curr = positiveIndex;
                prev = curr - 1;
                t = distance[curr] / (distance[curr] - distance[prev]);
                vertex = polygon[curr] + t * (polygon[prev] - polygon[curr]);
                output.polygon.push_back(vertex);

                // Include the vertices on the positive side of line.
                while (curr < numVertices && distance[curr] > C_<T>(0))
                {
                    output.polygon.push_back(polygon[curr++]);
                }

                // Compute the kast clip vertex on the line.
                if (curr < numVertices)
                {
                    prev = curr - 1;
                }
                else
                {
                    curr = 0;
                    prev = numVertices - 1;
                }
                t = distance[curr] / (distance[curr] - distance[prev]);
                vertex = polygon[curr] + t * (polygon[prev] - polygon[curr]);
                output.polygon.push_back(vertex);
            }
            else  // positiveIndex is 0
            {
                // Include the vertices on the positive side of line.
                curr = 0;
                while (curr < numVertices && distance[curr] > C_<T>(0))
                {
                    output.polygon.push_back(polygon[curr++]);
                }

                // Compute the last clip vertex on the line.
                prev = curr - 1;
                t = distance[curr] / (distance[curr] - distance[prev]);
                vertex = polygon[curr] + t * (polygon[prev] - polygon[curr]);
                output.polygon.push_back(vertex);

                // Skip the vertices on the negative side of the line.
                while (curr < numVertices && distance[curr] <= C_<T>(0))
                {
                    curr++;
                }

                // Compute the first clip vertex on the line.
                if (curr < numVertices)
                {
                    prev = curr - 1;
                    t = distance[curr] / (distance[curr] - distance[prev]);
                    vertex = polygon[curr] + t * (polygon[prev] - polygon[curr]);
                    output.polygon.push_back(vertex);

                    // Keep the vertices on the positive side of the line.
                    while (curr < numVertices && distance[curr] > C_<T>(0))
                    {
                        output.polygon.push_back(polygon[curr++]);
                    }
                }
                else
                {
                    curr = 0;
                    prev = numVertices - 1;
                    t = distance[curr] / (distance[curr] - distance[prev]);
                    vertex = polygon[curr] + t * (polygon[prev] - polygon[curr]);
                    output.polygon.push_back(vertex);
                }
            }

            output.intersect = true;
            return output;
        }

    private:
        friend class UnitTestIntrHalfspace2Polygon2;
    };
}
