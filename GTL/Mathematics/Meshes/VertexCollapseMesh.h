// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#pragma once

#include <GTL/Utility/MinHeap.h>
#include <GTL/Mathematics/Algebra/Vector.h>
#include <GTL/Mathematics/Geometry/2D/TriangulateEC.h>
#include <GTL/Mathematics/Meshes/DynamicVETManifoldMesh.h>
#include <GTL/Mathematics/Primitives/2D/Polygon2.h>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <map>
#include <memory>
#include <set>
#include <utility>
#include <vector>

namespace gtl
{
    template <typename T>
    class VertexCollapseMesh
    {
    public:
        // Construction.
        VertexCollapseMesh(std::vector<Vector3<T>> const& positions,
            std::vector<std::array<std::size_t, 3>> const& triangles)
            :
            mPositions(positions),
            mMesh(VCVertex::Create)
        {
            GTL_ARGUMENT_ASSERT(
                positions.size() >= 3 && triangles.size() >= 1,
                "Invalid positions or triangles.");

            // Build the manifold mesh from the inputs.
            for (auto const& triangle : triangles)
            {
                auto inserted = mMesh.Insert(triangle[0], triangle[1], triangle[2]);
                GTL_RUNTIME_ASSERT(
                    inserted != nullptr,
                    "The mesh is not manifold.");
            }

            // Locate the vertices (if any) on the mesh boundary.
            auto const& vmap = mMesh.GetVertices();
            for (auto const& eelement : mMesh.GetEdges())
            {
                auto edge = eelement.second.get();
                if (!edge->T[1])
                {
                    for (std::size_t i = 0; i < 2; ++i)
                    {
                        auto velement = vmap.find(edge->V[i]);
                        auto vertex = static_cast<VCVertex*>(velement->second.get());
                        vertex->isBoundary = true;
                    }
                }
            }

            // Build the priority queue of weights for the interior vertices.
            mMinHeap.Reset(vmap.size());
            for (auto const& velement : vmap)
            {
                auto vertex = static_cast<VCVertex*>(velement.second.get());

                T weight{};
                if (vertex->isBoundary)
                {
                    weight = std::numeric_limits<T>::max();
                }
                else
                {
                    weight = vertex->ComputeWeight(mPositions);
                }

                std::size_t heapKey = mMinHeap.Insert(velement.first, weight);
                mHeapKeys.insert(std::make_pair(velement.first, heapKey));
            }
        }

        // Decimate the mesh using vertex collapses
        struct Record
        {
            Record()
                :
                vertex(invalid),
                removed{},
                inserted{}
            {
            }

            // The index of the interior vertex that is removed from the mesh.
            // The triangles adjacent to the vertex are 'removed' from the
            // mesh. The polygon boundary of the adjacent triangles is
            // triangulated and the new triangles are 'inserted' into the
            // mesh.
            std::size_t vertex;
            std::vector<TriangleKey<true>> removed;
            std::vector<TriangleKey<true>> inserted;
        };

        // Return 'true' when a vertex collapse occurs. Once the function
        // returns 'false', no more vertex collapses are allowed so you may
        // then stop calling the function. The implementation has several
        // consistency tests that should not fail with a theoretically correct
        // implementation. If a test fails, the function returns 'false' and
        // the record.vertex is set to invalid.
        bool DoCollapse(Record& record)
        {
            record.vertex = invalid;
            record.removed.clear();
            record.inserted.clear();

            while (mMinHeap.GetNumElements() > 0)
            {
                std::size_t v = invalid;
                T weight = std::numeric_limits<T>::max();
                mMinHeap.GetMinimum(v, weight);
                if (weight == std::numeric_limits<T>::max())
                {
                    // There are no more interior vertices to collapse.
                    return false;
                }

                auto const& vmap = mMesh.GetVertices();
                auto velement = vmap.find(v);
                if (velement == vmap.end())
                {
                    // Unexpected condition.
                    return false;
                }

                auto vertex = static_cast<VCVertex*>(velement->second.get());
                std::vector<TriangleKey<true>> removed{}, inserted{};
                std::vector<std::size_t> linkVertices{};
                Status result = TriangulateLink(vertex, removed, inserted, linkVertices);
                if (result == Status::UNEXPECTED_ERROR)
                {
                    return false;
                }

                if (result == Status::ALLOWED)
                {
                    result = Collapsed(removed, inserted, linkVertices);
                    if (result == Status::UNEXPECTED_ERROR)
                    {
                        return false;
                    }

                    if (result == Status::ALLOWED)
                    {
                        // Remove the vertex and associated weight.
                        mMinHeap.Remove(v, weight);
                        mHeapKeys.erase(v);

                        // Update the weights of the link vertices.
                        for (auto vlink : linkVertices)
                        {
                            velement = vmap.find(vlink);
                            GTL_RUNTIME_ASSERT(
                                velement != vmap.end(),
                                "Unexpected condition.");

                            vertex = static_cast<VCVertex*>(velement->second.get());
                            if (!vertex->isBoundary)
                            {
                                auto iter = mHeapKeys.find(vlink);
                                GTL_RUNTIME_ASSERT(
                                    iter == mHeapKeys.end(),
                                    "Unexpected condition.");

                                weight = vertex->ComputeWeight(mPositions);
                                mMinHeap.Update(iter->second, weight);
                            }
                        }

                        record.vertex = v;
                        record.removed = std::move(removed);
                        record.inserted = std::move(inserted);
                        return true;
                    }
                    // else:  result == Status::DEFERRED
                }

                // To get here, result must be Status::DEFERRED. The vertex
                // collapse would cause mesh fold-over. Temporarily set the
                // edge weight to infinity. After removal of other triangles,
                // the vertex weight will be updated to a finite value and the
                // vertex possibly can be removed at that time.
                auto iter = mHeapKeys.find(v);
                GTL_RUNTIME_ASSERT(
                    iter == mHeapKeys.end(),
                    "Unexpected condition.");

                mMinHeap.Update(iter->second, std::numeric_limits<T>::max());
            }

            // We do not expect to reach this line of code, even for a closed
            // mesh. However, the compiler does not know this, yet requires
            // a return value.
            return false;
        }

        // Access the current state of the mesh, whether the original built
        // in the constructor or a decimated mesh during DoCollapse calls.
        inline DynamicVETManifoldMesh const& GetMesh() const
        {
            return mMesh;
        }

    private:
        static std::size_t constexpr invalid = std::numeric_limits<std::size_t>::max();

        struct VCVertex : public DynamicVETManifoldMesh::Vertex
        {
            VCVertex(std::size_t v)
                :
                DynamicVETManifoldMesh::Vertex(v),
                normal{},
                isBoundary(false)
            {
            }

            static std::unique_ptr<DynamicVETManifoldMesh::Vertex> Create(std::size_t v)
            {
                return std::make_unique<VCVertex>(v);
            }

            // The weight depends on the area of the triangles sharing the
            // vertex and the lengths of the projections of the adjacent
            // vertices onto the vertex normal line. A side effect of the
            // call is that the vertex normal is computed and stored.
            T ComputeWeight(std::vector<Vector3<T>> const& positions)
            {
                T weight = C_<T>(0);
                MakeZero(normal);
                for (auto const& tri : TAdjacent)
                {
                    Vector3<T> E0 = positions[tri->V[1]] - positions[tri->V[0]];
                    Vector3<T> E1 = positions[tri->V[2]] - positions[tri->V[0]];
                    Vector3<T> N = Cross(E0, E1);
                    normal += N;
                    weight += Length(N);
                }
                Normalize(normal);

                for (auto index : VAdjacent)
                {
                    Vector3<T> diff = positions[index] - positions[V];
                    weight += std::fabs(Dot(normal, diff));
                }

                return weight;
            }

            Vector3<T> normal;
            bool isBoundary;
        };

        // The functions TriangulateLink and Collapsed return one of the
        // enumerates described next.
        //
        // NO_MORE_ALLOWED:
        //     Either the mesh has no more interior vertices or a collapse
        //     will lead to a mesh fold-over or to a nonmanifold mesh. The
        //     returned value 'v' is invalid (0x80000000) and 'removed' and
        //     'inserted' are empty.
        //
        // ALLOWED:
        //     An interior vertex v has been removed. This is allowed using
        //     the following algorithm. The vertex normal is the weighted
        //     average of non-unit-length normals of triangles sharing v. The
        //     weights are the triangle areas. The adjacent vertices are
        //     projected onto a plane containing v and having normal equal to
        //     the vertex normal. If the projection is a simple polygon in
        //     the plane, the collapse is allowed. The triangles sharing v
        //     are 'removed', the polygon is triangulated, and the new
        //     triangles are 'inserted' into the mesh.
        //
        // DEFERRED:
        //     If the projection polygon described in the previous case is not
        //     simple (at least one pair of edges overlaps at some
        //     edge-interior point), the collapse would produce a fold-over in
        //     the mesh. We do not collapse in this case. It is possible
        //     that such a vertex occurs in a later collapse as its neighbors
        //     are adjusted by collapses. When this case occurs, v is valid
        //     (even though the collapse was not allowed) but 'removed' and
        //     'inserted' are empty.
        //
        // UNEXPECTED_ERROR:
        //     The code has several tests for conditions that are not expected
        //     to occur for a theoretically correct implementation. If you
        //     receive this error, file a bug report and provide a data set
        //     that caused the error.
        enum class Status
        {
            NO_MORE_ALLOWED,
            ALLOWED,
            DEFERRED,
            UNEXPECTED_ERROR
        };

        Status TriangulateLink(VCVertex* vertex, std::vector<TriangleKey<true>>& removed,
            std::vector<TriangleKey<true>>& inserted, std::vector<std::size_t>& linkVertices) const
        {
            // Create the (CCW) polygon boundary of the link of the vertex.
            // The incoming vertex is interior, so the number of triangles
            // sharing the vertex is equal to the number of vertices of the
            // polygon. A precondition of the function call is that the
            // vertex normal has already been computed.

            // Get the edges of the link that are opposite the incoming
            // vertex.
            std::size_t const numVertices = vertex->TAdjacent.size();
            removed.resize(numVertices);
            std::size_t j = 0;
            std::map<std::size_t, std::size_t> edgeMap{};
            for (auto tri : vertex->TAdjacent)
            {
                for (std::size_t i = 0; i < 3; ++i)
                {
                    if (tri->V[i] == vertex->V)
                    {
                        edgeMap.insert(std::make_pair(tri->V[(i + 1) % 3], tri->V[(i + 2) % 3]));
                        break;
                    }
                }
                removed[j++] = TriangleKey<true>(tri->V[0], tri->V[1], tri->V[2]);
            }
            if (edgeMap.size() != vertex->TAdjacent.size())
            {
                return Status::UNEXPECTED_ERROR;
            }

            // Connect the edges into a polygon.
            linkVertices.resize(numVertices);
            auto iter = edgeMap.begin();
            for (std::size_t i = 0; i < numVertices; ++i)
            {
                linkVertices[i] = iter->first;
                iter = edgeMap.find(iter->second);
                if (iter == edgeMap.end())
                {
                    return Status::UNEXPECTED_ERROR;
                }
            }
            if (iter->first != linkVertices[0])
            {
                return Status::UNEXPECTED_ERROR;
            }

            // Project the polygon onto the plane containing the incoming
            // vertex and having the vertex normal. The projected polygon
            // is computed so that the incoming vertex is projected to (0,0).
            Vector3<T> center = mPositions[vertex->V];
            Vector3<T> N = vertex->normal, U{}, V{};
            ComputeOrthonormalBasis(1, N, U, V);
            std::vector<Vector2<T>> projected(numVertices);
            for (std::size_t i = 0; i < numVertices; ++i)
            {
                Vector3<T> diff = mPositions[linkVertices[i]] - center;
                projected[i][0] = Dot(U, diff);
                projected[i][1] = Dot(V, diff);
            }

            // The polygon must be simple in order to triangulate it.
            Polygon2<T> polygon(projected, true);
            if (polygon.IsSimple())
            {
                TriangulateEC<T> triangulator(numVertices, projected.data());
                triangulator();
                auto const& triangles = triangulator.GetTriangles();
                if (triangles.size() == 0)
                {
                    return Status::UNEXPECTED_ERROR;
                }

                inserted.resize(triangles.size());
                for (std::size_t t = 0; t < triangles.size(); ++t)
                {
                    inserted[t] = TriangleKey<true>(
                        linkVertices[triangles[t][0]],
                        linkVertices[triangles[t][1]],
                        linkVertices[triangles[t][2]]);
                }
                return Status::ALLOWED;
            }
            else
            {
                return Status::DEFERRED;
            }
        }

        Status Collapsed(std::vector<TriangleKey<true>> const& removed,
            std::vector<TriangleKey<true>> const& inserted, std::vector<std::size_t> const& linkVertices)
        {
            // The triangles that were disconnected from the link edges are
            // guaranteed to allow manifold reconnection to 'inserted'
            // triangles. On the insertion, each diagonal of the link becomes
            // a mesh edge and shares two (link) triangles. It is possible
            // that the mesh already contains the (diagonal) edge, which will
            // lead to a nonmanifold connection, which we cannot allow. The
            // following code traps this condition and restores the mesh to
            // its state before the 'Remove(...)' call.
            bool isCollapsible = true;
            auto const& emap = mMesh.GetEdges();
            std::set<EdgeKey<false>> edges{};
            for (auto const& tri : inserted)
            {
                for (std::size_t k0 = 2, k1 = 0; k1 < 3; k0 = k1++)
                {
                    EdgeKey<false> edge(tri[k0], tri[k1]);
                    if (edges.find(edge) == edges.end())
                    {
                        edges.insert(edge);
                    }
                    else
                    {
                        // The edge has been visited twice, so it is a
                        // diagonal of the link.

                        auto eelement = emap.find(edge);
                        if (eelement != emap.end())
                        {
                            if (eelement->second->T[1])
                            {
                                // The edge will not allow a manifold
                                // connection.
                                isCollapsible = false;
                                break;
                            }
                        }

                        edges.erase(edge);
                    }
                };

                if (!isCollapsible)
                {
                    return Status::DEFERRED;
                }
            }

            // Remove the old triangle neighborhood, which will lead to the
            // vertex itself being removed from the mesh.
            for (auto const& tri : removed)
            {
                mMesh.Remove(tri[0], tri[1], tri[2]);
            }

            // Insert the new triangulation.
            for (auto const& tri : inserted)
            {
                mMesh.Insert(tri[0], tri[1], tri[2]);
            }

            // If the Remove(...) calls remove a boundary vertex that is in
            // the link vertices, the Insert(...) calls will insert the
            // boundary vertex again. We must re-tag those boundary vertices.
            auto const& vmap = mMesh.GetVertices();
            std::size_t const numVertices = linkVertices.size();
            for (std::size_t i0 = numVertices - 1, i1 = 0; i1 < numVertices; i0 = i1++)
            {
                EdgeKey<false> ekey(linkVertices[i0], linkVertices[i1]);
                auto eelement = emap.find(ekey);
                if (eelement == emap.end())
                {
                    return Status::UNEXPECTED_ERROR;
                }

                auto edge = eelement->second.get();
                if (!edge)
                {
                    return Status::UNEXPECTED_ERROR;
                }

                if (edge->T[0] && !edge->T[1])
                {
                    for (std::size_t k = 0; k < 2; ++k)
                    {
                        auto velement = vmap.find(edge->V[k]);
                        if (velement == vmap.end())
                        {
                            return Status::UNEXPECTED_ERROR;
                        }

                        auto vertex = static_cast<VCVertex*>(velement->second.get());
                        vertex->isBoundary = true;
                    }
                }
            }

            return Status::ALLOWED;
        }

        std::vector<Vector3<T>> mPositions;
        DynamicVETManifoldMesh mMesh;
        MinHeap<T> mMinHeap;
        std::map<std::size_t, std::size_t> mHeapKeys;
    };
}
