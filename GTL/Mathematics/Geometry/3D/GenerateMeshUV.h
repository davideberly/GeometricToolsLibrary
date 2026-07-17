// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.17

#pragma once

// This class is an implementation of the barycentric mapping algorithm
// described in Section 5.3 of the book
//     Polygon Mesh Processing
//     Mario Botsch, Leif Kobbelt, Mark Pauly, Pierre Alliez, Bruno Levy
//     AK Peters, Ltd., Natick MA, 2010
// It uses the mean value weights described in Section 5.3.1 to allow the mesh
// geometry to influence the texture coordinate generation, and it uses
// Gauss-Seidel iteration to solve the sparse linear system. The authors'
// advice is that the Gauss-Seidel approach works well for at most about 5000
// vertices, presumably the convergence rate degrading as the number of
// vertices increases.
//
// The algorithm implemented here has an additional preprocessing step that
// computes a topological distance transform of the vertices. The boundary
// texture coordinates are propagated inward by updating the vertices in
// topological distance order, leading to fast convergence for large numbers
// of vertices.

#include <GTL/Mathematics/Algebra/Vector.h>
#include <GTL/Mathematics/Meshes/DynamicETManifoldMesh.h>
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <limits>
#include <set>
#include <thread>
#include <utility>
#include <vector>

namespace gtl
{
    template <typename T>
    class GenerateMeshUV
    {
    public:
        // Set the number of threads to 0 when you want the code to run in the
        // main thread of the applications. Set the number of threads to a
        // positive number when you want the code to run multithreaded on the
        // CPU. Derived classes that use the GPU ignore the number of threads.
        // Provide a callback when you want to monitor each iteration of the
        // uv-solver. The input to the progress callback is the current
        // iteration; it starts at 1 and increases to the numIterations input
        // to the operator() member function.
        GenerateMeshUV(std::size_t numThreads,
            std::function<void(std::size_t)> const* progress = nullptr)
            :
            mNumThreads(numThreads),
            mProgress(progress),
            mNumVertices(0),
            mVertices(nullptr),
            mTCoords(nullptr),
            mNumBoundaryEdges(0),
            mBoundaryStart(0)
        {
        }

        virtual ~GenerateMeshUV() = default;

        // The incoming mesh must be edge-triangle manifold and have rectangle
        // topology (simply connected, closed polyline boundary). The arrays
        // 'vertices' and 'tcoords' must both have 'numVertices' elements.
        // Set 'useSquareTopology' to true for the generated coordinates to
        // live in the uv-square [0,1]^2. Set it to false for the generated
        // coordinates to live in a convex polygon that inscribes the uv-disk
        // of center (1/2,1/2) and radius 1/2.
        void operator()(std::size_t numIterations, bool useSquareTopology,
            std::size_t numVertices, Vector3<T> const* vertices, std::size_t numIndices,
            std::size_t const* indices, Vector2<T>* tcoords)
        {
            // Ensure that numIterations is even, which avoids having a memory
            // copy from the temporary ping-pong buffer to 'tcoords'.
            if (numIterations & 1)
            {
                ++numIterations;
            }

            mNumVertices = numVertices;
            mVertices = vertices;
            mTCoords = tcoords;

            // The linear system solver has a first pass to initialize the
            // texture coordinates to ensure the Gauss-Seidel iteration
            // converges rapidly. This requires the texture coordinates all
            // start as (-1,-1).
            for (std::size_t i = 0; i < numVertices; ++i)
            {
                mTCoords[i][0] = (T)-1;
                mTCoords[i][1] = (T)-1;
            }

            // Create the manifold mesh data structure.
            mGraph.Clear();
            std::size_t const numTriangles = numIndices / 3;
            for (std::size_t t = 0; t < numTriangles; ++t)
            {
                std::size_t v0 = *indices++;
                std::size_t v1 = *indices++;
                std::size_t v2 = *indices++;
                mGraph.Insert(v0, v1, v2);
            }

            TopologicalVertexDistanceTransform();

            if (useSquareTopology)
            {
                AssignBoundaryTextureCoordinatesSquare();
            }
            else
            {
                AssignBoundaryTextureCoordinatesDisk();
            }

            ComputeMeanValueWeights();
            SolveSystem(numIterations);
        }

    protected:
        // A CPU-based implementation is provided by this class. The derived
        // classes using the GPU override this function.
        virtual void SolveSystemInternal(std::size_t numIterations)
        {
            if (mNumThreads > 1)
            {
                SolveSystemCPUMultiple(numIterations);
            }
            else
            {
                SolveSystemCPUSingle(numIterations);
            }
        }

        // Constructor inputs.
        std::size_t mNumThreads;
        std::function<void(std::size_t)> const* mProgress;

        // Convenience members that store the input parameters to operator().
        std::size_t mNumVertices;
        Vector3<T> const* mVertices;
        Vector2<T>* mTCoords;

        // The edge-triangle manifold graph, where each edge is shared by at
        // most two triangles.
        DynamicETManifoldMesh mGraph;

        // The mVertexInfo array stores -1 for the interior vertices. For a
        // boundary edge <v0,v1> that is counterclockwise,
        // mVertexInfo[v0] = v1, which gives us an orded boundary polyline.
        std::vector<std::size_t> mVertexInfo;
        std::size_t mNumBoundaryEdges, mBoundaryStart;
        typedef DynamicETManifoldMesh::Edge Edge;
        std::set<Edge*> mInteriorEdges;

        // The vertex graph required to set up a sparse linear system of
        // equations to determine the texture coordinates.
        struct Vertex
        {
            // The topological distance from the boundary of the mesh.
            std::size_t distance;

            // The value range0 is the index into mVertexGraphData for the
            // first adjacent vertex. The value range1 is the number of
            // adjacent vertices.
            std::size_t range0, range1;

            // TODO: The members used to be 'std::int32_t'. Figure out the HLSL
            // layout and padding.
            // 
            // Unused on the CPU. The padding is necessary for the HLSL and
            // GLSL programs in GPUGenerateMeshUV.h.
            std::size_t padding;
        };

        std::vector<Vertex> mVertexGraph;
        std::vector<std::pair<std::size_t, T>> mVertexGraphData;

        // The vertices are listed in the order determined by a topological
        // distance transform. Boundary vertices have 'distance' 0. Any
        // vertices that are not boundary vertices but are edge-adjacent to
        // boundary vertices have 'distance' 1. Neighbors of those have
        // distance '2', and so on. The mOrderedVertices array stores
        // distance-0 vertices first, distance-1 vertices second, and so on.
        std::vector<std::size_t> mOrderedVertices;

    private:
        static std::size_t constexpr invalid = std::numeric_limits<std::size_t>::max();

        void TopologicalVertexDistanceTransform()
        {
            // Initialize the graph information.
            mVertexInfo.resize(mNumVertices);
            std::fill(mVertexInfo.begin(), mVertexInfo.end(), invalid);
            mVertexGraph.resize(mNumVertices);
            mVertexGraphData.resize(2 * mGraph.GetEdges().size());
            std::pair<std::size_t, T> initialData = std::make_pair(invalid, C_<T>(-1));
            std::fill(mVertexGraphData.begin(), mVertexGraphData.end(), initialData);
            mOrderedVertices.resize(mNumVertices);
            mInteriorEdges.clear();
            mNumBoundaryEdges = 0;
            mBoundaryStart = invalid;

            // Count the number of adjacent vertices for each vertex. For
            // data sets with a large number of vertices, this is a
            // preprocessing step to avoid a dynamic data structure that has
            // a large number of std:map objects that take a very long time
            // to destroy when a debugger is attached to the executable.
            // Instead, we allocate a single array that stores all the
            // adjacency information. It is also necessary to bundle the
            // data this way for a GPU version of the algorithm.
            std::vector<std::size_t> numAdjacencies(mNumVertices);
            std::fill(numAdjacencies.begin(), numAdjacencies.end(), 0);

            for (auto const& element : mGraph.GetEdges())
            {
                ++numAdjacencies[element.first[0]];
                ++numAdjacencies[element.first[1]];

                if (element.second->T[1])
                {
                    // This is an interior edge.
                    mInteriorEdges.insert(element.second.get());
                }
                else
                {
                    // This is a boundary edge. Determine the ordering of the
                    // vertex indices to make the edge counterclockwise.
                    ++mNumBoundaryEdges;
                    std::size_t v0 = element.second->V[0];
                    std::size_t v1 = element.second->V[1];
                    auto tri = element.second->T[0];
                    for (std::size_t i = 0; i < 3; ++i)
                    {
                        std::size_t v2 = tri->V[i];
                        if (v2 != v0 && v2 != v1)
                        {
                            // The vertex is opposite the boundary edge.
                            v0 = tri->V[(i + 1) % 3];
                            v1 = tri->V[(i + 2) % 3];
                            mVertexInfo[v0] = v1;
                            mBoundaryStart = std::min(mBoundaryStart, v0);
                            break;
                        }
                    }
                }
            }

            // Set the range data for each vertex.
            for (std::size_t vIndex = 0, aIndex = 0; vIndex < mNumVertices; ++vIndex)
            {
                std::size_t numAdjacent = numAdjacencies[vIndex];
                mVertexGraph[vIndex].range0 = aIndex;
                mVertexGraph[vIndex].range1 = numAdjacent;
                aIndex += numAdjacent;
                mVertexGraph[vIndex].padding = 0;
            }

            // Compute a topological distance transform of the vertices.
            std::set<std::size_t> currFront{};
            for (auto const& element : mGraph.GetEdges())
            {
                std::size_t v0 = element.second->V[0];
                std::size_t v1 = element.second->V[1];
                for (std::size_t i = 0; i < 2; ++i)
                {
                    if (mVertexInfo[v0] == invalid)
                    {
                        mVertexGraph[v0].distance = invalid;
                    }
                    else
                    {
                        mVertexGraph[v0].distance = 0;
                        currFront.insert(v0);
                    }

                    // Insert v1 into the first available slot of the
                    // adjacency array.
                    std::size_t range0 = mVertexGraph[v0].range0;
                    std::size_t range1 = mVertexGraph[v0].range1;
                    for (std::size_t j = 0; j < range1; ++j)
                    {
                        std::pair<std::size_t, T>& data = mVertexGraphData[range0 + j];
                        if (data.second == C_<T>(-1))
                        {
                            data.first = v1;
                            data.second = C_<T>(0);
                            break;
                        }
                    }

                    std::swap(v0, v1);
                }
            }

            // Use a breadth-first search to propagate the distance
            // information.
            std::size_t nextDistance = 1;
            std::size_t numFrontVertices = currFront.size();
            std::copy(currFront.begin(), currFront.end(), mOrderedVertices.begin());
            while (currFront.size() > 0)
            {
                std::set<std::size_t> nextFront{};
                for (auto v : currFront)
                {
                    std::size_t range0 = mVertexGraph[v].range0;
                    std::size_t range1 = mVertexGraph[v].range1;
                    auto* current = &mVertexGraphData[range0];
                    for (std::size_t j = 0; j < range1; ++j, ++current)
                    {
                        std::size_t a = current->first;
                        if (mVertexGraph[a].distance == invalid)
                        {
                            mVertexGraph[a].distance = nextDistance;
                            nextFront.insert(a);
                        }
                    }
                }
                std::copy(nextFront.begin(), nextFront.end(), mOrderedVertices.begin() + numFrontVertices);
                numFrontVertices += nextFront.size();
                currFront = std::move(nextFront);
                ++nextDistance;
            }
        }

        void AssignBoundaryTextureCoordinatesSquare()
        {
            // Map the boundary of the mesh to the unit square [0,1]^2. The
            // selection of square vertices is such that the relative
            // distances between boundary vertices and the relative distances
            // between polygon vertices is preserved, except that the four
            // corners of the square are required to have boundary points
            // mapped to them. The first boundary point has an implied
            // distance of zero. The value distance[i] is the length of the
            // boundary polyline from vertex 0 to vertex i+1.
            std::vector<T> distance(mNumBoundaryEdges);
            T total = C_<T>(0);
            std::size_t v0 = mBoundaryStart, v1{}, i{};
            for (i = 0; i < mNumBoundaryEdges; ++i)
            {
                v1 = mVertexInfo[v0];
                total += Length(mVertices[v1] - mVertices[v0]);
                distance[i] = total;
                v0 = v1;
            }

            T invTotal = C_<T>(1) / total;
            for (auto& d : distance)
            {
                d *= invTotal;
            }

            auto begin = distance.begin(), end = distance.end();
            std::size_t endYMin = std::lower_bound(begin, end, C_<T>(1, 4)) - begin;
            std::size_t endXMax = std::lower_bound(begin, end, C_<T>(1, 2)) - begin;
            std::size_t endYMax = std::lower_bound(begin, end, C_<T>(3, 4)) - begin;
            std::size_t endXMin = distance.size() - 1;

            // The first polygon vertex is (0,0). The remaining vertices are
            // chosen counterclockwise around the square.
            v0 = mBoundaryStart;
            mTCoords[v0][0] = C_<T>(0);
            mTCoords[v0][1] = C_<T>(0);
            for (i = 0; i < endYMin; ++i)
            {
                v1 = mVertexInfo[v0];
                mTCoords[v1][0] = distance[i] * C_<T>(4);
                mTCoords[v1][1] = C_<T>(0);
                v0 = v1;
            }

            v1 = mVertexInfo[v0];
            mTCoords[v1][0] = C_<T>(1);
            mTCoords[v1][1] = C_<T>(0);
            v0 = v1;
            for (++i; i < endXMax; ++i)
            {
                v1 = mVertexInfo[v0];
                mTCoords[v1][0] = (T)1;
                mTCoords[v1][1] = distance[i] * C_<T>(4) - C_<T>(1);
                v0 = v1;
            }

            v1 = mVertexInfo[v0];
            mTCoords[v1][0] = C_<T>(1);
            mTCoords[v1][1] = C_<T>(1);
            v0 = v1;
            for (++i; i < endYMax; ++i)
            {
                v1 = mVertexInfo[v0];
                mTCoords[v1][0] = C_<T>(3) - distance[i] * C_<T>(4);
                mTCoords[v1][1] = C_<T>(1);
                v0 = v1;
            }

            v1 = mVertexInfo[v0];
            mTCoords[v1][0] = C_<T>(0);
            mTCoords[v1][1] = C_<T>(1);
            v0 = v1;
            for (++i; i < endXMin; ++i)
            {
                v1 = mVertexInfo[v0];
                mTCoords[v1][0] = C_<T>(0);
                mTCoords[v1][1] = C_<T>(4) - distance[i] * C_<T>(4);
                v0 = v1;
            }
        }

        void AssignBoundaryTextureCoordinatesDisk()
        {
            // Map the boundary of the mesh to a convex polygon. The selection
            // of convex polygon vertices is such that the relative distances
            // between boundary vertices and the relative distances between
            // polygon vertices is preserved. The first boundary point has an
            // implied distance of zero. The value distance[i] is the length
            // of the boundary polyline from vertex 0 to vertex i+1.
            std::vector<T> distance(mNumBoundaryEdges);
            T total = C_<T>(0);
            std::size_t v0 = mBoundaryStart;
            for (std::size_t i = 0; i < mNumBoundaryEdges; ++i)
            {
                std::size_t v1 = mVertexInfo[v0];
                total += Length(mVertices[v1] - mVertices[v0]);
                distance[i] = total;
                v0 = v1;
            }

            // The convex polygon lives in [0,1]^2 and inscribes a circle with
            // center (1/2,1/2) and radius 1/2. The polygon center is not
            // necessarily the circle center! This is the case when a
            // boundary edge has length larger than half the total length of
            // the boundary polyline; we do not expect such data for our
            // meshes. The first polygon vertex is (1/2,0). The remaining
            // vertices are chosen counterclockwise around the polygon.
            T multiplier = C_TWO_PI<T> / total;
            v0 = mBoundaryStart;
            mTCoords[v0][0] = C_<T>(1);
            mTCoords[v0][1] = C_<T>(1, 2);
            for (std::size_t i = 1, im1 = 0; i < mNumBoundaryEdges; ++i, ++im1)
            {
                std::size_t v1 = mVertexInfo[v0];
                T angle = multiplier * distance[im1];
                mTCoords[v1][0] = (std::cos(angle) + C_<T>(1)) * C_<T>(1, 2);
                mTCoords[v1][1] = (std::sin(angle) + C_<T>(1)) * C_<T>(1, 2);
                v0 = v1;
            }
        }

        void ComputeMeanValueWeights()
        {
            for (auto const& edge : mInteriorEdges)
            {
                std::size_t v0 = edge->V[0];
                std::size_t v1 = edge->V[1];
                for (std::size_t i = 0; i < 2; ++i)
                {
                    // Compute the direction from X0 to X1 and compute the
                    // length of the edge (X0,X1).
                    Vector3<T> X0 = mVertices[v0];
                    Vector3<T> X1 = mVertices[v1];
                    Vector3<T> X1mX0 = X1 - X0;
                    T x1mx0length = Normalize(X1mX0);
                    T weight{};
                    if (x1mx0length > C_<T>(0))
                    {
                        // Compute the weight for X0 associated with X1.
                        weight = C_<T>(0);
                        for (std::size_t j = 0; j < 2; ++j)
                        {
                            // Find the vertex of triangle T[j] opposite edge
                            // <X0,X1>.
                            auto tri = edge->T[j];
                            std::size_t k{};
                            for (k = 0; k < 3; ++k)
                            {
                                std::size_t v2 = tri->V[k];
                                if (v2 != v0 && v2 != v1)
                                {
                                    Vector3<T> X2 = mVertices[v2];
                                    Vector3<T> X2mX0 = X2 - X0;
                                    T x2mx0Length = Normalize(X2mX0);
                                    if (x2mx0Length > C_<T>(0))
                                    {
                                        T dot = Dot(X2mX0, X1mX0);
                                        T cs = std::min(std::max(dot, C_<T>(-1)), C_<T>(1));
                                        T angle = std::acos(cs);
                                        weight += std::tan(angle * C_<T>(1, 2));
                                    }
                                    else
                                    {
                                        weight += C_<T>(1);
                                    }
                                    break;
                                }
                            }
                        }
                        weight /= x1mx0length;
                    }
                    else
                    {
                        weight = C_<T>(1);
                    }

                    std::size_t range0 = mVertexGraph[v0].range0;
                    std::size_t range1 = mVertexGraph[v0].range1;
                    for (std::size_t j = 0; j < range1; ++j)
                    {
                        std::pair<std::size_t, T>& data = mVertexGraphData[range0 + j];
                        if (data.first == v1)
                        {
                            data.second = weight;
                        }
                    }

                    std::swap(v0, v1);
                }
            }
        }

        void SolveSystem(std::size_t numIterations)
        {
            // On the first pass, average only neighbors whose texture
            // coordinates have been computed. This is a good initial guess
            // for the linear system and leads to relatively fast convergence
            // of the Gauss-Seidel iterates.
            for (std::size_t i = mNumBoundaryEdges; i < mNumVertices; ++i)
            {
                std::size_t v0 = mOrderedVertices[i];
                std::size_t range0 = mVertexGraph[v0].range0;
                std::size_t range1 = mVertexGraph[v0].range1;
                auto const* current = &mVertexGraphData[range0];
                Vector2<T> tcoord{};  // zero vector
                T weight{}, weightSum = C_<T>(0);
                for (std::size_t j = 0; j < range1; ++j, ++current)
                {
                    std::size_t v1 = current->first;
                    if (mTCoords[v1][0] != C_<T>(-1))
                    {
                        weight = current->second;
                        weightSum += weight;
                        tcoord += weight * mTCoords[v1];
                    }
                }
                tcoord /= weightSum;
                mTCoords[v0] = tcoord;
            }

            SolveSystemInternal(numIterations);
        }

        void SolveSystemCPUSingle(std::size_t numIterations)
        {
            // Use ping-pong buffers for the texture coordinates.
            std::vector<Vector2<T>> tcoords(mNumVertices);
            for (std::size_t i = 0; i < mNumVertices; ++i)
            {
                tcoords[i] = mTCoords[i];
            }
            Vector2<T>* inTCoords = mTCoords;
            Vector2<T>* outTCoords = tcoords.data();

            // The value numIterations is even, so we always swap an even
            // number of times. This ensures that on exit from the loop,
            // outTCoords is tcoords.
            for (std::size_t i = 1; i <= numIterations; ++i)
            {
                if (mProgress)
                {
                    (*mProgress)(i);
                }

                for (std::size_t j = mNumBoundaryEdges; j < mNumVertices; ++j)
                {
                    std::size_t v0 = mOrderedVertices[j];
                    std::size_t range0 = mVertexGraph[v0].range0;
                    std::size_t range1 = mVertexGraph[v0].range1;
                    auto const* current = &mVertexGraphData[range0];
                    Vector2<T> tcoord{};  // zero vector
                    T weight{}, weightSum = C_<T>(0);
                    for (std::size_t k = 0; k < range1; ++k, ++current)
                    {
                        std::size_t v1 = current->first;
                        weight = current->second;
                        weightSum += weight;
                        tcoord += weight * inTCoords[v1];
                    }
                    tcoord /= weightSum;
                    outTCoords[v0] = tcoord;
                }

                std::swap(inTCoords, outTCoords);
            }
        }

        void SolveSystemCPUMultiple(std::size_t numIterations)
        {
            // Use ping-pong buffers for the texture coordinates.
            std::vector<Vector2<T>> tcoords(mNumVertices);
            for (std::size_t i = 0; i < mNumVertices; ++i)
            {
                tcoords[i] = mTCoords[i];
            }
            Vector2<T>* inTCoords = mTCoords;
            Vector2<T>* outTCoords = tcoords.data();

            // Partition the data for multiple threads.
            std::size_t numV = mNumVertices - mNumBoundaryEdges;
            std::size_t numVPerThread = numV / mNumThreads;
            std::vector<std::size_t> vmin(mNumThreads), vmax(mNumThreads);
            for (std::size_t t = 0; t < mNumThreads; ++t)
            {
                vmin[t] = mNumBoundaryEdges + t * numVPerThread;
                vmax[t] = vmin[t] + numVPerThread - 1;
            }
            vmax[mNumThreads - 1] = mNumVertices - 1;

            // The value numIterations is even, so we always swap an even
            // number of times. This ensures that on exit from the loop,
            // outTCoords is tcoords.
            for (std::size_t i = 1; i <= numIterations; ++i)
            {
                if (mProgress)
                {
                    (*mProgress)(i);
                }

                // Execute Gauss-Seidel iterations in multiple threads.
                std::vector<std::thread> process(mNumThreads);
                for (std::size_t t = 0; t < mNumThreads; ++t)
                {
                    process[t] = std::thread([this, t, &vmin, &vmax, inTCoords, outTCoords]()
                    {
                        for (std::size_t j = vmin[t]; j <= vmax[t]; ++j)
                        {
                            std::size_t v0 = mOrderedVertices[j];
                            std::size_t range0 = mVertexGraph[v0].range0;
                            std::size_t range1 = mVertexGraph[v0].range1;
                            auto const* current = &mVertexGraphData[range0];
                            Vector2<T> tcoord{};  // zero vector
                            T weight{}, weightSum = C_<T>(0);
                            for (std::size_t k = 0; k < range1; ++k, ++current)
                            {
                                std::size_t v1 = current->first;
                                weight = current->second;
                                weightSum += weight;
                                tcoord += weight * inTCoords[v1];
                            }
                            tcoord /= weightSum;
                            outTCoords[v0] = tcoord;
                        }
                    });
                }

                // Wait for all threads to finish.
                for (std::size_t t = 0; t < mNumThreads; ++t)
                {
                    process[t].join();
                }

                std::swap(inTCoords, outTCoords);
            }
        }

    private:
        friend class UnitTestGenerateMeshUV;
    };
}
