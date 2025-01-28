// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.28

#pragma once

// A data structure used to keep track of a set of unique features (edges,
// triangles) in a vertex-edge-triangle mesh.

#include <GTL/Mathematics/Meshes/EdgeKey.h>
#include <GTL/Mathematics/Meshes/TriangleKey.h>
#include <GTL/Utility/Exceptions.h>
#include <algorithm>
#include <array>
#include <cstddef>
#include <limits>
#include <thread>
#include <vector>

namespace gtl
{
    template <std::size_t Dimension>
    class VETFeaturesKS
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

            // The vertex mVertexPool[v] stores all the features sharing
            // v, where v is the smallest index of <v0,v1> for edges or
            // <v0,v1,v2> for triangles. A consequences is that mVertexPool
            // acts as a set of unique edges or unique triangles. However,
            // for directed edges, v does not have to be the smallest index.
            // In this case, mVertexPool is a set of unique directed edges.
            // It is possible that both <v0,v1> and <v1,v0> are in the set.
            std::size_t numAdjacent;
            std::vector<std::array<std::size_t, Dimension>> adjacent;
        };

        VETFeaturesKS()
            :
            mAdjacentGrowth(0),
            mVertexPool{}
        {
            static_assert(
                Dimension == 1 || Dimension == 2,
                "Only edges and triangles are supported by VETFeaturesKS.");
        }

        // If the number of threads is larger than 1, the initialization of
        // the vertices is multithreaded.
        VETFeaturesKS(std::size_t maxVertices, std::size_t adjacentGrowth, std::size_t numThreads)
            :
            mAdjacentGrowth(0),
            mVertexPool{}
        {
            static_assert(
                Dimension == 1 || Dimension == 2,
                "Only edges and triangles are supported by VETFeaturesKS.");

            Reset(maxVertices, adjacentGrowth, numThreads);
        }

        virtual ~VETFeaturesKS() = default;

        void Reset(std::size_t maxVertices, std::size_t adjacentGrowth, std::size_t numThreads)
        {
            if (maxVertices >= Dimension + 1)
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

        // Insert an edge (Dimension = 1) or a triangle (Dimension = 2).
        bool Insert(std::array<std::size_t, Dimension + 1> const& feature)
        {
            auto& vertex = mVertexPool[feature[0]];
            for (std::size_t v = 0; v < vertex.numAdjacent; ++v)
            {
                auto const& adjacent = vertex.adjacent[v];
                if (std::equal(adjacent.begin(), adjacent.end(), feature.begin() + 1))
                {
                    // The triangle is already in the pool, so there is
                    // nothing to do.
                    return false;
                }
            }

            // Insert the triangle into the pool.
            if (vertex.numAdjacent == vertex.adjacent.size())
            {
                // The current triangle storage is full. Resize it to allow
                // more insertions.
                vertex.adjacent.resize(vertex.adjacent.size() + mAdjacentGrowth);
            }
            auto& target = vertex.adjacent[vertex.numAdjacent];
            std::copy(feature.begin() + 1, feature.end(), target.begin());
            ++vertex.numAdjacent;
            return true;
        }

        // Remove an edge (Dimension = 1) or a triangle (Dimension = 2).
        bool Remove(std::array<std::size_t, Dimension + 1> const& feature)
        {
            // Determine whether the edge is already in the pool.
            auto& vertex = mVertexPool[feature[0]];
            std::size_t constexpr invalid = std::numeric_limits<std::size_t>::max();
            std::size_t location = invalid;
            for (std::size_t v = 0; v < vertex.numAdjacent; ++v)
            {
                auto const& adjacent = vertex.adjacent[v];
                if (std::equal(adjacent.begin(), adjacent.end(), feature.begin() + 1))
                {
                    location = v;
                    break;
                }
            }

            if (location != invalid)
            {
                // The triangle is in the pool; remove it. The subtraction
                // does not wrap around because vertex.numAdjacent >= 1 is
                // guaranteed.
                std::size_t const last = vertex.numAdjacent - 1;
                if (location < last)
                {
                    // The location is interior to the array.
                    vertex.adjacent[location] = vertex.adjacent[last];
                }
                // else: The location is at the end of the array.
                --vertex.numAdjacent;
                return true;
            }

            // The edge is not in the pool, so there is nothing to do.
            return false;
        }

        // Test for existence of an edge (Dimension = 1) or a triangle
        // (Dimension = 2).
        bool Exists(std::array<std::size_t, Dimension + 1> const& feature) const
        {
            auto const& vertex = mVertexPool[feature[0]];
            for (std::size_t v = 0; v < vertex.numAdjacent; ++v)
            {
                auto const& adjacent = vertex.adjacent[v];
                if (std::equal(adjacent.begin(), adjacent.end(), feature.begin() + 1))
                {
                    return true;
                }
            }
            return false;
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

    protected:
        std::size_t mAdjacentGrowth;
        std::vector<Vertex> mVertexPool;
    };

    using VETEdgesKS = VETFeaturesKS<1>;
    using VETTrianglesKS = VETFeaturesKS<2>;
}
