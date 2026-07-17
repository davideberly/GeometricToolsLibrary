// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#pragma once

// Read the comments in BVTree.h regarding tree construction. Although this
// class appears to be non-abstract, the BoundingVolume type has requirements
// for its interface. In this sense, BVTreeOfTriangles is abstract.

#include <GTL/Mathematics/Geometry/3D/BVTree.h>
#include <GTL/Mathematics/Intersection/3D/IntrLine3Triangle3.h>
#include <GTL/Mathematics/Intersection/3D/IntrRay3Triangle3.h>
#include <GTL/Mathematics/Intersection/3D/IntrSegment3Triangle3.h>
#include <array>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <set>
#include <vector>

namespace gtl
{
    // To support the operations in BVTree<T,BoundingVolume>, the interface
    // of class BoundingVolume must include
    // 
    //   BoundingVolume();
    //   ~BoundingVolume();
    //   void GetSplittingAxis(Vector3<T>& origin, Vector3<T>& direction);
    // 
    // To support the operations in BVTreeOfTriangles<T,BoundingVolume>, the
    // interface of class BoundingVolume must include also
    // 
    //   static bool IntersectLine(Vector3<T> const& P, Vector3<T> const& Q,
    //       BoundingVolume& boundingVolume);
    // 
    //   static bool IntersectRay(Vector3<T> const& P, Vector3<T> const& Q,
    //       BoundingVolume& boundingVolume);
    // 
    //   static bool IntersectSegment(Vector3<T> const& P, Vector3<T> const& Q,
    //       BoundingVolume& boundingVolume);
    //
    // The line is parameterized by P+t*Q for all real t. The ray is
    // parameterized by P+t*Q for nonnegative t. The segment is parameterized
    // by (1-t)*P+t*Q for t in [0,1].

    template <typename T, typename BoundingVolume>
    class BVTreeOfTriangles : public BVTree<T, BoundingVolume>
    {
    public:
        BVTreeOfTriangles()
            :
            BVTree<T, BoundingVolume>{},
            mVertices{},
            mTriangles{}
        {
        }

        // The input height specifies the desired height of the tree and must
        // be no larger than 31. If std::numeric_limits<std::size_t>::max(), the
        // the entire tree is built and the actual height is computed from
        // vertices.size(). If larger than 31, the height is clamped to 31.
        void Create(
            std::vector<Vector3<T>> const& vertices,
            std::vector<std::array<std::size_t, 3>> const& triangles,
            std::size_t height = std::numeric_limits<std::size_t>::max())
        {
            GTL_ARGUMENT_ASSERT(
                vertices.size() >= 3 && triangles.size() > 0,
                "Expecting at least 3 vertices and at least 1 triangle.");

            mVertices = vertices;
            mTriangles = triangles;

            // Compute the triangle centroids.
            std::vector<Vector3<T>> centroids(mTriangles.size());
            T const three = C_<T>(3);
            for (std::size_t t = 0; t < mTriangles.size(); ++t)
            {
                auto const& tri = mTriangles[t];
                centroids[t] = (mVertices[tri[0]] + mVertices[tri[1]] + mVertices[tri[2]]) / three;
            }

            // Create the bounding volume tree for centroids.
            BVTree<T, BoundingVolume>::Create(std::move(centroids), height);
        }

        // Member access.
        inline std::vector<Vector3<T>> const& GetVertices() const
        {
            return mVertices;
        }

        inline std::vector<std::array<std::size_t, 3>> const& GetTriangles() const
        {
            return mTriangles;
        }

        // Generate a list of triangles intersected by a linear component
        // (line, ray or segment). The line is parameterized by P + t * Q,
        // where Q is a unit-length direction and t is any real number. The
        // ray is parameterized by P + t * Q, where Q is a unit-length
        // direction and t >= 0. The segment is parameterized by
        // (1-t) * P + t * Q = P + t * (Q - P), where P and Q are the
        // endpoints of the segment and 0 <= t <= 1.
        static std::uint32_t constexpr LINE_QUERY = 0;
        static std::uint32_t constexpr RAY_QUERY = 1;
        static std::uint32_t constexpr SEGMENT_QUERY = 2;

        struct Intersection
        {
            Intersection()
                :
                triangleIndex(std::numeric_limits<std::size_t>::max()),
                point{},
                parameter(C_<T>(0))
            {
            }

            Intersection(std::size_t inTriangleIndex, Vector3<T> const& inPoint, T const& inParameter)
                :
                triangleIndex(inTriangleIndex),
                point(inPoint),
                parameter(inParameter)
            {
            }

            bool operator<(Intersection const& other) const
            {
                return parameter < other.parameter;
            }

            std::size_t triangleIndex;
            Vector3<T> point;
            T parameter;
        };

        // Compute intersections of the linear component and triangles. These
        // are sorted by the parameter of the linear component.
        void Execute(std::uint32_t queryType, Vector3<T> const& P, Vector3<T> const& Q,
            std::set<Intersection>& intersections)
        {
            std::size_t constexpr invalid = std::numeric_limits<std::size_t>::max();
            intersections.clear();

            BoundingVolumeQuery IntersectBoundingVolume{};
            TriangleQuery IntersectTriangle{};
            switch (queryType)
            {
            case LINE_QUERY:
                IntersectBoundingVolume = BoundingVolume::IntersectLine;
                IntersectTriangle = IntersectLineTriangle;
                break;
            case RAY_QUERY:
                IntersectBoundingVolume = BoundingVolume::IntersectRay;
                IntersectTriangle = IntersectRayTriangle;
                break;
            case SEGMENT_QUERY:
                IntersectBoundingVolume = BoundingVolume::IntersectSegment;
                IntersectTriangle = IntersectSegmentTriangle;
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
                        // The linear component does not intersect the box.
                        // There are no triangles intersected in the subtree
                        // rooted at this node. Do not continue the
                        // intersection search to child nodes if they exist.
                    }
                }
                else // node.leftChild == invalid && node.rightChild == invalid
                {
                    Vector3<T> point{};
                    T parameter{};
                    for (std::size_t i = node.minIndex; i <= node.maxIndex; ++i)
                    {
                        std::size_t triangleIndex = this->mPartition[i];
                        auto const& tri = mTriangles[triangleIndex];
                        Triangle3<T> triangle(mVertices[tri[0]], mVertices[tri[1]], mVertices[tri[2]]);
                        if (IntersectTriangle(P, Q, triangle, point, parameter))
                        {
                            intersections.insert(Intersection(triangleIndex, point, parameter));
                        }
                    }
                }
            }
        }

    protected:
        // Function signature for {line,ray,segment}-boundingVolume
        // test-intersection queries.
        using BoundingVolumeQuery = bool (*)(Vector3<T> const&,
            Vector3<T> const&, BoundingVolume const&);

        // Function signature for {line,ray,segment}-triangle
        // find-intersection queries.
        using TriangleQuery = bool (*)(Vector3<T> const&, Vector3<T> const&,
            Triangle3<T> const&, Vector3<T>&, T&);

        static bool IntersectLineTriangle(Vector3<T> const& P, Vector3<T> const& Q,
            Triangle3<T> const& triangle, Vector3<T>& point, T& parameter)
        {
            FIQuery<T, Line3<T>, Triangle3<T>> query{};
            auto output = query(Line3<T>(P, Q), triangle);
            point = output.point[0];
            parameter = output.parameter[0];
            return output.intersect;
        }

        static bool IntersectRayTriangle(Vector3<T> const& P, Vector3<T> const& Q,
            Triangle3<T> const& triangle, Vector3<T>& point, T& parameter)
        {
            FIQuery<T, Ray3<T>, Triangle3<T>> query{};
            auto output = query(Ray3<T>(P, Q), triangle);
            point = output.point[0];
            parameter = output.parameter[0];
            return output.intersect;
        }

        static bool IntersectSegmentTriangle(Vector3<T> const& P, Vector3<T> const& Q,
            Triangle3<T> const& triangle, Vector3<T>& point, T& parameter)
        {
            FIQuery<T, Segment3<T>, Triangle3<T>> query{};
            auto output = query(Segment3<T>(P, Q), triangle);
            point = output.point[0];
            parameter = output.parameter[0];
            return output.intersect;
        }

        std::vector<Vector3<T>> mVertices;
        std::vector<std::array<std::size_t, 3>> mTriangles;

    private:
        friend class UnitTestBVTreeOfTriangles;
    };
}
