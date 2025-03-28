// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

// The Polygon2 object represents a simple polygon. The vertices must all be
// unique and the indices represent edges, each edge a pair of consecutive
// vertices. Let n be the number of indices. The edges are edge[i0] =
// (indices[i0], indices[i1]) for 0 <= i0 <= n-1 and i1 = i0+1. The implied
// last edge is defined by edge[n-1] = (indices[n-1], indices[0]). This
// ensures that the polyline defined by the edges is closed. The number of
// indices must be 3 or larger. The user is required to provided a polygon
// without self-intersections; that is, each vertex is shared by exactly two
// edges and two edges cannot intersect at a point that is an interior point
// of one of the edges.
// 
// Comparison operators are not provided. The semantics of equal polygons are
// complicated and (at the moment) not useful. The vertices of one polygon can
// be a cyclic permutation of the other polygon, but the polygons are the same
// geometrically. It is not clear how to implement an efficient comparison
// that does not process all possible cyclic permutations.

#include <GTL/Mathematics/Algebra/Vector.h>
#include <GTL/Mathematics/Intersection/2D/IntrSegment2Segment2.h>
#include <cmath>
#include <cstddef>
#include <vector>

namespace gtl
{
    template <typename T>
    class Polygon2
    {
    public:
        // The polygon has the specified number of vertices, which is also the
        // number of indices/edges. The user can set the vertices and indices
        // as needed.
        Polygon2(std::size_t inNumVertices, bool inCounterClockwise)
            :
            vertices(inNumVertices, Vector2<T>::Zero()),
            counterClockwise(inCounterClockwise)
        {
            GTL_ARGUMENT_ASSERT(
                vertices.size() >= 3,
                "Invalid number of vertices.");
        }

        // The polygon is specified by a collection of unique vertices.
        Polygon2(std::vector<Vector2<T>> const& inVertices, bool inCounterClockwise)
            :
            vertices(inVertices),
            counterClockwise(inCounterClockwise)
        {
            GTL_ARGUMENT_ASSERT(
                vertices.size() >= 3,
                "Invalid number of vertices.");
        }

        // The polygon is specified as a subset of unique vertices of a vertex
        // pool. The indices are lookups into the vertex pool.
        Polygon2(std::vector<Vector2<T>> const& inVertexPool,
            std::vector<std::size_t> const& inIndices, bool inCounterClockwise)
            :
            vertices(inIndices.size()),
            counterClockwise(inCounterClockwise)
        {
            GTL_ARGUMENT_ASSERT(
                inIndices.size() >= 3,
                "Invalid inputs.");

            for (std::size_t i = 0; i < vertices.size(); ++i)
            {
                vertices[i] = inVertexPool[inIndices[i]];
            }
        }

        // Geometric queries. These produce correct results regardless of
        // whether the vertices are listed in clockwise or counterclockwise
        // order.
        Vector2<T> ComputeVertexAverage() const
        {
            Vector2<T> average{};  // zero vector
            for (auto const& vertex : vertices)
            {
                average += vertex;
            }
            average /= static_cast<T>(vertices.size());
            return average;
        }

        T ComputePerimeterLength() const
        {
            T length = C_<T>(0);
            Vector2<T> v0 = vertices.back();
            for (auto const& v1 : vertices)
            {
                length += Length(v1 - v0);
                v0 = v1;
            }
            return length;
        }

        T ComputeArea() const
        {
            T area = C_<T>(0);
            std::size_t const n = vertices.size();
            Vector2<T> v0 = vertices[n - 2];
            Vector2<T> v1 = vertices[n - 1];
            for (auto const& v2 : vertices)
            {
                area += v1[0] * (v2[1] - v0[1]);
                v0 = v1;
                v1 = v2;
            }
            area *= C_<T>(1, 2);
            return std::fabs(area);
        }

        bool IsSimple() const
        {
            TIQuery<T, Segment2<T>, Segment2<T>> query{};
            Segment2<T> seg0{}, seg1{};

            for (std::size_t i0 = 0; i0 < vertices.size(); ++i0)
            {
                std::size_t i0p1 = (i0 + 1) % vertices.size();
                seg0.p[0] = vertices[i0];
                seg0.p[1] = vertices[i0p1];

                std::size_t i1min = (i0 + 2) % vertices.size();
                std::size_t i1max = (i0 - 2 + vertices.size()) % vertices.size();
                for (std::size_t i1 = i1min; i1 <= i1max; ++i1)
                {
                    std::size_t i1p1 = (i1 + 1) % vertices.size();
                    seg1.p[0] = vertices[i1];
                    seg1.p[1] = vertices[i1p1];

                    auto output = query(seg0, seg1);
                    if (output.intersect)
                    {
                        return false;
                    }
                }
            }
            return true;
        }

        std::vector<Vector2<T>> vertices;
        bool counterClockwise;

    private:
        friend class UnitTestPolygon2;
    };
}
