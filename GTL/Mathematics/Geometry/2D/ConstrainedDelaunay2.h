// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

// Compute the Delaunay triangulation of the input point and then insert
// edges that are constrained to be in the triangulation. For each such
// edge, a retriangulation of the triangle strip containing the edge is
// required. NOTE: If two constrained edges overlap at a point that is
// an interior point of each edge, the second insertion will interfere
// with the retriangulation of the first edge. Although the code here
// will do what is requested, a pair of such edges usually indicates the
// upstream process that generated the edges is not doing what it should.
//
// The input type T must satisfy std::is_floating_point<T>::value = true.

#include <GTL/Mathematics/Geometry/2D/Delaunay2.h>
#include <GTL/Mathematics/Geometry/2D/ExactToLine2.h>
#include <GTL/Mathematics/Meshes/DynamicVETManifoldMesh.h>
#include <array>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <limits>
#include <type_traits>
#include <unordered_set>
#include <vector>

namespace gtl
{
    template <typename T>
    class ConstrainedDelaunay2 : public Delaunay2<T>
    {
    public:
        // The class is a functor to support computing the constrained
        // Delaunay triangulation of multiple data sets using the same class
        // object.
        virtual ~ConstrainedDelaunay2() = default;

        ConstrainedDelaunay2()
            :
            Delaunay2<T>(),
            mInsertedEdges{},
            mCDTMesh{},
            mNode(numNodes)
        {
            static_assert(
                std::is_floating_point<T>::value,
                "The input type must be a floating-point type.");
        }

        // This operator computes the Delaunay triangulation only. Edges are
        // inserted later.
        void operator()(std::size_t numPoints, Vector2<T> const* points)
        {
            InitialTriangulation(numPoints, points);
        }

        void operator()(std::vector<Vector2<T>> const& points)
        {
            InitialTriangulation(points.size(), points.data());
        }

        // The 'edge' is the constrained edge to be inserted into the
        // triangulation. If that edge is already in the triangulation, the
        // function returns without any retriangulation and 'partitionedEdge'
        // contains the input 'edge'. If 'edge' is coincident with 1 or more
        // edges already in the triangulation, 'edge' is partitioned into
        // subedges which are then inserted. It is also possible that 'edge'
        // does not overlap already existing edges in the triangulation but
        // has interior points that are vertices in the triangulation; in
        // this case, 'edge' is partitioned and the subedges are inserted.
        // In either case, 'partitionEdge' is an ordered list of indices
        // into the triangulation vertices that are on the edge. It is
        // guaranteed that partitionedEdge.front() = edge[0] and
        // partitionedEdge.back() = edge[1].
        void Insert(std::array<std::size_t, 2> edge, std::vector<std::size_t>& partitionedEdge)
        {
            edge[0] = this->mEquivalentTo[edge[0]];
            edge[1] = this->mEquivalentTo[edge[1]];
            GTL_ARGUMENT_ASSERT(
                edge[0] != edge[1] &&
                0 <= edge[0] && edge[0] < this->mNumPoints &&
                0 <= edge[1] && edge[1] < this->mNumPoints,
                "Invalid argument.");

            // The partitionedEdge vector stores the endpoints of the incoming
            // edge if that edge does not contain interior points that are
            // vertices of the Delaunay triangulation. If the edge contains
            // one or more vertices in its interior, the edge is partitioned
            // into subedges, each subedge having vertex endpoints but no
            // interior point is a vertex. The partition is stored in the
            // partitionedEdge vector.
            std::vector<std::array<std::size_t, 2>> partition{};

            // When using exact arithmetic, a while(!edgeConsumed) loop
            // suffices. Just in case the code has a bug, guard against an
            // infinite loop.
            bool edgeConsumed = false;
            std::size_t const numTriangles = mCDTMesh.GetTriangles().size();
            for (std::size_t t = 0; t < numTriangles && !edgeConsumed; ++t)
            {
                EdgeKey<false> ekey(edge[0], edge[1]);
                if (mCDTMesh.GetEdges().find(ekey) != mCDTMesh.GetEdges().end())
                {
                    // The edge already exists in the triangulation.
                    mInsertedEdges.insert(ekey);
                    partition.push_back(edge);
                    break;
                }

                // Get the link edges for the vertex edge[0]. These edges are
                // opposite the link vertex.
                std::vector<std::array<std::size_t, 2>> linkEdges{};
                GetLinkEdges(edge[0], linkEdges);

                // Determine which link triangle contains the to-be-inserted
                // edge.
                for (auto const& linkEdge : linkEdges)
                {
                    // Compute on which side of the to-be-inserted edge the
                    // link vertices live. The triangles are not degenerate,
                    // so it is not possible for sign0 = sign1 = 0.
                    std::size_t e0Index = edge[0];
                    std::size_t e1Index = edge[1];
                    std::size_t v0Index = linkEdge[0];
                    std::size_t v1Index = linkEdge[1];
                    std::int32_t sign0 = this->ToLine(v0Index, e0Index, e1Index);
                    std::int32_t sign1 = this->ToLine(v1Index, e0Index, e1Index);
                    if (sign0 >= 0 && sign1 <= 0)
                    {
                        if (sign0 > 0)
                        {
                            if (sign1 < 0)
                            {
                                // The triangle <edge[0], v0, v1> strictly
                                // contains the to-be-inserted edge. Gather
                                // the triangles in the triangle strip
                                // containing the edge.
                                edgeConsumed = ProcessTriangleStrip(edge, v0Index, v1Index, partition);
                            }
                            else  // sign1 == 0 && sign0 > 0
                            {
                                // The to-be-inserted edge is coincident with
                                // the triangle edge <edge[0], v1>, and it is
                                // guaranteed that the vertex at v1 is an
                                // interior point of <edge[0],edge[1]> because
                                // we previously tested whether edge[] is in
                                // the triangulation.
                                edgeConsumed = ProcessCoincidentEdge(edge, v1Index, partition);
                            }
                        }
                        else  // sign0 == 0 && sign1 < 0
                        {
                            // The to-be-inserted edge is coincident with
                            // the triangle edge <edge[0], v0>, and it is
                            // guaranteed that the vertex at v0 is an
                            // interior point of <edge[0],edge[1]> because
                            // we previously tested whether edge[] is in
                            // the triangulation.
                            edgeConsumed = ProcessCoincidentEdge(edge, v0Index, partition);
                        }
                        break;
                    }
                }
            }

            // With a correct implementation, this GTL_RUNTIME_ASSERT should not
            // trigger.
            GTL_RUNTIME_ASSERT(
                partition.size() > 0,
                "Unexpected condition.");

            partitionedEdge.resize(partition.size() + 1);
            for (std::size_t i = 0; i < partition.size(); ++i)
            {
                partitionedEdge[i] = partition[i][0];
            }
            partitionedEdge.back() = partition.back()[1];
        }

        // All edges inserted via the Insert(...) call are stored for use
        // by the caller. If any edge passed to Insert(...) is partitioned
        // into subedges, the subedges are stored but not the original edge.
        using EdgeKeySet = std::unordered_set<EdgeKey<false>, EdgeKey<false>, EdgeKey<false>>;

        inline EdgeKeySet const& GetInsertedEdges() const
        {
            return mInsertedEdges;
        }

        // NOTE: The Delaunay interface functions GetIndices() and GetMesh()
        // are accessible through ConstrainedDelaunay2. The triangles
        // represented by these are for the original Delaunay2 triangulation,
        // but the ConstrainedDelaunay2 triangulation is not the same as the
        // Delaunay2 triangulation. To access the ConstrainedDelaunay2
        // triangles, use GetConstrainedTriangles() and GetConstrainedMesh().

        void GetConstrainedTriangles(std::vector<std::array<std::size_t, 3>>& triangles) const
        {
            auto const& tmap = mCDTMesh.GetTriangles();
            triangles.resize(tmap.size());
            std::size_t t = 0;
            for (auto const& element : tmap)
            {
                auto const& tkey = element.first;
                triangles[t++] = tkey;
            }
        }

        inline DynamicVETManifoldMesh const& GetConstrainedMesh() const
        {
            return mCDTMesh;
        }

        inline DynamicVETManifoldMesh& GetConstrainedMesh()
        {
            return mCDTMesh;
        }

    private:
        // The type of the read-only input vertices[] when converted for
        // rational arithmetic.
        using Rational = typename Delaunay2<T>::Rational;

        // The compute type used for exact sign classification.
        static std::size_t constexpr numWords = std::is_same<T, float>::value ? 70 : 526;
        using CRational = BSNumber<UIntegerFP32<numWords>>;

        void InitialTriangulation(std::size_t numPoints, Vector2<T> const* points)
        {
            Delaunay2<T>::operator()(numPoints, points);
            GTL_RUNTIME_ASSERT(
                this->GetDimension() == 2,
                "Currently, CDT does not support degenerate datasets.");

            mInsertedEdges.clear();
            mCDTMesh.Clear();

            auto const& indices = this->GetIndices();
            std::size_t const numTriangles = indices.size() / 3;
            for (std::size_t t = 0, i = 0; t < numTriangles; ++t)
            {
                std::size_t v0 = indices[i++];
                std::size_t v1 = indices[i++];
                std::size_t v2 = indices[i++];
                mCDTMesh.Insert(v0, v1, v2);
            }
        }

        // For a vertex at index v, return the edges of the adjacent
        // triangles, each triangle having v as a vertex and the returned
        // edge is opposite v. With a correct implementation, the
        // GTL_RUNTIME_ASSERTs should not trigger.
        void GetLinkEdges(std::size_t v, std::vector<std::array<std::size_t, 2>>& linkEdges)
        {
            auto const& vmap = mCDTMesh.GetVertices();
            auto viter = vmap.find(v);
            GTL_RUNTIME_ASSERT(
                viter != vmap.end(),
                "Unexpected condition.");

            auto const& vertex = viter->second;
            GTL_RUNTIME_ASSERT(
                vertex != nullptr,
                "Unexpected condition.");

            for (auto const& linkTri : vertex->TAdjacent)
            {
                std::size_t j{};
                for (j = 0; j < 3; ++j)
                {
                    if (linkTri->V[j] == vertex->V)
                    {
                        linkEdges.push_back({
                            linkTri->V[(j + 1) % 3], linkTri->V[(j + 2) % 3] });
                        break;
                    }
                }
                GTL_RUNTIME_ASSERT(
                    j < 3,
                    "Unexpected condition.");
            }
        }

        // The return value is 'true' if the edge did not have to be
        // subdivided because it has an interior point that is a vertex.
        // The return value is 'false' if it does have such a point, in
        // which case edge[0] is updated to the index of that vertex. The
        // caller must process the new edge.
        bool ProcessTriangleStrip(std::array<std::size_t, 2>& edge, std::size_t v0,
            std::size_t v1, std::vector<std::array<std::size_t, 2>>& partitionedEdge)
        {
            // With a correct implementation, none of the LogRuntimeErrors
            // should trigger.
            bool edgeConsumed = true;
            std::array<std::size_t, 2> localEdge = edge;

            // Locate and store the triangles in the triangle strip containing
            // the edge.
            DynamicETManifoldMesh tristrip{};
            tristrip.Insert(localEdge[0], v0, v1);

            auto const& tmap = mCDTMesh.GetTriangles();
            auto titer = tmap.find(TriangleKey<true>(localEdge[0], v0, v1));
            GTL_RUNTIME_ASSERT(
                titer != tmap.end(),
                "Unexpected condition.");

            DynamicETManifoldMesh::Triangle* tri = titer->second.get();
            GTL_RUNTIME_ASSERT(
                tri != nullptr,
                "Unexpected condition.");

            // Keep track of the right and left polylines that bound the
            // triangle strip. These polylines can have coincident edges.
            // In particular, this happens when the current triangle in the
            // strip shares an edge with a previous triangle in the strip
            // and the previous triangle is not the immediate predecessor
            // to the current triangle.
            std::vector<std::size_t> rightPolygon{}, leftPolygon{};
            rightPolygon.push_back(localEdge[0]);
            rightPolygon.push_back(v0);
            leftPolygon.push_back(localEdge[0]);
            leftPolygon.push_back(v1);

            // When using exact arithmetic, a for(;;) loop suffices. When
            // using floating-point arithmetic (which you should really not
            // do for CDT), guard against an infinite loop.
            std::size_t const numTriangles = tmap.size();
            std::size_t t{};
            for (t = 0; t < numTriangles; ++t)
            {
                // The current triangle is tri and has edge <v0,v1>. Get
                // the triangle adj that is adjacent to tri via this edge.
                auto adj = tri->GetAdjacentOfEdge(v0, v1);
                GTL_RUNTIME_ASSERT(
                    adj != nullptr,
                    "Unexpected condition.");

                tristrip.Insert(adj->V[0], adj->V[1], adj->V[2]);

                // Get the vertex of adj that is opposite edge <v0,v1>.
                std::size_t vOpposite = 0;
                bool found = adj->GetOppositeVertexOfEdge(v0, v1, vOpposite);
                GTL_RUNTIME_ASSERT(
                    found,
                    "Unexpected condition.");

                if (vOpposite == localEdge[1])
                {
                    // The triangle strip containing the edge is complete.
                    break;
                }

                // The next triangle in the strip depends on whether the
                // opposite vertex is left-of the edge, right-of the edge
                // or on the edge.
                std::int32_t querySign = this->ToLine(vOpposite, localEdge[0], localEdge[1]);
                if (querySign > 0)
                {
                    tri = adj;
                    v0 = vOpposite;
                    rightPolygon.push_back(v0);
                }
                else if (querySign < 0)
                {
                    tri = adj;
                    v1 = vOpposite;
                    leftPolygon.push_back(v1);
                }
                else
                {
                    // The to-be-inserted edge contains an interior point that
                    // is also a vertex in the triangulation. The edge must be
                    // subdivided. The first subedge is in a triangle strip
                    // that is processed by code below that is outside the
                    // loop. The second subedge must be processed by the
                    // caller.
                    localEdge[1] = vOpposite;
                    edge[0] = vOpposite;
                    edgeConsumed = false;
                    break;
                }
            }
            GTL_RUNTIME_ASSERT(
                t < numTriangles,
                "Unexpected condition.");

            // Insert the final endpoint of the to-be-inserted edge.
            rightPolygon.push_back(localEdge[1]);
            leftPolygon.push_back(localEdge[1]);

            // The retriangulation depends on counterclockwise ordering of
            // the boundary right and left polygons. The right polygon is
            // already counterclockwise ordered. The left polygon is
            // clockwise ordered, so reverse it.
            std::reverse(leftPolygon.begin(), leftPolygon.end());

            // Update the inserted edges.
            mInsertedEdges.insert(EdgeKey<false>(localEdge[0], localEdge[1]));
            partitionedEdge.push_back(localEdge);

            // Remove the triangle strip from the full triangulation. This
            // must occur before the retriangulation which inserts new
            // triangles into the full triangulation.
            for (auto const& element : tristrip.GetTriangles())
            {
                auto const& tkey = element.first;
                mCDTMesh.Remove(tkey[0], tkey[1], tkey[2]);
            }

            // Retriangulate the tristrip region.
            Retriangulate(leftPolygon);
            Retriangulate(rightPolygon);

            return edgeConsumed;
        }

        // Process a to-be-inserted edge that is coincident with an already
        // existing triangulation edge.
        bool ProcessCoincidentEdge(std::array<std::size_t, 2>& edge, std::size_t v,
            std::vector<std::array<std::size_t, 2>>& partitionedEdge)
        {
            mInsertedEdges.insert(EdgeKey<false>(edge[0], v));
            partitionedEdge.push_back({ edge[0], v });
            edge[0] = v;
            bool edgeConsumed = (v == edge[1]);
            return edgeConsumed;
        }

        // Retriangulate the polygon via a bisection-like method that finds
        // vertices closest to the current polygon base edge. The function
        // is naturally recursive, but simulated recursion is used to avoid
        // a large program stack by instead using the heap.
        void Retriangulate(std::vector<std::size_t> const& polygon)
        {
            std::vector<std::array<std::size_t, 2>> stack(polygon.size());
            std::size_t top = std::numeric_limits<std::size_t>::max();
            stack[++top] = { 0, polygon.size() - 1 };
            while (top != std::numeric_limits<std::size_t>::max())
            {
                std::array<std::size_t, 2> i = stack[top--];
                if (i[1] > i[0] + 1)
                {
                    // Get the vertex indices for the specified i-values.
                    std::size_t const v0 = polygon[i[0]];
                    std::size_t const v1 = polygon[i[1]];

                    // Select isplit in the index range [i[0]+1,i[1]-1] so
                    // that the vertex at index polygon[isplit] attains the
                    // minimum distance to the edge with vertices at the
                    // indices polygon[i[0]] and polygon[i[1]].
                    std::size_t isplit = SelectSplit(polygon, i[0], i[1]);
                    std::size_t vsplit = polygon[isplit];

                    // Insert the triangle into the Delaunay graph.
                    mCDTMesh.Insert(v0, vsplit, v1);

                    stack[++top] = { i[0], isplit };
                    stack[++top] = { isplit, i[1] };
                }
            }
        }

        // Determine the polygon vertex with index strictly between i0 and i1
        // that minimizes the pseudosquared distance from that vertex to the
        // line segment whose endpoints are at indices i0 and i1.
        std::size_t SelectSplit(std::vector<std::size_t> const& polygon, std::size_t i0, std::size_t i1)
        {
            std::size_t i2{};
            if (i1 == i0 + 2)
            {
                // This is the only candidate.
                i2 = i0 + 1;
            }
            else  // i1 - i0 > 2
            {
                // Select the index i2 in [i0+1,i1-1] for which the distance
                // from the vertex v2 at i2 to the edge <v0,v1> is minimized.
                // To allow exact arithmetic, use a pseudosquared distance
                // that avoids divisions and square roots.
                i2 = i0 + 1;
                std::size_t const v0 = polygon[i0];
                std::size_t const v1 = polygon[i1];
                std::size_t v2 = polygon[i2];

                // Precompute some common values that are used in all calls
                // to ComputePSD.
                auto const& irV0 = this->GetRPoint(v0);
                auto const& irV1 = this->GetRPoint(v1);
                auto const& irV2 = this->GetRPoint(v2);
                auto const& crV0x = (mNode[0] = irV0[0]);
                auto const& crV0y = (mNode[1] = irV0[1]);
                auto const& crV1x = (mNode[2] = irV1[0]);
                auto const& crV1y = (mNode[3] = irV1[1]);
                auto const& crV2x = (mNode[4] = irV2[0]);
                auto const& crV2y = (mNode[5] = irV2[1]);
                auto& crV1mV0x = mNode[6];
                auto& crV1mV0y = mNode[7];
                auto& crSqrLen10 = mNode[8];
                auto& crPSD = mNode[9];
                auto& crMinPSD = mNode[10];

                crV1mV0x = crV1x - crV0x;
                crV1mV0y = crV1y - crV0y;
                crSqrLen10 = crV1mV0x * crV1mV0x + crV1mV0y * crV1mV0y;

                // Locate the minimum pseudosquared distance.
                ComputePSD(crV0x, crV0y, crV1x, crV1y, crV2x, crV2y,
                    crV1mV0x, crV1mV0y, crSqrLen10, crMinPSD);
                for (std::size_t i = i2 + 1; i < i1; ++i)
                {
                    v2 = polygon[i];
                    auto const& irNextV2 = this->GetRPoint(v2);
                    mNode[4] = irNextV2[0];
                    mNode[5] = irNextV2[1];
                    ComputePSD(crV0x, crV0y, crV1x, crV1y, crV2x, crV2y,
                        crV1mV0x, crV1mV0y, crSqrLen10, crPSD);
                    if (crPSD < crMinPSD)
                    {
                        crMinPSD = crPSD;
                        i2 = i;
                    }
                }
            }
            return i2;
        }

        // Compute a pseudosquared distance from the vertex at v2 to the edge
        // <v0,v1>. The result is exact for rational arithmetic and does not
        // involve division. This allows ComputeType to be BSNumber<UInteger>
        // rather than BSRational<UInteger>, which leads to better
        // performance.
        void ComputePSD(
            CRational const& crV0x,
            CRational const& crV0y,
            CRational const& crV1x,
            CRational const& crV1y,
            CRational const& crV2x,
            CRational const& crV2y,
            CRational const& crV1mV0x,
            CRational const& crV1mV0y,
            CRational const& crSqrLen10,
            CRational& crPSD)
        {
            auto& crV2mV0x = mNode[11];
            auto& crV2mV0y = mNode[12];
            auto& crV2mV1x = mNode[13];
            auto& crV2mV1y = mNode[14];
            auto& crDot1020 = mNode[15];
            auto& crSqrLen20 = mNode[16];
            auto& crDot1021 = mNode[17];
            auto& crSqrLen21 = mNode[18];

            crV2mV0x = crV2x - crV0x;
            crV2mV0y = crV2y - crV0y;
            crDot1020 = crV1mV0x * crV2mV0x + crV1mV0y * crV2mV0y;

            if (crDot1020.GetSign() <= 0)
            {
                crSqrLen20 = crV2mV0x * crV2mV0x + crV2mV0y * crV2mV0y;
                crPSD = crSqrLen10 * crSqrLen20;
            }
            else
            {
                crV2mV1x = crV2x - crV1x;
                crV2mV1y = crV2y - crV1y;
                crDot1021 = crV1mV0x * crV2mV1x + crV1mV0y * crV2mV1y;
                if (crDot1021.GetSign() >= 0)
                {
                    crSqrLen21 = crV2mV1x * crV2mV1x + crV2mV1y * crV2mV1y;
                    crPSD = crSqrLen10 * crSqrLen21;
                }
                else
                {
                    crSqrLen20 = crV2mV0x * crV2mV0x + crV2mV0y * crV2mV0y;
                    crPSD = crSqrLen10 * crSqrLen20 - crDot1020 * crDot1020;
                }
            }
        }

        // All edges inserted via the Insert(...) call are stored for use
        // by the caller. If any edge passed to Insert(...) is partitioned
        // into subedges, the subedges are inserted into this member.
        EdgeKeySet mInsertedEdges;

        // The output mesh.
        DynamicVETManifoldMesh mCDTMesh;

        // Sufficient storage for the expression trees related to computing
        // the exact pseudosquared distances in SelectSplit and ComputePSD.
        static std::size_t constexpr numNodes = 19;
        mutable std::vector<CRational> mNode;

    private:
        friend class UnitTestConstrainedDelaunay2;
    };
}
