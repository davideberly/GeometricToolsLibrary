// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.17

#pragma once

// The queries consider the box to be a solid. The test-intersection query uses
// the method of separating axes.
// https://www.geometrictools.com/Documentation/MethodOfSeparatingAxes.pdf
// The set of potential separating directions includes the 2 edge normals of
// box0 and the 2 edge normals of box1. The integer 'separating' identifies
// the axis that reported separation; there may be more than one but only one
// is reported. The value is 0 when box0.axis[0] separates, 1 when
// box0.axis[1] separates, 2 when box1.axis[0] separates or 3 when
// box1.axis[1] separates.

#include <GTL/Mathematics/Intersection/IntersectionQuery.h>
#include <GTL/Mathematics/Primitives/ND/OrientedBox.h>
#include <GTL/Mathematics/Algebra/Vector.h>
#include <array>
#include <cmath>
#include <cstddef>
#include <limits>
#include <vector>

namespace gtl
{
    template <typename T>
    class TIQuery<T, OrientedBox2<T>, OrientedBox2<T>>
    {
    public:
        struct Output
        {
            Output()
                :
                intersect(false),
                separating(0)
            {
            }

            bool intersect;
            std::size_t separating;
        };

        Output operator()(OrientedBox2<T> const& box0, OrientedBox2<T> const& box1)
        {
            Output output{};

            // Convenience variables.
            Vector2<T> const* A0 = &box0.axis[0];
            Vector2<T> const* A1 = &box1.axis[0];
            Vector2<T> const& E0 = box0.extent;
            Vector2<T> const& E1 = box1.extent;

            // Compute difference of box centers, D = C1-C0.
            Vector2<T> D = box1.center - box0.center;

            std::array<std::array<T, 2>, 2> absA0dA1{};
            T rSum{};

            // Test box0.axis[0].
            absA0dA1[0][0] = std::fabs(Dot(A0[0], A1[0]));
            absA0dA1[0][1] = std::fabs(Dot(A0[0], A1[1]));
            rSum = E0[0] + E1[0] * absA0dA1[0][0] + E1[1] * absA0dA1[0][1];
            if (std::fabs(Dot(A0[0], D)) > rSum)
            {
                output.intersect = false;
                output.separating = 0;
                return output;
            }

            // Test axis box0.axis[1].
            absA0dA1[1][0] = std::fabs(Dot(A0[1], A1[0]));
            absA0dA1[1][1] = std::fabs(Dot(A0[1], A1[1]));
            rSum = E0[1] + E1[0] * absA0dA1[1][0] + E1[1] * absA0dA1[1][1];
            if (std::fabs(Dot(A0[1], D)) > rSum)
            {
                output.intersect = false;
                output.separating = 1;
                return output;
            }

            // Test axis box1.axis[0].
            rSum = E1[0] + E0[0] * absA0dA1[0][0] + E0[1] * absA0dA1[1][0];
            if (std::fabs(Dot(A1[0], D)) > rSum)
            {
                output.intersect = false;
                output.separating = 2;
                return output;
            }

            // Test axis box1.axis[1].
            rSum = E1[1] + E0[0] * absA0dA1[0][1] + E0[1] * absA0dA1[1][1];
            if (std::fabs(Dot(A1[1], D)) > rSum)
            {
                output.intersect = false;
                output.separating = 3;
                return output;
            }

            output.intersect = true;
            return output;
        }

    private:
        friend class UnitTestIntrOrientedBox2OrientedBox2;
    };

    template <typename T>
    class FIQuery<T, OrientedBox2<T>, OrientedBox2<T>>
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

            // If 'intersect' is true, the boxes intersect in a convex
            // 'polygon'.
            std::vector<Vector2<T>> polygon;
        };

        Output operator()(OrientedBox2<T> const& box0, OrientedBox2<T> const& box1)
        {
            Output output{};
            output.intersect = true;

            // Initialize the intersection polygon to box0, listing the
            // vertices in counterclockwise order.
            std::array<Vector2<T>, 4> vertex{};
            box0.GetVertices(vertex);
            output.polygon.push_back(vertex[0]);  // C - e0 * U0 - e1 * U1
            output.polygon.push_back(vertex[1]);  // C + e0 * U0 - e1 * U1
            output.polygon.push_back(vertex[3]);  // C + e0 * U0 + e1 * U1
            output.polygon.push_back(vertex[2]);  // C - e0 * U0 + e1 * U1

            // Clip the polygon using the lines defining edges of box1.  The
            // line normal points inside box1.  The line origin is the first
            // vertex of the edge when traversing box1 counterclockwise.
            box1.GetVertices(vertex);
            std::array<Vector2<T>, 4> normal =
            {
                box1.axis[1], -box1.axis[0], box1.axis[0], -box1.axis[1]
            };

            for (std::size_t i = 0; i < 4; ++i)
            {
                if (Outside(vertex[i], normal[i], output.polygon))
                {
                    // The boxes are separated.
                    output.intersect = false;
                    output.polygon.clear();
                    break;
                }
            }

            return output;
        }

    private:
        // The line normals are inner pointing. The function returns true
        // when the incoming polygon is outside the line, in which case the
        // boxes do not intersect. If the function returns false, the
        // outgoing polygon is the incoming polygon intersected with the
        // closed halfspacedefined by the line.
        bool Outside(Vector2<T> const& origin, Vector2<T> const& normal,
            std::vector<Vector2<T>>& polygon)
        {
            // Determine whether the polygon vertices are outside the polygon,
            // inside the polygon, or on the polygon boundary.
            std::size_t const numVertices = polygon.size();
            std::vector<T> distance(numVertices);
            std::size_t positive = 0, negative = 0;
            std::size_t positiveIndex = std::numeric_limits<std::size_t>::max();
            for (std::size_t i = 0; i < numVertices; ++i)
            {
                distance[i] = Dot(normal, polygon[i] - origin);
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
                // The polygon is strictly outside the line.
                return true;
            }

            if (negative == 0)
            {
                // The polygon is contained in the closed halfspace whose
                // boundary is the line.  It is fully visible and no clipping
                // is necessary.
                return false;
            }

            // The line transversely intersects the polygon. Clip the polygon.
            std::vector<Vector2<T>> clipPolygon{};
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
                clipPolygon.push_back(vertex);

                // Include the vertices on the positive side of line.
                while (curr < numVertices && distance[curr] > C_<T>(0))
                {
                    clipPolygon.push_back(polygon[curr++]);
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
                clipPolygon.push_back(vertex);
            }
            else  // positiveIndex is 0
            {
                // Include the vertices on the positive side of line.
                curr = 0;
                while (curr < numVertices && distance[curr] > C_<T>(0))
                {
                    clipPolygon.push_back(polygon[curr++]);
                }

                // Compute the last clip vertex on the line.
                prev = curr - 1;
                t = distance[curr] / (distance[curr] - distance[prev]);
                vertex = polygon[curr] + t * (polygon[prev] - polygon[curr]);
                clipPolygon.push_back(vertex);

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
                    clipPolygon.push_back(vertex);

                    // Keep the vertices on the positive side of the line.
                    while (curr < numVertices && distance[curr] > C_<T>(0))
                    {
                        clipPolygon.push_back(polygon[curr++]);
                    }
                }
                else
                {
                    curr = 0;
                    prev = numVertices - 1;
                    t = distance[curr] / (distance[curr] - distance[prev]);
                    vertex = polygon[curr] + t * (polygon[prev] - polygon[curr]);
                    clipPolygon.push_back(vertex);
                }
            }

            polygon = clipPolygon;
            return false;
        }

    private:
        friend class UnitTestIntrOrientedBox2OrientedBox2;
    };
}
