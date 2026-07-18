// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.18

#pragma once

#include <GTL/Utility/TypeTraits.h>
#include <GTL/Mathematics/Intersection/IntersectionQuery.h>
#include <GTL/Mathematics/Meshes/EdgeKey.h>
#include <GTL/Mathematics/Meshes/UniqueVerticesSimplices.h>
#include <GTL/Mathematics/Primitives/3D/ConvexMesh3.h>
#include <GTL/Mathematics/Primitives/3D/Plane3.h>
#include <array>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <map>
#include <utility>
#include <vector>

namespace gtl
{
    template <typename T>
    class FIQuery<T, ConvexMesh3<T>, Plane3<T>>
    {
    public:
        // Convenient type definitions.
        using CM = ConvexMesh3<T>;
        using Vertex = typename CM::Vertex;
        using Triangle = typename CM::Triangle;

        // The configuration describes geometrically how the input convex
        // polyhedron and the plane intersect.
        static std::uint32_t constexpr CFG_EMPTY = 0x00000000;
        static std::uint32_t constexpr CFG_POS_SIDE = 0x00000010;
        static std::uint32_t constexpr CFG_NEG_SIDE = 0x00000020;

        // The plane intersects the convex polyhedron transversely. The set of
        // intersection is a convex polygon. The convex polyhedron is split
        // into two convex polyhedra, one on the positive side of the plane
        // and one on the negative side of the plane, both polyhedra sharing
        // the convex polygon of intersection.
        static std::uint32_t constexpr CFG_SPLIT = CFG_POS_SIDE | CFG_NEG_SIDE;  // 48

        // The convex polyhedron is strictly on the positive side of the
        // plane.
        static std::uint32_t constexpr CFG_POS_SIDE_STRICT = CFG_POS_SIDE;  // 16

        // The convex polyhedron is on the positive side of the plane with one
        // vertex in the plane.
        static std::uint32_t constexpr CFG_POS_SIDE_VERTEX = CFG_POS_SIDE | 1;  // 17

        // The convex polyhedron is on the positive side of the plane with one
        // edge in the plane.
        static std::uint32_t constexpr CFG_POS_SIDE_EDGE = CFG_POS_SIDE | 2;  // 18

        // The convex polyhedron is on the positive side of the plane with a
        // polygonal face in the plane. The face can consist of multiple
        // triangles.
        static std::uint32_t constexpr CFG_POS_SIDE_POLYGON = CFG_POS_SIDE | 4;  // 20

        // Flags for any of the tangential cases (vertex touching, edge
        // touching, face touching).
        static std::uint32_t constexpr CFG_POS_SIDE_TANGENT = CFG_POS_SIDE | 7;  // 23

        // The convex polyhedron is strictly on the negative side of the
        // plane.
        static std::uint32_t constexpr CFG_NEG_SIDE_STRICT = CFG_NEG_SIDE;  // 32

        // The convex polyhedron is on the negative side of the plane with one
        // vertex in the plane.
        static std::uint32_t constexpr CFG_NEG_SIDE_VERTEX = CFG_NEG_SIDE | 1;  // 33

        // The convex polyhedron is on the negative side of the plane with one
        // edge in the plane.
        static std::uint32_t constexpr CFG_NEG_SIDE_EDGE = CFG_NEG_SIDE | 2;  // 34

        // The convex polyhedron is on the negative side of the plane with a
        // polygonal face in the plane. The face can consist of multiple
        // triangles.
        static std::uint32_t constexpr CFG_NEG_SIDE_POLYGON = CFG_NEG_SIDE | 4;  // 36

        // Flags for any of the tangential cases (vertex touching, edge
        // touching, face touching).
        static std::uint32_t constexpr CFG_NEG_SIDE_TANGENT = CFG_NEG_SIDE | 7;  // 39

        // Requested information for the query to compute.
        static std::uint32_t constexpr REQ_CONFIGURATION_ONLY = 0x00000000;
        static std::uint32_t constexpr REQ_INTR_MESH = 0x00000001;
        static std::uint32_t constexpr REQ_INTR_POLYGON = 0x00000002;
        static std::uint32_t constexpr REQ_INTR_BOTH = REQ_INTR_MESH | REQ_INTR_POLYGON;
        static std::uint32_t constexpr REQ_POLYHEDRON_POS = 0x00000004;
        static std::uint32_t constexpr REQ_POLYHEDRON_NEG = 0x00000008;
        static std::uint32_t constexpr REQ_POLYHEDRON_BOTH = REQ_POLYHEDRON_POS | REQ_POLYHEDRON_NEG;
        static std::uint32_t constexpr REQ_ALL = 0x0000000F;

        struct Output
        {
            Output()
                :
                configuration(CFG_EMPTY),
                requested(REQ_CONFIGURATION_ONLY),
                intersectionMesh{},
                intersectionPolygon{},
                positivePolyhedron{},
                negativePolyhedron{}
            {
            }

            // The configuration describes geometrically how the input convex
            // polyhedron and the plane intersect.
            std::uint32_t configuration;

            // You can specify the information you want from the query.
            std::uint32_t requested;

            // The intersection of the convex polyhedron and the plane is
            // either empty, a single vertex, a single edge or a convex
            // polygon. The intersection members have the properties:
            //   empty:    mVertices has 0 elements, mMesh is empty
            //   vertex:   mVertices has 1 element, mMesh is empty
            //   edge:     mVertices has 2 elements, mMesh is empty
            //   polygon:  mVertices has 3 or more elements, mMesh is a
            //             triangulation of the convex polygon
            // The convex polygon vertices are indexed by 'polygon' in the
            // order consistent with that of the positive polyhedron
            // triangles. The indices are into intersection.vertices.
            ConvexMesh3<T> intersectionMesh;
            std::vector<Vertex> intersectionPolygon;

            // If the configuration is POSITIVE_* or SPLIT, this convex
            // polyhedron is the portion of the input convex polyhedron on the
            // positive side of the plane with possibly a vertex or edge on
            // the plane.
            ConvexMesh3<T> positivePolyhedron;

            // If the configuration is NEGATIVE_* or SPLIT, this convex
            // polyhedron is the portion of the input convex polyhedron on the
            // negative side of the plane with possibly a vertex or edge on
            // the plane.
            ConvexMesh3<T> negativePolyhedron;
        };

        Output operator() (ConvexMesh3<T> const& polyhedron,
            Plane3<T> const& plane, std::uint32_t requested)
        {
            static_assert(is_arbitrary_precision<T>::value, "T must be arbitrary precision.");
            static_assert(has_division_operator<T>::value, "T must support division.");

            Output output{};
            output.requested = requested;

            // Storage for (Dot(N,X) - c) for each vertex X, where N is a
            // plane normal (not necessarily unit length) and c is the
            // corresponding plane constant.
            std::size_t numPositive = 0, numNegative = 0, numZero = 0;
            std::size_t const numVertices = polyhedron.vertices.size();
            std::vector<T> dot(numVertices);
            std::vector<std::int32_t> sign(numVertices);
            for (std::size_t i = 0; i < numVertices; ++i)
            {
                dot[i] = Dot(plane.normal, polyhedron.vertices[i]) - plane.constant;
                if (dot[i] > C_<T>(0))
                {
                    sign[i] = +1;
                    ++numPositive;
                }
                else if (dot[i] < C_<T>(0))
                {
                    sign[i] = -1;
                    ++numNegative;
                }
                else
                {
                    sign[i] = 0;
                    ++numZero;
                }
            }

            if (numPositive == 0)
            {
                output.configuration = CFG_NEG_SIDE | (numZero < 3 ? numZero > 0 : 4);

                if ((requested & REQ_POLYHEDRON_NEG) != 0)
                {
                    output.negativePolyhedron = polyhedron;
                }

                if (numZero > 0 && ((requested & REQ_INTR_BOTH) != 0))
                {
                    GetIntersection(polyhedron, numZero, sign, output);
                }
            }
            else if (numNegative == 0)
            {
                output.configuration = CFG_POS_SIDE | (numZero < 3 ? numZero > 0 : 4);

                if ((requested & REQ_POLYHEDRON_POS) != 0)
                {
                    output.positivePolyhedron = polyhedron;
                }

                if (numZero > 0 && ((requested & REQ_INTR_BOTH) != 0))
                {
                    GetIntersection(polyhedron, numZero, sign, output);
                }
            }
            else
            {
                output.configuration = CFG_SPLIT;

                if (requested != REQ_CONFIGURATION_ONLY)
                {
                    SplitPolyhedron(polyhedron, dot, sign, output);
                }
            }
            return output;
        }

    private:
        static void GetIntersection(CM const& polyhedron, std::size_t numZero,
            std::vector<std::int32_t> const& sign, Output& output)
        {
            bool const wantIntrMesh = (output.requested & REQ_INTR_MESH) != 0;
            bool const wantIntrPolygon = (output.requested & REQ_INTR_POLYGON) != 0;

            if (numZero == 1)
            {
                GetIntersectionVertex(polyhedron, sign, wantIntrMesh,
                    wantIntrPolygon, output);
            }
            else if (numZero == 2)
            {
                GetIntersectionEdge(polyhedron, sign, wantIntrMesh,
                    wantIntrPolygon, output);
            }
            else  // numZero >= 3
            {
                GetIntersectionPolygon(polyhedron, sign, wantIntrMesh,
                    wantIntrPolygon, output);
            }
        }

        static void GetIntersectionVertex(CM const& polyhedron,
            std::vector<std::int32_t> const& sign, bool wantIntrMesh, bool wantIntrPolygon,
            Output& output)
        {
            output.intersectionMesh.configuration = ConvexMesh3<T>::CFG_POINT;
            if (wantIntrMesh)
            {
                output.intersectionMesh.vertices.resize(1);
            }
            if (wantIntrPolygon)
            {
                output.intersectionPolygon.resize(1);
            }

            std::size_t const numVertices = polyhedron.vertices.size();
            for (std::size_t i = 0; i < numVertices; ++i)
            {
                if (sign[i] == 0)
                {
                    if (wantIntrMesh)
                    {
                        output.intersectionMesh.vertices[0] = polyhedron.vertices[i];
                    }
                    if (wantIntrPolygon)
                    {
                        output.intersectionPolygon[0] = polyhedron.vertices[i];
                    }
                    return;
                }
            }
        }

        static void GetIntersectionEdge(CM const& polyhedron,
            std::vector<std::int32_t> const& sign, bool wantIntrMesh, bool wantIntrPolygon,
            Output& output)
        {
            output.intersectionMesh.configuration = ConvexMesh3<T>::CFG_SEGMENT;
            if (wantIntrMesh)
            {
                output.intersectionMesh.vertices.resize(2);
            }
            if (wantIntrPolygon)
            {
                output.intersectionPolygon.resize(2);
            }

            std::size_t const numVertices = polyhedron.vertices.size();
            for (std::size_t i = 0, numFound = 0; i < numVertices; ++i)
            {
                if (sign[i] == 0)
                {
                    if (wantIntrMesh)
                    {
                        output.intersectionMesh.vertices[numFound] = polyhedron.vertices[i];
                    }
                    if (wantIntrPolygon)
                    {
                        output.intersectionPolygon[numFound] = polyhedron.vertices[i];
                    }
                    if (++numFound == 2)
                    {
                        return;
                    }
                }
            }
        }

        static void GetIntersectionPolygon(CM const& polyhedron,
            std::vector<std::int32_t> const& sign, bool wantIntrMesh, bool wantIntrPolygon,
            Output& output)
        {
            output.intersectionMesh.configuration = ConvexMesh3<T>::CFG_POLYGON;

            std::vector<Triangle> intersectionMeshTriangles;
            intersectionMeshTriangles.reserve(polyhedron.triangles.size());
            for (auto const& triangle : polyhedron.triangles)
            {
                if (sign[triangle[0]] == 0 && sign[triangle[1]] == 0 && sign[triangle[2]] == 0)
                {
                    intersectionMeshTriangles.push_back(triangle);
                }
            }

            std::vector<Vertex> outVertices{};
            std::vector<Triangle> outTriangles{};
            UniqueVerticesSimplices<Vertex, std::size_t, 3> uvt{};
            uvt.RemoveDuplicateAndUnusedVertices(polyhedron.vertices,
                intersectionMeshTriangles, outVertices, outTriangles);

            if (wantIntrPolygon)
            {
                // Get the boundary edges with ordering consistent with the
                // triangle face chirality.
                std::map<EdgeKey<false>, std::array<std::size_t, 2>> edgeMap{};
                for (auto const& triangle : outTriangles)
                {
                    for (std::size_t j0 = 2, j1 = 0; j1 < 3; j0 = j1++)
                    {
                        EdgeKey<false> edge(triangle[j0], triangle[j1]);
                        auto iter = edgeMap.find(edge);
                        if (iter != edgeMap.end())
                        {
                            // The edge is now visited twice, so it cannot be a
                            // boundary edge.
                            edgeMap.erase(iter);
                        }
                        else
                        {
                            // The edge is visited the first time, so it might be
                            // a boundary edge.
                            std::array<std::size_t, 2> value = { triangle[j1], triangle[j0] };
                            edgeMap.insert(std::make_pair(edge, value));
                        }
                    }
                }

                // Construct the boundary polygon.
                std::size_t constexpr smax = std::numeric_limits<std::size_t>::max();
                std::vector<std::size_t> polygonIndices(edgeMap.size(), smax);
                for (auto const& element : edgeMap)
                {
                    polygonIndices[element.second[0]] = element.second[1];
                }

                output.intersectionPolygon.resize(edgeMap.size());
                for (std::size_t i = 0; i < output.intersectionPolygon.size(); ++i)
                {
                    output.intersectionPolygon[i] = outVertices[polygonIndices[i]];
                }
            }

            if (wantIntrMesh)
            {
                output.intersectionMesh.vertices = std::move(outVertices);
                output.intersectionMesh.triangles = std::move(outTriangles);
            }
        }

        static void SplitPolyhedron(CM const& polyhedron, std::vector<T> const& dot,
            std::vector<std::int32_t> const& sign, Output& output)
        {
            bool const wantPosMesh = (output.requested & REQ_POLYHEDRON_POS) != 0;
            bool const wantNegMesh = (output.requested & REQ_POLYHEDRON_NEG) != 0;
            bool const wantIntrMesh = (output.requested & REQ_INTR_MESH) != 0;
            bool const wantIntrPolygon = (output.requested & REQ_INTR_POLYGON) != 0;

            // The split polyhedra use the input polyhedron's vertices and any
            // edge-interior intersections between the plane and the mesh
            // edges. The center point of the polygon of intersection (if any)
            // is also used as a vertex.
            std::vector<Vertex> splitVertices{};
            std::map<EdgeKey<false>, std::size_t> eiVMap{};
            GetVertexCandidates(polyhedron, dot, sign, splitVertices, eiVMap);

            // Split each triangle face of the polyhedron by the plane.
            std::vector<Triangle> posMesh{}, negMesh{};
            std::map<std::size_t, std::size_t> posIntersection;
            DoSplit(polyhedron, sign, eiVMap, wantPosMesh, posMesh,
                wantNegMesh, negMesh, posIntersection);

            // Get the polygon of intersection. This is used by all of the
            // requested features.
            std::vector<std::size_t> polygon{};
            GetIntersectionPolygon(posIntersection, splitVertices,
                wantIntrPolygon, polygon, output);

            if (wantPosMesh || wantNegMesh || wantIntrMesh)
            {
                // Get the polyhedra split by the plane. The polygon of
                // intersection is also computed and used to close the
                // polyhedra.
                GetSplitPolyhedra(splitVertices, polygon, wantIntrMesh,
                    wantPosMesh, posMesh, wantNegMesh, negMesh, output);
            }
        }

        static void GetVertexCandidates(CM const& polyhedron,
            std::vector<T> const& dot, std::vector<std::int32_t> const& sign,
            std::vector<Vertex>& splitVertices,
            std::map<EdgeKey<false>, std::size_t>& eiVMap)
        {
            // Get the edges of the polyhedron.
            std::set<EdgeKey<false>> edgeMap{};
            for (auto const& triangle : polyhedron.triangles)
            {
                edgeMap.insert(EdgeKey<false>(triangle[0], triangle[1]));
                edgeMap.insert(EdgeKey<false>(triangle[1], triangle[2]));
                edgeMap.insert(EdgeKey<false>(triangle[2], triangle[0]));
            }

            // The vertex candidates include the original vertices, any
            // edge-interior intersections between the plane and polyhedron,
            // and the average of the convex-polygon intersection (if there
            // is such an intersection). The number of reserved elements of
            // splitVertices is large enough to avoid resizing of the array
            // later.
            splitVertices.reserve(polyhedron.vertices.size() + edgeMap.size() + 1);
            for (auto const& vertex : polyhedron.vertices)
            {
                splitVertices.push_back(vertex);
            }

            // Compute edge-interior points of intersection between the plane
            // and the mesh edges. The eiVMap container allows accessing the
            // edge-interior vertices when each triangle face of the
            // polyhedron is processed for intersection with the plane.
            for (auto const& element : edgeMap)
            {
                std::size_t v0 = element[0];
                std::size_t v1 = element[1];
                if (sign[v0] * sign[v1] < 0)
                {
                    T denom = dot[v1] - dot[v0];
                    T w0 = dot[v1] / denom;
                    T w1 = -dot[v0] / denom;
                    auto eiVertex = w0 * polyhedron.vertices[v0] + w1 * polyhedron.vertices[v1];
                    eiVMap.insert(std::make_pair(EdgeKey<false>(v0, v1), splitVertices.size()));
                    splitVertices.push_back(eiVertex);
                }
            }

            // The average point will be appended to splitVertices later when
            // necessary.
        }

        static void DoSplit(CM const& polyhedron, std::vector<std::int32_t> const& sign,
            std::map<EdgeKey<false>, std::size_t>& eiVMap,
            bool wantPosMesh, std::vector<Triangle>& posMesh,
            bool wantNegMesh, std::vector<Triangle>& negMesh,
            std::map<std::size_t, std::size_t>& posIntersection)
        {
            for (auto const& triangle : polyhedron.triangles)
            {
                std::size_t v0 = triangle[0], v1 = triangle[1], v2 = triangle[2];
                std::size_t v01{}, v12{}, v20{};

                if (sign[v0] > 0)
                {
                    if (sign[v1] > 0)
                    {
                        if (sign[v2] > 0)
                        {
                            // +++
                            if (wantPosMesh)
                            {
                                posMesh.push_back({ v0, v1, v2 });
                            }
                        }
                        else if (sign[v2] < 0)
                        {
                            // ++-
                            v12 = eiVMap[EdgeKey<false>(v1, v2)];
                            v20 = eiVMap[EdgeKey<false>(v2, v0)];
                            if (wantPosMesh)
                            {
                                posMesh.push_back({ v0, v12, v20 });
                                posMesh.push_back({ v0, v1, v12 });
                            }
                            if (wantNegMesh)
                            {
                                negMesh.push_back({ v2, v20, v12 });
                            }
                            posIntersection.insert(std::make_pair(v20, v12));
                        }
                        else
                        {
                            // ++0
                            if (wantPosMesh)
                            {
                                posMesh.push_back({ v0, v1, v2 });
                            }
                        }
                    }
                    else if (sign[v1] < 0)
                    {
                        if (sign[v2] > 0)
                        {
                            // +-+
                            v01 = eiVMap[EdgeKey<false>(v0, v1)];
                            v12 = eiVMap[EdgeKey<false>(v1, v2)];
                            if (wantPosMesh)
                            {
                                posMesh.push_back({ v0, v01, v12 });
                                posMesh.push_back({ v0, v12, v2 });
                            }
                            if (wantNegMesh)
                            {
                                negMesh.push_back({ v1, v12, v01 });
                            }
                            posIntersection.insert(std::make_pair(v12, v01));
                        }
                        else if (sign[v2] < 0)
                        {
                            // +--
                            v01 = eiVMap[EdgeKey<false>(v0, v1)];
                            v20 = eiVMap[EdgeKey<false>(v2, v0)];
                            if (wantPosMesh)
                            {
                                posMesh.push_back({ v0, v01, v20 });
                            }
                            if (wantNegMesh)
                            {
                                negMesh.push_back({ v1, v20, v01 });
                                negMesh.push_back({ v1, v2, v20 });
                            }
                            posIntersection.insert(std::make_pair(v20, v01));
                        }
                        else
                        {
                            // +-0
                            v01 = eiVMap[EdgeKey<false>(v0, v1)];
                            if (wantPosMesh)
                            {
                                posMesh.push_back({ v2, v0, v01 });
                            }
                            if (wantNegMesh)
                            {
                                negMesh.push_back({ v2, v01, v1 });
                            }
                            posIntersection.insert(std::make_pair(v2, v01));
                        }
                    }
                    else
                    {
                        if (sign[v2] > 0)
                        {
                            // +0+
                            if (wantPosMesh)
                            {
                                posMesh.push_back({ v0, v1, v2 });
                            }
                        }
                        else if (sign[v2] < 0)
                        {
                            // +0-
                            v20 = eiVMap[EdgeKey<false>(v2, v0)];
                            if (wantPosMesh)
                            {
                                posMesh.push_back({ v1, v20, v0 });
                            }
                            if (wantNegMesh)
                            {
                                negMesh.push_back({ v1, v2, v20 });
                            }
                            posIntersection.insert(std::make_pair(v20, v1));
                        }
                        else
                        {
                            // +00
                            if (wantPosMesh)
                            {
                                posMesh.push_back({ v0, v1, v2 });
                            }
                            posIntersection.insert(std::make_pair(v2, v1));
                        }
                    }
                }
                else if (sign[v0] < 0)
                {
                    if (sign[v1] > 0)
                    {
                        if (sign[v2] > 0)
                        {
                            // -++
                            v01 = eiVMap[EdgeKey<false>(v0, v1)];
                            v20 = eiVMap[EdgeKey<false>(v2, v0)];
                            if (wantPosMesh)
                            {
                                posMesh.push_back({ v1, v20, v01 });
                                posMesh.push_back({ v1, v2, v20 });
                            }
                            if (wantNegMesh)
                            {
                                negMesh.push_back({ v0, v01, v20 });
                            }
                            posIntersection.insert(std::make_pair(v01, v20));
                        }
                        else if (sign[v2] < 0)
                        {
                            // -+-
                            v01 = eiVMap[EdgeKey<false>(v0, v1)];
                            v12 = eiVMap[EdgeKey<false>(v1, v2)];
                            if (wantPosMesh)
                            {
                                posMesh.push_back({ v1, v12, v01 });
                            }
                            if (wantNegMesh)
                            {
                                negMesh.push_back({ v0, v01, v12 });
                                negMesh.push_back({ v0, v12, v2 });
                            }
                            posIntersection.insert(std::make_pair(v01, v12));
                        }
                        else
                        {
                            // -+0
                            v01 = eiVMap[EdgeKey<false>(v0, v1)];
                            if (wantPosMesh)
                            {
                                posMesh.push_back({ v1, v2, v01 });
                            }
                            if (wantNegMesh)
                            {
                                negMesh.push_back({ v2, v0, v01 });
                            }
                            posIntersection.insert(std::make_pair(v01, v2));
                        }
                    }
                    else if (sign[v1] < 0)
                    {
                        if (sign[v2] > 0)
                        {
                            // --+
                            v12 = eiVMap[EdgeKey<false>(v1, v2)];
                            v20 = eiVMap[EdgeKey<false>(v2, v0)];
                            if (wantPosMesh)
                            {
                                posMesh.push_back({ v2, v20, v12 });
                            }
                            if (wantNegMesh)
                            {
                                negMesh.push_back({ v0, v1, v12 });
                                negMesh.push_back({ v0, v12, v20 });
                            }
                            posIntersection.insert(std::make_pair(v12, v20));
                        }
                        else if (sign[v2] < 0)
                        {
                            // ---
                            if (wantNegMesh)
                            {
                                negMesh.push_back({ v0, v1, v2 });
                            }
                        }
                        else
                        {
                            // --0
                            if (wantNegMesh)
                            {
                                negMesh.push_back({ v0, v1, v2 });
                            }
                        }
                    }
                    else
                    {
                        if (sign[v2] > 0)
                        {
                            // -0+
                            v20 = eiVMap[EdgeKey<false>(v2, v0)];
                            if (wantPosMesh)
                            {
                                posMesh.push_back({ v2, v20, v1 });
                            }
                            if (wantNegMesh)
                            {
                                negMesh.push_back({ v0, v1, v20 });
                            }
                            posIntersection.insert(std::make_pair(v1, v20));
                        }
                        else if (sign[v2] < 0)
                        {
                            // -0-
                            if (wantNegMesh)
                            {
                                negMesh.push_back({ v0, v1, v2 });
                            }
                        }
                        else
                        {
                            // -00
                            if (wantNegMesh)
                            {
                                negMesh.push_back({ v0, v1, v2 });
                            }
                        }
                    }
                }
                else
                {
                    if (sign[v1] > 0)
                    {
                        if (sign[v2] > 0)
                        {
                            // 0++
                            if (wantPosMesh)
                            {
                                posMesh.push_back({ v0, v1, v2 });
                            }
                        }
                        else if (sign[v2] < 0)
                        {
                            // 0+-
                            v12 = eiVMap[EdgeKey<false>(v1, v2)];
                            if (wantPosMesh)
                            {
                                posMesh.push_back({ v1, v12, v0 });
                            }
                            if (wantNegMesh)
                            {
                                negMesh.push_back({ v2, v0, v12 });
                            }
                            posIntersection.insert(std::make_pair(v0, v12));
                        }
                        else
                        {
                            // 0+0
                            if (wantPosMesh)
                            {
                                posMesh.push_back({ v0, v1, v2 });
                            }
                            posIntersection.insert(std::make_pair(v0, v2));
                        }
                    }
                    else if (sign[v1] < 0)
                    {
                        if (sign[v2] > 0)
                        {
                            // 0-+
                            v12 = eiVMap[EdgeKey<false>(v1, v2)];
                            if (wantPosMesh)
                            {
                                posMesh.push_back({ v2, v0, v12 });
                            }
                            if (wantNegMesh)
                            {
                                negMesh.push_back({ v1, v12, v0 });
                            }
                            posIntersection.insert(std::make_pair(v12, v0));
                        }
                        else if (sign[v2] < 0)
                        {
                            // 0--
                            if (wantNegMesh)
                            {
                                negMesh.push_back({ v0, v1, v2 });
                            }
                        }
                        else
                        {
                            // 0-0
                            if (wantNegMesh)
                            {
                                negMesh.push_back({ v0, v1, v2 });
                            }
                        }
                    }
                    else
                    {
                        if (sign[v2] > 0)
                        {
                            // 00+
                            if (wantPosMesh)
                            {
                                posMesh.push_back({ v0, v1, v2 });
                            }
                            posIntersection.insert(std::make_pair(v1, v0));
                        }
                        else if (sign[v2] < 0)
                        {
                            // 00-
                            if (wantNegMesh)
                            {
                                negMesh.push_back({ v0, v1, v2 });
                            }
                        }
                        else
                        {
                            // 000
                            // This case cannot occur with exact arithmetic,
                            // because it would have been trapped previously
                            // by tests numPositive == 0 or numNegative == 0.
                            GTL_RUNTIME_ERROR(
                                "This case cannot occur with exact arithmetic.");
                        }
                    }
                }
            }
        }

        static void GetIntersectionPolygon(std::map<std::size_t, std::size_t> const& posIntersection,
            std::vector<Vertex>& splitVertices, bool wantIntrPolygon,
            std::vector<std::size_t>& polygon, Output& output)
        {
            std::size_t const numVertices = posIntersection.size();
            polygon.resize(numVertices);
            auto posIter = posIntersection.begin();
            for (std::size_t i = 0; i < numVertices; ++i)
            {
                polygon[i] = posIter->first;
                posIter = posIntersection.find(posIter->second);
            }

            if (wantIntrPolygon)
            {
                output.intersectionPolygon.resize(numVertices);
                for (std::size_t i = 0; i < numVertices; ++i)
                {
                    output.intersectionPolygon[i] = splitVertices[polygon[i]];
                }
            }
        }

        static void GetSplitPolyhedra(std::vector<Vertex>& splitVertices,
            std::vector<std::size_t> const& polygon, bool wantIntrMesh,
            bool wantPosMesh, std::vector<Triangle>& posMesh,
            bool wantNegMesh, std::vector<Triangle>& negMesh, Output& output)
        {
            // Triangulate the polygon for use by the positive polyhedron. A
            // triangle fan will not work always work when the polygon has
            // collinear vertices. The average of the polygon vertices is
            // inserted as an extra vertex. The triangulation includes each
            // triangle that is formed by the average point and an edge of the
            // polygon. The negative polyhedron uses the same triangulation
            // but with opposite chirality. NOTE: To avoid biases in the
            // average due to vertex distribution, use the center of mass of
            // the polygon instead.
            Vertex average{ C_<T>(0), C_<T>(0), C_<T>(0) };
            for (auto const& i : polygon)
            {
                average += splitVertices[i];
            }
            std::size_t const numVertices = polygon.size();
            average /= static_cast<T>(numVertices);
            std::size_t avrIndex = splitVertices.size();
            splitVertices.push_back(average);

            std::vector<Triangle> intrMesh{};
            for (std::size_t i0 = numVertices - 1, i1 = 0; i1 < numVertices; i0 = i1++)
            {
                if (wantPosMesh)
                {
                    posMesh.push_back({ avrIndex, polygon[i0], polygon[i1] });
                }

                if (wantNegMesh)
                {
                    negMesh.push_back({ avrIndex, polygon[i1], polygon[i0] });
                }

                if (wantIntrMesh)
                {
                    intrMesh.push_back({ avrIndex, polygon[i0], polygon[i1] });
                }
            }

            UniqueVerticesSimplices<Vertex, std::size_t, 3> uvt{};

            if (wantPosMesh)
            {
                output.positivePolyhedron.configuration = CM::CFG_POLYHEDRON;
                uvt.RemoveDuplicateAndUnusedVertices(splitVertices, posMesh,
                    output.positivePolyhedron.vertices,
                    output.positivePolyhedron.triangles);
            }

            if (wantNegMesh)
            {
                output.negativePolyhedron.configuration = CM::CFG_POLYHEDRON;
                uvt.RemoveDuplicateAndUnusedVertices(splitVertices, negMesh,
                    output.negativePolyhedron.vertices,
                    output.negativePolyhedron.triangles);
            }

            if (wantIntrMesh)
            {
                output.intersectionMesh.configuration = CM::CFG_POLYGON;
                uvt.RemoveDuplicateAndUnusedVertices(splitVertices, intrMesh,
                    output.intersectionMesh.vertices,
                    output.intersectionMesh.triangles);
            }
        }

    private:
        friend class UnitTestIntrConvexMesh3Plane3;
    };
}
