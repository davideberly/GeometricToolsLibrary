// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.28

#pragma once

// The fundamental problem is to compute the triangulation of a polygon tree.
// The outer polygons have counterclockwise ordered vertices. The inner
// polygons have clockwise ordered vertices. The algorithm uses Constrained
// Delaunay Triangulation and the implementation allows polygons to share
// vertices and edges.
//
// The polygons are not required to be simple in the sense that a vertex can
// be shared by an even number of edges, where the number is larger than 2.
// The input points can have duplicates, which the triangulator handles
// correctly. The algorithm supports coincident vertex-edge and coincident
// edge-edge configurations. See the document
//   https://www.geometrictools.com/Documentation/TriangulationByCDT.pdf
// for examples.
//
// If two edges intersect at edge-interior points, the current implementation
// cannot handle this. A pair of such edges cannot simultaneously be inserted
// into the constrained triangulation without affecting each other's local
// re-triangulation. A pending work item is to add validation code and fix
// an incoming malformed polygon tree during the triangulation.
//
// The input points are a vertex pool. The input tree is a PolygonTree object,
// defined in PolygonTree.h. Any outer polygon has vertices points[outer[0]]
// through points[outer[outer.size()-1]] listed in counterclockwise order. Any
// inner polygon has vertices points[inner[0]] through
// points[inner[inner.size()-1]] listed in clockwise order. The output tree
// contains the triangulation of the polygon tree on a per-node basis. If
// coincident vertex-edge or coincident edge-edge configurations exist in
// the polygon tree, the corresponding output polygons differ from the input
// polygons in that they have more vertices due to edge splits. The triangle
// chirality (winding order) is the same as the containing polygon.
//
// The input type T must satisfy std::is_floating_point<T>::value = true.

#include <GTL/Mathematics/Geometry/2D/ConstrainedDelaunay2.h>
#include <GTL/Mathematics/Geometry/2D/PolygonTree.h>
#include <array>
#include <limits>
#include <map>
#include <memory>
#include <numeric>
#include <queue>
#include <set>
#include <type_traits>
#include <utility>
#include <vector>

namespace gtl
{
    template <typename T>
    class TriangulateCDT
    {
    public:
        TriangulateCDT()
            :
            mCDT{}
        {
            static_assert(
                std::is_floating_point<T>::value,
                "The input type must be a floating-point type.");
        }

        // Triangulate a polygon tree using Constrained Delaunay
        // triangulation. The code uses a blend of interval arithmetic and
        // rational arithmetic. The code runs single-threaded when
        // lgNumThreads = 0. It runs multithreaded when lgNumThreads > 0,
        // where the number of threads is 2^{lgNumThreads} > 1.
        void operator()(
            std::size_t numInputPoints,
            Vector2<T> const* inputPoints,
            std::shared_ptr<PolygonTree> const& inputTree,
            PolygonTreeEx& outputTree)
        {
            GTL_ARGUMENT_ASSERT(
                numInputPoints >= 3 && inputPoints != nullptr && inputTree,
                "Invalid argument.");

            CopyAndCompactify(inputTree, outputTree);
            Triangulate(numInputPoints, inputPoints, outputTree);
        }

        void operator()(
            std::vector<Vector2<T>> const& inputPoints,
            std::shared_ptr<PolygonTree> const& inputTree,
            PolygonTreeEx& outputTree)
        {
            GTL_ARGUMENT_ASSERT(
                inputPoints.size() >= 3 && inputTree != nullptr,
                "Invalid argument.");

            CopyAndCompactify(inputTree, outputTree);
            Triangulate(inputPoints.size(), inputPoints.data(), outputTree);
        }

    private:
        void CopyAndCompactify(std::shared_ptr<PolygonTree> const& input,
            PolygonTreeEx& output)
        {
            output.nodes.clear();
            output.interiorTriangles.clear();
            output.interiorNodeIndices.clear();
            output.exteriorTriangles.clear();
            output.exteriorNodeIndices.clear();
            output.insideTriangles.clear();
            output.insideNodeIndices.clear();
            output.outsideTriangles.clear();
            output.allTriangles.clear();

            // Count the number of nodes in the tree.
            std::size_t numNodes = 1;  // the root node
            std::queue<std::shared_ptr<PolygonTree>> queue{};
            queue.push(input);
            while (queue.size() > 0)
            {
                auto const& node = queue.front();
                queue.pop();
                numNodes += node->child.size();
                for (auto const& child : node->child)
                {
                    queue.push(child);
                }
            }

            // Create the PolygonTreeEx nodes.
            output.nodes.resize(numNodes);
            for (std::size_t i = 0; i < numNodes; ++i)
            {
                output.nodes[i].self = i;
            }
            output.nodes[0].chirality = +1;
            output.nodes[0].parent = std::numeric_limits<std::size_t>::max();

            std::size_t current = 0, last = 0, minChild = 1;
            queue.push(input);
            while (queue.size() > 0)
            {
                auto const& node = queue.front();
                queue.pop();
                auto& exnode = output.nodes[current++];
                exnode.polygon = node->polygon;
                exnode.minChild = minChild;
                exnode.supChild = minChild + node->child.size();
                minChild = exnode.supChild;
                for (auto const& child : node->child)
                {
                    auto& exchild = output.nodes[++last];
                    exchild.chirality = -exnode.chirality;
                    exchild.parent = exnode.self;
                    queue.push(child);
                }
            }
        }

        void Triangulate(std::size_t numInputPoints, Vector2<T> const* inputPoints,
            PolygonTreeEx& tree)
        {
            // The constrained Delaunay triangulator will be given the unique
            // points referenced by the polygons in the tree. The tree
            // 'polygon' indices are relative to inputPoints[], but they are
            // temporarily mapped to indices relative to 'points'. Once the
            // triangulation is complete, the indices are restored to those
            // relative to inputPoints[].
            std::vector<Vector2<T>> points{};
            std::vector<std::size_t> remapping{};
            RemapPolygonTree(numInputPoints, inputPoints, tree, points, remapping);
            GTL_RUNTIME_ASSERT(
                points.size() >= 3,
                "A polygon tree must have at least one triangle.");

            std::set<EdgeKey<false>> edges{};
            ConstrainedTriangulate(tree, points, edges);
            ClassifyTriangles(tree, edges);

            RestorePolygonTree(tree, remapping);
        }

        // On return, 'points' are the unique inputPoints[] values referenced by
        // the tree. The tree 'polygon' members are modified to be indices
        // into 'points' rather than inputPoints[]. The 'remapping' allows us to
        // restore the tree 'polygon' members to be indices into inputPoints[]
        // after the triangulation is computed. The 'edges' stores all the
        // polygon edges that must be in the triangulation.
        void RemapPolygonTree(
            std::size_t numInputPoints,
            Vector2<T> const* inputPoints,
            PolygonTreeEx& tree,
            std::vector<Vector2<T>>& points,
            std::vector<std::size_t>& remapping)
        {
            std::map<Vector2<T>, std::size_t> pointMap{};
            points.reserve(numInputPoints);
            std::size_t currentIndex = 0;

            // The remapping is initially the identity, remapping[j] = j.
            remapping.resize(numInputPoints);
            std::iota(remapping.begin(), remapping.end(), 0);

            std::queue<std::size_t> queue{};
            queue.push(0);
            while (queue.size() > 0)
            {
                PolygonTreeEx::Node& node = tree.nodes[queue.front()];
                queue.pop();
                std::size_t const numIndices = node.polygon.size();
                for (std::size_t i = 0; i < numIndices; ++i)
                {
                    auto const& point = inputPoints[node.polygon[i]];
                    auto iter = pointMap.find(point);
                    if (iter == pointMap.end())
                    {
                        // The point is encountered the first time.
                        pointMap.insert(std::make_pair(point, currentIndex));
                        remapping[currentIndex] = node.polygon[i];
                        node.polygon[i] = currentIndex;
                        points.push_back(point);
                        ++currentIndex;
                    }
                    else
                    {
                        // The point is a duplicate. On the remapping, the
                        // polygon[] value is set to the index for the
                        // first occurrence of the duplicate.
                        remapping[iter->second] = node.polygon[i];
                        node.polygon[i] = iter->second;
                    }

                }

                for (std::size_t c = node.minChild; c < node.supChild; ++c)
                {
                    queue.push(c);
                }
            }
        }

        void RestorePolygonTree(PolygonTreeEx& tree, std::vector<std::size_t> const& remapping)
        {
            std::queue<std::size_t> queue{};
            queue.push(0);
            while (queue.size() > 0)
            {
                auto& node = tree.nodes[queue.front()];
                queue.pop();

                for (auto& index : node.polygon)
                {
                    index = remapping[index];
                }
                for (auto& tri : node.triangulation)
                {
                    for (std::size_t j = 0; j < 3; ++j)
                    {
                        tri[j] = remapping[tri[j]];
                    }
                }

                for (std::size_t c = node.minChild; c < node.supChild; ++c)
                {
                    queue.push(c);
                }
            }

            for (auto& tri : tree.interiorTriangles)
            {
                for (std::size_t j = 0; j < 3; ++j)
                {
                    tri[j] = remapping[tri[j]];
                }
            }

            for (auto& tri : tree.exteriorTriangles)
            {
                for (std::size_t j = 0; j < 3; ++j)
                {
                    tri[j] = remapping[tri[j]];
                }
            }

            for (auto& tri : tree.insideTriangles)
            {
                for (std::size_t j = 0; j < 3; ++j)
                {
                    tri[j] = remapping[tri[j]];
                }
            }

            for (auto& tri : tree.outsideTriangles)
            {
                for (std::size_t j = 0; j < 3; ++j)
                {
                    tri[j] = remapping[tri[j]];
                }
            }

            for (auto& tri : tree.allTriangles)
            {
                for (std::size_t j = 0; j < 3; ++j)
                {
                    tri[j] = remapping[tri[j]];
                }
            }
        }

        void ConstrainedTriangulate(
            PolygonTreeEx& tree,
            std::vector<Vector2<T>> const& points,
            std::set<EdgeKey<false>>& edges)
        {
            // Use constrained Delaunay triangulation.
            mCDT(points);
            GTL_RUNTIME_ASSERT(
                mCDT.GetDimension() == 2,
                "The input points must have intrinsic dimension 2.");

            std::vector<std::size_t> outEdge{};
            std::queue<std::size_t> queue{};
            queue.push(0);
            while (queue.size() > 0)
            {
                auto& node = tree.nodes[queue.front()];
                queue.pop();

                std::vector<std::size_t> replacement{};
                std::size_t numIndices = node.polygon.size();
                for (std::size_t i0 = numIndices - 1, i1 = 0; i1 < numIndices; i0 = i1++)
                {
                    // Insert the polygon edge into the constrained Delaunay
                    // triangulation.
                    std::array<std::size_t, 2> edge = { node.polygon[i0], node.polygon[i1] };
                    outEdge.clear();
                    mCDT.Insert(edge, outEdge);
                    if (outEdge.size() > 2)
                    {
                        // The polygon edge intersects additional vertices in
                        // the triangulation. The outEdge[] edge values are
                        // { edge[0], other_vertices, edge[1] } which are
                        // ordered along the line segment.
                        replacement.insert(replacement.end(), outEdge.begin() + 1, outEdge.end());
                    }
                    else
                    {
                        replacement.push_back(node.polygon[i1]);
                    }
                }
                if (replacement.size() > node.polygon.size())
                {
                    node.polygon = std::move(replacement);
                }

                numIndices = node.polygon.size();
                for (std::size_t i0 = numIndices - 1, i1 = 0; i1 < numIndices; i0 = i1++)
                {
                    edges.insert(EdgeKey<false>(node.polygon[i0], node.polygon[i1]));
                }

                for (std::size_t c = node.minChild; c < node.supChild; ++c)
                {
                    queue.push(c);
                }
            }

            // Store the triangles in allTriangles for potential use by the
            // caller.
            auto const& tmap = mCDT.GetConstrainedMesh().GetTriangles();
            tree.allTriangles.resize(tmap.size());
            std::size_t t = 0;
            for (auto const& element : tmap)
            {
                auto const& tri = element.first;
                tree.allTriangles[t++] = tri;
            }
        }

        void ClassifyTriangles(PolygonTreeEx& tree, std::set<EdgeKey<false>>& edges)
        {
            // With a correct implementation, the GTL_RUNTIME_ASSERT should
            // not trigger.
            ClassifyDFS(tree, 0, edges);
            GTL_RUNTIME_ASSERT(
                edges.size() == 0,
                "Unexpected condition.");

            GetOutsideTriangles(tree);
            GetInsideTriangles(tree);
        }

        void ClassifyDFS(PolygonTreeEx& tree, std::size_t index,
            std::set<EdgeKey<false>>& edges)
        {
            auto& node = tree.nodes[index];
            for (std::size_t c = node.minChild; c < node.supChild; ++c)
            {
                ClassifyDFS(tree, c, edges);
            }

            auto const& mesh = mCDT.GetConstrainedMesh();
            auto const& emap = mesh.GetEdges();
            std::set<TriangleKey<true>> region{};
            std::size_t const numIndices = node.polygon.size();
            for (std::size_t i0 = numIndices - 1, i1 = 0; i1 < numIndices; i0 = i1++)
            {
                // With a correct implementation, the GTL_RUNTIME_ASSERTs
                // should not trigger.
                std::size_t const v0 = node.polygon[i0];
                std::size_t const v1 = node.polygon[i1];
                EdgeKey<false> ekey(v0, v1);
                auto eiter = emap.find(ekey);
                GTL_RUNTIME_ASSERT(
                    eiter != emap.end(),
                    "Unexpected condition.");

                auto const* edge = eiter->second.get();
                GTL_RUNTIME_ASSERT(
                    edge != nullptr,
                    "Unexpected condition.");

                auto tri0 = edge->T[0];
                GTL_RUNTIME_ASSERT(
                    tri0 != nullptr,
                    "Unexpected condition.");

                if (tri0->WhichSideOfEdge(v0, v1) == node.chirality)
                {
                    region.insert(TriangleKey<true>(tri0->V[0], tri0->V[1], tri0->V[2]));
                }
                else
                {
                    auto tri1 = edge->T[1];
                    if (tri1)
                    {
                        region.insert(TriangleKey<true>(tri1->V[0], tri1->V[1], tri1->V[2]));
                    }
                }
            }

            FillRegion(edges, region);
            ExtractTriangles(region, node);
            for (std::size_t i0 = numIndices - 1, i1 = 0; i1 < numIndices; i0 = i1++)
            {
                edges.erase(EdgeKey<false>(node.polygon[i0], node.polygon[i1]));
            }
        }

        // On input, the set has the initial seeds for the desired region. A
        // breadth-first search is performed to find the connected component
        // of the seeds. The component is bounded by an outer polygon and the
        // inner polygons of its children.
        void FillRegion(std::set<EdgeKey<false>> const& edges,
            std::set<TriangleKey<true>>& region)
        {
            std::queue<TriangleKey<true>> regionQueue{};
            for (auto const& tkey : region)
            {
                regionQueue.push(tkey);
            }

            auto const& mesh = mCDT.GetConstrainedMesh();
            auto const& tmap = mesh.GetTriangles();
            while (regionQueue.size() > 0)
            {
                // With a correct implementation, the runtime assertions
                // should not trigger.
                TriangleKey<true> tkey = regionQueue.front();
                regionQueue.pop();
                auto titer = tmap.find(tkey);
                GTL_RUNTIME_ASSERT(
                    titer != tmap.end(),
                    "Unexpected condition.");

                auto const* tri = titer->second.get();
                GTL_RUNTIME_ASSERT(
                    tri != nullptr,
                    "Unexpected condition.");

                for (std::size_t j = 0; j < 3; ++j)
                {
                    auto edge = tri->E[j];
                    if (edge)
                    {
                        auto siter = edges.find(EdgeKey<false>(edge->V[0], edge->V[1]));
                        if (siter == edges.end())
                        {
                            // The edge is not constrained, so it allows the
                            // search to continue.
                            auto adj = tri->T[j];
                            if (adj)
                            {
                                TriangleKey<true> akey(adj->V[0], adj->V[1], adj->V[2]);
                                auto riter = region.find(akey);
                                if (riter == region.end())
                                {
                                    // The adjacent triangle has not yet been
                                    // visited, so place it in the queue to
                                    // continue the search.
                                    region.insert(akey);
                                    regionQueue.push(akey);
                                }
                            }
                        }
                    }
                }
            }
        }

        // Store the region triangles in a triangulation object and remove
        // those triangles from the mesh in preparation for processing the
        // next layer of triangles.
        void ExtractTriangles(std::set<TriangleKey<true>> const& region,
            PolygonTreeEx::Node& node)
        {
            auto& mesh = mCDT.GetConstrainedMesh();
            node.triangulation.reserve(region.size());
            if (node.chirality > 0)
            {
                for (auto const& tri : region)
                {
                    node.triangulation.push_back({ tri[0], tri[1], tri[2] });
                    (void)mesh.Remove(tri[0], tri[1], tri[2]);
                }
            }
            else  // node.chirality < 0
            {
                for (auto const& tri : region)
                {
                    node.triangulation.push_back({ tri[0], tri[2], tri[1] });
                    (void)mesh.Remove(tri[0], tri[1], tri[2]);
                }
            }
        }

        void GetOutsideTriangles(PolygonTreeEx& tree)
        {
            auto& mesh = mCDT.GetConstrainedMesh();
            auto const& tmap = mesh.GetTriangles();
            tree.outsideTriangles.resize(tmap.size());
            std::size_t t = 0;
            for (auto const& tri : tmap)
            {
                auto const& tkey = tri.first;
                tree.outsideTriangles[t++] = tkey;
            }
            mesh.Clear();
        }

        // Get the triangles in the polygon tree, classifying each as
        // interior (in region bounded by outer polygon and its contained
        // inner polygons) or exterior (in region bounded by inner polygon
        // and its contained outer polygons). The inside triangles are the
        // union of the interior and exterior triangles.
        void GetInsideTriangles(PolygonTreeEx& tree)
        {
            std::size_t const numTriangles = tree.allTriangles.size();
            std::size_t const numOutside = tree.outsideTriangles.size();
            std::size_t const numInside = numTriangles - numOutside;
            tree.interiorTriangles.reserve(numTriangles);
            tree.interiorNodeIndices.reserve(numTriangles);
            tree.exteriorTriangles.reserve(numTriangles);
            tree.exteriorNodeIndices.reserve(numTriangles);
            tree.insideTriangles.reserve(numInside);
            tree.insideNodeIndices.reserve(numInside);

            for (std::size_t nIndex = 0; nIndex < tree.nodes.size(); ++nIndex)
            {
                auto const& node = tree.nodes[nIndex];
                for (auto& tri : node.triangulation)
                {
                    if (node.chirality > 0)
                    {
                        tree.interiorTriangles.push_back(tri);
                        tree.interiorNodeIndices.push_back(nIndex);
                    }
                    else
                    {
                        tree.exteriorTriangles.push_back(tri);
                        tree.exteriorNodeIndices.push_back(nIndex);
                    }

                    tree.insideTriangles.push_back(tri);
                    tree.insideNodeIndices.push_back(nIndex);
                }
            }
        }

        ConstrainedDelaunay2<T> mCDT;

    private:
        friend class UnitTestTriangulateCDT;
    };
}
