// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.28

#pragma once

// DynamicVETManifoldMesh represents a vertex-edge-triangle manifold mesh for
// which triangle insertions and removals can occur at any time. The triangle
// chirality (winding order) is not required to be consistent among the
// inserted triangles. You can force consistent chirality using class member
// functions.
// 
// The underlying C++ container classes lead to significant memory allocation
// and deallocation costs and are expensive for find operations. If you know
// the triangles in advance and no insertions or removals will occur, consider
// using StaticVETManifoldMesh which performs much better, minimizes the
// memory management costs and allows for multithreading.

#include <GTL/Mathematics/Meshes/DynamicETManifoldMesh.h>
#include <cstddef>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <utility>

namespace gtl
{
    class DynamicVETManifoldMesh : public DynamicETManifoldMesh
    {
    public:
        // Vertex data types.
        class Vertex;
        using VCreator = std::unique_ptr<Vertex>(*)(std::size_t);
        using VMap = std::unordered_map<std::size_t, std::unique_ptr<Vertex>>;

        // Vertex object.
        class Vertex
        {
        public:
            virtual ~Vertex() = default;

            Vertex(std::size_t vIndex)
                :
                V(vIndex),
                VAdjacent{},
                EAdjacent{},
                TAdjacent{}
            {
            }

            // The index into the vertex pool of the mesh.
            std::size_t V;

            // Adjacent objects.
            std::unordered_set<std::size_t> VAdjacent;
            std::unordered_set<Edge*> EAdjacent;
            std::unordered_set<Triangle*> TAdjacent;
        };


        // Construction and destruction.
        virtual ~DynamicVETManifoldMesh() = default;

        DynamicVETManifoldMesh(VCreator vCreator = nullptr, ECreator eCreator = nullptr, TCreator tCreator = nullptr)
            :
            DynamicETManifoldMesh(eCreator, tCreator),
            mVCreator(vCreator ? vCreator : CreateVertex),
            mVMap{}
        {
        }

        // Support for a deep copy of the mesh.  The mVMap, mEMap, and mTMap
        // objects have dynamically allocated memory for vertices, edges, and
        // triangles.  A shallow copy of the pointers to this memory is
        // problematic.  Allowing sharing, say, via std::shared_ptr, is an
        // option but not really the intent of copying the mesh graph.
        DynamicVETManifoldMesh(DynamicVETManifoldMesh const& mesh)
            :
            DynamicVETManifoldMesh{}
        {
            *this = mesh;
        }

        DynamicVETManifoldMesh& operator=(DynamicVETManifoldMesh const& mesh)
        {
            Clear();
            mVCreator = mesh.mVCreator;
            DynamicETManifoldMesh::operator=(mesh);
            return *this;
        }

        // Member access.
        inline VMap const& GetVertices() const
        {
            return mVMap;
        }

        // If <v0,v1,v2> is not in the mesh, a Triangle object is created and
        // returned; otherwise, <v0,v1,v2> is in the mesh and nullptr is
        // returned. If the insertion leads to a nonmanifold mesh, the call
        // fails with a nullptr returned.
        virtual Triangle* Insert(std::size_t v0, std::size_t v1, std::size_t v2) override
        {
            Triangle* tri = DynamicETManifoldMesh::Insert(v0, v1, v2);
            if (!tri)
            {
                return nullptr;
            }

            for (std::size_t i = 0; i < 3; ++i)
            {
                std::size_t vIndex = tri->V[i];
                auto vItem = mVMap.find(vIndex);
                Vertex* vertex = nullptr;
                if (vItem == mVMap.end())
                {
                    std::unique_ptr<Vertex> newVertex = mVCreator(vIndex);
                    vertex = newVertex.get();
                    mVMap[vIndex] = std::move(newVertex);
                }
                else
                {
                    vertex = vItem->second.get();
                }

                vertex->TAdjacent.insert(tri);

                for (std::size_t j = 0; j < 3; ++j)
                {
                    // With a correct implementation, the GTL_RUNTIME_ASSERT
                    // should not trigger.
                    auto edge = tri->E[j];
                    GTL_RUNTIME_ASSERT(
                        edge != nullptr,
                        "Expecting an edge.");

                    if (edge->V[0] == vIndex)
                    {
                        vertex->VAdjacent.insert(edge->V[1]);
                        vertex->EAdjacent.insert(edge);
                    }
                    else if (edge->V[1] == vIndex)
                    {
                        vertex->VAdjacent.insert(edge->V[0]);
                        vertex->EAdjacent.insert(edge);
                    }
                }
            }

            return tri;
        }

        // If <v0,v1,v2> is in the mesh, it is removed and 'true' is returned;
        // otherwise, <v0,v1,v2> is not in the mesh and 'false' is returned.
        virtual bool Remove(std::size_t v0, std::size_t v1, std::size_t v2) override
        {
            auto tItem = mTMap.find(TriangleKey<true>(v0, v1, v2));
            if (tItem == mTMap.end())
            {
                return false;
            }

            Triangle* tri = tItem->second.get();
            for (std::size_t i = 0; i < 3; ++i)
            {
                // With a correct implementation, the LogRuntimeErrors
                // should not trigger.
                std::size_t vIndex = tri->V[i];
                auto vItem = mVMap.find(vIndex);
                GTL_RUNTIME_ASSERT(
                    vItem != mVMap.end(),
                    "Expecting a vertex.");

                Vertex* vertex = vItem->second.get();
                for (std::size_t j = 0; j < 3; ++j)
                {
                    auto edge = tri->E[j];
                    GTL_RUNTIME_ASSERT(
                        edge != nullptr,
                        "Expecting an edge.");

                    if (edge->T[0] && !edge->T[1])
                    {
                        if (edge->V[0] == vIndex)
                        {
                            vertex->VAdjacent.erase(edge->V[1]);
                            vertex->EAdjacent.erase(edge);
                        }
                        else if (edge->V[1] == vIndex)
                        {
                            vertex->VAdjacent.erase(edge->V[0]);
                            vertex->EAdjacent.erase(edge);
                        }
                    }
                }

                vertex->TAdjacent.erase(tri);

                if (vertex->TAdjacent.size() == 0)
                {
                    GTL_RUNTIME_ASSERT(
                        vertex->VAdjacent.size() == 0 &&
                        vertex->EAdjacent.size() == 0,
                        "Malformed mesh: Inconsistent vertex adjacency information.");
                    mVMap.erase(vItem);
                }
            }

            return DynamicETManifoldMesh::Remove(v0, v1, v2);
        }

        // Destroy the vertices, edges, and triangles to obtain an empty mesh.
        virtual void Clear() override
        {
            mVMap.clear();
            DynamicETManifoldMesh::Clear();
        }

    protected:
        // The vertex data and default vertex creation.
        static std::unique_ptr<Vertex> CreateVertex(std::size_t vIndex)
        {
            return std::make_unique<Vertex>(vIndex);
        }

        VCreator mVCreator;
        VMap mVMap;

    private:
        friend class UnitTestDynamicVETManifoldMesh;
    };
}
