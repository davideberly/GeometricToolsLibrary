// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.28

#pragma once

// Compute the Delaunay triangulation of 2D points using an incremental
// insertion algorithm. The only way to ensure a correct result for the
// input points is to use an exact predicate for computing signs of
// various expressions. The implementation uses interval arithmetic and
// rational arithmetic for the predicate. The input type T must satisfy
// std::is_floating_point<T>::value = true.

#include <GTL/Mathematics/Geometry/2D/ExactColinear2.h>
#include <GTL/Mathematics/Geometry/2D/ExactToCircumcircle2.h>
#include <GTL/Mathematics/Geometry/2D/ExactToLine2.h>
#include <GTL/Mathematics/Meshes/VETManifoldMeshKS.h>
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <iterator>
#include <limits>
#include <memory>
#include <numeric>
#include <queue>
#include <type_traits>
#include <vector>

namespace gtl
{
    template <typename T>
    class Delaunay2
    {
    public:
        // The VETManifoldMeshKS mesh object stores adjacency information
        // for the vertices of the Delaunay triangle mesh. The information for
        // each vertex is stored in a std::vector, and the number of elements
        // of the std::vector must be specified. The default initial size is
        // the following constant. During runtime, if the std::vector reaches
        // its capacity, it must be resized. Each resize adds a chunk with
        // the size specified by the following constant.
        static constexpr std::size_t defaultAdjacentGrowth = 16;

        Delaunay2(std::size_t adjacentGrowth = defaultAdjacentGrowth)
            :
            mNumPoints(0),
            mPoints(nullptr),
            mEquivalentTo{},
            mRPoints{},
            mConverted{},
            mECOQuery{},
            mETCQuery{},
            mETLQuery{},
            mDimension(0),
            mVertices{},
            mIndices{},
            mMesh{},
            mAdjacentGrowth(adjacentGrowth > 0 ? adjacentGrowth : defaultAdjacentGrowth),
            mHull{},
            mCandidates{},
            mNumCandidates(0),
            mVisible{},
            mNumVisible(0),
            mBoundary{},
            mNumBoundary(0)
        {
            static_assert(
                std::is_floating_point<T>::value,
                "The input type must be a floating-point type.");
        }

        virtual ~Delaunay2() = default;

        // Compute an exact Delaunay triangulation using a blend of interval
        // arithmetic and rational arithmetic. Call GetDimension() to know
        // the dimension of the unique input points. If dimension is 0, the
        // input consists of 1 single point. If dimension is 1, the input
        // points lie on a line; the output of the triangulationn is a sorted
        // list of points (the indices are sorted). If dimension is 2, the
        // input points lie on a plane and are not colinear.
        void operator()(std::size_t numPoints, Vector2<T> const* points)
        {
            GTL_ARGUMENT_ASSERT(
                numPoints > 0 && points != nullptr,
                "Invalid argument.");

            // Provide access to the points for the member functions.
            mNumPoints = numPoints;
            mPoints = points;
            mMesh.Reset(mNumPoints, mAdjacentGrowth, 0);

            // The triangulation has V-2 triangles, so the worst-case number
            // of edges is smaller than 3*(V-2).
            mCandidates.resize(numPoints);
            mNumCandidates = 0;
            mVisible.resize(numPoints);
            mNumVisible = 0;
            mBoundary.resize(3 * mNumPoints);
            mNumBoundary = 0;

            // Allocate storage for any rational points that must be computed
            // in the exact sign predicates.
            mRPoints.resize(numPoints);
            mConverted.resize(numPoints);
            std::fill(mConverted.begin(), mConverted.end(), 0);

            // Store the convex hull vertices for use in point insertions.
            mHull = std::make_unique<Hull>(numPoints);

            // Sort all the points indirectly.
            auto lessThanPoints = [this](std::size_t s0, std::size_t s1)
            {
                return mPoints[s0] < mPoints[s1];
            };

            auto equalPoints = [this](std::size_t s0, std::size_t s1)
            {
                return mPoints[s0] == mPoints[s1];
            };

            std::vector<std::size_t> sorted(numPoints);
            mEquivalentTo.resize(numPoints);
            std::iota(sorted.begin(), sorted.end(), 0);
            std::iota(mEquivalentTo.begin(), mEquivalentTo.end(), 0);
            std::stable_sort(sorted.begin(), sorted.end(), lessThanPoints);

            // Eliminate duplicates but keep track of equivalence classes
            // of points that are duplicated. This information is used in
            // ConstrainedDelaunay2 but can be used in other applications
            // that require the information. The algorithm used here is a
            // modification of the one specified at
            //   https://en.cppreference.com/w/cpp/algorithm/unique
            // for a possible implementation of std::unique.
            std::vector<std::size_t>::iterator first = sorted.begin();
            std::vector<std::size_t>::iterator last = sorted.end();
            std::vector<std::size_t>::iterator current = first;
            while (++first != last)
            {
                if (equalPoints(*current, *first))
                {
                    mEquivalentTo[*first] = *current;
                }
                else
                {
                    if (++current != first)
                    {
                        *current = std::move(*first);
                    }
                }
            }
            std::vector<std::size_t>::iterator newEnd = ++current;
            sorted.erase(newEnd, sorted.end());

            ComputeTriangulation(sorted);
        }

        void operator()(std::vector<Vector2<T>> const& points)
        {
            return operator()(points.size(), points.data());
        }

        // The dimension is 0 (the input is a single point), 1 (the input
        // is a set of colinear points) or 2 (the triangulation exists).
        inline std::size_t GetDimension() const
        {
            return mDimension;
        }

        // Get the indices into the input 'points[]' that correspond to unique
        // output vertices.
        inline std::vector<std::size_t> const& GetVertices() const
        {
            return mVertices;
        }

        // Get the indices into the input 'points[]'. The returned array is
        // organized according to the dimension.
        //   0: The input is a single point. The returned array has size 1
        //      with index corresponding to that point.
        //   1: The input is a set of N colinear points. The returned array
        //      has size N and represents a contiguous set of line segments.
        //      The i-th line segment has 2D endpoints points[indices[2*i]]
        //      and points[indices[2*i+1]].
        //   2: The input is fully planar. The returned array has T triples
        //      of indices, each triple corresponding to a triangle in the
        //      triangulation. The triangle vertices are counterclockwise
        //      ordered.
        inline std::vector<std::size_t> const& GetIndices() const
        {
            return mIndices;
        }

        // Get the mesh, which is valid only when the dimension is 2.
        inline VETManifoldMeshKS const& GetMesh() const
        {
            return mMesh;
        }

    protected:
        // Supporting type for rational arithmetic used in the exact
        // predicate for the relationship between a point and a line.
        using Rational = BSNumber<UIntegerFP32<2>>;

        // Memoized access to the rational representation of the points.
        Vector2<Rational> const& GetRPoint(std::size_t index)
        {
            if (mConverted[index] == 0)
            {
                mConverted[index] = 1;
                for (std::size_t i = 0; i < 2; ++i)
                {
                    mRPoints[index][i] = mPoints[index][i];
                }
            }
            return mRPoints[index];
        }

        // Given a line with origin V0 and direction <V0,V1> and a query
        // point P, ToLine returns
        //   +1, P on right of line
        //   -1, P on left of line
        //    0, P on the line
        std::int32_t ToLine(std::size_t p, std::size_t v0, std::size_t v1)
        {
            auto const& P = mPoints[p];
            auto const& V0 = mPoints[v0];
            auto const& V1 = mPoints[v1];

            auto GetRPoints = [this, &p, &v0, &v1]()
            {
                return std::array<Vector2<Rational> const*, 3>
                {
                    &GetRPoint(p),
                    &GetRPoint(v0),
                    &GetRPoint(v1)
                };
            };

            return mETLQuery(P, V0, V1, GetRPoints);
        }

        // The input points to operator().
        std::size_t mNumPoints;
        Vector2<T> const* mPoints;

        // Lookup information to map a nonunique point in an equivalence
        // class to a representative of that class.
        std::vector<std::size_t> mEquivalentTo;

        // A blend of interval arithmetic and exact arithmetic is used to
        // ensure correctness. The rational representations are computed on
        // demand and are memoized. The mConverted array has elements 0 or 1,
        // where initially the values are 0. The first time mRPoints[i] is
        // encountered, mConverted[i] is 0. The floating-point vector
        // mPoints[i] is converted to a rational vector, after which
        // mConverted[i] is set to 1 to avoid converting again if the
        // floating-point vector is encountered in another predicate
        // computation.
        std::vector<Vector2<Rational>> mRPoints;
        std::vector<std::uint32_t> mConverted;

        // Support for exact predicates.
        ExactColinear2<T> mECOQuery;
        ExactToCircumcircle2<T> mETCQuery;
        ExactToLine2<T> mETLQuery;

        // The output data.
        std::size_t mDimension;
        std::vector<std::size_t> mVertices;
        std::vector<std::size_t> mIndices;
        VETManifoldMeshKS mMesh;
        std::size_t mAdjacentGrowth;

        // The convex hull for the those points already processed by
        // Update(...). It is empty when the Delaunay triangulation is a
        // single point or a union of contiguous and colinear line segments.
        // The hull representation uses a circular queue.
        class Hull
        {
        public:
            Hull(std::size_t size)
                :
                mNumActiveVertices(0),
                mFirst(0),
                mVertices(size),
                mNextAvailable(size - 1),
                mFree(size)
            {
                GTL_ARGUMENT_ASSERT(
                    size >= 3,
                    "Invalid hull capacity.");

                for (std::size_t i = 0, ri = size - 1; i < size; ++i, --ri)
                {
                    mFree[i] = ri;
                }
            }

            void Initialize(std::vector<std::size_t> const& colinear, std::size_t last)
            {
                std::size_t const numColinear = colinear.size();
                GTL_ARGUMENT_ASSERT(
                    numColinear >= 2,
                    "Invalid initial hull.");

                std::size_t curr = 0, prev = numColinear, next = 1, rcurr = numColinear - 1;
                while (curr < numColinear)
                {
                    auto& vertex = mVertices[curr];
                    vertex.v = colinear[rcurr];
                    vertex.prev = prev;
                    vertex.next = next;
                    prev = curr;
                    curr = next++;
                    --rcurr;
                }
                auto& vertex = mVertices[numColinear];
                vertex.v = last;
                vertex.prev = prev;
                vertex.next = 0;

                mNumActiveVertices = numColinear + 1;
                mFirst = numColinear;
                mNextAvailable = mVertices.size() - 1;
                for (std::size_t i = 0; i < mNumActiveVertices; ++i)
                {
                    mFree[mNextAvailable--] = invalid;
                }
            }

            void GetVisible(std::function<bool(std::size_t, std::size_t, std::size_t)> const& IsVisible,
                std::size_t p, std::size_t& iFirstVisible, std::size_t& iLastVisible,
                std::vector<EdgeKey<true>>& visibleEdges) const
            {
                std::size_t const jSup = GetCapacity();
                std::size_t const iFirst = GetFirst();
                std::size_t const vFirst = GetVertex(iFirst);
                std::size_t iCurr = iFirst;
                std::size_t vCurr = vFirst;
                for (std::size_t j = 0; j < jSup; ++j)
                {
                    std::size_t iPrev = GetPrev(iCurr);
                    std::size_t vPrev = GetVertex(iPrev);
                    if (IsVisible(p, vPrev, vCurr))
                    {
                        visibleEdges.push_back(EdgeKey<true>(vPrev, vCurr));
                        iCurr = iPrev;
                        vCurr = vPrev;
                    }
                    else
                    {
                        break;
                    }
                }
                iFirstVisible = iCurr;

                iCurr = iFirst;
                vCurr = vFirst;
                for (std::size_t j = 0; j < jSup; ++j)
                {
                    std::size_t iNext = GetNext(iCurr);
                    std::size_t vNext = GetVertex(iNext);
                    if (IsVisible(p, vCurr, vNext))
                    {
                        visibleEdges.push_back(EdgeKey<true>(vCurr, vNext));
                        iCurr = iNext;
                        vCurr = vNext;
                    }
                    else
                    {
                        break;
                    }
                }
                iLastVisible = iCurr;
            }

            void Update(std::size_t p, std::size_t iFirstVisible, std::size_t iLastVisible)
            {
                GTL_ARGUMENT_ASSERT(
                    iFirstVisible != iLastVisible,
                    "At least two vertices must be visible to p.");

                std::size_t i0 = GetNext(iFirstVisible);
                std::size_t i1 = GetPrev(iLastVisible);
                if (i0 == iLastVisible && i1 == iFirstVisible)
                {
                    // A single hull edge <v0,v1> is visible to p. Remove
                    // this edge and insert two edges <v0,p> and <p,v1>.
                    mFirst = mFree[mNextAvailable];
                    mFree[mNextAvailable--] = invalid;
                    auto& vFirst = mVertices[iFirstVisible];
                    vFirst.next = mFirst;
                    auto& vLast = mVertices[iLastVisible];
                    vLast.prev = mFirst;
                    auto& pNode = mVertices[mFirst];
                    pNode.v = p;
                    pNode.prev = iFirstVisible;
                    pNode.next = iLastVisible;
                    ++mNumActiveVertices;
                }
                else if (i0 != i1)
                {
                    // Memory management is needed. Remove the visible
                    // vertices strictly between the first and last visible
                    // vertices. Use the last visible vertex node to store
                    // the vertex p.
                    auto& vFirst = mVertices[iFirstVisible];
                    vFirst.next = i1;
                    auto& vLast = mVertices[i1];
                    vLast.v = p;
                    vLast.prev = iFirstVisible;
                    mFirst = i1;

                    // Move the remaining visible vertex nodes to the free
                    // nodes.
                    while (i0 != i1)
                    {
                        auto& vertex = mVertices[i0];
                        mFree[++mNextAvailable] = i0;
                        i0 = vertex.next;
                        vertex.v = invalid;
                        vertex.prev = invalid;
                        vertex.next = invalid;
                        --mNumActiveVertices;
                    }
                }
                else
                {
                    // Two hull edges <v0,v1> and <v1,v2> are visible to p.
                    // Replace them by <v0,p> and <p,v2>. No memory management
                    // is needed.
                    mVertices[i0].v = p;
                    mFirst = i0;
                }
            }

            inline std::size_t GetCapacity() const
            {
                return mVertices.size();
            }

            inline std::size_t GetNumActiveVertices() const
            {
                return mNumActiveVertices;
            }

            inline std::size_t GetVertex(std::size_t i) const
            {
                return mVertices[i].v;
            }

            inline std::size_t GetFirst() const
            {
                return mFirst;
            }

            inline std::size_t GetPrev(std::size_t i) const
            {
                return mVertices[i].prev;
            }

            inline std::size_t GetNext(std::size_t i) const
            {
                return mVertices[i].next;
            }

        private:
            static std::size_t constexpr invalid = std::numeric_limits<std::size_t>::max();

            struct Vertex
            {
                Vertex()
                    :
                    v(invalid),
                    prev(invalid),
                    next(invalid)
                {
                }

                std::size_t v, prev, next;
            };

            std::size_t mNumActiveVertices;
            std::size_t mFirst;
            std::vector<Vertex> mVertices;
            std::size_t mNextAvailable;
            std::vector<std::size_t> mFree;
        };

        std::unique_ptr<Hull> mHull;

    private:
        static std::size_t constexpr invalid = std::numeric_limits<std::size_t>::max();

        void ComputeTriangulation(std::vector<std::size_t> const& sorted)
        {
            mDimension = 0;
            mVertices.clear();
            mIndices.clear();

            // The first point is always part of the triangulation. The
            // input points are unique, so if the constructor inputs are a
            // single point, numSorted is 1 and we can return immediately
            // (0-dimensional output).
            mIndices.push_back(sorted[0]);
            if (sorted.size() == 1)
            {
                mVertices.resize(1);
                mVertices[0] = mIndices[0];
                return;
            }
            mDimension = 1;

            // Insert points until the input is determined to be 1-dimensional
            // or at least 2-dimensional.
            std::size_t current = 1;
            if (GetTriangulation1(sorted, current))
            {
                mVertices.resize(mIndices.size());
                std::copy(mIndices.begin(), mIndices.end(), mVertices.begin());
                return;
            }
            mDimension = 2;

            // The input points are fully planar; continue inserting points.
            GetTriangulation2(sorted, current);

            // Get an array of indices for the unique vertices of the
            // triangulation.
            mMesh.GetVertices(mVertices);

            // Get the array of 3-tuples of indices that represent the
            // triangles of the triangulation.
            mIndices.reserve(6 * (mNumPoints - 1));
            mIndices.clear();
            VETTrianglesKS unique(mNumPoints, mAdjacentGrowth, 0);
            auto const& vertexPool = mMesh.GetVertexPool();
            for (std::size_t v = 0; v < vertexPool.size(); ++v)
            {
                auto const& vertex = vertexPool[v];
                for (std::size_t e = 0; e < vertex.numAdjacent; ++e)
                {
                    TriangleKey<true> tri(v, vertex.adjacent[e][0], vertex.adjacent[e][1]);
                    if (unique.Insert(tri))
                    {
                        mIndices.push_back(tri[0]);
                        mIndices.push_back(tri[1]);
                        mIndices.push_back(tri[2]);
                    }
                }
            }
        }

        bool GetTriangulation1(std::vector<std::size_t> const& sorted, std::size_t& current)
        {
            mIndices.push_back(sorted[current]);  // indices[1]
            for (++current; current < sorted.size(); ++current)
            {
                if (!Colinear(mIndices[0], mIndices[1], sorted[current]))
                {
                    return false;
                }
                mIndices.push_back(sorted[current]);
            }
            return true;
        }

        void GetTriangulation2(std::vector<std::size_t> const& sorted, std::size_t& current)
        {
            // The output points previous to the current one are colinear. To
            // initialize the 2D triangulation, use triangles from the current
            // point to the line segment endpoints. The sign test is to ensure
            // the 2D triangles are counterclockwise ordered.
            std::int32_t sign = ToLine(sorted[current], mIndices[0], mIndices[1]);
            if (sign < 0)
            {
                std::reverse(mIndices.begin(), mIndices.end());
            }

            std::size_t h0 = sorted[current], h1{}, h2{};
            for (std::size_t i1 = 0, i2 = 1; i2 < mIndices.size(); i1 = i2++)
            {
                h1 = mIndices[i1];
                h2 = mIndices[i2];
                auto inserted = mMesh.Insert(h0, h2, h1, false);
                GTL_RUNTIME_ASSERT(
                    inserted,
                    "Expecting to insert triangle.");
            }

            // Initialize the convex hull of the triangulation.
            mHull->Initialize(mIndices, sorted[current]);

            // The triangulation is not maintained in mMesh, so there is
            // no need to add elements to mIndices. At the time the full
            // triangulation is known, mIndices will be assigned the triangle
            // indices of mMesh.
            for (++current; current < sorted.size(); ++current)
            {
                Update(sorted[current]);
            }
        }

        // Test whether 3 points are colinear.
        bool Colinear(std::size_t v0, std::size_t v1, std::size_t v2)
        {
            auto const& V0 = mPoints[v0];
            auto const& V1 = mPoints[v1];
            auto const& V2 = mPoints[v2];

            auto GetRPoints = [this, &v0, &v1, &v2]()
            {
                return std::array<Vector2<Rational> const*, 3>
                {
                    &GetRPoint(v0),
                    &GetRPoint(v1),
                    &GetRPoint(v2)
                };
            };

            return mECOQuery(V0, V1, V2, GetRPoints);
        }

        // For a triangle with counterclockwise vertices V0, V1 and V2 and a
        // query point P, ToCircumcircle returns
        //   +1, P outside circumcircle of triangle
        //   -1, P inside circumcircle of triangle
        //    0, P on circumcircle of triangle
        std::int32_t ToCircumcircle(std::size_t p, std::size_t v0, std::size_t v1, std::size_t v2)
        {
            auto const& P = mPoints[p];
            auto const& V0 = mPoints[v0];
            auto const& V1 = mPoints[v1];
            auto const& V2 = mPoints[v2];

            auto GetRPoints = [this, &p, &v0, &v1, &v2]()
            {
                return std::array<Vector2<Rational> const*, 4>
                {
                    &GetRPoint(p),
                    &GetRPoint(v0),
                    &GetRPoint(v1),
                    &GetRPoint(v2)
                };
            };

            return mETCQuery(P, V0, V1, V2, GetRPoints);
        }

        void ClassifyHullEdge(std::size_t p, std::size_t v0, std::size_t v1)
        {
            auto const& vertex = mMesh.GetVertex(v0);
            GTL_RUNTIME_ASSERT(
                vertex.numAdjacent > 0,
                "Expecting edge <v0,v1> to be in the triangulation."
            );

            std::size_t v2 = invalid;
            for (std::size_t e = 0; e < vertex.numAdjacent; ++e)
            {
                if (vertex.adjacent[e][0] == v1)
                {
                    v2 = vertex.adjacent[e][1];
                    break;
                }
            }
            GTL_RUNTIME_ASSERT(
                v2 != invalid,
                "Expecting <v0,v1,v2> to be in the triangulation.");

            TriangleKey<true> tri(v0, v1, v2);
            bool foundTri = false;
            for (std::size_t j = 0; j < mNumCandidates; ++j)
            {
                if (mCandidates[j] == tri)
                {
                    foundTri = true;
                    break;
                }
            }

            if (!foundTri)
            {
                if (ToCircumcircle(p, tri[0], tri[1], tri[2]) <= 0)
                {
                    // Point P is in the circumcircle.
                    mCandidates[mNumCandidates++] = tri;
                }
                else
                {
                    // Point P is not in the circumcircle but the
                    // hull edge is visible.
                    mVisible[mNumVisible++] = EdgeKey<true>(v0, v1);
                }
            }
        }

        void GetAndRemoveInsertionPolygon(std::size_t p)
        {
            // Locate the triangles that make up the insertion polygon. A
            // breadth-first search is used.
            while (mNumCandidates > 0)
            {
                TriangleKey<true> tri = mCandidates[0];
                if (mNumCandidates > 1)
                {
                    mCandidates[0] = mCandidates[mNumCandidates - 1];
                }
                --mNumCandidates;

                for (std::size_t i0 = 1, i1 = 2, i2 = 0; i2 < 3; i0 = i1, i1 = i2++)
                {
                    std::size_t vOpposite = mMesh.GetOppositeVertex(tri[i0], tri[i1], tri[i2], true);
                    if (vOpposite != invalid)
                    {
                        TriangleKey<true> adj(vOpposite, tri[i2], tri[i1]);
                        bool foundAdj = false;
                        for (std::size_t j = 0; j < mNumCandidates; ++j)
                        {
                            if (mCandidates[j] == adj)
                            {
                                foundAdj = true;
                                break;
                            }
                        }

                        if (!foundAdj)
                        {
                            if (ToCircumcircle(p, adj[0], adj[1], adj[2]) <= 0)
                            {
                                // Point P is in the circumcircle.
                                mCandidates[mNumCandidates++] = adj;
                            }
                        }
                    }
                }

                for (std::size_t i0 = 2, i1 = 0; i1 < 3; i0 = i1++)
                {
                    EdgeKey<true> edge(tri[i1], tri[i0]);
                    std::size_t location = invalid;
                    for (std::size_t j = 0; j < mNumBoundary; ++j)
                    {
                        if (mBoundary[j] == edge)
                        {
                            location = j;
                            break;
                        }
                    }

                    if (location != invalid)
                    {
                        std::size_t last = mNumBoundary - 1;
                        if (location < last)
                        {
                            // The location is interior to the array.
                            mBoundary[location] = mBoundary[last];
                        }
                        // else: The location is at the end of the array.
                        --mNumBoundary;
                    }
                    else
                    {
                        mBoundary[mNumBoundary++] = EdgeKey<true>(tri[i0], tri[i1]);
                    }
                }

                // With a correct implementation, the GTL_RUNTIME_ASSERT
                // should not trigger.
                auto removed = mMesh.Remove(tri[0], tri[1], tri[2]);
                GTL_RUNTIME_ASSERT(
                    removed,
                    "Expecting to remove triangle.");
            }
        }

        void Update(std::size_t p)
        {
            // Based on the initial sorting of points, the incoming point is
            // always outside the convex hull. Update the triangulation and
            // the hull to include this point.
            auto IsVisible = [this](std::size_t p, std::size_t v0, std::size_t v1)
            {
                return ToLine(p, v0, v1) > 0;
            };

            std::vector<EdgeKey<true>> visibleEdges{};
            std::size_t iFirstVisible = 0, iLastVisible = 0;
            mHull->GetVisible(IsVisible, p, iFirstVisible, iLastVisible, visibleEdges);

            mNumCandidates = 0;
            mNumVisible = 0;
            for (auto const& edge : visibleEdges)
            {
                ClassifyHullEdge(p, edge[0], edge[1]);
            }

            // Get the boundary of the insertion subpolygon C that
            // contains the triangles whose circumcircles contain point P.
            mNumBoundary = 0;
            GetAndRemoveInsertionPolygon(p);

            // The insertion polygon P consists of the triangles formed by
            // point i and the back edges of C and by the visible edges of
            // mGraph-C.
            for (std::size_t i = 0; i < mNumBoundary; ++i)
            {
                auto const& edge = mBoundary[i];
                if (ToLine(p, edge[0], edge[1]) < 0)
                {
                    // This is a back edge of the boundary. With a correct
                    // implementation, the GTL_RUNTIME_ASSERT should not
                    // trigger.
                    auto inserted = mMesh.Insert(p, edge[0], edge[1], false);
                    GTL_RUNTIME_ASSERT(
                        inserted,
                        "Expecting to insert a triangle.");
                }
            }

            for (std::size_t i = 0; i < mNumVisible; ++i)
            {
                auto const& edge = mVisible[i];
                // With a correct implementation, the GTL_RUNTIME_ASSERT
                // should not trigger.
                auto inserted = mMesh.Insert(p, edge[1], edge[0], false);
                GTL_RUNTIME_ASSERT(
                    inserted,
                    "Expecting to insert a triangle.");
            }

            // Update the hull to remove the edges visible to p and insert
            // the 2 new edges emanating from p.
            mHull->Update(p, iFirstVisible, iLastVisible);
        }

        // Support for classifying hull edges and removing insertion polygons.
        std::vector<TriangleKey<true>> mCandidates;
        std::size_t mNumCandidates;
        std::vector<EdgeKey<true>> mVisible;
        std::size_t mNumVisible;
        std::vector<EdgeKey<true>> mBoundary;
        std::size_t mNumBoundary;

    private:
        friend class UnitTestDelaunay2;
    };
}
