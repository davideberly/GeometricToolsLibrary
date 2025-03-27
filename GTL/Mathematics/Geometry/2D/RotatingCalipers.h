// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

// The rotating calipers algorithm finds all antipodal vertex-edge pairs for a
// convex polygon. The algorithm is O(n) in time for n polygon edges. The
// brute-force method that finds extreme points for a perpendicular direction
// for each edge and searching all polygon vertices is O(n^2). The search for
// extreme points can use a form of bisection, which reduces the algorithm to
// O(n log n). A description can be found at
// http://www-cgrl.cs.mcgill.ca/~godfried/research/calipers.html
// https://web.archive.org/web/20150330010154/http://cgm.cs.mcgill.ca/~orm/rotcal.html

#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#include <GTL/Mathematics/Algebra/Vector.h>
#include <array>
#include <cstddef>
#include <type_traits>
#include <utility>
#include <vector>

namespace gtl
{
    template <typename T>
    class RotatingCalipers
    {
    public:
        // The Antipode members are lookups into the input vertices[] to
        // ComputeAntipodes(...).
        struct Antipode
        {
            Antipode()
                :
                vertex(0),
                edge{ 0, 0 }
            {
            }

            std::size_t vertex;
            std::array<std::size_t, 2> edge;
        };

        static void ComputeAntipodes(std::vector<Vector2<T>> const& vertices,
            std::vector<Antipode>& antipodes)
        {
            static_assert(std::is_floating_point<T>::value,
                "The input type must be 'float' or 'double'.");

            // Internally, the Antipode members are lookups into indices[].
            // The members are re-mapped to lookups into vertices[] after
            // all antipodes are created.
            std::vector<Vector2<Rational>> rVertices{};
            std::vector<std::size_t> indices{};
            CreatePolygon(vertices, rVertices, indices);
            GTL_RUNTIME_ASSERT(
                indices.size() >= 3,
                "The convex polygon must have at least 3 noncollinear vertices.");

            Antipode antipode{};
            ComputeInitialAntipode(rVertices, indices, antipode);
            antipodes.clear();
            antipodes.push_back(antipode);

            for (std::size_t i = 1; i < indices.size(); ++i)
            {
                ComputeNextAntipode(rVertices, indices, antipode);
                antipodes.push_back(antipode);
            }

            // Re-map the antipode members to be lookups into vertices[].
            for (auto& element : antipodes)
            {
                element.vertex = indices[element.vertex];
                element.edge[0] = indices[element.edge[0]];
                element.edge[1] = indices[element.edge[1]];
            }
        }

    private:
        // Parameters for the largest rational number possible in the rational
        // arithmetic for angle comparison.
        static std::size_t constexpr NumWords = std::is_same<T, float>::value ? 54 : 394;
        using Rational = BSNumber<UIntegerFP32<NumWords>>;

        // The rotating calipers algorithm requires the convex polygon to have
        // no duplicate points and no collinear points. Such points must be
        // removed first. To ensure correctness, rational arithmetic is used.
        // This requires converting the floating-point vertices to rational
        // vertices. To minimize rational operations for the conversion, only
        // the final convex polygon vertices are converted.
        static void CreatePolygon(
            std::vector<Vector2<T>> const& vertices,
            std::vector<Vector2<Rational>>& rVertices,
            std::vector<std::size_t>& indices)
        {
            static_assert(std::is_floating_point<T>::value,
                "The input type must be 'float' or 'double'.");

            std::size_t const numVertices = vertices.size();
            rVertices.resize(numVertices);
            indices.reserve(numVertices);

            auto const& vback = vertices.back();
            auto const& vfront = vertices.front();
            Vector2<Rational> rV0 = { vback[0], vback[1] };
            Vector2<Rational> rV1 = { vfront[0], vfront[1] };
            Vector2<Rational> rEPrev = rV1 - rV0;
            for (std::size_t i0 = 0, i1 = 1; i0 < numVertices; ++i0)
            {
                auto const& V0 = vertices[i0];
                auto const& V1 = vertices[i1];
                rV0 = { V0[0], V0[1] };
                rV1 = { V1[0], V1[1] };
                Vector2<Rational> rENext = rV1 - rV0;

                Rational rDP = DotPerp(rEPrev, rENext);
                if (rDP != C_<Rational>(0))
                {
                    indices.push_back(i0);
                    rVertices[i0] = rV0;
                }

                rEPrev = rENext;
                if (++i1 == numVertices)
                {
                    i1 = 0;
                }
            }
        }

        static void ComputeInitialAntipode(
            std::vector<Vector2<Rational>> const& vertices,
            std::vector<std::size_t>& indices,
            Antipode& antipode)
        {
            static_assert(std::is_floating_point<T>::value,
                "The input type must be 'float' or 'double'.");

            std::size_t const numIndices = indices.size();
            antipode.edge = { numIndices - 1, 0 };

            Vector2<Rational> const& origin = vertices[indices[antipode.edge[0]]];
            Vector2<Rational> U = vertices[indices[antipode.edge[1]]] - origin;

            Vector2<Rational> extreme{};  // zero vector
            antipode.vertex = 0;
            for (std::size_t i = 0; i < numIndices; ++i)
            {
                Vector2<Rational> diff = vertices[indices[i]] - origin;
                Vector2<Rational> c =
                {
                    U[0] * diff[0] + U[1] * diff[1],
                    U[0] * diff[1] - U[1] * diff[0]
                };

                if (c[1] > extreme[1] || (c[1] == extreme[1] && c[0] < extreme[0]))
                {
                    antipode.vertex = i;
                    extreme = c;
                }
            }
        }

        static void ComputeNextAntipode(
            std::vector<Vector2<Rational>> const& vertices,
            std::vector<std::size_t>& indices,
            Antipode& antipode)
        {
            static_assert(std::is_floating_point<T>::value,
                "The input type must be 'float' or 'double'.");

            // Given edges E0 and E1 we know that the angle between them is
            // determined by Dot(E0,E1)/(|E0|*|E1|) = cos(angle). The angle
            // is in (0,pi/2] when Dot(E0,E1) >= 0 or in (pi/2,pi) when
            // Dot(E0,E1) < 0. To allow for exact arithmetic, observe that
            //   sin^2(angle) = 1 - cos^2(angle)
            //                = 1 - Dot(E0,E1)^2/(|E0|^2*|E|^2)
            // The comparator function for angles in (0,pi) compares the
            // squared sine values and the signs of the dot product of edges.

            // Compute the edges associated with the current antipodal edge.
            std::size_t const numIndices = indices.size();
            std::size_t i0 = indices[antipode.edge[0]];
            std::size_t i1 = indices[antipode.edge[1]];
            std::size_t enext = antipode.edge[1] + 1;
            if (enext == numIndices)
            {
                enext = 0;
            }
            std::size_t i2 = indices[enext];

            // Compute the edges associated with the current antipodal vertex.
            std::size_t j0 = indices[antipode.vertex];
            std::size_t vnext = antipode.vertex + 1;
            if (vnext == numIndices)
            {
                vnext = 0;
            }
            std::size_t j1 = indices[vnext];

            std::array<Vector2<Rational>, 2> D0 =
            {
                vertices[j1] - vertices[j0],
                vertices[i0] - vertices[i1]
            };

            std::array<Vector2<Rational>, 2> D1 =
            {
                -D0[1],
                vertices[i2] - vertices[i1]
            };

            if (AngleLessThan(D0, D1))
            {
                // The angle at the antipodal vertex is minimum.
                std::swap(antipode.vertex, antipode.edge[1]);
                antipode.edge[0] = antipode.edge[1];
                antipode.edge[1] = vnext;
            }
            else
            {
                // The angle at the antipodal edge is minimum. The
                // antipodal vertex does not change.
                antipode.edge[0] = antipode.edge[1];
                antipode.edge[1] = enext;
            }
        }

        // Test Angle(D0[0],D0[1]) < Angle(D1[0],D1[1]). It is known that
        // D1[0] = -D0[1].
        static bool AngleLessThan(
            std::array<Vector2<Rational>, 2> const& D0,
            std::array<Vector2<Rational>, 2> const& D1)
        {
            static_assert(std::is_floating_point<T>::value,
                "The input type must be 'float' or 'double'.");

            Rational dot0 = Dot(D0[0], D0[1]);
            Rational dot1 = Dot(D1[0], D1[1]);

            if (dot0 >= C_<Rational>(0))
            {
                // angle0 in (0,pi/2]
                if (dot1 < C_<Rational>(0))
                {
                    // angle1 in (pi/2,pi), so angle0 < angle1
                    return true;
                }

                // angle1 in (0,pi/2], sin^2(angle) is increasing function
                Rational sqrLen00 = Dot(D0[0], D0[0]);
                Rational sqrLen11 = Dot(D1[1], D1[1]);
                return dot0 * dot0 * sqrLen11 > dot1 * dot1 * sqrLen00;
            }
            else
            {
                // angle0 in (pi/2,pi)
                if (dot1 >= C_<Rational>(0))
                {
                    // angle1 in (0,pi/2], so angle1 < angle0
                    return false;
                }

                // angle1 in (pi/2,pi), sin^2(angle) is decreasing function
                Rational sqrLen00 = Dot(D0[0], D0[0]);
                Rational sqrLen11 = Dot(D1[1], D1[1]);
                return dot0 * dot0 * sqrLen11 < dot1 * dot1 * sqrLen00;
            }
        }

    private:
        friend class UnitTestRotatingCalipers;
    };
}
