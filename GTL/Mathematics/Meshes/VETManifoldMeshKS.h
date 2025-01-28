// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.28

#pragma once

// The VETManifoldMeshKS class represents a vertex-edge-triangle manifold
// mesh. Suffix "KS" is an acronym for "Known Size"; the number of vertices
// is known at runtime. This supports minimal heap allocations and
// deallocations that are noticeable when profiling code that uses the
// general-purpose VETManifoldMesh, the latter having a significant impact
// on CPU usage for memory management.
//
// The triangles are required to have counterclockwise ordering of vertices.
//
// TODO: The Vertex::adjacent are resized when an insertion requires it. If
// you choose 'adjacentGrowth' sufficiently large, there will be no resizing.
// However, no-resizing is not guaranteed unless you have metaknowledge of
// the maximum number of triangles sharing a vertex. An alternative is to
// have a VETManifoldMeshKS member 'mEdges[]' that stores the opposite edges.
// This array can be preallocated as a doubly linked list that uses array
// indices for links rather than dynamically allocated list nodes. This
// requires constructor input maxEdges and, perhaps, edgeGrowth. Currently,
// there is such a data structure in Delaunay2 for maintaining the closed
// polyline that represents a 2-dimensional convex hull.

#include <GTL/Mathematics/Meshes/VETFeaturesKS.h>
#include <algorithm>
#include <array>
#include <cstddef>
#include <limits>
#include <set>
#include <thread>
#include <vector>

namespace gtl
{
    class VETManifoldMeshKS
    {
    public:
        struct Vertex
        {
            Vertex()
                :
                numAdjacent(0),
                adjacent{}
            {
            }

            // The vertex mVertexPool[v] stores all the triangles sharing v.
            // The directed edges opposite the triangles are stored. Adjacent
            // triangle t has indices
            //   <v0, v1, v2> = <v, adjacent[t][0], adjacent[t][1]>
            // The triangles are ordered counterclockwise, so the directed
            // adjacent are <v0, v1>, <v1, v2> and <v2, v0>.
            std::size_t numAdjacent;
            std::vector<std::array<std::size_t, 2>> adjacent;
        };

        VETManifoldMeshKS()
            :
            mAdjacentGrowth(0),
            mVertexPool{}
        {
        }

        // If the number of threads is larger than 1, the initialization of
        // the vertices is multithreaded.
        VETManifoldMeshKS(std::size_t maxVertices, std::size_t adjacentGrowth, std::size_t numThreads)
            :
            mAdjacentGrowth(0),
            mVertexPool{}
        {
            Reset(maxVertices, adjacentGrowth, numThreads);
        }

        virtual ~VETManifoldMeshKS() = default;

        void Reset(std::size_t maxVertices, std::size_t adjacentGrowth, std::size_t numThreads)
        {
            if (maxVertices >= 3)
            {
                mAdjacentGrowth = adjacentGrowth;
                mVertexPool.resize(maxVertices);
                if (numThreads > 1)
                {
                    // Partition the vertices for multithreading.
                    std::vector<std::size_t> vMin(numThreads), vSup(numThreads);
                    std::size_t load = maxVertices / numThreads;
                    vMin.front() = 0;
                    vSup.back() = maxVertices;
                    for (std::size_t i = 0; i < numThreads; ++i)
                    {
                        vMin[i] = i * load;
                        vSup[i] = (i + 1) * load;
                    }
                    vSup.back() = maxVertices;

                    std::vector<std::thread> process(numThreads);
                    for (std::size_t i = 0; i < numThreads; ++i)
                    {
                        process[i] = std::thread(
                            [this, i, &vMin, &vSup]()
                            {
                                for (std::size_t v = vMin[i]; v < vSup[i]; ++v)
                                {
                                    auto& vertex = mVertexPool[v];
                                    vertex.numAdjacent = 0;
                                    vertex.adjacent.resize(mAdjacentGrowth);
                                }
                            });
                    }

                    for (std::size_t i = 0; i < numThreads; ++i)
                    {
                        process[i].join();
                    }
                }
                else
                {
                    for (std::size_t v = 0; v < maxVertices; ++v)
                    {
                        auto& vertex = mVertexPool[v];
                        vertex.numAdjacent = 0;
                        vertex.adjacent.resize(mAdjacentGrowth);
                    }
                }
            }
            else
            {
                for (std::size_t v = 0; v < maxVertices; ++v)
                {
                    auto& vertex = mVertexPool[v];
                    vertex.numAdjacent = 0;
                    vertex.adjacent.clear();
                }
                mAdjacentGrowth = 0;
                mVertexPool.clear();
            }
        }

        // The triangle <v0, v1, v2> must be counterclockwise ordered. Some
        // clients might know that insertions never violate the manifold
        // restriction; in these cases, pass 'false' for the last parameter.
        // An example is ConvexHull3.
        bool Insert(std::size_t v0, std::size_t v1, std::size_t v2, bool testForNonmanifold)
        {
            if (v0 < mVertexPool.size() && v1 < mVertexPool.size() && v2 < mVertexPool.size())
            {
                // The triangle to insert.
                std::array<std::size_t, 3> v{ v0, v1, v2 };
                if (testForNonmanifold)
                {
                    // Ensure that the insertion does not lead to a
                    // nonmanifold mesh.
                    for (std::size_t j0 = 2, j1 = 0; j1 < 3; j0 = j1++)
                    {
                        Vertex& vertex = mVertexPool[v[j0]];
                        for (std::size_t i = 0; i < vertex.numAdjacent; ++i)
                        {
                            if (vertex.adjacent[i][0] == v[j1])
                            {
                                // The directed edge <v[j0], v[j1]> is already
                                // contained by a triangle sharing v[j0]. The
                                // mesh will become nonmanifold if the edge
                                // were allowed to be inserted.
                                return false;
                            }
                        }
                    }
                }

                // Insert the triangle.
                for (std::size_t j2 = 0, j0 = 1, j1 = 2; j2 < 3; j0 = j1, j1 = j2++)
                {
                    Vertex& vertex = mVertexPool[v[j2]];
                    if (vertex.numAdjacent == vertex.adjacent.size())
                    {
                        // The current edge storage is full. Resize it to allow
                        // more insertions.
                        vertex.adjacent.resize(vertex.adjacent.size() + mAdjacentGrowth);
                    }
                    vertex.adjacent[vertex.numAdjacent++] = { v[j0], v[j1] };
                }
                return true;
            }
            return false;
        }

        // The triangle <v0, v1, v2> must be counterclockwise ordered.
        bool Remove(std::size_t v0, std::size_t v1, std::size_t v2)
        {
            if (v0 < mVertexPool.size() && v1 < mVertexPool.size() && v2 < mVertexPool.size())
            {
                // The triangle to remove.
                std::array<std::size_t, 3> v{ v0, v1, v2 };

                // Verify that the triangle exists.
                std::size_t constexpr invalid = std::numeric_limits<std::size_t>::max();
                std::array<std::size_t, 3> location{ invalid, invalid, invalid };
                for (std::size_t j0 = 2, j1 = 0; j1 < 3; j0 = j1++)
                {
                    Vertex& vertex = mVertexPool[v[j0]];
                    for (std::size_t i = 0; i < vertex.numAdjacent; ++i)
                    {
                        if (vertex.adjacent[i][0] == v[j1])
                        {
                            location[j0] = i;
                            break;
                        }
                    }
                    if (location[j0] == invalid)
                    {
                        return false;
                    }
                }

                // Remove the triangle. Maintain a compact array. If the
                // location is already at the end of the array, it is
                // sufficient to decrement numAdjacent. If the location is
                // interior to the array, swap the last edge into the
                // vacated slot.
                for (std::size_t j = 0; j < 3; ++j)
                {
                    Vertex& vertex = mVertexPool[v[j]];
                    // vertex.numAdjacent >= 1 is guaranteed, so there is no
                    // wraparound caused by the subtraction.
                    std::size_t const last = vertex.numAdjacent - 1;
                    if (location[j] < last)
                    {
                        // The location is interior to the array.
                        vertex.adjacent[location[j]] = vertex.adjacent[last];
                    }
                    // else: The location is at the end of the array.
                    --vertex.numAdjacent;
                }
                return true;
            }
            return false;
        }

        // Test for existence of the vertex v.
        bool ExistsVertex(std::size_t v0) const
        {
            if (v0 < mVertexPool.size())
            {
                auto const& vertex = mVertexPool[v0];
                return vertex.numAdjacent > 0;
            }
            return false;
        }

        // Test for existence of the directed edge <v0, v1>.
        bool ExistsEdge(std::size_t v0, std::size_t v1) const
        {
            if (v0 < mVertexPool.size() && v1 < mVertexPool.size())
            {
                auto const& vertex = mVertexPool[v0];
                for (std::size_t e = 0; e < vertex.numAdjacent; ++e)
                {
                    if (vertex.adjacent[e][0] == v1)
                    {
                        return true;
                    }
                }
            }
            return false;
        }

        // Test for existence of the ordered triangle <v0, v1, v2>.
        bool ExistsTriangle(std::size_t v0, std::size_t v1, std::size_t v2) const
        {
            if (v0 < mVertexPool.size() && v1 < mVertexPool.size() && v2 < mVertexPool.size())
            {
                auto const& vertex = mVertexPool[v0];
                for (std::size_t e = 0; e < vertex.numAdjacent; ++e)
                {
                    if (vertex.adjacent[e][0] == v1 && vertex.adjacent[e][1] == v2)
                    {
                        return true;
                    }
                }
            }
            return false;
        }

        void GetVertices(std::vector<std::size_t>& vertices) const
        {
            std::size_t numVertices = 0;
            for (std::size_t v = 0; v < mVertexPool.size(); ++v)
            {
                auto const& vertex = mVertexPool[v];
                if (vertex.numAdjacent > 0)
                {
                    ++numVertices;
                }
            }

            vertices.resize(numVertices);
            numVertices = 0;
            for (std::size_t v = 0; v < mVertexPool.size(); ++v)
            {
                auto const& vertex = mVertexPool[v];
                if (vertex.numAdjacent > 0)
                {
                    vertices[numVertices++] = v;
                }
            }
        }

        void GetEdges(std::vector<EdgeKey<false>>& adjacent) const
        {
            std::set<EdgeKey<false>> unique{};
            for (std::size_t v = 0; v < mVertexPool.size(); ++v)
            {
                auto const& vertex = mVertexPool[v];
                for (std::size_t e = 0; e < vertex.numAdjacent; ++e)
                {
                    unique.insert(EdgeKey<false>(
                        vertex.adjacent[e][0],
                        vertex.adjacent[e][1]));
                }
            }

            adjacent.resize(unique.size());
            std::copy(unique.begin(), unique.end(), adjacent.begin());
        }

        void GetTriangles(std::vector<TriangleKey<true>>& triangles) const
        {
            std::set<TriangleKey<true>> unique{};
            for (std::size_t v = 0; v < mVertexPool.size(); ++v)
            {
                auto const& vertex = mVertexPool[v];
                for (std::size_t e = 0; e < vertex.numAdjacent; ++e)
                {
                    unique.insert(TriangleKey<true>(
                        v,
                        vertex.adjacent[e][0],
                        vertex.adjacent[e][1]));
                }
            }

            triangles.resize(unique.size());
            std::copy(unique.begin(), unique.end(), triangles.begin());
        }

        Vertex const& GetVertex(std::size_t v) const
        {
            if (v < mVertexPool.size())
            {
                return mVertexPool[v];
            }

            GTL_OUTOFRANGE_ERROR(
                "Vertex index out of range.");
        }

        Vertex& GetVertex(std::size_t v)
        {
            if (v < mVertexPool.size())
            {
                return mVertexPool[v];
            }

            GTL_OUTOFRANGE_ERROR(
                "Vertex index out of range.");
        }

        inline std::vector<Vertex> const& GetVertexPool() const
        {
            return mVertexPool;
        }

        inline std::vector<Vertex>& GetVertexPool()
        {
            return mVertexPool;
        }

        std::size_t GetOppositeVertex(std::size_t v0, std::size_t v1, std::size_t v2,
            bool testForTriangleExistence) const
        {
            if (testForTriangleExistence)
            {
                if (!ExistsTriangle(v0, v1, v2))
                {
                    return std::numeric_limits<std::size_t>::max();
                }
            }

            Vertex const& vertex = mVertexPool[v2];
            for (std::size_t e = 0; e < vertex.numAdjacent; ++e)
            {
                if (vertex.adjacent[e][0] == v1)
                {
                    return vertex.adjacent[e][1];
                }
            }
            return std::numeric_limits<std::size_t>::max();
        }

    protected:
        std::size_t mAdjacentGrowth;
        std::vector<Vertex> mVertexPool;

    private:
        friend class UnitTestVETManifoldMeshKS;
    };
}
