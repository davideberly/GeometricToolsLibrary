// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.10.11

#pragma once

// Compute the minimum spanning tree of a vertex-edge graph. The code is an
// implementation of Prim's algorithm based on the pseudocode in
//   Introduction to Algorithms, 4th edition (April 5, 2022)
//   Thomas H. Cormen, Charles E. Leiserson, Ronald L. Rivest, Clifford Stein,
//   The MIT Press, Cambridge, Massachusetts
// The pseudocode uses a priority queue that is sorted based on a set of keys.
// In C++, it difficult (if not impossible) to use std::priority_queue
// directly in the implementation. There appears to be no way to tell the
// priority queue to update when a key[] is modified outside the class. The
// GTE MinHeap class provides this capability.
// 
// The WeightType must be a scalar type that has comparison operators '<'
// and '<=' This is a requirement of GTE's MinHeap, but in GTL only the '<'
// will be required.
// 
// The edges[] input to Execute(...) must be unique. The EdgeKey<false>(v0,v1)
// object stores unordered edges of the form (min(v0,v1), max(v0,v1)). The
// v0 and v1 vertex indices must be contained in the vertex input, and v0 and
// v1 must be different numbers.
// 
// The weights[] input must have the same number of elements as edges[]. Also,
// the weights must be positive.
// 
// Set validateInputs to true to have Execute test for valid input. This is an
// expensive operation that the caller might not want if it is known the
// inputs are valid.
// 
// The output minimumSpanningTree[] is the minimum spanning tree.
// 
// The output backEdges[] are the graph edges not in the minimum spanning
// tree. The tree has no cycles, but if you were to insert a back edge into
// the tree, the resulting graph has a cycle.

#include <GTL/Utility/Exceptions.h>
#include <GTL/Utility/MinHeap.h>
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <map>
#include <set>
#include <utility>
#include <vector>

namespace gtl
{
    template <typename WeightType>
    class MinimumSpanningTree
    {
    public:
        static std::size_t constexpr nil = std::numeric_limits<std::size_t>::max();
        static WeightType constexpr zeroWeight = static_cast<WeightType>(0);
        static WeightType constexpr maxWeight = std::numeric_limits<WeightType>::max();

        using Edge = std::array<std::size_t, 2>;

        // The vertices are in {0,...,numVertices-1}. The edges[] need not
        // reference all the vertices.
        static void Execute(
            std::vector<Edge> const& edges,
            std::vector<WeightType> const& weights,
            bool validateInputs,
            std::vector<Edge>& minimumSpanningTree,
            std::vector<Edge>& backEdges)
        {
            minimumSpanningTree.clear();
            backEdges.clear();

            if (validateInputs)
            {
                ValidateInputs(edges, weights);
            }

            // Map the distinct vertex indices to consecutive indices from 0
            // to maxVertices-1. The map key is the vertex index and the map
            // value is its counterpart in the consecutive indices.
            std::map<std::size_t, std::size_t> vertexMap{};
            std::vector<std::size_t> inverseVertexMap{};
            CreateVertexMap(edges, vertexMap, inverseVertexMap);

            // Create an edge map using the remapped vertex indices. At the
            // same time, create a vertex adjacency map.
            std::map<Edge, WeightType> edgeMap{};
            std::map<std::size_t, std::vector<std::size_t>> adjacencyMap{};
            CreateEdgeAndAdjacencyMaps(edges, weights, vertexMap, edgeMap, adjacencyMap);

            // Use a priority queue to extract the minimum spanning tree. THe
            // vertex indices are the remapped ones.
            ExtractMinimumSpanningTree(vertexMap.size(), edgeMap, adjacencyMap,
                minimumSpanningTree);

            // Remove the minimum spanning tree edges from the edge map. The
            // remaining elements are back edges, but include both (v0,v1)
            // and (v1,v0). The duplicates are omitted by storing only those
            // edges for which v0 < v1.
            ExtractBackEdges(edgeMap, minimumSpanningTree, backEdges);

            // Convert back to the original vertex indices.
            ConvertToOriginalIndices(inverseVertexMap, minimumSpanningTree, backEdges);
        }

    private:
        static void ValidateInputs(
            std::vector<Edge> const& edges,
            std::vector<WeightType> const& weights)
        {
            GTL_ARGUMENT_ASSERT(
                edges.size() == weights.size(),
                "The edges.size() and weights.size() must match.");

            std::set<Edge> uniqueEdges{};
            for (std::size_t e = 0; e < edges.size(); ++e)
            {
                GTL_ARGUMENT_ASSERT(
                    weights[e] > zeroWeight,
                    "Encountered a nonpositive weight.");

                auto const& edge = edges[e];
                GTL_ARGUMENT_ASSERT(
                    edge[0] != nil && edge[1] != nil && edge[0] != edge[1],
                    "Encountered a degenerate edge.");

                if (edge[0] < edge[1])
                {
                    uniqueEdges.insert(edge);
                }
                else
                {
                    uniqueEdges.insert({ edge[1], edge[0] });
                }
                }

            GTL_ARGUMENT_ASSERT(
                edges.size() == uniqueEdges.size(),
                "Encountered a duplicate edge.");
        }

        static void CreateVertexMap(
            std::vector<Edge> const& edges,
            std::map<std::size_t, std::size_t>& vertexMap,
            std::vector<std::size_t>& inverseVertexMap)
        {
            std::size_t numVertices = 0;
            for (auto const& edge : edges)
            {
                for (std::size_t i = 0; i < 2; ++i)
                {
                    auto vIter = vertexMap.find(edge[i]);
                    if (vIter == vertexMap.end())
                    {
                        vertexMap.insert(std::make_pair(edge[i], numVertices));
                        ++numVertices;
                    }
                }
            }

            inverseVertexMap.resize(numVertices);
            for (auto const& element : vertexMap)
            {
                inverseVertexMap[element.second] = element.first;
            }
        }

        static void CreateEdgeAndAdjacencyMaps(
            std::vector<Edge> const& edges,
            std::vector<WeightType> const& weights,
            std::map<std::size_t, std::size_t> const& vertexMap,
            std::map<Edge, WeightType>& edgeMap,
            std::map<std::size_t, std::vector<std::size_t>>& adjacencyMap)
        {
            for (std::size_t e = 0; e < edges.size(); ++e)
            {
                auto const& edge = edges[e];
                auto const& weight = weights[e];

                std::size_t v0 = vertexMap.find(edge[0])->second;
                std::size_t v1 = vertexMap.find(edge[1])->second;
                std::array<Edge, 2> remapped =
                {{
                    { v0, v1 },
                    { v1, v0 }
                }};

                // The GTL_RUNTIME_ASSERT call is required if validateInput is
                // false in the call to Execute(...).
                for (std::size_t i = 0; i < 2; ++i)
                {
                    auto result = edgeMap.insert(std::make_pair(remapped[i], weight));
                    GTL_RUNTIME_ASSERT(
                        result.second == true,
                        "Unexpected result for validated edges.");

                    auto aIter = adjacencyMap.find(remapped[i][0]);
                    if (aIter != adjacencyMap.end())
                    {
                        aIter->second.push_back(remapped[i][1]);
                    }
                    else
                    {
                        adjacencyMap.insert(std::make_pair(remapped[i][0],
                            std::vector<std::size_t>{ remapped[i][1] }));
                    }
                }
            }
        }

        static void ExtractMinimumSpanningTree(
            std::size_t numVertices,
            std::map<Edge, WeightType> const& edgeMap,
            std::map<std::size_t, std::vector<std::size_t>> const& adjacencyMap,
            std::vector<Edge>& minimumSpanningTree)
        {
            // Initialize the priority queue. The heap objects are edges in
            // the graph. These are managed outside the heap to avoid copying
            // objects to the heap via Insert(...) calls, which can be
            // expensive if the objects are complex structures storing a large
            // amount of data.
            MinHeap<WeightType> heap(numVertices);
            std::vector<Edge> treeEdges(numVertices);
            for (std::size_t handle = 0; handle < numVertices; ++handle)
            {
                treeEdges[handle] = { nil, handle };
            }

            // The heap weights are edge weights for the graph. These are
            // managed inside the heap so that the heap can propagate values
            // to attain the minimum-heap condition. The keys[] values are
            // inputs to heap.GetNode(...) in order to access a desired
            // heap Node. The nodes store (handle,weight) pairs.
            std::vector<std::size_t> keys(numVertices);
            std::size_t handle = 0;
            keys[handle] = heap.Insert(handle, zeroWeight);
            for (++handle; handle < numVertices; ++handle)
            {
                keys[handle] = heap.Insert(handle, maxWeight);
            }

            // Extract the minimum spanning tree. The map find(...) calls
            // are guaranteed to be successful because of the design of
            // adjacencyMap and of edgeMap.
            std::vector<std::uint32_t> inHeap(numVertices, 1);
            minimumSpanningTree.reserve(numVertices);
            while (heap.GetNumElements() > 0)
            {
                WeightType weight{};
                heap.Remove(handle, weight);
                auto const& treeEdge = treeEdges[handle];
                inHeap[treeEdge[1]] = 0;
                minimumSpanningTree.push_back(treeEdge);

                for (auto adj : adjacencyMap.find(treeEdge[1])->second)
                {
                    if (inHeap[adj])
                    {
                        // The node.weight is the current minimum weight for
                        // the already visited adjacent vertices.
                        auto const& node = heap.GetNode(keys[adj]);

                        // Get the weight of the next visited adjacent vertex.
                        weight = edgeMap.find({ treeEdge[1], adj })->second;

                        if (weight < node.weight)
                        {
                            // The next visited adjacent vertex has smallest
                            // weight. Update the heap with this weight.
                            treeEdges[node.handle][0] = treeEdge[1];
                            heap.Update(keys[adj], weight);
                        }
                    }
                }
            }
        }

        static void ExtractBackEdges(
            std::map<Edge, WeightType>& edgeMap,
            std::vector<Edge> const& minimumSpanningTree,
            std::vector<Edge>& backEdges)
        {
            // Remove the tree edges from the graph.
            for (auto const& treeEdge : minimumSpanningTree)
            {
                edgeMap.erase(treeEdge);
                edgeMap.erase({ treeEdge[1], treeEdge[0] });
            }

            // Extract the back edges. They occurs in pairs, so
            // eliminate one of the pair using vertex ordering.
            std::size_t index = 0;
            backEdges.resize(edgeMap.size() / 2);
            for (auto const& element : edgeMap)
            {
                if (element.first[0] < element.first[1])
                {
                    backEdges[index++] = element.first;
                }
            }
        }

        static void ConvertToOriginalIndices(
            std::vector<std::size_t> const& inverseVertexMap,
            std::vector<Edge>& minimumSpanningTree,
            std::vector<Edge>& backEdges)
        {
            for (auto& treeEdge : minimumSpanningTree)
            {
                for (std::size_t i = 0; i < 2; ++i)
                {
                    if (treeEdge[i] != nil)
                    {
                        treeEdge[i] = inverseVertexMap[treeEdge[i]];
                    }
                }
            }

            for (auto& backEdge : backEdges)
            {
                for (std::size_t i = 0; i < 2; ++i)
                {
                    backEdge[i] = inverseVertexMap[backEdge[i]];
                }
            }
        }
    };
}
