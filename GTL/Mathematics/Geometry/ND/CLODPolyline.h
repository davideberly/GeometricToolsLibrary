// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.15

#pragma once

// The continuous level of detail (CLOD) algorithm implemented here is
// described in
// https://www.geometrictools.com/Documentation/PolylineReduction.pdf
// It is an algorithm to reduce incrementally the number of vertices in a
// polyline (open or closed).  The sequence of vertex collapses is based on
// vertex weights associated with distance from vertices to polyline segments.

#include <GTL/Utility/MinHeap.h>
#include <GTL/Mathematics/Distance/ND/DistPointSegment.h>
#include <cstddef>
#include <limits>
#include <vector>

namespace gtl
{
    template <typename T, std::size_t N>
    class CLODPolyline
    {
    public:
        // The number of vertices must be 2 or larger. The vertices are
        // assumed to be ordered. The segments are <V[i],V[i+1]> for
        // 0 <= i <= numVertices-2 for an open polyline. If the polyline is
        // closed, an additional segment is <V[numVertices-1],V[0]>.
        CLODPolyline(std::vector<Vector<T, N>> const& vertices, bool closed)
            :
            mNumVertices(vertices.size()),
            mVertices(vertices),
            mClosed(closed),
            mNumEdges(0),
            mVMin(mClosed ? 3 : 2),
            mVMax(mNumVertices),
            mIndices{}
        {
            GTL_ARGUMENT_ASSERT(
                closed ? mNumVertices >= 3 : mNumVertices >= 2,
                "Invalid inputs.");

            // Compute the sequence of vertex collapses. The polyline starts
            // out at the full level of detail (mNumVertices equals mVMax).
            Collapse(mVertices, mClosed, mIndices, mNumEdges, mEdges);
        }

        ~CLODPolyline() = default;

        // Member access.
        inline std::size_t GetNumVertices() const
        {
            return mNumVertices;
        }

        inline std::vector<Vector<T, N>> const& GetVertices() const
        {
            return mVertices;
        }

        inline bool GetClosed() const
        {
            return mClosed;
        }

        inline std::size_t GetNumEdges() const
        {
            return mNumEdges;
        }

        inline std::vector<std::size_t> const& GetEdges() const
        {
            return mEdges;
        }

        // Accessors to level of detail (MinLOD <= LOD <= MaxLOD is required).
        inline std::size_t GetMinLevelOfDetail() const
        {
            return mVMin;
        }

        inline std::size_t GetMaxLevelOfDetail() const
        {
            return mVMax;
        }

        inline std::size_t GetLevelOfDetail() const
        {
            return mNumVertices;
        }

        void SetLevelOfDetail(std::size_t numVertices)
        {
            if (numVertices < mVMin || numVertices > mVMax)
            {
                return;
            }

            // Decrease the level of detail.
            while (mNumVertices > numVertices)
            {
                --mNumVertices;
                mEdges[mIndices[mNumVertices]] = mEdges[2 * mNumEdges - 1];
                --mNumEdges;
            }

            // Increase the level of detail.
            while (mNumVertices < numVertices)
            {
                ++mNumEdges;
                mEdges[mIndices[mNumVertices]] = mNumVertices;
                ++mNumVertices;
            }
        }

    private:
        // Support for vertex collapses in a polyline.
        static void Collapse(std::vector<Vector<T, N>>& vertices,
            bool closed, std::vector<std::size_t>& indices, std::size_t& numEdges,
            std::vector<std::size_t>& edges)
        {
            std::size_t numVertices = vertices.size();
            indices.resize(numVertices);

            if (closed)
            {
                numEdges = numVertices;
                edges.resize(2 * numEdges);

                if (numVertices == 3)
                {
                    indices[0] = 0;
                    indices[1] = 1;
                    indices[2] = 3;
                    edges[0] = 0;
                    edges[1] = 1;
                    edges[2] = 1;
                    edges[3] = 2;
                    edges[4] = 2;
                    edges[5] = 0;
                    return;
                }
            }
            else
            {
                numEdges = numVertices - 1;
                edges.resize(2 * numEdges);

                if (numVertices == 2)
                {
                    indices[0] = 0;
                    indices[1] = 1;
                    edges[0] = 0;
                    edges[1] = 1;
                    return;
                }
            }

            // Create the heap of weights.
            MinHeap<T> heap(numVertices);
            std::size_t qm1 = numVertices - 1;
            if (closed)
            {
                std::size_t qm2 = numVertices - 2;
                heap.Insert(0, GetWeight(qm1, 0, 1, vertices));
                heap.Insert(qm1, GetWeight(qm2, qm1, 0, vertices));
            }
            else
            {
                heap.Insert(0, std::numeric_limits<T>::max());
                heap.Insert(qm1, std::numeric_limits<T>::max());
            }
            for (std::size_t m = 0, z = 1, p = 2; z < qm1; ++m, ++z, ++p)
            {
                heap.Insert(z, GetWeight(m, z, p, vertices));
            }

            // Create the level of detail information for the polyline.
            std::vector<std::size_t> collapses(numVertices);
            CollapseVertices(heap, numVertices, collapses);
            ComputeEdges(numVertices, closed, collapses, indices, numEdges, edges);
            ReorderVertices(numVertices, vertices, collapses, numEdges, edges);
        }

        // Weight calculation for vertex triple <V[m],V[z],V[p]>.
        static T GetWeight(std::size_t m, std::size_t z, std::size_t p,
            std::vector<Vector<T, N>>& vertices)
        {
            Vector<T, N> direction = vertices[p] - vertices[m];
            T length = Normalize(direction);
            if (length > C_<T>(0))
            {
                Segment<T, N> segment(vertices[m], vertices[p]);
                DCPQuery<T, Vector<T, N>, Segment<T, N>> query{};
                T distance = query(vertices[z], segment).distance;
                return distance / length;
            }
            else
            {
                return std::numeric_limits<T>::max();
            }
        }

        // Create data structures for the polyline.
        static void CollapseVertices(MinHeap<T>& heap, std::size_t numVertices,
            std::vector<std::size_t>& collapses)
        {
            T weight{};
            for (std::size_t k = 0, i = numVertices - 1; k < numVertices; ++k, --i)
            {
                heap.Remove(collapses[i], weight);
            }
        }

        static void ComputeEdges(std::size_t numVertices, bool closed,
            std::vector<std::size_t>& collapses, std::vector<std::size_t>& indices,
            std::size_t numEdges, std::vector<std::size_t>& edges)
        {
            // Compute the edges (first to collapse is last in array). Do not
            // collapse the last line segment of an open polyline. Do not
            // collapse further when a closed polyline becomes a triangle.
            std::size_t vIndex{}, eIndex = 2 * numEdges - 1;
            if (closed)
            {
                for (std::size_t k = 0, i = numVertices - 1; k < numVertices; ++k, --i)
                {
                    vIndex = collapses[i];
                    edges[eIndex--] = (vIndex + 1) % numVertices;
                    edges[eIndex--] = vIndex;
                }
            }
            else
            {
                for (std::size_t i = numVertices - 1; i >= 2; --i)
                {
                    vIndex = collapses[i];
                    edges[eIndex--] = vIndex + 1;
                    edges[eIndex--] = vIndex;
                }

                vIndex = collapses[0];
                edges[0] = vIndex;
                edges[1] = vIndex + 1;
            }

            // In the given edge order, find the index in the edge array
            // that corresponds to a collapse vertex and save the index
            // for the dynamic change in level of detail. This relies on
            // the assumption that a vertex is shared by at most two
            // edges.
            eIndex = 2 * numEdges - 1;
            for (std::size_t k = 0, i = numVertices - 1; k < numVertices; ++k, --i)
            {
                vIndex = collapses[i];
                for (std::size_t e = 0; e < 2 * numEdges; ++e)
                {
                    if (vIndex == edges[e])
                    {
                        indices[i] = e;
                        edges[e] = edges[eIndex];
                        break;
                    }
                }
                eIndex -= 2;

                if (closed)
                {
                    if (eIndex == 5)
                    {
                        break;
                    }
                }
                else
                {
                    if (eIndex == 1)
                    {
                        break;
                    }
                }
            }

            // Restore the edge array to full level of detail.
            if (closed)
            {
                for (std::size_t i = 3; i < numVertices; ++i)
                {
                    edges[indices[i]] = collapses[i];
                }
            }
            else
            {
                for (std::size_t i = 2; i < numVertices; ++i)
                {
                    edges[indices[i]] = collapses[i];
                }
            }
        }

        static void ReorderVertices(std::size_t numVertices, std::vector<Vector<T, N>>& vertices,
            std::vector<std::size_t>& collapses, std::size_t numEdges, std::vector<std::size_t>& edges)
        {
            std::vector<std::size_t> permute(numVertices);
            std::vector<Vector<T, N>> permutedVertex(numVertices);

            for (std::size_t i = 0; i < numVertices; ++i)
            {
                std::size_t vIndex = collapses[i];
                permute[vIndex] = i;
                permutedVertex[i] = vertices[vIndex];
            }

            for (std::size_t i = 0; i < 2 * numEdges; ++i)
            {
                edges[i] = permute[edges[i]];
            }

            vertices = permutedVertex;
        }

        // The polyline vertices.
        std::size_t mNumVertices;
        std::vector<Vector<T, N>> mVertices;
        bool mClosed;

        // The polyline edges.
        std::size_t mNumEdges;
        std::vector<std::size_t> mEdges;

        // The level of detail information.
        std::size_t mVMin, mVMax;
        std::vector<std::size_t> mIndices;

    private:
        friend class UnitTestCLODPolyline;
    };
}
