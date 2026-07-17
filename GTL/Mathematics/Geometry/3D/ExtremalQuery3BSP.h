// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.17

#pragma once

// Compute the extreme vertices of a convex polyhedron in a specified
// direction using Binary Space Partitioning (BSP) trees. For details, see
//   https://www.geometrictools.com/Documentation/ExtremalPolytopeQueries.pdf

#include <GTL/Mathematics/Geometry/3D/ExtremalQuery3.h>
#include <GTL/Mathematics/Arithmetic/IEEEFunctions.h>
#include <GTL/Mathematics/Meshes/DynamicVETManifoldMesh.h>
#include <GTL/Utility/RangeIteration.h>
#include <array>
#include <cstddef>
#include <limits>
#include <map>
#include <set>
#include <stack>
#include <queue>
#include <unordered_set>
#include <utility>
#include <vector>

namespace gtl
{
    template <typename T>
    class ExtremalQuery3BSP : public ExtremalQuery3<T>
    {
    public:
        ExtremalQuery3BSP(ConvexPolyhedron3<T> const& polytope)
            :
            ExtremalQuery3<T>(polytope),
            mTriToNormal{},
            mNodes{},
            mTreeDepth(0)
        {
            // Create the adjacency information for the polytope.
            DynamicVETManifoldMesh mesh{};
            std::size_t const numTriangles = this->mPolytope.indices.size() / 3;
            for (std::size_t t = 0; t < numTriangles; ++t)
            {
                std::array<std::size_t, 3> V = { 0, 0, 0 };
                for (std::size_t j = 0; j < 3; ++j)
                {
                    V[j] = this->mPolytope.indices[3 * t + j];
                }
                auto* triangle = mesh.Insert(V[0], V[1], V[2]);
                mTriToNormal.insert(std::make_pair(triangle, t));
            }

            // Create the set of unique arcs which are used to create the BSP
            // tree.
            std::multiset<SphericalArc> arcs{};
            CreateSphericalArcs(mesh, arcs);

            // Create the BSP tree to be used in the extremal query.
            CreateBSPTree(arcs);
        }

        // Disallow copying and assignment.
        ExtremalQuery3BSP(ExtremalQuery3BSP const&) = delete;
        ExtremalQuery3BSP& operator=(ExtremalQuery3BSP const&) = delete;

        // Compute the extreme vertices in the specified direction and return
        // the indices of the vertices in the polyhedron vertex array.
        virtual void GetExtremeVertices(Vector3<T> const& direction,
            std::size_t& positiveDirection, std::size_t& negativeDirection) override
        {
            // Do a nonrecursive depth-first search of the BSP tree to
            // determine spherical polygon contains the incoming direction D.
            // Index 0 is the root of the BSP tree.
            std::size_t current = 0;
            while (current != SphericalArc::invalid)
            {
                SphericalArc& node = mNodes[current];
                std::int32_t sign = isign(Dot(direction, node.normal));
                if (sign >= 0)
                {
                    current = node.posChild;
                    if (current == SphericalArc::invalid)
                    {
                        // At a leaf node.
                        positiveDirection = node.posVertex;
                    }
                }
                else
                {
                    current = node.negChild;
                    if (current == SphericalArc::invalid)
                    {
                        // At a leaf node.
                        positiveDirection = node.negVertex;
                    }
                }
            }

            // Do a nonrecursive depth-first search of the BSP tree to
            // determine spherical polygon contains the reverse incoming
            // direction -D.
            current = 0;  // the root of the BSP tree
            while (current != SphericalArc::invalid)
            {
                SphericalArc& node = mNodes[current];
                std::int32_t sign = isign(Dot(direction, node.normal));
                if (sign <= 0)
                {
                    current = node.posChild;
                    if (current == SphericalArc::invalid)
                    {
                        // At a leaf node.
                        negativeDirection = node.posVertex;
                    }
                }
                else
                {
                    current = node.negChild;
                    if (current == SphericalArc::invalid)
                    {
                        // At a leaf node.
                        negativeDirection = node.negVertex;
                    }
                }
            }
        }

        // Tree statistics.
        inline std::size_t GetNumNodes() const
        {
            return mNodes.size();
        }

        inline std::size_t GetTreeDepth() const
        {
            return mTreeDepth;
        }

    private:
        class SphericalArc
        {
        public:
            static std::size_t constexpr invalid = std::numeric_limits<std::size_t>::max();

            // Construction.
            SphericalArc()
                :
                nIndex{ invalid, invalid },
                separation(0),
                normal{},
                posVertex(invalid),
                negVertex(invalid),
                posChild(invalid),
                negChild(invalid)
            {
            }

            // The arcs are stored in a multiset ordered by increasing
            // separation. The multiset will be traversed in reverse order.
            // This heuristic is designed to create BSP trees whose top-most
            // nodes can eliminate as many arcs as possible during an extremal
            // query.
            bool operator<(SphericalArc const& arc) const
            {
                return separation < arc.separation;
            }

            // Indices N[] into the face normal array for the endpoints of the
            // arc.
            std::array<std::size_t, 2> nIndex;

            // The number of arcs in the path from normal N[0] to normal N[1].
            // For spherical polygon edges, the number is 1. The number is 2
            // or larger for bisector arcs of the spherical polygon.
            std::size_t separation;

            // The normal is Cross(FaceNormal[N[0]],FaceNormal[N[1]]).
            Vector3<T> normal;

            // Indices into the vertex array for the extremal points for the
            // two regions sharing the arc. As the arc is traversed from
            // normal N[0] to normal N[1], PosVertex is the index for the
            // extreme vertex to the left of the arc and NegVertex is the
            // index for the extreme vertex to the right of the arc.
            std::size_t posVertex, negVertex;

            // Support for BSP trees stored as contiguous nodes in an array.
            std::size_t posChild, negChild;
        };

        typedef DynamicVETManifoldMesh::Triangle Triangle;

        void SortAdjacentTriangles(std::size_t vIndex,
            std::unordered_set<Triangle*> const& tAdj,
            std::vector<Triangle*>& tAdjSorted)
        {
            // Copy the set of adjacent triangles into a vector container.
            std::size_t const numTriangles = tAdj.size();
            tAdjSorted.resize(numTriangles);

            // Traverse the triangles adjacent to vertex V using edge-triangle
            // adjacency information to produce a sorted array of adjacent
            // triangles.
            Triangle* tri = *tAdj.begin();
            for (std::size_t i = 0; i < numTriangles; ++i)
            {
                for (std::size_t prev = 2, curr = 0; curr < 3; prev = curr++)
                {
                    if (tri->V[curr] == vIndex)
                    {
                        tAdjSorted[i] = tri;
                        tri = tri->T[prev];
                        break;
                    }
                }
            }
        }

        void CreateSphericalArcs(DynamicVETManifoldMesh& mesh, std::multiset<SphericalArc>& arcs)
        {
            std::size_t const prev[3] = { 2, 0, 1 };
            std::size_t const next[3] = { 1, 2, 0 };

            for (auto const& element : mesh.GetEdges())
            {
                auto const& edge = element.second;

                SphericalArc arc{};

                auto iter = mTriToNormal.find(edge->T[0]);
                GTL_RUNTIME_ASSERT(
                    iter != mTriToNormal.end(),
                    "Unexpected condition.");
                arc.nIndex[0] = iter->second;

                iter = mTriToNormal.find(edge->T[1]);
                GTL_RUNTIME_ASSERT(
                    iter != mTriToNormal.end(),
                    "Unexpected condition.");
                arc.nIndex[1] = iter->second;

                arc.separation = 1;
                arc.normal = Cross(this->mFaceNormals[arc.nIndex[0]], this->mFaceNormals[arc.nIndex[1]]);

                Triangle* adj = edge->T[0];
                std::size_t j{};
                for (j = 0; j < 3; ++j)
                {
                    if (adj->V[j] != edge->V[0] && adj->V[j] != edge->V[1])
                    {
                        arc.posVertex = adj->V[prev[j]];
                        arc.negVertex = adj->V[next[j]];
                        break;
                    }
                }
                GTL_RUNTIME_ASSERT(
                    j < 3,
                    "Unexpected condition.");

                arcs.insert(arc);
            }

            CreateSphericalBisectors(mesh, arcs);
        }

        void CreateSphericalBisectors(DynamicVETManifoldMesh& mesh, std::multiset<SphericalArc>& arcs)
        {
            std::queue<std::pair<std::size_t, std::size_t>> queue{};
            for (auto const& element : mesh.GetVertices())
            {
                // Sort the normals into a counterclockwise spherical polygon
                // when viewed from outside the sphere.
                auto const& vertex = element.second;
                std::size_t const vIndex = vertex->V;
                std::vector<Triangle*> tAdjSorted{};
                SortAdjacentTriangles(vIndex, vertex->TAdjacent, tAdjSorted);
                std::size_t const numTriangles = vertex->TAdjacent.size();
                queue.push(std::make_pair(0, numTriangles));
                while (!queue.empty())
                {
                    std::pair<std::size_t, std::size_t> item = queue.front();
                    queue.pop();
                    std::size_t i0 = item.first, i1 = item.second;
                    std::size_t separation = i1 - i0;
                    if (separation > 1 && separation + 1 != numTriangles)
                    {
                        if (i1 < numTriangles)
                        {
                            SphericalArc arc{};

                            auto iter = mTriToNormal.find(tAdjSorted[i0]);
                            GTL_RUNTIME_ASSERT(
                                iter != mTriToNormal.end(),
                                "Unexpected condition.");
                            arc.nIndex[0] = iter->second;

                            iter = mTriToNormal.find(tAdjSorted[i1]);
                            GTL_RUNTIME_ASSERT(
                                iter != mTriToNormal.end(),
                                "Unexpected condition.");
                            arc.nIndex[1] = iter->second;

                            arc.separation = separation;

                            arc.normal = Cross(this->mFaceNormals[arc.nIndex[0]],
                                this->mFaceNormals[arc.nIndex[1]]);

                            arc.posVertex = vIndex;
                            arc.negVertex = vIndex;
                            arcs.insert(arc);
                        }
                        std::size_t imid = (i0 + i1 + 1) / 2;
                        if (imid != i1)
                        {
                            queue.push(std::make_pair(i0, imid));
                            queue.push(std::make_pair(imid, i1));
                        }
                    }
                }
            }
        }

        void CreateBSPTree(std::multiset<SphericalArc>& arcs)
        {
            // The tree has at least a root.
            mTreeDepth = 1;

            for (auto const& arc : gtl::reverse(arcs))
            {
                InsertArc(arc);
            }

            // The leaf nodes are not counted in the traversal of InsertArc.
            // The depth must be incremented to account for leaves.
            ++mTreeDepth;
        }

        void InsertArc(SphericalArc const& arc)
        {
            // The incoming arc is stored at the end of the nodes array.
            if (mNodes.size() > 0)
            {
                // Do a nonrecursive depth-first search of the current BSP
                // tree to place the incoming arc. Index 0 is the root of the
                // BSP tree.
                std::stack<std::size_t> candidates{};
                candidates.push(0);
                while (!candidates.empty())
                {
                    std::size_t current = candidates.top();
                    candidates.pop();
                    SphericalArc* node = &mNodes[current];

                    std::int32_t sign0{};
                    if (arc.nIndex[0] == node->nIndex[0] || arc.nIndex[0] == node->nIndex[1])
                    {
                        sign0 = 0;
                    }
                    else
                    {
                        T dot = Dot(this->mFaceNormals[arc.nIndex[0]], node->normal);
                        sign0 = isign(dot);
                    }

                    std::int32_t sign1{};
                    if (arc.nIndex[1] == node->nIndex[0] || arc.nIndex[1] == node->nIndex[1])
                    {
                        sign1 = 0;
                    }
                    else
                    {
                        T dot = Dot(this->mFaceNormals[arc.nIndex[1]], node->normal);
                        sign1 = isign(dot);
                    }

                    std::size_t doTest = 0;
                    if (sign0 * sign1 < 0)
                    {
                        // The new arc straddles the current arc, so propagate
                        // it to both child nodes.
                        doTest = 3;
                    }
                    else if (sign0 > 0 || sign1 > 0)
                    {
                        // The new arc is on the positive side of the current
                        // arc.
                        doTest = 1;
                    }
                    else if (sign0 < 0 || sign1 < 0)
                    {
                        // The new arc is on the negative side of the current
                        // arc.
                        doTest = 2;
                    }
                    // else: sign0 = sign1 = 0, in which case no propagation
                    // is needed because the current BSP node will handle the
                    // correct partitioning of the arcs during extremal
                    // queries.

                    std::size_t depth{};

                    if (doTest & 1)
                    {
                        if (node->posChild != SphericalArc::invalid)
                        {
                            candidates.push(node->posChild);
                            depth = candidates.size();
                            if (depth > mTreeDepth)
                            {
                                mTreeDepth = depth;
                            }
                        }
                        else
                        {
                            node->posChild = mNodes.size();
                            mNodes.push_back(arc);

                            // The push_back can cause a reallocation, so the
                            // current pointer must be refreshed.
                            node = &mNodes[current];
                        }
                    }

                    if (doTest & 2)
                    {
                        if (node->negChild != SphericalArc::invalid)
                        {
                            candidates.push(node->negChild);
                            depth = candidates.size();
                            if (depth > mTreeDepth)
                            {
                                mTreeDepth = depth;
                            }
                        }
                        else
                        {
                            node->negChild = mNodes.size();
                            mNodes.push_back(arc);
                        }
                    }
                }
            }
            else
            {
                // root node
                mNodes.push_back(arc);
            }
        }

        // Lookup table for indexing into mFaceNormals.
        std::map<Triangle*, std::size_t> mTriToNormal;

        // Fixed-size storage for the BSP nodes.
        std::vector<SphericalArc> mNodes;
        std::size_t mTreeDepth;

    private:
        friend class UnitTestExtremalQuery3BSP;
    };
}
