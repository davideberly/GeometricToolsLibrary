// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

// Incremental insertion and removal of vertices in a Delaunay triangulation.
// The triangles are counterclockwise ordered.
//
// The removal code is an implementation of the algorithm in
//     Olivier Devillers,
//     "On Deletion in Delaunay Triangulations",
//     International Journal of Computational Geometry and Applications,
//     World Scientific Publishing, 2002, 12, pp. 193-205.
//     https://hal.inria.fr/inria-00167201/document
// The weight function for the priority queue, implemented as a min-heap, is
// the negative of the function power(p,circle(q0,q1,q2)) function described
// in the paper.
// 
// The paper appears to assume that the removal point is an interior point of
// the trianglation. Just as the insertion algorithms are different for
// interior points and for boundary points, the removal algorithms are
// different for interior points and for boundary points.
// 
// The paper mentions that degeneracies (colinear points, cocircular points)
// are handled by jittering. Although one hopes that jittering prevents
// degeneracies--and perhaps probabilistically this is acceptable, the only
// guarantee for a correct result is to use exact arithmetic on the input
// points. The implementation here uses a blend of interval and rational
// arithmetic for exactness; the input points are not jittered.
//
// The details of the algorithms and implementation are provided in
// https://www.geometrictools.com/Documentation/IncrementalDelaunayTriangulation.pdf
//
// The input type must be 'float' or 'double'. The compute type is defined
// internally and has enough bits of precision to handle any floating-point
// inputs.


#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#include <GTL/Mathematics/Algebra/Vector.h>
#include <GTL/Mathematics/Geometry/2D/ExactToCircumcircle2.h>
#include <GTL/Mathematics/Geometry/2D/ExactToLine2.h>
#include <GTL/Mathematics/Meshes/DynamicVETManifoldMesh.h>
#include <GTL/Utility/MinHeap.h>
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <limits>
#include <map>
#include <set>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace gtl
{
    template <typename T>
    class IncrementalDelaunay2
    {
    public:
        // A bounding rectangle for the input points must be specified.
        IncrementalDelaunay2(T const& xMin, T const& yMin, T const& xMax, T const& yMax)
            :
            mXMin(xMin),
            mYMin(yMin),
            mXMax(xMax),
            mYMax(yMax),
            mVertexIndexMap{},
            mVertices{},
            mIRVertices{},
            mETLQuery{},
            mETCQuery{},
            mToLineWrapper{},
            mGraph{},
            mIndex{ { { 0, 1 }, { 1, 2 }, { 2, 0 } } },
            mTriangles{},
            mAdjacencies{},
            mTrianglesAndAdjacenciesNeedUpdate(true),
            mQueryPoint{},
            mIRQueryPoint{}
        {
            static_assert(
                std::is_floating_point<T>::value,
                "The input type must be a floating-point type.");

            GTL_ARGUMENT_ASSERT(
                mXMin < mXMax && mYMin < mYMax,
                "Invalid bounding rectangle.");

            mToLineWrapper = [this](std::size_t vPrev, std::size_t vCurr, std::size_t vNext)
            {
                return ToLine(vPrev, vCurr, vNext);
            };

            // Create a supertriangle that contains the input rectangle.
            T xDelta = mXMax - mXMin;
            T yDelta = mYMax - mYMin;
            T x0 = mXMin - xDelta;
            T y0 = mYMin - yDelta;
            T x1 = mXMin + C_<T>(5) * xDelta;
            T y1 = y0;
            T x2 = x0;
            T y2 = mYMin + C_<T>(5) * yDelta;
            Vector2<T> supervertex0{ x0, y0 };
            Vector2<T> supervertex1{ x1, y1 };
            Vector2<T> supervertex2{ x2, y2 };

            // Insert the supertriangle vertices into the vertex storage.
            mVertexIndexMap.emplace(supervertex0, 0);
            mVertexIndexMap.emplace(supervertex1, 1);
            mVertexIndexMap.emplace(supervertex2, 2);
            mVertices.emplace_back(supervertex0);
            mVertices.emplace_back(supervertex1);
            mVertices.emplace_back(supervertex2);
            mIRVertices.emplace_back(IRVector{ x0, y0 });
            mIRVertices.emplace_back(IRVector{ x1, y1 });
            mIRVertices.emplace_back(IRVector{ x2, y2 });

            // Insert the supertriangle into the triangulation.
            auto inserted = mGraph.Insert(0, 1, 2);
            GTL_RUNTIME_ASSERT(
                inserted != nullptr,
                "Failed to insert supertriangle.");
        }

        ~IncrementalDelaunay2() = default;

        // Insert a point into the triangulation. The return value is the
        // index associated with the vertex in the vertex map. The
        // supertriangle vertices are at indices 0, 1, and 2. If the input
        // point already exists, its vertex-map index is simply returned. If
        // the position is outside the domain specified in the constructor,
        // an exception is thrown.
        std::size_t Insert(Vector2<T> const& position)
        {
            mTrianglesAndAdjacenciesNeedUpdate = true;

            GTL_ARGUMENT_ASSERT(
                mXMin <= position[0] && position[0] <= mXMax &&
                mYMin <= position[1] && position[1] <= mYMax,
                "The position is outside the domain specified in the constructor.");

            auto iter = mVertexIndexMap.find(position);
            if (iter != mVertexIndexMap.end())
            {
                // The vertex already exists.
                return iter->second;
            }

            // Store the position in the various pools.
            std::size_t posIndex = mVertices.size();
            mVertexIndexMap.emplace(position, posIndex);
            mVertices.emplace_back(position);
            mIRVertices.emplace_back(IRVector{ position[0], position[1] });

            Update(posIndex);
            return posIndex;
        }

        // Remove a point from the triangulation. The return value is the index
        // associated with the vertex in the vertex map when that vertex exists.
        // If the vertex does not exist, the return value is
        // std::numeric_limit<std::size_t>::max().
        std::size_t Remove(Vector2<T> const& position)
        {
            mTrianglesAndAdjacenciesNeedUpdate = true;

            auto iter = mVertexIndexMap.find(position);
            if (iter == mVertexIndexMap.end())
            {
                // The position is not a vertex of the triangulation.
                return invalid;
            }
            std::size_t vRemovalIndex = iter->second;

            if (mVertexIndexMap.size() == 4)
            {
                // Only a single point has been inserted previously into the
                // triangulation.
                for (std::size_t i0 = 2, i1 = 0; i1 < 3; i0 = i1++)
                {
                    auto removed = mGraph.Remove(vRemovalIndex, i0, i1);
                    GTL_RUNTIME_ASSERT(
                        removed,
                        "Unexpected removal failure.");
                }

                auto inserted = mGraph.Insert(0, 1, 2);
                GTL_RUNTIME_ASSERT(
                    inserted != nullptr,
                    "Unexpected insertion failure.");

                mVertexIndexMap.erase(iter);
                return vRemovalIndex;
            }

            // Locate the position in the vertices of the graph.
            auto const& vMap = mGraph.GetVertices();
            auto vIter = vMap.find(vRemovalIndex);
            GTL_RUNTIME_ASSERT(
                vIter != vMap.end(),
                "Expecting to find the to-be-removed vertex in the triangulation.");

            bool removalPointOnBoundary = false;
            for (auto vIndex : vIter->second->VAdjacent)
            {
                if (IsSupervertex(vIndex))
                {
                    // The triangle has a supervertex, so the removal point
                    // is on the boundary of the Delaunay triangulation.
                    removalPointOnBoundary = true;
                    break;
                }
            }

            auto const& adjacents = vIter->second->TAdjacent;
            std::vector<std::size_t> polygon{};
            DeleteRemovalPolygon(vRemovalIndex, adjacents, polygon);

            if (removalPointOnBoundary)
            {
                RetriangulateBoundaryRemovalPolygon(vRemovalIndex, polygon);
            }
            else
            {
                RetriangulateInteriorRemovalPolygon(vRemovalIndex, polygon);
            }

            mVertexIndexMap.erase(iter);
            return vRemovalIndex;
        }

        // Get the current triangulation including the supervertices and
        // triangles containing supervertices.
        void GetTriangulation(std::vector<Vector2<T>>& vertices,
            std::vector<std::array<std::size_t, 3>>& triangles)
        {
            vertices.resize(mVertices.size());
            std::copy(mVertices.begin(), mVertices.end(), vertices.begin());

            auto const& tMap = mGraph.GetTriangles();
            triangles.reserve(tMap.size());
            triangles.clear();
            for (auto const& tri : tMap)
            {
                auto const& tKey = tri.first;
                triangles.push_back(tKey);
            }
        }

        // Get the current graph, which includes all triangles whether
        // Delaunay or those containing a supervertex.
        inline DynamicVETManifoldMesh const& GetGraph() const
        {
            return mGraph;
        }

        // Queries associated with the mesh of Delaunay triangles. The
        // triangles containing a supervertex are not included in these
        // queries.
        inline std::size_t GetNumVertices() const
        {
            return mVertices.size();
        }

        inline std::vector<Vector2<T>> const& GetVertices() const
        {
            return mVertices;
        }

        std::size_t GetNumTriangles() const
        {
            if (mTrianglesAndAdjacenciesNeedUpdate)
            {
                UpdateTrianglesAndAdjacencies();
                mTrianglesAndAdjacenciesNeedUpdate = false;
            }

            return mTriangles.size();
        }

        std::vector<std::array<std::size_t, 3>> const& GetTriangles() const
        {
            if (mTrianglesAndAdjacenciesNeedUpdate)
            {
                UpdateTrianglesAndAdjacencies();
                mTrianglesAndAdjacenciesNeedUpdate = false;
            }

            return mTriangles;
        }

        std::vector<std::array<std::size_t, 3>> const& GetAdjacencies() const
        {
            if (mTrianglesAndAdjacenciesNeedUpdate)
            {
                UpdateTrianglesAndAdjacencies();
                mTrianglesAndAdjacenciesNeedUpdate = false;
            }

            return mAdjacencies;
        }

        // Get the vertex indices for triangle t. The function returns 'true'
        // when t is a valid triangle index, in which case 'triangle' is valid;
        // otherwise, the function returns 'false' and 'triangle' is invalid.
        bool GetTriangle(std::size_t t, std::array<std::size_t, 3>& triangle) const
        {
            if (mTrianglesAndAdjacenciesNeedUpdate)
            {
                UpdateTrianglesAndAdjacencies();
                mTrianglesAndAdjacenciesNeedUpdate = false;
            }

            if (t < mTriangles.size())
            {
                triangle = mTriangles[t];
                return true;
            }
            return false;
        }

        // Get the indices for triangles adjacent to triangle t. The function
        // returns 'true' when t is a valid triangle index, in which case
        // 'adjacent' is valid; otherwise, the function returns 'false' and
        // 'adjacent' is invalid. When valid, triangle t has ordered vertices
        // <V[0], V[1], V[2]>. The value adjacent[0] is the index for the
        // triangle adjacent to edge <V[0], V[1]>, adjacent[1] is the index
        // for the triangle adjacent to edge <V[1], V[2]>, and adjacent[2] is
        // the index for the triangle adjacent to edge <V[2], V[0]>.
        bool GetAdjacent(std::size_t t, std::array<std::size_t, 3>& adjacent) const
        {
            if (mTrianglesAndAdjacenciesNeedUpdate)
            {
                UpdateTrianglesAndAdjacencies();
                mTrianglesAndAdjacenciesNeedUpdate = false;
            }

            if (t < mAdjacencies.size())
            {
                adjacent = mAdjacencies[t];
                return true;
            }
            return false;
        }

        // Get the convex polygon that is the hull of the Delaunay triangles.
        // The polygon is counterclockwise ordered with vertices V[hull[0]],
        // V[hull[1]], ..., V[hull.size()-1].
        void GetHull(std::vector<std::size_t>& hull) const
        {
            if (mTrianglesAndAdjacenciesNeedUpdate)
            {
                UpdateTrianglesAndAdjacencies();
                mTrianglesAndAdjacenciesNeedUpdate = false;
            }

            // The hull edges are shared by the triangles with exactly one
            // supervertex.
            std::map<std::size_t, std::size_t> edges{};
            auto const& vmap = mGraph.GetVertices();
            for (std::size_t v = 0; v < 3; ++v)
            {
                auto vIter = vmap.find(v);
                GTL_RUNTIME_ASSERT(
                    vIter != vmap.end(),
                    "Expecting the supervertices to exist in the graph.");

                for (auto const& adj : vIter->second->TAdjacent)
                {
                    for (std::size_t i0 = 1, i1 = 2, i2 = 0; i2 < 3; i0 = i1, i1 = i2, ++i2)
                    {
                        if (adj->V[i0] == v)
                        {
                            if (IsDelaunayVertex(adj->V[i1]) && IsDelaunayVertex(adj->V[i2]))
                            {
                                edges.insert(std::make_pair(adj->V[i2], adj->V[i1]));
                                break;
                            }
                        }
                    }
                }
            }

            // Repackage the edges into a convex polygon with vertices ordered
            // counterclockwise.
            std::size_t numEdges = edges.size();
            hull.resize(numEdges);
            auto eIter = edges.begin();
            std::size_t vStart = eIter->first;
            std::size_t vNext = eIter->second;
            std::size_t i = 0;
            hull[0] = vStart;
            while (vNext != vStart)
            {
                hull[++i] = vNext;
                eIter = edges.find(vNext);
                GTL_RUNTIME_ASSERT(
                    eIter != edges.end(),
                    "Expecting to find a hull edge.");
                vNext = eIter->second;
            }
        }

        // Support for searching the Delaunay triangles that contains the
        // point p. If there is a containing triangle, the returned value is
        // a triangle index i with 0 <= i < GetNumTriangles(). If there is
        // not a containing triangle, 'invalid' is returned. The computations
        // are performed using exact rational arithmetic.
        //
        // The SearchInfo input stores information about the triangle search
        // when looking for the triangle (if any) that contains p. The first
        // triangle searched is 'initialTriangle'. On return 'path' stores the
        // ordered triangle indices visited during the search. The last
        // visited triangle has index 'finalTriangle' and vertex indices
        // 'finalV[0,1,2]', stored in counterclockwise order. The last edge of
        // the search is <finalV[0], finalV[1]>. For spatially coherent inputs
        // p for numerous calls to this function, you will want to specify
        // 'finalTriangle' from the previous call as 'initialTriangle' for the
        // next call, which should reduce search times.

        static std::size_t constexpr invalid = std::numeric_limits<std::size_t>::max();

        struct SearchInfo
        {
            SearchInfo()
                :
                initialTriangle(invalid),
                finalTriangle(invalid),
                finalV{ invalid, invalid, invalid },
                numPath(0),
                path{}
            {
            }

            std::size_t initialTriangle;
            std::size_t finalTriangle;
            std::array<std::size_t, 3> finalV;
            std::size_t numPath;
            std::vector<std::size_t> path;
        };

        std::size_t GetContainingTriangle(Vector2<T> const& p, SearchInfo& info) const
        {
            if (mTrianglesAndAdjacenciesNeedUpdate)
            {
                UpdateTrianglesAndAdjacencies();
                mTrianglesAndAdjacenciesNeedUpdate = false;
            }

            mQueryPoint = p;
            mIRQueryPoint = IRVector{ p[0], p[1] };

            std::size_t const numTriangles = mTriangles.size();
            info.path.resize(numTriangles);
            info.numPath = 0;
            std::size_t tIndex{};
            if (info.initialTriangle < numTriangles)
            {
                tIndex = info.initialTriangle;
            }
            else
            {
                info.initialTriangle = 0;
                tIndex = 0;
            }

            for (std::size_t t = 0; t < numTriangles; ++t)
            {
                auto const& v = mTriangles[tIndex];
                auto const& adj = mAdjacencies[tIndex];

                info.finalTriangle = tIndex;
                info.finalV = v;
                info.path[info.numPath++] = tIndex;

                std::size_t i0{}, i1{}, i2{};
                for (i0 = 1, i1 = 2, i2 = 0; i2 < 3; i0 = i1, i1 = i2++)
                {
                    // ToLine(pIndex, v0Index, v1Index) uses mQueryPoint when
                    // pIndex is set to 'invalid'.
                    if (ToLine(invalid, v[i0], v[i1]) > 0)
                    {
                        tIndex = adj[i0];
                        if (tIndex == invalid)
                        {
                            info.finalV[0] = v[i0];
                            info.finalV[1] = v[i1];
                            info.finalV[2] = v[i2];
                            return invalid;
                        }
                        break;
                    }
                }
                if (i2 == 3)
                {
                    return tIndex;
                }
            }
            return invalid;
        }

    private:
        // The minimum-size rational type of the input points.
        using Rational = BSNumber<UIntegerFP32<2>>;
        using IRVector = Vector2<Rational>;

        // The compute type used for exact sign classification.
        static std::size_t constexpr ComputeNumWords = std::is_same<T, float>::value ? 36 : 264;
        using CRational = BSNumber<UIntegerFP32<ComputeNumWords>>;
        using CRVector = Vector2<CRational>;

        // The rectangular domain in which all input points live.
        T mXMin, mYMin, mXMax, mYMax;

        // The current vertices.
        std::map<Vector2<T>, std::size_t> mVertexIndexMap;
        std::vector<Vector2<T>> mVertices;
        std::vector<IRVector> mIRVertices;

        // Support for exact predicates.
        mutable ExactToLine2<T> mETLQuery;
        mutable ExactToCircumcircle2<T> mETCQuery;

        // Wrap the ToLine function for use in retriangulating the removal
        // polygon.
        std::function<std::int32_t(std::size_t, std::size_t, std::size_t)> mToLineWrapper;

        // Support for inserting a point into the triangulation.  The graph
        // is the current triangulation. The mIndex array provides indexing
        using Triangle = DynamicVETManifoldMesh::Triangle;
        using DirectedEdgeKeySet = std::set<EdgeKey<true>>;
        using TrianglePtrSet = std::set<Triangle*>;
        DynamicVETManifoldMesh mGraph;

        // Indexing for the vertices of the triangle adjacent to a vertex.
        // The edge adjacent to vertex j is <mIndex[j][0], mIndex[j][1]> and
        // is listed so that the triangle interior is to your left as you walk
        // around the edges.
        std::array<std::array<std::size_t, 2>, 3> const mIndex;


        // Given a line with origin V0 and direction <V0,V1> and a query
        // point P, ToLine returns
        //   +1, P on right of line
        //   -1, P on left of line
        //    0, P on the line
        std::int32_t ToLine(std::size_t p, std::size_t v0, std::size_t v1) const
        {
            auto const& P = (p != invalid ? mVertices[p] : mQueryPoint);
            auto const& V0 = mVertices[v0];
            auto const& V1 = mVertices[v1];

            auto GetIRVertices = [this, &p, &v0, &v1]()
            {
                return std::array<IRVector const*, 3>
                {
                    (p != invalid ? &mIRVertices[p] : &mIRQueryPoint),
                    &mIRVertices[v0],
                    &mIRVertices[v1]
                };
            };

            return mETLQuery(P, V0, V1, GetIRVertices);
        }

        // For a triangle with counterclockwise vertices V0, V1 and V2 and a
        // query point P, ToCircumcircle returns
        //   +1, P outside circumcircle of triangle
        //   -1, P inside circumcircle of triangle
        //    0, P on circumcircle of triangle
        std::int32_t ToCircumcircle(std::size_t p, std::size_t v0, std::size_t v1, std::size_t v2) const
        {
            auto const& P = mVertices[p];
            auto const& V0 = mVertices[v0];
            auto const& V1 = mVertices[v1];
            auto const& V2 = mVertices[v2];

            auto GetIRVertices = [this, &p, &v0, &v1, &v2]()
            {
                return std::array<IRVector const*, 4>
                {
                    &mIRVertices[p],
                    &mIRVertices[v0],
                    &mIRVertices[v1],
                    &mIRVertices[v2]
                };
            };

            return mETCQuery(P, V0, V1, V2, GetIRVertices);
        }


        template <typename IntegerType>
        inline bool IsDelaunayVertex(IntegerType vIndex) const
        {
            return vIndex >= 3;
        }

        template <typename IntegerType>
        inline bool IsSupervertex(IntegerType vIndex) const
        {
            return vIndex < 3;
        }

        bool GetContainingTriangle(std::size_t pIndex, Triangle*& tri)
        {
            std::size_t const numTriangles = mGraph.GetTriangles().size();
            for (std::size_t t = 0; t < numTriangles; ++t)
            {
                std::size_t j{};
                for (j = 0; j < 3; ++j)
                {
                    std::size_t v0Index = tri->V[mIndex[j][0]];
                    std::size_t v1Index = tri->V[mIndex[j][1]];
                    if (ToLine(pIndex, v0Index, v1Index) > 0)
                    {
                        // Point i sees edge <v0,v1> from outside the triangle.
                        auto adjTri = tri->T[j];
                        if (adjTri)
                        {
                            // Traverse to the triangle sharing the face.
                            tri = adjTri;
                            break;
                        }
                        else
                        {
                            // We reached a hull edge, so the point is outside
                            // the hull.
                            return false;
                        }
                    }

                }

                if (j == 3)
                {
                    // The point is inside all four edges, so the point is
                    // inside a triangle.
                    return true;
                }
            }

            GTL_RUNTIME_ERROR(
                "Unexpected termination of loop while searching for a triangle.");
        }

        void GetAndRemoveInsertionPolygon(std::size_t pIndex,
            TrianglePtrSet& candidates, DirectedEdgeKeySet& boundary)
        {
            // Locate the triangles that make up the insertion polygon.
            DynamicETManifoldMesh polygon{};
            while (candidates.size() > 0)
            {
                Triangle* tri = *candidates.begin();
                candidates.erase(candidates.begin());

                for (std::size_t j = 0; j < 3; ++j)
                {
                    Triangle* adj = tri->T[j];
                    if (adj && candidates.find(adj) == candidates.end())
                    {
                        std::size_t v0Index = adj->V[0];
                        std::size_t v1Index = adj->V[1];
                        std::size_t v2Index = adj->V[2];
                        if (ToCircumcircle(pIndex, v0Index, v1Index, v2Index) <= 0)
                        {
                            // Point P is in the circumcircle.
                            candidates.insert(adj);
                        }
                    }
                }

                auto inserted = polygon.Insert(tri->V[0], tri->V[1], tri->V[2]);
                GTL_RUNTIME_ASSERT(
                    inserted != nullptr,
                    "Unexpected insertion failure.");

                auto removed = mGraph.Remove(tri->V[0], tri->V[1], tri->V[2]);
                GTL_RUNTIME_ASSERT(
                    removed,
                    "Unexpected removal failure.");
            }

            // Get the boundary edges of the insertion polygon.
            for (auto const& element : polygon.GetTriangles())
            {
                Triangle* tri = element.second.get();
                for (std::size_t j = 0; j < 3; ++j)
                {
                    if (!tri->T[j])
                    {
                        EdgeKey<true> ekey(tri->V[mIndex[j][0]], tri->V[mIndex[j][1]]);
                        boundary.insert(ekey);
                    }
                }
            }
        }

        void Update(std::size_t pIndex)
        {
            auto const& tmap = mGraph.GetTriangles();
            Triangle* tri = tmap.begin()->second.get();
            if (GetContainingTriangle(pIndex, tri))
            {
                // The point is inside the convex hull. The insertion polygon
                // contains only triangles in the current triangulation; the
                // hull does not change.

                // Use a depth-first search for those triangles whose
                // circumcircles contain point P.
                TrianglePtrSet candidates{};
                candidates.insert(tri);

                // Get the boundary of the insertion polygon C that contains
                // the triangles whose circumcircles contain point P. Polygon
                // Polygon C contains this point.
                DirectedEdgeKeySet boundary;
                GetAndRemoveInsertionPolygon(pIndex, candidates, boundary);

                // The insertion polygon consists of the triangles formed by
                // point P and the faces of C.
                for (auto const& key : boundary)
                {
                    std::size_t v0Index = key[0];
                    std::size_t v1Index = key[1];
                    if (ToLine(pIndex, v0Index, v1Index) < 0)
                    {
                        auto inserted = mGraph.Insert(pIndex, key[0], key[1]);
                        GTL_RUNTIME_ASSERT(
                            inserted != nullptr,
                            "Unexpected insertion failure.");
                    }
                }
            }
            else
            {
                // The point is outside the convex hull. The insertion
                // polygon is formed by point P and any triangles in the
                // current triangulation whose circumcircles contain point P.

                // Locate the convex hull of the triangles.
                DirectedEdgeKeySet hull{};
                for (auto const& element : tmap)
                {
                    Triangle* t = element.second.get();
                    for (std::size_t j = 0; j < 3; ++j)
                    {
                        if (!t->T[j])
                        {
                            hull.insert(EdgeKey<true>(t->V[mIndex[j][0]], t->V[mIndex[j][1]]));
                        }
                    }
                }

                // Iterate over all the hull edges and use the ones visible to
                // point P to locate the insertion polygon.
                auto const& emap = mGraph.GetEdges();
                TrianglePtrSet candidates{};
                DirectedEdgeKeySet visible{};
                for (auto const& key : hull)
                {
                    std::size_t v0Index = key[0];
                    std::size_t v1Index = key[1];
                    if (ToLine(pIndex, v0Index, v1Index) > 0)
                    {
                        auto iter = emap.find(EdgeKey<false>(key[0], key[1]));
                        GTL_RUNTIME_ASSERT(
                            iter != emap.end() && iter->second->T[1] == nullptr,
                            "This condition should not occur for rational arithmetic.");

                        Triangle* adj = iter->second->T[0];
                        if (adj && candidates.find(adj) == candidates.end())
                        {
                            std::size_t a0Index = adj->V[0];
                            std::size_t a1Index = adj->V[1];
                            std::size_t a2Index = adj->V[2];
                            if (ToCircumcircle(pIndex, a0Index, a1Index, a2Index) <= 0)
                            {
                                // Point P is in the circumcircle.
                                candidates.insert(adj);
                            }
                            else
                            {
                                // Point P is not in the circumcircle but
                                // the hull edge is visible.
                                visible.insert(key);
                            }
                        }
                    }
                }

                // Get the boundary of the insertion subpolygon C that
                // contains the triangles whose circumcircles contain point P.
                DirectedEdgeKeySet boundary{};
                GetAndRemoveInsertionPolygon(pIndex, candidates, boundary);

                // The insertion polygon P consists of the triangles formed by
                // point i and the back edges of C and by the visible edges of
                // mGraph-C.
                for (auto const& key : boundary)
                {
                    std::size_t v0Index = key[0];
                    std::size_t v1Index = key[1];
                    if (ToLine(pIndex, v0Index, v1Index) < 0)
                    {
                        // This is a back edge of the boundary.
                        auto inserted = mGraph.Insert(pIndex, key[0], key[1]);
                        GTL_RUNTIME_ASSERT(
                            inserted != nullptr,
                            "Unexpected insertion failure.");
                    }
                }
                for (auto const& key : visible)
                {
                    auto inserted = mGraph.Insert(pIndex, key[1], key[0]);
                    GTL_RUNTIME_ASSERT(
                        inserted != nullptr,
                        "Unexpected insertion failure.");
                }
            }
        }

    private:
        // Support for triangulating the removal polygon.

        // Let Vc be a vertex in the removal polygon. If Vc is not an ear, its
        // weight is +infinity. If Vc is an ear, let Vp be its predecessor and
        // let Vn be its successor when traversing counterclockwise. Let P be
        // the removal point. The weight is
        //   W = -H(Vp, Vc, Vn, P) / D(Vp, Vc, Vn) = WNumer / WDenom
        // where
        //           +-                -+
        //           | Vp.x  Vc.x  Vn.x |       +-                    -+
        //   D = det | Vp.y  Vc.y  Vn.y | = det | Vc.x-Vp.x  Vn.x-Vp.x |
        //           |   1     1     1  |       | Vc.y-Vp.y  Vn.y-Vp.y |
        //           +-                -+       +-                    -+
        // and
        //           +-                             -+
        //           | Vp.x    Vc.x    Vn.x    P.x   |
        //   H = det | Vp.y    Vc.y    Vn.y    P.y   |
        //           | |Vp|^2  |Vc|^2  |Vn|^2  |P|^2 |
        //           |   1       1       1       1   |
        //           +-                             -+
        //
        //            +-                                          -+
        //            | Vc.x-Vp.x      Vn.x-Vp.x      P.x-Vp.x     |
        //     = -det | Vc.y-Vp.y      Vn.y-Vp.y      P.y-Vp.y     |
        //            | |Vc|^2-|Vp|^2  |Vn|^2-|Vp|^2  |P|^2-|Vp|^2 |
        //            +-                                          -+
        // 
        // To use BSNumber-based rationals, the weight is a ratio stored as a
        // pair (WNumer, WDenom) with WDenom > 0. The comparison of weights is
        // WN0/WD0 < WN1/WD1, implemented as WN0*WD1 < WN1*WD0. Additionally,
        // a Boolean flag isFinite is used to distinguish between a finite
        // ratio (for convex vertices) and a weight that is infinite (for
        // reflex vertices).
        class RPWeight
        {
        public:
            enum class Type
            {
                finite,
                infinite,
                unmodifiable
            };

            RPWeight(Type inType = Type::unmodifiable)
                :
                numerator(0),
                denominator(inType == Type::finite ? 1 : 0),
                type(inType)
            {
            }

            bool operator==(RPWeight const& other) const
            {
                if (type == Type::finite)
                {
                    if (other.type == Type::finite)
                    {
                        return numerator == other.numerator && denominator == other.denominator;
                    }
                    else // other.type is infinite or unmodifiable
                    {
                        return false;
                    }
                }
                else if (type == Type::infinite)
                {
                    if (other.type == Type::finite)
                    {
                        return false;
                    }
                    else  // other.type is infinite or unmodifiable
                    {
                        return other.type == Type::infinite;
                    }
                }
                else  // type is unmodifiable
                {
                    return other.type == Type::unmodifiable;
                }
            }

            bool operator!=(RPWeight const& other) const
            {
                return !operator==(other);
            }

            bool operator<(RPWeight const& other) const
            {
                // finite < infinite < unmodifiable

                if (type == Type::finite)
                {
                    if (other.type == Type::finite)
                    {
                        CRational lhs = numerator * other.denominator;
                        CRational rhs = other.numerator * denominator;
                        return lhs < rhs;
                    }
                    else // other.type is infinite or unmodifiable
                    {
                        return true;
                    }
                }
                else if (type == Type::infinite)
                {
                    if (other.type == Type::finite)
                    {
                        return false;
                    }
                    else  // other.type is infinite or unmodifiable
                    {
                        return other.type == Type::unmodifiable;
                    }
                }
                else  // type is unmodifiable
                {
                    return false;
                }
            }

            bool operator<=(RPWeight const& other) const
            {
                return !other.operator<(*this);
            }

            bool operator>(RPWeight const& other) const
            {
                return other.operator<(*this);
            }

            bool operator>=(RPWeight const& other) const
            {
                return !operator<(other);
            }

            // The finite weight is numerator/denominator with a nonnegative
            // numerator and a positive denominator. If the weight is
            // infinite, the numerator and denominator values are invalid.
            CRational numerator, denominator;
            Type type;
        };

        class RPVertex
        {
        public:
            RPVertex()
                :
                vIndex(invalid),
                isConvex(false),
                iPrev(invalid),
                iNext(invalid),
                key(invalid)
            {
            }

            // The index relative to IncrementalDelaunay2 mVertices[].
            std::size_t vIndex;

            // A vertex is either convex or reflex.
            bool isConvex;

            // Vertex indices for the polygon. These are indices relative to
            // RPPolygon mVertices[].
            std::size_t iPrev, iNext;

            // Support for the priority queue of ears. The key is the
            // first of the pair returned by MinHeap::Insert.
            std::size_t key;
        };

        class RPPolygon
        {
        public:
            RPPolygon(std::vector<std::size_t> const& polygon,
                std::function<std::int32_t(std::size_t, std::size_t, std::size_t)> const& ToLine)
                :
                mNumActive(polygon.size()),
                mVertices(polygon.size())
            {
                // Create a circular list of the polygon vertices for dynamic
                // removal of vertices.
                std::size_t const numVertices = mVertices.size();
                for (std::size_t i = 0; i < numVertices; ++i)
                {
                    RPVertex& vertex = mVertices[i];
                    vertex.vIndex = polygon[i];
                    vertex.iPrev = (i > 0 ? i - 1 : numVertices - 1);
                    vertex.iNext = (i < numVertices - 1 ? i + 1 : 0);
                }

                // Create a linear list of the polygon convex vertices and a
                // linear list of the polygon reflex vertices, both used for
                // dynamic removal of vertices.
                for (std::size_t i = 0; i < numVertices; ++i)
                {
                    // Determine whether the vertex is convex or reflex.
                    std::size_t vPrev = invalid, vCurr = invalid, vNext = invalid;
                    GetTriangle(i, vPrev, vCurr, vNext);
                    mVertices[i].isConvex = (ToLine(vPrev, vCurr, vNext) < 0);
                }
            }

            inline RPVertex const& Vertex(std::size_t i) const
            {
                return mVertices[i];
            }

            inline RPVertex& Vertex(std::size_t i)
            {
                return mVertices[i];
            }

            void GetTriangle(std::size_t i, std::size_t& vPrev, std::size_t& vCurr, std::size_t& vNext) const
            {
                RPVertex const& vertex = mVertices[i];
                vCurr = vertex.vIndex;
                vPrev = mVertices[vertex.iPrev].vIndex;
                vNext = mVertices[vertex.iNext].vIndex;
            }

            void Classify(std::size_t i,
                std::function<std::int32_t(std::size_t, std::size_t, std::size_t)> const& ToLine)
            {
                std::size_t vPrev = invalid, vCurr = invalid, vNext = invalid;
                GetTriangle(i, vPrev, vCurr, vNext);
                mVertices[i].isConvex = (ToLine(vPrev, vCurr, vNext) < 0);
            }

            inline std::size_t GetNumActive() const
            {
                return mNumActive;
            }

            std::size_t GetActive() const
            {
                for (std::size_t i = 0; i < mVertices.size(); ++i)
                {
                    if (mVertices[i].iPrev != invalid)
                    {
                        return i;
                    }
                }

                GTL_RUNTIME_ERROR(
                    "Expecting to find an active vertex.");
            }

            inline void Remove(std::size_t i)
            {
                // Remove the vertex from the polygon.
                RPVertex& vertex = mVertices[i];
                std::size_t iPrev = vertex.iPrev;
                std::size_t iNext = vertex.iNext;
                mVertices[iPrev].iNext = iNext;
                mVertices[iNext].iPrev = iPrev;

                vertex.vIndex = invalid;
                vertex.isConvex = false;
                vertex.iPrev = invalid;
                vertex.iNext = invalid;
                vertex.key = invalid;

                --mNumActive;
            }

        private:
            std::size_t mNumActive;
            std::vector<RPVertex> mVertices;
        };

        RPWeight ComputeWeight(std::size_t iConvexIndex, std::size_t vRemovalIndex,
            RPPolygon& rpPolygon)
        {
            // Get the triangle <VP,VC,VN> with convex vertex VC.
            std::size_t vPrev = invalid, vCurr = invalid, vNext = invalid;
            rpPolygon.GetTriangle(iConvexIndex, vPrev, vCurr, vNext);

            auto const& irVP = mIRVertices[vPrev];
            auto const& irVC = mIRVertices[vCurr];
            auto const& irVN = mIRVertices[vNext];
            auto const& irPR = mIRVertices[vRemovalIndex];

            CRVector VP{}, VC{}, VN{}, PR{};
            VP[0] = irVP[0];
            VP[1] = irVP[1];
            VC[0] = irVC[0];
            VC[1] = irVC[1];
            VN[0] = irVN[0];
            VN[1] = irVN[1];
            PR[0] = irPR[0];
            PR[1] = irPR[1];

            auto subVCVP = VC - VP;
            auto subVNVP = VN - VP;
            auto subPRVP = PR - VP;
            auto addVCVP = VC + VP;
            auto addVNVP = VN + VP;
            auto addPRVP = PR + VP;
            auto c20 = DotPerp(subVNVP, subPRVP);
            auto c21 = DotPerp(subPRVP, subVCVP);
            auto c22 = DotPerp(subVCVP, subVNVP);
            auto a20 = Dot(subVCVP, addVCVP);
            auto a21 = Dot(subVNVP, addVNVP);
            auto a22 = Dot(subPRVP, addPRVP);

            RPWeight weight(RPWeight::Type::finite);
            weight.numerator = a20 * c20 + a21 * c21 + a22 * c22;
            weight.numerator.Negate();
            weight.denominator = std::move(c22);
            if (weight.denominator.GetSign() < 0)
            {
                weight.numerator.Negate();
                weight.denominator.Negate();
            }
            return weight;
        }

        void DoEarClipping(MinHeap<RPWeight>& earHeap,
            std::function<RPWeight(std::size_t)> const& WeightFunction,
            RPPolygon& rpPolygon)
        {
            // Remove the finite-weight vertices from the priority queue,
            // one at a time.
            std::size_t handle = 0;
            RPWeight weight{};
            while (earHeap.GetNumElements() >= 3)
            {
                // Get the ear of minimum weight. The vertex at index i must
                // be convex.
                (void)earHeap.GetMinimum(handle, weight);
                if (weight.type != RPWeight::Type::finite)
                {
                    break;
                }
                (void)earHeap.Remove(handle, weight);

                // Get the triangle associated with the ear.
                std::size_t vPrev = invalid, vCurr = invalid, vNext = invalid;
                rpPolygon.GetTriangle(handle, vPrev, vCurr, vNext);

                // Insert the triangle into the graph.
                auto inserted = mGraph.Insert(vPrev, vCurr, vNext);
                GTL_RUNTIME_ASSERT(
                    inserted != nullptr,
                    "Unexpected insertion failure.");
                if (earHeap.GetNumElements() < 3)
                {
                    earHeap.Reset(0);
                    break;
                }

                // Remove the vertex from the polygon. The previous and next
                // neighbor indices are required to update the adjacent
                // vertices after the removal.
                RPVertex const& vertex = rpPolygon.Vertex(handle);
                std::size_t iPrev = vertex.iPrev;
                std::size_t iNext = vertex.iNext;
                rpPolygon.Remove(handle);

                // Removal of the ear can cause an adjacent vertex to become
                // an ear or to stop being an ear.
                RPVertex& vertexP = rpPolygon.Vertex(iPrev);
                bool wasConvex = vertexP.isConvex;
                rpPolygon.Classify(iPrev, mToLineWrapper);
                bool nowConvex = vertexP.isConvex;
                if (wasConvex)
                {
                    // The 'vertex' is convex. If 'vertexP' was convex, it
                    // cannot become reflex after the ear is clipped.
                    GTL_RUNTIME_ASSERT(
                        nowConvex,
                        "Unexpected condition.");

                    if (earHeap.GetWeight(vertexP.key).type != RPWeight::Type::unmodifiable)
                    {
                        earHeap.Update(vertexP.key, WeightFunction(iPrev));
                    }
                }
                else // 'vertexP' was reflex
                {
                    if (nowConvex)
                    {
                        if (earHeap.GetWeight(vertexP.key).type != RPWeight::Type::unmodifiable)
                        {
                            earHeap.Update(vertexP.key, WeightFunction(iPrev));
                        }
                    }
                }

                RPVertex& vertexN = rpPolygon.Vertex(iNext);
                wasConvex = vertexN.isConvex;
                rpPolygon.Classify(iNext, mToLineWrapper);
                nowConvex = vertexN.isConvex;
                if (wasConvex)
                {
                    // The 'vertex' is convex. If 'vertexN' was convex, it
                    // cannot become reflex after the ear is clipped.
                    GTL_RUNTIME_ASSERT(
                        nowConvex,
                        "Unexpected condition.");

                    if (earHeap.GetWeight(vertexN.key).type != RPWeight::Type::unmodifiable)
                    {
                        earHeap.Update(vertexN.key, WeightFunction(iNext));
                    }
                }
                else // 'vertexN' was reflex
                {
                    if (nowConvex)
                    {
                        if (earHeap.GetWeight(vertexN.key).type != RPWeight::Type::unmodifiable)
                        {
                            earHeap.Update(vertexN.key, WeightFunction(iNext));
                        }
                    }
                }
            }
        }

        void DeleteRemovalPolygon(std::size_t vRemovalIndex,
            std::unordered_set<Triangle*> const& adjacents,
            std::vector<std::size_t>& polygon)
        {
            // Get the edges of the removal polygon. The polygon is star
            // shaped relative to the removal position.
            std::map<std::size_t, std::size_t> edges{};
            for (auto const& adj : adjacents)
            {
                std::size_t i{};
                for (i = 0; i < 3; ++i)
                {
                    if (vRemovalIndex == adj->V[i])
                    {
                        break;
                    }
                }
                GTL_RUNTIME_ASSERT(
                    i < 3,
                    "Unexpected condition.");

                std::size_t opposite1 = adj->V[(i + 1) % 3];
                std::size_t opposite2 = adj->V[(i + 2) % 3];
                edges.insert(std::make_pair(opposite1, opposite2));
            }

            // Remove the triangles.
            for (auto const& edge : edges)
            {
                bool removed = mGraph.Remove(vRemovalIndex, edge.first, edge.second);
                GTL_RUNTIME_ASSERT(
                    removed,
                    "Unexpected removal failure.");
            }

            // Create the removal polygon; its vertices are counterclockwise
            // ordered.
            polygon.reserve(edges.size());
            polygon.clear();
            std::size_t vStart = edges.begin()->first;
            std::size_t vCurr = edges.begin()->second;
            polygon.push_back(vStart);
            while (vCurr != vStart)
            {
                polygon.push_back(vCurr);
                auto eIter = edges.find(vCurr);
                GTL_RUNTIME_ASSERT(
                    eIter != edges.end(),
                    "Unexpected condition.");

                vCurr = eIter->second;
            }
        }

        void RetriangulateInteriorRemovalPolygon(std::size_t vRemovalIndex,
            std::vector<std::size_t> const& polygon)
        {
            // Create a representation of 'polygon' that can be processed
            // using a priority queue.
            RPPolygon rpPolygon(polygon, mToLineWrapper);

            auto WeightFunction = [this, &rpPolygon, vRemovalIndex](std::size_t i)
            {
                return ComputeWeight(i, vRemovalIndex, rpPolygon);
            };

            // Create a priority queue of vertices. Convex vertices have a
            // finite and positive weight. Reflex vertices have a weight of
            // +infinity.
            MinHeap<RPWeight> earHeap(polygon.size());
            RPWeight const posInfinity(RPWeight::Type::infinite);
            RPWeight weight{};
            for (std::size_t i = 0; i < polygon.size(); ++i)
            {
                RPVertex& vertex = rpPolygon.Vertex(i);
                if (vertex.isConvex)
                {
                    weight = WeightFunction(i);
                }
                else
                {
                    weight = posInfinity;
                }
                vertex.key = earHeap.Insert(i, weight);
            }

            // Remove the finite-weight vertices from the priority queue,
            // one at a time.
            DoEarClipping(earHeap, WeightFunction, rpPolygon);
            GTL_RUNTIME_ASSERT(
                earHeap.GetNumElements() == 0,
                "Expecting the hole to be completely filled.");
        }

        void RetriangulateBoundaryRemovalPolygon(std::size_t vRemovalIndex,
            std::vector<std::size_t> const& polygon)
        {
            std::size_t const numPolygon = polygon.size();
            if (numPolygon >= 3)
            {
                // Create a representation of 'polygon' that can be processed
                // using a priority queue.
                RPPolygon rpPolygon(polygon, mToLineWrapper);

                auto WeightFunction = [this, &rpPolygon, vRemovalIndex](std::size_t i)
                {
                    return ComputeWeight(i, vRemovalIndex, rpPolygon);
                };

                auto ZeroWeightFunction = [](std::size_t = 0)
                {
                    return RPWeight(RPWeight::Type::finite);
                };

                // Create a priority queue of vertices. The removal index
                // (polygon[0] = vRemovalIndex) and its two vertex neighbors
                // have a weight of +infinity. Of the other vertices, convex
                // vertices have a finite and positive weight and reflex
                // vertices have a weight of +infinity.
                MinHeap<RPWeight> earHeap(polygon.size());
                RPWeight const rigid(RPWeight::Type::unmodifiable);
                RPWeight const posInfinity(RPWeight::Type::infinite);

                std::size_t iPrev = numPolygon - 2, iCurr = iPrev + 1, iNext = 0;
                for (; iNext < numPolygon; iPrev = iCurr, iCurr = iNext, ++iNext)
                {
                    RPVertex& vertexPrev = rpPolygon.Vertex(iPrev);
                    RPVertex& vertexCurr = rpPolygon.Vertex(iCurr);
                    RPVertex& vertexNext = rpPolygon.Vertex(iNext);
                    if (IsSupervertex(vertexPrev.vIndex) ||
                        IsSupervertex(vertexCurr.vIndex) ||
                        IsSupervertex(vertexNext.vIndex))
                    {
                        vertexCurr.key = earHeap.Insert(iCurr, rigid);
                    }
                    else
                    {
                        if (vertexCurr.isConvex)
                        {
                            vertexCurr.key = earHeap.Insert(iCurr, WeightFunction(iCurr));
                        }
                        else
                        {
                            vertexCurr.key = earHeap.Insert(iCurr, posInfinity);
                        }
                    }
                }

                // Remove the finite-weight vertices from the priority queue,
                // one at a time. This process fills in the subpolygon of the
                // removal polygon that is contained by the Delaunay
                // triangulation.
                DoEarClipping(earHeap, WeightFunction, rpPolygon);

                // Get the subpolygon of the removal polygon that is external
                // to the Delaunay triangulation.
                std::size_t numExternal = rpPolygon.GetNumActive();
                std::vector<std::size_t> external(numExternal);
                iCurr = rpPolygon.GetActive();
                for (std::size_t i = 0; i < numExternal; ++i)
                {
                    external[i] = iCurr;
                    rpPolygon.Classify(iCurr, mToLineWrapper);
                    iCurr = rpPolygon.Vertex(iCurr).iNext;
                }

                earHeap.Reset(numExternal);
                for (std::size_t i = 0; i < numExternal; ++i)
                {
                    std::size_t index = external[i];
                    RPVertex& vertex = rpPolygon.Vertex(index);
                    if (IsSupervertex(vertex.vIndex))
                    {
                        vertex.key = earHeap.Insert(index, rigid);
                    }
                    else
                    {
                        if (vertex.isConvex)
                        {
                            vertex.key = earHeap.Insert(index, ZeroWeightFunction());
                        }
                        else
                        {
                            vertex.key = earHeap.Insert(index, posInfinity);
                        }
                    }
                }

                // Remove the finite-weight vertices from the priority queue,
                // one at a time. This process fills in a portion or all of
                // the subpolygon of the removal polygon that is external to
                // the Delaunay triangulation.
                DoEarClipping(earHeap, ZeroWeightFunction, rpPolygon);
                if (earHeap.GetNumElements() == 0)
                {
                    // The external polygon contained only 1 supervertex.
                    return;
                }

                // The remaining external polygon is a triangle fan with
                // 2 or 3 supervertices.
                numExternal = rpPolygon.GetNumActive();
                external.resize(numExternal);
                iCurr = rpPolygon.GetActive();
                for (std::size_t i = 0; i < numExternal; ++i)
                {
                    external[i] = iCurr;
                    rpPolygon.Classify(iCurr, mToLineWrapper);
                    iCurr = rpPolygon.Vertex(iCurr).iNext;
                }

                earHeap.Reset(numExternal);
                iPrev = numExternal - 2;
                iCurr = iPrev + 1;
                iNext = 0;
                for (; iNext < numExternal; iPrev = iCurr, iCurr = iNext, ++iNext)
                {
                    std::size_t index = external[iCurr];
                    RPVertex& vertexPrev = rpPolygon.Vertex(external[iPrev]);
                    RPVertex& vertexCurr = rpPolygon.Vertex(index);
                    RPVertex& vertexNext = rpPolygon.Vertex(external[iNext]);
                    if (IsSupervertex(vertexCurr.vIndex))
                    {
                        if (IsDelaunayVertex(vertexPrev.vIndex) ||
                            IsDelaunayVertex(vertexNext.vIndex))
                        {
                            GTL_RUNTIME_ASSERT(
                                vertexCurr.isConvex,
                                "Unexpected condition.");

                            vertexCurr.key = earHeap.Insert(index, ZeroWeightFunction());
                        }
                        else
                        {
                            vertexCurr.key = earHeap.Insert(index, rigid);
                        }
                    }
                    else
                    {
                        vertexCurr.key = earHeap.Insert(index, posInfinity);
                    }
                }

                // Remove the finite-weight vertices from the priority queue,
                // one at a time. This process fills in the triangle fan of
                // the subpolygon of the removal polygon that is external to
                // the Delaunay triangulation.
                DoEarClipping(earHeap, ZeroWeightFunction, rpPolygon);
                GTL_RUNTIME_ASSERT(
                    earHeap.GetNumElements() == 0,
                    "Expecting the hole to be completely filled.");
            }
            else // numPolygon == 2
            {
                std::size_t vOtherIndex = (polygon[0] == vRemovalIndex ? polygon[1] : polygon[0]);
                mGraph.Clear();
                for (std::size_t i0 = 2, i1 = 0; i1 < 3; i0 = i1++)
                {
                    auto inserted = mGraph.Insert(vOtherIndex, i0, i1);
                    GTL_RUNTIME_ASSERT(
                        inserted != nullptr,
                        "Unexpected insertion failure.");
                }
            }
        }

    private:
        // Support for queries associated with the mesh of Delaunay triangles.
        mutable std::vector<std::array<std::size_t, 3>> mTriangles;
        mutable std::vector<std::array<std::size_t, 3>> mAdjacencies;
        mutable bool mTrianglesAndAdjacenciesNeedUpdate;
        mutable Vector2<T> mQueryPoint;
        mutable IRVector mIRQueryPoint;

        void UpdateTrianglesAndAdjacencies() const
        {
            // Assign integer values to the triangles.
            auto const& tmap = mGraph.GetTriangles();
            if (tmap.size() == 0)
            {
                return;
            }

            std::unordered_map<Triangle*, std::size_t> permute{};
            permute[nullptr] = invalid;
            std::size_t numTriangles = 0;
            for (auto const& element : tmap)
            {
                if (IsDelaunayVertex(element.first[0]) &&
                    IsDelaunayVertex(element.first[1]) &&
                    IsDelaunayVertex(element.first[2]))
                {
                    permute[element.second.get()] = numTriangles++;
                }
                else
                {
                    permute[element.second.get()] = invalid;
                }
            }

            mTriangles.resize(numTriangles);
            mAdjacencies.resize(numTriangles);
            std::size_t t = 0;
            for (auto const& element : tmap)
            {
                auto const& tri = element.second;
                if (permute[element.second.get()] != invalid)
                {
                    for (std::size_t j = 0; j < 3; ++j)
                    {
                        mTriangles[t][j] = tri->V[j];
                        mAdjacencies[t][j] = permute[tri->T[j]];
                    }
                    ++t;
                }
            }
        }

    private:
        friend class UnitTestIncrementalDelaunay2;
    };
}
