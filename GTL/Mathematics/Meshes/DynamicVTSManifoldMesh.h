// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.15

#pragma once

// The DynamicVTSManifoldMesh class represents a triangle-tetrahedron manifold
// mesh but additionally stores vertex adjacency information. The class allows
// insertion and removal of tetrahedra at any time. The 'V' stands for vertex,
// the 'T' stands for triangle (face) and the 'S' stands for simplex
// (tetrahedron).
// 
// The underlying C++ container classes lead to significant memory allocation
// and deallocation costs and are expensive for find operations. If you know
// the triangles in advance and no insertions or removals will occur, consider
// using StaticVTSManifoldMesh which performs much better, minimizes the
// memory management costs and allows for multithreading.

#include <GTL/Mathematics/Meshes/DynamicTSManifoldMesh.h>
#include <cstddef>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <utility>

namespace gtl
{
    class DynamicVTSManifoldMesh : public DynamicTSManifoldMesh
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
                TAdjacent{},
                SAdjacent{}
            {
            }

            // The index into the vertex pool of the mesh.
            std::size_t V;

            // Adjacent objects.
            std::unordered_set<std::size_t> VAdjacent;
            std::unordered_set<Triangle*> TAdjacent;
            std::unordered_set<Tetrahedron*> SAdjacent;
        };


        // Construction and destruction.
        virtual ~DynamicVTSManifoldMesh() = default;

        DynamicVTSManifoldMesh(VCreator vCreator = nullptr, TCreator tCreator = nullptr, SCreator sCreator = nullptr)
            :
            DynamicTSManifoldMesh(tCreator, sCreator),
            mVCreator(vCreator ? vCreator : CreateVertex),
            mVMap{}
        {
        }

        // Support for a deep copy of the mesh. The mVMap, mTMap and mSMap
        // objects have dynamically allocated memory for vertices, triangles
        // and tetrahedra. A shallow copy of the pointers to this memory is
        // problematic. Allowing sharing, say, via std::shared_ptr, is an
        // option but not really the intent of copying the mesh graph.
        DynamicVTSManifoldMesh(DynamicVTSManifoldMesh const& mesh)
            :
            DynamicVTSManifoldMesh{}
        {
            *this = mesh;
        }

        DynamicVTSManifoldMesh& operator=(DynamicVTSManifoldMesh const& mesh)
        {
            Clear();
            mVCreator = mesh.mVCreator;
            DynamicTSManifoldMesh::operator=(mesh);
            return *this;
        }

        // Member access.
        inline VMap const& GetVertices() const
        {
            return mVMap;
        }

        // If <v0,v1,v2> is not in the mesh, a Triangle object is created and
        // returned; otherwise, <v0,v1,v2> is in the mesh and nullptr is
        // returned.  If the insertion leads to a nonmanifold mesh, the call
        // fails with a nullptr returned.
        virtual Tetrahedron* Insert(std::size_t v0, std::size_t v1, std::size_t v2, std::size_t v3) override
        {
            Tetrahedron* tetra = DynamicTSManifoldMesh::Insert(v0, v1, v2, v3);
            if (!tetra)
            {
                return nullptr;
            }

            for (std::size_t i = 0; i < 4; ++i)
            {
                std::size_t vIndex = tetra->V[i];
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

                vertex->SAdjacent.insert(tetra);

                for (std::size_t j = 0; j < 4; ++j)
                {
                    auto tri = tetra->T[j];
                    GTL_RUNTIME_ASSERT(
                        tri != nullptr,
                        "Unexpected condition.");

                    if (tri->V[0] == vIndex)
                    {
                        vertex->VAdjacent.insert(tri->V[1]);
                        vertex->VAdjacent.insert(tri->V[2]);
                        vertex->TAdjacent.insert(tri);
                    }
                    else if (tri->V[1] == vIndex)
                    {
                        vertex->VAdjacent.insert(tri->V[0]);
                        vertex->VAdjacent.insert(tri->V[2]);
                        vertex->TAdjacent.insert(tri);
                    }
                    else if (tri->V[2] == vIndex)
                    {
                        vertex->VAdjacent.insert(tri->V[0]);
                        vertex->VAdjacent.insert(tri->V[1]);
                        vertex->TAdjacent.insert(tri);
                    }
                }
            }

            return tetra;
        }

        // If <v0,v1,v2> is in the mesh, it is removed and 'true' is returned;
        // otherwise, <v0,v1,v2> is not in the mesh and 'false' is returned.
        virtual bool Remove(std::size_t v0, std::size_t v1, std::size_t v2, std::size_t v3) override
        {
            auto sItem = mSMap.find(TetrahedronKey<true>(v0, v1, v2, v3));
            if (sItem == mSMap.end())
            {
                return false;
            }

            Tetrahedron* tetra = sItem->second.get();
            for (std::size_t i = 0; i < 4; ++i)
            {
                std::size_t vIndex = tetra->V[i];
                auto vItem = mVMap.find(vIndex);
                GTL_RUNTIME_ASSERT(
                    vItem != mVMap.end(),
                    "Unexpected condition.");

                Vertex* vertex = vItem->second.get();
                for (std::size_t j = 0; j < 4; ++j)
                {
                    auto tri = tetra->T[j];
                    GTL_RUNTIME_ASSERT(
                        tri != nullptr,
                        "Unexpected condition.");

                    if (tri->S[0] && !tri->S[1])
                    {
                        if (tri->V[0] == vIndex)
                        {
                            vertex->VAdjacent.erase(tri->V[1]);
                            vertex->VAdjacent.erase(tri->V[2]);
                            vertex->TAdjacent.erase(tri);
                        }
                        else if (tri->V[1] == vIndex)
                        {
                            vertex->VAdjacent.erase(tri->V[0]);
                            vertex->VAdjacent.erase(tri->V[2]);
                            vertex->TAdjacent.erase(tri);
                        }
                        else if (tri->V[2] == vIndex)
                        {
                            vertex->VAdjacent.erase(tri->V[0]);
                            vertex->VAdjacent.erase(tri->V[1]);
                            vertex->TAdjacent.erase(tri);
                        }
                    }
                }

                vertex->SAdjacent.erase(tetra);

                if (vertex->SAdjacent.size() == 0)
                {
                    GTL_RUNTIME_ASSERT(
                        vertex->VAdjacent.size() == 0 && vertex->TAdjacent.size() == 0,
                        "Unexpected condition.");

                    mVMap.erase(vItem);
                }
            }

            return DynamicTSManifoldMesh::Remove(v0, v1, v2, v3);
        }

        // Destroy the vertices, edges, and triangles to obtain an empty mesh.
        virtual void Clear() override
        {
            mVMap.clear();
            DynamicTSManifoldMesh::Clear();
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
        friend class UnitTestDynamicVTSManifoldMesh;
    };
}
