// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.19

#pragma once

// The test-intersection query is based on the document
// https://www.geometrictools.com/Documentation/MethodOfSeparatingAxes.pdf
// The find-intersection query clips the triangle against the faces of
// the oriented box.

#include <GTL/Mathematics/Intersection/IntersectionQuery.h>
#include <GTL/Mathematics/Intersection/ND/IntrConvexPolygonHyperplane.h>
#include <GTL/Mathematics/Primitives/ND/CanonicalBox.h>
#include <GTL/Mathematics/Primitives/ND/Triangle.h>
#include <GTL/Mathematics/Algebra/Vector.h>
#include <array>
#include <cmath>
#include <cstddef>
#include <vector>

namespace gtl
{
    template <typename T>
    class TIQuery<T, Triangle3<T>, CanonicalBox3<T>>
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

        Output operator()(Triangle3<T> const& triangle, CanonicalBox3<T> const& box)
        {
            Output output{};
            std::array<Vector3<T>, 3> edge{};

            // Test direction of triangle normal.
            edge[0] = triangle.v[1] - triangle.v[0];
            edge[1] = triangle.v[2] - triangle.v[1];
            if (SeparatedByTriangleNormal(triangle, box, edge))
            {
                output.intersect = false;
                return output;
            }

            // Test directions of box faces.
            if (SeparatedByBoxFaceNormal(triangle, box))
            {
                output.intersect = false;
                return output;
            }

            // Test directions of triangle-box edge cross products.
            edge[2] = triangle.v[0] - triangle.v[2];
            output.intersect = !SeparatedByTriangleEdgeCrossBoxEdge(triangle, box, edge);
            return output;
        }

    private:
        bool SeparatedByTriangleNormal(Triangle3<T> const& triangle, CanonicalBox3<T> const& box,
            std::array<Vector3<T>, 3>& edge)
        {
            Vector3<T> direction = Cross(edge[0], edge[1]);
            T dot = Dot(direction, triangle.v[0]);

            T radius =
                std::fabs(box.extent[0] * direction[0]) +
                std::fabs(box.extent[1] * direction[1]) +
                std::fabs(box.extent[2] * direction[2]);

            bool separated = !(std::fabs(dot) <= radius);
            return separated;
        }

        bool SeparatedByBoxFaceNormal(Triangle3<T> const& triangle, CanonicalBox3<T> const& box)
        {
            for (std::size_t i = 0; i < 3; ++i)
            {
                auto const& dot0 = triangle.v[0][i];
                auto const& dot1 = triangle.v[1][i];
                auto const& dot2 = triangle.v[2][i];
                T minDot = dot0, maxDot = dot0;
                if (dot1 < minDot) { minDot = dot1; }
                else if (dot1 > maxDot) { maxDot = dot1; }
                if (dot2 < minDot) { minDot = dot2; }
                else if (dot2 > maxDot) { maxDot = dot2; }
                bool separated = (+box.extent[i] < minDot || maxDot < -box.extent[i]);
                if (separated)
                {
                    return true;
                }
            }
            return false;
        }

        bool SeparatedByTriangleEdgeCrossBoxEdge(Triangle3<T> const& triangle, CanonicalBox3<T> const& box,
            std::array<Vector3<T>, 3>& edge)
        {
            T const zero = C_<T>(0);
            std::array<std::array<Vector3<T>, 3>, 3> cross =
            {{
                {{
                    { zero, -edge[0][2], +edge[0][1] },
                    { zero, -edge[1][2], +edge[1][1] },
                    { zero, -edge[2][2], +edge[2][1] }
                }},
                {{
                    { +edge[0][2], zero, -edge[0][0] },
                    { +edge[1][2], zero, -edge[1][0] },
                    { +edge[2][2], zero, -edge[2][0] }
                }},
                {{
                    { -edge[0][1], +edge[0][0], zero },
                    { -edge[1][1], +edge[1][0], zero },
                    { -edge[2][1], +edge[2][0], zero },
                }}
            }};

            for (std::size_t i0 = 0; i0 < 3; ++i0)
            {
                for (std::size_t i1 = 0; i1 < 3; ++i1)
                {
                    Vector3<T> const& direction = cross[i0][i1];

                    T min{}, max{};
                    GetTriangleProjection(direction, triangle, min, max);

                    T radius =
                        std::fabs(box.extent[0] * direction[0]) +
                        std::fabs(box.extent[1] * direction[1]) +
                        std::fabs(box.extent[2] * direction[2]);

                    bool separated = (radius < min || max < -radius);
                    if (separated)
                    {
                        return true;
                    }
                }
            }
            return false;
        }

        void GetTriangleProjection(Vector3<T> const& direction, Triangle3<T> const& triangle, T& min, T& max)
        {
            std::array<T, 3> dot
            {
                Dot(direction, triangle.v[0]),
                Dot(direction, triangle.v[1]),
                Dot(direction, triangle.v[2])
            };

            min = dot[0];
            max = min;

            if (dot[1] < min)
            {
                min = dot[1];
            }
            else if (dot[1] > max)
            {
                max = dot[1];
            }

            if (dot[2] < min)
            {
                min = dot[2];
            }
            else if (dot[2] > max)
            {
                max = dot[2];
            }
        }

    private:
        friend class UnitTestTriangle3CanonicalBox3;
    };

    template <typename T>
    class FIQuery<T, Triangle3<T>, CanonicalBox3<T>>
    {
    public:
        using PPQuery = FIQuery<T, std::vector<Vector3<T>>, Plane3<T>>;
        using PPOutput = typename PPQuery::Output;

        struct Output
        {
            Output()
                :
                insidePolygon{},
                outsidePolygons{}
            {
            }

            std::vector<Vector3<T>> insidePolygon;
            std::vector<std::vector<Vector3<T>>> outsidePolygons;
        };

        Output operator()(Triangle3<T> const& triangle, CanonicalBox3<T> const& box)
        {
            Output output{};

            // Start with the triangle and clip it against each face of the
            // box. The largest number of vertices for the polygon of
            // intersection is 7.
            output.insidePolygon.resize(3);
            for (std::size_t i = 0; i < 3; ++i)
            {
                output.insidePolygon[i] = triangle.v[i];
            }

            // Create planes for the box faces that with normals that point
            // inside the box.
            std::array<Plane3<T>, 6> planes{};
            planes[0].normal = -Vector3<T>::Unit(0);
            planes[0].constant = -box.extent[0];
            planes[1].normal = -Vector3<T>::Unit(1);
            planes[1].constant = -box.extent[1];
            planes[2].normal = -Vector3<T>::Unit(2);
            planes[2].constant = -box.extent[2];
            planes[3].normal = +Vector3<T>::Unit(0);
            planes[3].constant = -box.extent[0];
            planes[4].normal = +Vector3<T>::Unit(1);
            planes[4].constant = -box.extent[1];
            planes[5].normal = +Vector3<T>::Unit(2);
            planes[5].constant = -box.extent[2];

            for (auto const& plane : planes)
            {
                PPOutput ppOutput = PPQuery{}(output.insidePolygon, plane);
                switch (ppOutput.configuration)
                {
                case PPQuery::Configuration::SPLIT:
                    output.insidePolygon = ppOutput.positivePolygon;
                    output.outsidePolygons.push_back(ppOutput.negativePolygon);
                    break;
                case PPQuery::Configuration::POSITIVE_SIDE_VERTEX:
                case PPQuery::Configuration::POSITIVE_SIDE_EDGE:
                case PPQuery::Configuration::POSITIVE_SIDE_STRICT:
                    // The output.insidePolygon is already
                    // ppOutput.positivePolygon, but to make it clear,
                    // assign it here.
                    output.insidePolygon = ppOutput.positivePolygon;
                    break;
                case PPQuery::Configuration::NEGATIVE_SIDE_VERTEX:
                case PPQuery::Configuration::NEGATIVE_SIDE_EDGE:
                case PPQuery::Configuration::NEGATIVE_SIDE_STRICT:
                    output.insidePolygon.clear();
                    output.outsidePolygons.push_back(ppOutput.negativePolygon);
                    return output;
                case PPQuery::Configuration::CONTAINED:
                    // A triangle coplanar with a box face will be processed
                    // as if it were inside the box.
                    output.insidePolygon = ppOutput.intersection;
                    break;
                default:
                    output.insidePolygon.clear();
                    output.outsidePolygons.clear();
                    break;
                }
            }

            return output;
        }

    private:
        friend class UnitTestTriangle3CanonicalBox3;
    };
}
