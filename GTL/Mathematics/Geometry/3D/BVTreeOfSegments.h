// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.19

#pragma once

// Read the comments in BVTree.h regarding tree construction. Although this
// class appears to be non-abstract, the BoundingVolume type has requirements
// for its interface. In this sense, BVTreeOfSegments is abstract.

#include <GTL/Mathematics/Geometry/3D/BVTree.h>

namespace gtl
{
    template <typename T, typename BoundingVolume>
    class BVTreeOfSegments : public BVTree<T, BoundingVolume>
    {
    public:
        BVTreeOfSegments()
            :
            BVTree<T, BoundingVolume>{},
            mVertices{},
            mSegments{}
        {
        }

        // The input height specifies the desired height of the tree and must
        // be no larger than 31. If std::numeric_limits<std::size_t>::max(), the
        // the entire tree is built and the actual height is computed from
        // vertices.size(). If larger than 31, the height is clamped to 31.
        void Create(
            std::vector<Vector3<T>> const& vertices,
            std::vector<std::array<size_t, 2>> const& segments,
            std::size_t height = std::numeric_limits<std::size_t>::max())
        {
            GTL_ARGUMENT_ASSERT(
                vertices.size() > 0,
                "Expecting vertices to create a bounding volume tree.");

            mVertices = vertices;
            mSegments = segments;

            // Compute the segment centroids.
            std::vector<Vector3<T>> centroids(mSegments.size());
            T const half = C_<T>(1, 2);
            for (std::size_t i = 0; i < mSegments.size(); ++i)
            {
                auto const& seg = mSegments[i];
                centroids[i] = half * (mVertices[seg[0]] + mVertices[seg[1]]);
            }

            // Create the bounding volume tree for centroids.
            BVTree<T, BoundingVolume>::Create(std::move(centroids), height);
        }

        // Member access.
        inline std::vector<Vector3<T>> const& GetVertices() const
        {
            return mVertices;
        }

        inline std::vector<std::array<size_t, 2>> const& GetSegments() const
        {
            return mSegments;
        }

        // Generate a list of leaf nodes intersected by a linear component
        // (line, ray or segment). The line is parameterized by P + t * Q,
        // where Q is a unit-length direction and t is any real number. The
        // ray is parameterized by P + t * Q, where Q is a unit-length
        // direction and t >= 0. The segment is parameterized by
        // (1-t) * P + t * Q = P + t * (Q - P), where P and Q are the
        // endpoints of the segment and 0 <= t <= 1.
        static std::uint32_t constexpr LINE_QUERY = 0;
        static std::uint32_t constexpr RAY_QUERY = 1;
        static std::uint32_t constexpr SEGMENT_QUERY = 2;

        // Compute intersections of the linear component and leaf nodes. The
        // indices[] are lookups into the mNodes[] member of the base class.
        // The nodeIndices are ordered according to the depth-first traversal
        // of the tree.
        void Execute(std::uint32_t queryType, Vector3<T> const& P, Vector3<T> const& Q,
            std::vector<std::size_t>& nodeIndices)
        {
            std::size_t constexpr invalid = std::numeric_limits<std::size_t>::max();
            nodeIndices.clear();

            BoundingVolumeQuery IntersectBoundingVolume{};
            switch (queryType)
            {
            case LINE_QUERY:
                IntersectBoundingVolume = BoundingVolume::IntersectLine;
                break;
            case RAY_QUERY:
                IntersectBoundingVolume = BoundingVolume::IntersectRay;
                break;
            case SEGMENT_QUERY:
                IntersectBoundingVolume = BoundingVolume::IntersectSegment;
                break;
            default:
                GTL_ARGUMENT_ERROR(
                    "Invalid query type.");
            }

            std::vector<std::size_t> indexStack(2 * this->mHeight);
            std::size_t top = 0;
            indexStack[0] = 0;
            while (top != std::numeric_limits<std::size_t>::max())
            {
                std::size_t nodeIndex = indexStack[top--];
                auto const& node = this->mNodes[nodeIndex];

                // For the balanced tree created by BVTree<T>, an interior
                // node has two valid children and a leaf node has two invalid
                // children. This is true even if the height passed to
                // BVTree<T>::Create is smaller than the actual height.
                if (node.leftChild != invalid && node.rightChild != invalid)
                {
                    // The node is interior.
                    if (IntersectBoundingVolume(P, Q, node.boundingVolume))
                    {
                        // The linear component intersects the box. Continue
                        // the intersection search to child nodes if they
                        // exist.
                        indexStack[++top] = node.rightChild;
                        indexStack[++top] = node.leftChild;
                    }
                    else
                    {
                        // The linear component does not intersect the box. Do
                        // not continue the intersection search to child nodes
                        // if they exist.
                    }
                }
                else // node.leftChild == invalid && node.rightChild == invalid
                {
                    nodeIndices.push_back(nodeIndex);
                }
            }
        }

    protected:
        // Function signature for {line,ray,segment}-boundingVolume
        // test-intersection queries.
        using BoundingVolumeQuery = bool (*)(Vector3<T> const&,
            Vector3<T> const&, BoundingVolume const&);

        std::vector<Vector3<T>> mVertices;
        std::vector<std::array<size_t, 2>> mSegments;

    private:
        friend class UnitTestBVTreeOfSegments;
    };
}
