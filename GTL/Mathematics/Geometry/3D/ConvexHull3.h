// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.28

#pragma once

// Compute the convex hull of 3D points using incremental insertion. The only
// way to ensure a correct result for the input vertices is to use an exact
// predicate for computing signs of various expressions. The implementation
// uses interval arithmetic and rational arithmetic for the predicate.
//
// The main cost of the algorithm is testing which side of a plane a point is
// located. This test uses interval arithmetic to determine an exact sign,
// if possible. If that test fails, rational arithmetic is used. For typical
// datasets, the indeterminate sign from interval arithmetic happens rarely.
//
// TODO: The divide-and-conquer algorithm computes two convex hulls for a set
// of points. The two hulls are then merged into a single convex hull. The
// merge step is not the theoretical one that attempts to determine mutual
// visibility of the two hulls; rather, it combines the hulls into a single
// set of points and computes the convex hull of that set. This can be
// improved by using the left subhull in its current form and inserting points
// from the right subhull one at a time. It might be possible to insert points
// and stop the process when the partially merged polyhedron is the convex
// hull.

#include <GTL/Mathematics/Geometry/2D/ConvexHull2.h>
#include <GTL/Mathematics/Geometry/3D/ExactColinear3.h>
#include <GTL/Mathematics/Geometry/3D/ExactToPlane3.h>
#include <GTL/Mathematics/Algebra/Vector.h>
#include <GTL/Mathematics/Meshes/VETManifoldMeshKS.h>
#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <numeric>
#include <queue>
#include <set>
#include <thread>
#include <type_traits>
#include <utility>

namespace gtl
{
    template <typename T>
    class ConvexHull3
    {
    public:
        // The VETManifoldMeshKS mesh object stores adjacency information
        // for the vertices of the convex hull mesh. The information for
        // each vertex is stored in a std::vector, and the number of elements
        // of the std::vector must be specified. The default initial size is
        // the following constant. During runtime, if the std::vector reaches
        // its capacity, it must be resized. Each resize adds a chunk with
        // the size specified by the following constant.
        static constexpr std::size_t defaultAdjacentGrowth = 16;

        ConvexHull3(std::size_t adjacentGrowth = defaultAdjacentGrowth)
            :
            mNumThreads(0),
            mNumPoints(0),
            mPoints(nullptr),
            mEquivalentTo{},
            mRPoints{},
            mConverted{},
            mDimension(0),
            mVertices{},
            mHull{},
            mMesh{},
            mAdjacentGrowth(adjacentGrowth)
        {
            static_assert(
                std::is_floating_point<T>::value,
                "The input type must be a floating-point type.");
        }

        // Compute the exact convex hull using a blend of interval arithmetic
        // and rational arithmetic. The code runs single-threaded when
        // lgNumThreads = 0. It runs multithreaded when lgNumThreads > 0,
        // where the number of threads is 2^{lgNumThreads} > 1.
        void operator()(std::size_t numPoints, Vector3<T> const* points, std::size_t lgNumThreads)
        {
            GTL_ARGUMENT_ASSERT(
                numPoints > 0 && points != nullptr,
                "Invalid argument.");

            mNumThreads = (static_cast<std::size_t>(1) << lgNumThreads);

            // Provide access to the points for the member functions.
            mNumPoints = numPoints;
            mPoints = points;
            mMesh.Reset(mNumPoints, mAdjacentGrowth, mNumThreads);

            // Allocate storage for any rational points that must be computed
            // in the exact sign predicates.
            mRPoints.resize(numPoints);
            mConverted.resize(numPoints);
            std::fill(mConverted.begin(), mConverted.end(), 0);

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
            std::iota(sorted.begin(), sorted.end(), 0);
            std::sort(sorted.begin(), sorted.end(), lessThanPoints);

            // Eliminate duplicates but keep track of equivalence classes
            // of points that are duplicated. This information is used in
            // ConstrainedDelaunay2 and can be used in other applications
            // that require the information. The algorithm used here is a
            // modification of the one specified at
            //   https://en.cppreference.com/w/cpp/algorithm/unique
            // for a possible implementation of std::unique.
            mEquivalentTo.resize(numPoints);
            std::iota(mEquivalentTo.begin(), mEquivalentTo.end(), 0);
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

            if (lgNumThreads > 0)
            {
                // Execute in multiple threads.
                std::size_t numThreads = mNumThreads;
                std::vector<std::thread> process(numThreads);

                // Partition the sorted points for the first pass.
                std::vector<std::size_t> inNumSorted(numThreads);
                std::vector<std::size_t*> inSorted(numThreads);
                std::vector<std::vector<std::size_t>> outVertices(numThreads);
                std::size_t load = sorted.size() / numThreads;
                inNumSorted.back() = sorted.size();
                inSorted.front() = sorted.data();
                for (std::size_t i0 = 0, i1 = 1; i1 < numThreads; i0 = i1++)
                {
                    inNumSorted[i0] = load;
                    inNumSorted.back() -= load;
                    inSorted[i1] = inSorted[i0] + load;
                }

                while (numThreads > 1)
                {
                    // Divide ...
                    for (std::size_t i = 0; i < numThreads; ++i)
                    {
                        process[i] = std::thread(
                            [this, i, &inNumSorted, &inSorted, &outVertices]()
                            {
                                std::size_t dimension = 0;
                                VETManifoldMeshKS mesh(mNumPoints, mAdjacentGrowth,
                                    mNumThreads);
                                ComputeHull(inNumSorted[i], inSorted[i], dimension,
                                    outVertices[i], mesh);
                            });
                    }

                    numThreads /= 2;

                    // ... and conquer
                    auto target = sorted.begin();
                    inSorted[0] = sorted.data();
                    for (std::size_t i = 0, k = 0; i < numThreads; ++i)
                    {
                        process[2 * i].join();
                        process[2 * i + 1].join();

                        inNumSorted[i] = 0;
                        std::vector<std::size_t>::iterator begin = target;
                        for (std::size_t j = 0; j < 2; ++j, ++k)
                        {
                            std::size_t numVertices = outVertices[k].size();
                            inNumSorted[i] += numVertices;
                            std::copy(outVertices[k].begin(), outVertices[k].end(), target);
                            target += numVertices;
                        }
                        inSorted[i + 1] = inSorted[i] + inNumSorted[i];
                        std::sort(begin, target, lessThanPoints);
                    }
                }

                ComputeHull(inNumSorted[0], inSorted[0], mDimension, mVertices, mMesh);
            }
            else
            {
                // Execute single=threaded, in the main thread only.
                ComputeHull(sorted.size(), sorted.data(), mDimension, mVertices, mMesh);
            }

            // Get the array of 3-tuples of indices that represent the hull
            // triangles.
            if (mDimension == 3)
            {
                // For manifold mesh representing a convex polyhedron with V
                // vertices, the number of triangles is 2(V-2) and the number
                // of edges is 3(V-2).
                mHull.reserve(6 * (mNumPoints - 1));
                mHull.clear();
                VETTrianglesKS unique(mNumPoints, mAdjacentGrowth, mNumThreads);
                auto const& vertexPool = mMesh.GetVertexPool();
                for (std::size_t v = 0; v < vertexPool.size(); ++v)
                {
                    auto const& vertex = vertexPool[v];
                    for (std::size_t e = 0; e < vertex.numAdjacent; ++e)
                    {
                        TriangleKey<true> tri(v, vertex.adjacent[e][0], vertex.adjacent[e][1]);
                        if (unique.Insert(tri))
                        {
                            mHull.push_back(tri[0]);
                            mHull.push_back(tri[1]);
                            mHull.push_back(tri[2]);
                        }
                    }
                }
            }
        }

        void operator()(std::vector<Vector3<T>> const& points, std::size_t lgNumThreads)
        {
            operator()(points.size(), points.data(), lgNumThreads);
        }

        // Access to the inputs to operator().
        inline std::size_t GetNumPoints() const
        {
            return mNumPoints;
        }

        inline Vector3<T> const* GetPoints() const
        {
            return mPoints;
        }

        // The equivalence array mEquivalentTo[i] = j keeps track of duplicate
        // points. The point[i] is equal to the point[j]. For a dataset with
        // no duplicates, mEquivalentTo[i] = i for all i.
        inline std::vector<std::size_t> const& GetEquivalentTo() const
        {
            return mEquivalentTo;
        }

        // Memoized access to the rational representation of the points.
        using Rational = BSNumber<UIntegerFP32<2>>;

        Vector3<Rational> const& GetRPoint(std::size_t index) const
        {
            if (mConverted[index] == 0)
            {
                mConverted[index] = 1;
                for (std::size_t i = 0; i < 3; ++i)
                {
                    mRPoints[index][i] = mPoints[index][i];
                }
            }
            return mRPoints[index];
        }

        // The dimension is 0 (hull is a single point), 1 (hull is a line
        // segment), 2 (hull is a convex polygon in 3D) or 3 (hull is a convex
        // polyhedron).
        inline std::size_t GetDimension() const
        {
            return mDimension;
        }

        // Get the indices into the input 'points[]' that correspond to hull
        // vertices.
        inline std::vector<std::size_t> const& GetVertices() const
        {
            return mVertices;
        }

        // Get the indices into the input 'points[]' that correspond to hull
        // vertices. The returned array is organized according to the hull
        // dimension.
        //   0: The hull is a single point. The returned array has size 1 with
        //      index corresponding to that point.
        //   1: The hull is a line segment. The returned array has size 2 with
        //      indices corresponding to the segment endpoints.
        //   2: The hull is a convex polygon in 3D. The returned array has
        //      size N with indices corresponding to the polygon vertices.
        //      The vertices are ordered.
        //   3: The hull is a convex polyhedron. The returned array has T
        //      triples of indices, each triple corresponding to a triangle
        //      face of the hull. The face vertices are counterclockwise when
        //      viewed by an observer outside the polyhedron. It is possible
        //      that some triangle faces are coplanar.
        // The number of vertices and triangles can vary depending on the
        // number of threads used for computation. This is not an error. For
        // example, when running with N threads it is possible to have a
        // convex quadrilateral face formed by 2 coplanar triangles {v0,v1,v2}
        // and {v0,v2,v3}. When running with M threads, it is possible that
        // the same convex quadrilateral face is formed by 4 coplanar triangles
        // {v0,v1,v2}, {v1,v2,v4}, {v2,v3,v4} and {v3,v0,v4}, where the
        // vertices v0, v2 and v4 are colinear. In both cases, if V is the
        // number of vertices and T is the number of triangles, then the
        // number of edges is E = T/2 and Euler's formula is satisfied:
        // V - E + T = 2.
        inline std::vector<std::size_t> const& GetHull() const
        {
            return mHull;
        }

        // Get the hull mesh, which is valid only when the dimension is 3.
        // This allows access to the graph of vertices, edges and triangles
        // of the convex (polyhedron) hull.
        inline VETManifoldMeshKS const& GetMesh() const
        {
            return mMesh;
        }

    private:
        void ComputeHull(std::size_t numSorted, std::size_t* sorted, std::size_t& dimension,
            std::vector<std::size_t>& vertices, VETManifoldMeshKS& mesh)
        {
            std::vector<std::size_t> hull{};
            hull.reserve(numSorted);
            dimension = 0;
            vertices.clear();

            // The first point is always part of the hull. The input points
            // are unique, so if the constructor inputs are a single point,
            // numSorted is 1 and we can return immediately (0-dimensional
            // hull).
            hull.push_back(sorted[0]);
            if (numSorted == 1)
            {
                vertices = hull;
                return;
            }
            dimension = 1;

            // The code uses exact predicates for determining whether 3 points
            // are colinear. The query is defined here so that it is
            // thread-local, which is necessary because a class-scope query
            // has non-static class-member data that would need to be
            // protected in a critical section when multiple threads try to
            // write to it.
            ExactColinear3<T> colinearQuery{};

            // Insert points until the hull is determined to be 1-dimensional
            // or at least 2-dimensional.
            std::size_t current = 1;
            if (Hull1(hull, numSorted, sorted, colinearQuery, dimension, current))
            {
                vertices = hull;
                return;
            }

            // The code uses exact predicates for determining how incoming
            // points are positioned relative to planes of hull faces. The
            // query is defined here so that it is thread-local, which is
            // necessary because a class-scope query has non-static
            // class-member data that would need to be protected in a critical
            // section when multiple threads try to write to it.
            ExactToPlane3<T> toPlaneQuery{};

            // Insert points until the hull is determined to be 2-dimensional
            // or 3-dimensional.
            if (Hull2(hull, numSorted, sorted, toPlaneQuery, dimension, current))
            {
                vertices = hull;
                return;
            }

            // The hull is 3-dimensional; continue inserting points.
            Hull3(hull, numSorted, sorted, toPlaneQuery, mesh, current);

            // Get an array of indices for the unique vertices of the hull.
            mesh.GetVertices(vertices);
        }

        // Support for computing a 1-dimensional convex hull.
        bool Hull1(std::vector<std::size_t>& hull, std::size_t numSorted, std::size_t* sorted,
            ExactColinear3<T>& query, std::size_t& dimension, std::size_t& current)
        {
            hull.push_back(sorted[current]);  // hull[1]
            for (++current; current < numSorted; ++current)
            {
                if (!Colinear(query, sorted[current], hull[0], hull[1]))
                {
                    dimension = 2;
                    break;
                }
                hull.push_back(sorted[current]);
            }

            if (hull.size() > 2)
            {
                // Eliminate the non-extreme colinear points.
                std::size_t hmin = hull.front();
                std::size_t hmax = hull.back();
                hull.clear();
                hull.push_back(hmin);
                hull.push_back(hmax);
            }

            return dimension == 1;
        }

        bool Hull2(std::vector<std::size_t>& hull, std::size_t numSorted, std::size_t* sorted,
            ExactToPlane3<T>& query, std::size_t& dimension, std::size_t& current)
        {
            hull.push_back(sorted[current]);  // hull[2]
            for (++current; current < numSorted; ++current)
            {
                if (ToPlane(query, sorted[current], hull[0], hull[1], hull[2]) != 0)
                {
                    dimension = 3;
                    break;
                }
                hull.push_back(sorted[current]);
            }

            if (hull.size() > 3)
            {
                // Compute the planar convex hull of the points. The coplanar
                // points are projected onto a 2D plane determined by the
                // maximum absolute component of the normal of the first
                // triangle. The extreme points of the projected hull generate
                // the extreme points of the planar hull in 3D.
                std::array<Vector3<CRational>, 3> rVertex{};
                for (std::size_t i = 0; i < 3; ++i)
                {
                    Vector3<Rational> const& rSource = GetRPoint(hull[i]);
                    Vector3<CRational>& rTarget = rVertex[i];
                    for (std::size_t j = 0; j < 3; ++j)
                    {
                        rTarget[j] = rSource[j];
                    }
                }
                auto const rDiff1 = rVertex[1] - rVertex[0];
                auto const rDiff2 = rVertex[2] - rVertex[0];
                auto rNormal = Cross(rDiff1, rDiff2);

                // The signs are used to select 2 of the 3 point components so
                // that when the planar hull is viewed from the side of the
                // plane to which rNormal is directed, the triangles are
                // counterclockwise ordered.
                std::array<std::int32_t, 3> sign{};
                for (std::size_t i = 0; i < 3; ++i)
                {
                    sign[i] = rNormal[i].GetSign();
                    if (sign[i] < 0)
                    {
                        sign[i] = 1;
                        rNormal[i].Negate();
                    }
                };

                std::pair<std::size_t, std::size_t> c{};
                if (rNormal[0] > rNormal[1])
                {
                    if (rNormal[0] > rNormal[2])
                    {
                        c = (sign[0] > 0 ? std::make_pair(1, 2) : std::make_pair(2, 1));
                    }
                    else
                    {
                        c = (sign[2] > 0 ? std::make_pair(0, 1) : std::make_pair(1, 0));
                    }
                }
                else
                {
                    if (rNormal[1] > rNormal[2])
                    {
                        c = (sign[1] > 0 ? std::make_pair(2, 0) : std::make_pair(0, 2));
                    }
                    else
                    {
                        c = (sign[2] > 0 ? std::make_pair(0, 1) : std::make_pair(1, 0));
                    }
                }

                std::vector<Vector2<T>> projections(hull.size());
                for (std::size_t i = 0; i < projections.size(); ++i)
                {
                    std::size_t h = hull[i];
                    projections[i][0] = mPoints[h][c.first];
                    projections[i][1] = mPoints[h][c.second];
                }

                ConvexHull2<T> ch2{};
                ch2(projections);
                auto const& hull2 = ch2.GetHull();

                std::vector<std::size_t> tempHull(hull2.size());
                for (std::size_t i = 0; i < hull2.size(); ++i)
                {
                    tempHull[i] = hull[hull2[i]];
                }
                hull.clear();
                for (std::size_t i = 0; i < hull2.size(); ++i)
                {
                    hull.push_back(tempHull[i]);
                }
            }

            return dimension == 2;
        }

        // Support for computing a 3-dimensional convex hull.
        void Hull3(std::vector<std::size_t>& hull, std::size_t numSorted, std::size_t* sorted,
            ExactToPlane3<T>& query, VETManifoldMeshKS& mesh, std::size_t& current)
        {
            // The hull points previous to the current one are coplanar and
            // are the vertices of a convex polygon. To initialize the 3D
            // hull, use triangles from a triangle fan of the convex polygon
            // and use triangles connecting the current point to the edges
            // of the convex polygon. The vertex ordering of these triangles
            // depends on whether sorted[current] is on the positive or
            // negative side of the plane determined by hull[0], hull[1] and
            // hull[2].
            std::int32_t sign = ToPlane(query, sorted[current], hull[0], hull[1], hull[2]);
            std::size_t h0{}, h1{}, h2{};
            if (sign > 0)
            {
                h0 = hull[0];
                for (std::size_t i1 = 1, i2 = 2; i2 < hull.size(); i1 = i2++)
                {
                    h1 = hull[i1];
                    h2 = hull[i2];
                    auto inserted = mesh.Insert(h0, h2, h1, false);
                    GTL_RUNTIME_ASSERT(
                        inserted,
                        "Unexpected insertion failure.");
                }

                h0 = sorted[current];
                for (std::size_t i1 = hull.size() - 1, i2 = 0; i2 < hull.size(); i1 = i2++)
                {
                    h1 = hull[i1];
                    h2 = hull[i2];
                    auto inserted = mesh.Insert(h0, h1, h2, false);
                    GTL_RUNTIME_ASSERT(
                        inserted,
                        "Unexpected insertion failure.");
                }
            }
            else
            {
                h0 = hull[0];
                for (std::size_t i1 = 1, i2 = 2; i2 < hull.size(); i1 = i2++)
                {
                    h1 = hull[i1];
                    h2 = hull[i2];
                    auto inserted = mesh.Insert(h0, h1, h2, false);
                    GTL_RUNTIME_ASSERT(
                        inserted,
                        "Unexpected insertion failure.");
                }

                h0 = sorted[current];
                for (std::size_t i1 = hull.size() - 1, i2 = 0; i2 < hull.size(); i1 = i2++)
                {
                    h1 = hull[i1];
                    h2 = hull[i2];
                    auto inserted = mesh.Insert(h0, h2, h1, false);
                    GTL_RUNTIME_ASSERT(
                        inserted,
                        "Unexpected insertion failure.");
                }
            }

            // The hull is now maintained in mesh, so there is no need
            // to add elements to hull. At the time the full hull is known,
            // hull will be assigned the triangle indices.
            std::queue<TriangleKey<true>> visible;
            VETTrianglesKS visited(mNumPoints, mAdjacentGrowth, mNumThreads);
            std::vector<std::array<std::size_t, 2>> terminator;
            for (++current; current < numSorted; ++current)
            {
                // The index h0 refers to the previously inserted hull point.
                // The index h1 refers to the current point to be inserted
                // into the hull.
                auto const& vertex = mesh.GetVertex(h0);
                GTL_RUNTIME_ASSERT(
                    vertex.numAdjacent > 0 && vertex.adjacent.size() > 0,
                    "Unexpected condition");

                h1 = sorted[current];

                // The sorting guarantees that the point at h0 is visible to
                // the point at h1. Find the triangles that share h0 and are
                // visible to h1
                for (std::size_t e = 0; e < vertex.numAdjacent; ++e)
                {
                    std::size_t v0 = vertex.adjacent[e][0], v1 = vertex.adjacent[e][1];
                    sign = ToPlane(query, h1, h0, v0, v1);
                    if (sign > 0)
                    {
                        TriangleKey<true> tri(h0, v0, v1);
                        visible.push(tri);
                        visited.Insert(tri);
                        break;
                    }
                }
                GTL_RUNTIME_ASSERT(
                    visible.size() > 0,
                    "Unexpected condition.");

                // Remove the connected component of visible triangles. Save
                // the terminator edges for insertion of the new visible set
                // of triangles.
                std::size_t constexpr invalid = std::numeric_limits<std::size_t>::max();
                terminator.clear();
                while (visible.size() > 0)
                {
                    TriangleKey<true> tri = visible.front();
                    visible.pop();
                    for (std::size_t i0 = 1, i1 = 2, i2 = 0; i2 < 3; i0 = i1, i1 = i2++)
                    {
                        std::size_t vOpposite = mesh.GetOppositeVertex(
                            tri[i0], tri[i1], tri[i2], false);
                        if (vOpposite != invalid)
                        {
                            TriangleKey<true> adj(vOpposite, tri[i2], tri[i1]);
                            if (!visited.Exists(adj))
                            {
                                sign = ToPlane(query, h1, adj[0], adj[1], adj[2]);
                                if (sign <= 0)
                                {
                                    // The shared edge of tri and adj is a
                                    // terminator.
                                    terminator.push_back({ tri[i1], tri[i2] });
                                }
                                else
                                {
                                    visible.push(adj);
                                    visited.Insert(adj);
                                }
                            }
                        }
                    }
                    visited.Remove(tri);
                    auto removed = mesh.Remove(tri[0], tri[1], tri[2]);
                    GTL_RUNTIME_ASSERT(
                        removed,
                        "Unexpected removal failure.");
                }

                // Insert the new hull triangles.
                for (auto const& edge : terminator)
                {
                    auto inserted = mesh.Insert(edge[0], edge[1], h1, false);
                    GTL_RUNTIME_ASSERT(
                        inserted,
                        "Unexpected insertion failure.");
                }

                // The current index h1 becomes the previous index h0 for the
                // next pass of the 'current' loop.
                h0 = h1;
            }
        }

        // Support for computing a 2-dimensional convex hull.
        static std::size_t constexpr numWords = std::is_same<T, float>::value ? 18 : 132;
        using CRational = BSNumber<UIntegerFP32<numWords>>;

        // Test whether 3 points are colinear.
        bool Colinear(ExactColinear3<T>& query, std::size_t v0, std::size_t v1, std::size_t v2)
        {
            auto const& V0 = mPoints[v0];
            auto const& V1 = mPoints[v1];
            auto const& V2 = mPoints[v2];

            auto GetRPoints = [this, &v0, &v1, &v2]()
            {
                return std::array<Vector3<Rational> const*, 3>
                {
                    &GetRPoint(v0),
                    &GetRPoint(v1),
                    &GetRPoint(v2)
                };
            };

            return query(V0, V1, V2, GetRPoints);
        }

        // For a plane with origin V0 and normal N = Cross(V1-V0, V2-V0),
        // ToPlane returns
        //   +1, P on positive side of plane (side to which N points)
        //   -1, P on negative side of plane (side to which -N points)
        //    0, P on the plane
        std::int32_t ToPlane(ExactToPlane3<T>& query, std::size_t p, std::size_t v0, std::size_t v1, std::size_t v2)
        {
            auto const& P = mPoints[p];
            auto const& V0 = mPoints[v0];
            auto const& V1 = mPoints[v1];
            auto const& V2 = mPoints[v2];

            auto GetRPoints = [this, &p, &v0, &v1, &v2]()
            {
                return std::array<Vector3<Rational> const*, 4>
                {
                    &GetRPoint(p),
                    &GetRPoint(v0),
                    &GetRPoint(v1),
                    &GetRPoint(v2)
                };
            };

            return query(P, V0, V1, V2, GetRPoints);
        }

    private:
        // The input points to operator().
        std::size_t mNumThreads;
        std::size_t mNumPoints;
        Vector3<T> const* mPoints;

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
        mutable std::vector<Vector3<Rational>> mRPoints;
        mutable std::vector<std::uint32_t> mConverted;

        // The output data.
        std::size_t mDimension;
        std::vector<std::size_t> mVertices;
        std::vector<std::size_t> mHull;
        VETManifoldMeshKS mMesh;
        std::size_t mAdjacentGrowth;

    private:
        friend class UnitTestConvexHull3;
    };
}
