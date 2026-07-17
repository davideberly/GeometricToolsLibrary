// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.15

#pragma once

// DynamicETNonmanifoldMesh represents an edge-triangle mesh for which
// triangle insertions and removals can occur at any time. The mesh is
// nonmanifold in that an edge can share 3 or more triangles.

#include <GTL/Utility/Exceptions.h>
#include <GTL/Mathematics/Meshes/EdgeKey.h>
#include <GTL/Mathematics/Meshes/TriangleKey.h>
#include <array>
#include <cstddef>
#include <limits>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace gtl
{
    class DynamicETNonmanifoldMesh
    {
    public:
        // Use the maximum std::size_t to denote an invalid index, effectively
        // representing -1.
        static std::size_t constexpr invalid = std::numeric_limits<std::size_t>::max();

        // Edge data types.
        class Edge;
        using ECreator = std::unique_ptr<Edge>(*)(std::size_t, std::size_t);
        using EMap = std::unordered_map<EdgeKey<false>, std::unique_ptr<Edge>,
            EdgeKey<false>, EdgeKey<false>>;

        // Triangle data types.
        class Triangle;
        using TCreator = std::unique_ptr<Triangle>(*)(std::size_t, std::size_t, std::size_t);
        using TMap = std::unordered_map<TriangleKey<true>, std::unique_ptr<Triangle>,
            TriangleKey<true>, TriangleKey<true>>;

        // Edge object.
        class Edge
        {
        public:
            virtual ~Edge() = default;

            Edge(std::size_t v0, std::size_t v1)
                :
                V{ v0, v1 },
                T{}
            {
            }

            // Vertices of the edge.
            std::array<std::size_t, 2> V;

            // Triangles sharing the edge.
            std::unordered_set<Triangle*> T;
        };

        // Triangle object.
        class Triangle
        {
        public:
            virtual ~Triangle() = default;

            Triangle(std::size_t v0, std::size_t v1, std::size_t v2)
                :
                V{ v0, v1, v2 },
                E{}
            {
            }

            // Vertices listed in counterclockwise order (V[0],V[1],V[2]).
            std::array<std::size_t, 3> V;

            // Adjacent edges. E[i] points to edge (V[i],V[(i+1)%3]).
            std::array<Edge*, 3> E;
        };


        DynamicETNonmanifoldMesh(ECreator eCreator = nullptr, TCreator tCreator = nullptr)
            :
            mECreator(eCreator ? eCreator : CreateEdge),
            mEMap{},
            mTCreator(tCreator ? tCreator : CreateTriangle),
            mTMap{}
        {
        }

        // Support for a deep copy of the mesh. The mEMap and mTMap objects
        // have dynamically allocated memory for edges and triangles. A
        // shallow copy of the pointers to this memory is problematic.
        // Allowing sharing, say, via std::shared_ptr, is an option but not
        // really the intent of copying the mesh graph.
        DynamicETNonmanifoldMesh(DynamicETNonmanifoldMesh const& mesh)
            :
            DynamicETNonmanifoldMesh{}
        {
            *this = mesh;
        }

        virtual ~DynamicETNonmanifoldMesh() = default;

        DynamicETNonmanifoldMesh& operator=(DynamicETNonmanifoldMesh const& mesh)
        {
            Clear();

            mECreator = mesh.mECreator;
            mTCreator = mesh.mTCreator;
            for (auto const& element : mesh.mTMap)
            {
                // The typecast avoids warnings about not storing the return
                // value in a named variable. The return value is discarded.
                (void)Insert(element.first[0], element.first[1], element.first[2]);
            }

            return *this;
        }

        // Member access.
        inline EMap const& GetEdges() const
        {
            return mEMap;
        }

        inline TMap const& GetTriangles() const
        {
            return mTMap;
        }

        // If <v0,v1,v2> is not in the mesh, a Triangle object is created and
        // returned; otherwise, <v0,v1,v2> is in the mesh and nullptr is
        // returned.
        virtual Triangle* Insert(std::size_t v0, std::size_t v1, std::size_t v2)
        {
            TriangleKey<true> tkey(v0, v1, v2);
            if (mTMap.find(tkey) != mTMap.end())
            {
                // The triangle already exists. Return a null pointer as a
                // signal to the caller that the insertion failed.
                return nullptr;
            }

            // Create the new triangle. It will be added to mTMap at the end
            // of the function so that if an assertion is triggered and the
            // function returns early, the (bad) triangle will not be part of
            // the mesh.
            std::unique_ptr<Triangle> newTri = mTCreator(v0, v1, v2);
            Triangle* tri = newTri.get();

            // Add the edges to the mesh if they do not already exist.
            for (std::size_t i0 = 2, i1 = 0; i1 < 3; i0 = i1++)
            {
                EdgeKey<false> ekey(tri->V[i0], tri->V[i1]);
                Edge* edge = nullptr;
                auto eiter = mEMap.find(ekey);
                if (eiter == mEMap.end())
                {
                    // This is the first time the edge is encountered.
                    std::unique_ptr<Edge> newEdge = mECreator(tri->V[i0], tri->V[i1]);
                    edge = newEdge.get();
                    mEMap[ekey] = std::move(newEdge);
                }
                else
                {
                    // The edge was previously encountered and created.
                    edge = eiter->second.get();
                    GTL_RUNTIME_ASSERT(
                        edge != nullptr,
                        "Expecting an edge.");
                }

                // Associate the edge with the triangle.
                tri->E[i0] = edge;

                // Update the adjacent set of triangles for the edge.
                edge->T.insert(tri);
            }

            mTMap[tkey] = std::move(newTri);
            return tri;
        }

        // If <v0,v1,v2> is in the mesh, it is removed and 'true' is returned;
        // otherwise, <v0,v1,v2> is not in the mesh and 'false' is returned.
        virtual bool Remove(std::size_t v0, std::size_t v1, std::size_t v2)
        {
            TriangleKey<true> tkey(v0, v1, v2);
            auto titer = mTMap.find(tkey);
            if (titer == mTMap.end())
            {
                // The triangle does not exist.
                return false;
            }

            // Get the triangle.
            Triangle* tri = titer->second.get();

            // Remove the edges and update adjacent triangles if necessary.
            for (std::size_t i = 0; i < 3; ++i)
            {
                // Inform the edges the triangle is being deleted.
                auto edge = tri->E[i];
                GTL_RUNTIME_ASSERT(
                    edge != nullptr,
                    "Expecting an edge.");

                // Remove the triangle from the edge's set of adjacent
                // triangles.
                std::size_t numRemoved = edge->T.erase(tri);
                GTL_RUNTIME_ASSERT(
                    numRemoved > 0,
                    "Expecting the triangle to be removed.");

                // Remove the edge if you have the last reference to it.
                if (edge->T.size() == 0)
                {
                    EdgeKey<false> ekey(edge->V[0], edge->V[1]);
                    mEMap.erase(ekey);
                }

                // TODO: DynamicETManifoldMesh has block of code
                // "Inform adjacent triangles the triangle is being deleted."
                // Is this needed here?
            }

            // Remove the triangle from the graph.
            mTMap.erase(tkey);
            return true;
        }

        // Destroy the edges and triangles to obtain an empty mesh.
        virtual void Clear()
        {
            mEMap.clear();
            mTMap.clear();
        }

        // A manifold mesh has the property that an edge is shared by at most
        // two triangles sharing.
        bool IsManifold() const
        {
            for (auto const& element : mEMap)
            {
                if (element.second->T.size() > 2)
                {
                    return false;
                }
            }
            return true;
        }

        // A manifold mesh is closed if each edge is shared twice. A closed
        // mesh is not necessarily oriented. For example, you could have a
        // mesh with spherical topology. The upper hemisphere has outer-facing
        // normals and the lower hemisphere has inner-facing normals. The
        // discontinuity in orientation occurs on the circle shared by the
        // hemispheres.
        bool IsClosed() const
        {
            for (auto const& element : mEMap)
            {
                if (element.second->T.size() != 2)
                {
                    return false;
                }
            }
            return true;
        }

        // Compute the connected components of the edge-triangle graph that
        // the mesh represents. The first function returns pointers into
        // 'this' object's containers, so you must consume the components
        // before clearing or destroying 'this'. The second function returns
        // triangle keys, which requires three times as much storage as the
        // pointers but allows you to clear or destroy 'this' before consuming
        // the components.
        void GetComponents(std::vector<std::vector<Triangle*>>& components) const
        {
            // visited: 0 (unvisited), 1 (discovered), 2 (finished)
            std::unordered_map<Triangle*, std::size_t> visited{};
            for (auto const& element : mTMap)
            {
                visited.insert(std::make_pair(element.second.get(), 0));
            }

            for (auto& element : mTMap)
            {
                Triangle* tri = element.second.get();
                if (visited[tri] == 0)
                {
                    std::vector<Triangle*> component{};
                    DepthFirstSearch(tri, visited, component);
                    components.push_back(std::move(component));
                }
            }
        }

        void GetComponents(std::vector<std::vector<TriangleKey<true>>>& components) const
        {
            // visited: 0 (unvisited), 1 (discovered), 2 (finished)
            std::unordered_map<Triangle*, std::size_t> visited{};
            for (auto const& element : mTMap)
            {
                visited.insert(std::make_pair(element.second.get(), 0));
            }

            for (auto& element : mTMap)
            {
                Triangle* tri = element.second.get();
                if (visited[tri] == 0)
                {
                    std::vector<Triangle*> component{};
                    DepthFirstSearch(tri, visited, component);

                    std::vector<TriangleKey<true>> keyComponent{};
                    keyComponent.reserve(component.size());
                    for (auto const& t : component)
                    {
                        keyComponent.push_back(TriangleKey<true>(t->V[0], t->V[1], t->V[2]));
                    }
                    components.push_back(keyComponent);
                }
            }
        }

    protected:
        // The edge data and default edge creation.
        static std::unique_ptr<Edge> CreateEdge(std::size_t v0, std::size_t v1)
        {
            return std::make_unique<Edge>(v0, v1);
        }

        // The triangle data and default triangle creation.
        static std::unique_ptr<Triangle> CreateTriangle(std::size_t v0, std::size_t v1, std::size_t v2)
        {
            return std::make_unique<Triangle>(v0, v1, v2);
        }

        // Support for computing connected components. This is a
        // straightforward depth-first search of the graph but uses a
        // preallocated stack rather than a recursive function that could
        // possibly overflow the call stack.
        void DepthFirstSearch(Triangle* tInitial,
            std::unordered_map<Triangle*, std::size_t>& visited,
            std::vector<Triangle*>& component) const
        {
            // Allocate the maximum-size stack that can occur in the
            // depth-first search. The stack is empty when the index top
            // is -1.
            std::vector<Triangle*> tStack(mTMap.size());
            std::size_t top = invalid;
            tStack[++top] = tInitial;
            while (top != invalid)
            {
                Triangle* tri = tStack[top];
                visited[tri] = 1;
                std::size_t i{};
                for (i = 0; i < 3; ++i)
                {
                    auto edge = tri->E[i];
                    GTL_RUNTIME_ASSERT(
                        edge != nullptr,
                        "Expecting an edge.");

                    bool foundUnvisited = false;
                    for (auto adj : edge->T)
                    {
                        GTL_RUNTIME_ASSERT(
                            adj != nullptr,
                            "Expecting a triangle.");

                        if (visited[adj] == 0)
                        {
                            tStack[++top] = adj;
                            foundUnvisited = true;
                            break;
                        }
                    }

                    if (foundUnvisited)
                    {
                        break;
                    }
                }
                if (i == 3)
                {
                    visited[tri] = 2;
                    component.push_back(tri);
                    --top;
                }
            }
        }

        ECreator mECreator;
        EMap mEMap;
        TCreator mTCreator;
        TMap mTMap;

    private:
        friend class UnitTestDynamicETNonmanifoldMesh;
    };
}
