// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

// The DynamicTSManifoldMesh class represents a triangle-tetrahedron manifold
// mesh for which tetrahedron insertions and removals can occur at any time.
// The 'T' stands for triangle (face) and the 'S' stands for simplex
// (tetrahedron).
// 
// The underlying C++ container classes lead to significant memory allocation
// and deallocation costs and are expensive for find operations. If you know
// the triangles in advance and no insertions or removals will occur, consider
// using StaticVTSManifoldMesh which performs much better, minimizes the
// memory management costs and allows for multithreading.

#include <GTL/Utility/Exceptions.h>
#include <GTL/Utility/HashCombine.h>
#include <GTL/Mathematics/Meshes/TetrahedronKey.h>
#include <GTL/Mathematics/Meshes/TriangleKey.h>
#include <algorithm>
#include <array>
#include <cstddef>
#include <memory>
#include <unordered_map>
#include <utility>

namespace gtl
{
    class DynamicTSManifoldMesh
    {
    public:
        // Triangle data types.
        class Triangle;
        using TCreator = std::unique_ptr<Triangle>(*)(std::size_t, std::size_t, std::size_t);
        using TMap = std::unordered_map<TriangleKey<false>, std::unique_ptr<Triangle>,
            TriangleKey<false>, TriangleKey<false>>;

        // Tetrahedron data types.
        class Tetrahedron;
        using SCreator = std::unique_ptr<Tetrahedron>(*)(std::size_t, std::size_t, std::size_t, std::size_t);
        using SMap = std::unordered_map<TetrahedronKey<true>, std::unique_ptr<Tetrahedron>,
            TetrahedronKey<true>, TetrahedronKey<true>>;

        // Triangle object.
        class Triangle
        {
        public:
            virtual ~Triangle() = default;

            Triangle(std::size_t v0, std::size_t v1, std::size_t v2)
                :
                V{ v0, v1, v2 },
                S{ nullptr, nullptr }
            {
            }

            // Vertices of the face.
            std::array<std::size_t, 3> V;

            // Tetrahedra sharing the face.
            std::array<Tetrahedron*, 2> S;
        };

        // Tetrahedron object.
        class Tetrahedron
        {
        public:
            virtual ~Tetrahedron() = default;

            Tetrahedron(std::size_t v0, std::size_t v1, std::size_t v2, std::size_t v3)
                :
                V{ v0, v1, v2, v3 },
                T{ nullptr, nullptr, nullptr, nullptr },
                S{ nullptr, nullptr, nullptr, nullptr }
            {
            }

            // Vertices, listed in an order so that each face vertices in
            // counterclockwise order when viewed from outside the
            // tetrahedron.
            std::array<std::size_t, 4> V;

            // Adjacent faces. T[i] points to the triangle face
            // opposite V[i].
            //   T[0] points to face (V[1],V[2],V[3])
            //   T[1] points to face (V[0],V[3],V[2])
            //   T[2] points to face (V[0],V[1],V[3])
            //   T[3] points to face (V[0],V[2],V[1])
            std::array<Triangle*, 4> T;

            // Adjacent tetrahedra. S[i] points to the adjacent tetrahedron
            // sharing face T[i].
            std::array<Tetrahedron*, 4> S;
        };


        DynamicTSManifoldMesh(TCreator tCreator = nullptr, SCreator sCreator = nullptr)
            :
            mTCreator(tCreator ? tCreator : CreateTriangle),
            mTMap{},
            mSCreator(sCreator ? sCreator : CreateTetrahedron),
            mSMap{},
            mThrowOnNonmanifoldInsertion(true)
        {
        }

        // Support for a deep copy of the mesh. The mTMap and mSMap objects
        // have dynamically allocated memory for triangles and tetrahedra. A
        // shallow copy of the pointers to this memory is problematic.
        // Allowing sharing, say, via std::shared_ptr, is an option but not
        // really the intent of copying the mesh graph.
        DynamicTSManifoldMesh(DynamicTSManifoldMesh const& mesh)
            :
            DynamicTSManifoldMesh{}
        {
            *this = mesh;
        }

        virtual ~DynamicTSManifoldMesh() = default;

        DynamicTSManifoldMesh& operator=(DynamicTSManifoldMesh const& mesh)
        {
            Clear();

            mTCreator = mesh.mTCreator;
            mSCreator = mesh.mSCreator;
            mThrowOnNonmanifoldInsertion = mesh.mThrowOnNonmanifoldInsertion;
            for (auto const& element : mesh.mSMap)
            {
                // The typecast avoids warnings about not storing the return
                // value in a named variable. The return value is discarded.
                (void)Insert(element.first[0], element.first[1],
                    element.first[2], element.first[3]);
            }

            return *this;
        }

        // Member access.
        inline TMap const& GetTriangles() const
        {
            return mTMap;
        }

        inline SMap const& GetTetrahedra() const
        {
            return mSMap;
        }

        // If the insertion of a tetrahedron fails because the mesh would
        // become nonmanifold, the default behavior is to throw an exception.
        // You can disable this behavior and continue gracefully without an
        // exception.
        bool ThrowOnNonmanifoldInsertion(bool doException)
        {
            std::swap(doException, mThrowOnNonmanifoldInsertion);
            return doException;  // return the previous state
        }

        // If <v0,v1,v2,v3> is not in the mesh, a Tetrahedron object is
        // created and returned; otherwise, <v0,v1,v2,v3> is in the mesh and
        // nullptr is returned. If the insertion leads to a nonmanifold mesh,
        // the call fails with a nullptr returned.
        virtual Tetrahedron* Insert(std::size_t v0, std::size_t v1, std::size_t v2, std::size_t v3)
        {
            TetrahedronKey<true> skey(v0, v1, v2, v3);
            if (mSMap.find(skey) != mSMap.end())
            {
                // The tetrahedron already exists. Return a null pointer as
                // a signal to the caller that the insertion failed.
                return nullptr;
            }

            // Create the new tetrahedron. It will be added to mTMap at the
            // end of the function so that if an assertion is triggered and
            // the function returns early, the (bad) triangle will not be
            // part of the mesh.
            std::unique_ptr<Tetrahedron> newTetra = mSCreator(v0, v1, v2, v3);
            Tetrahedron* tetra = newTetra.get();

            // Add the faces to the mesh if they do not already exist.
            for (std::size_t i = 0; i < 4; ++i)
            {
                auto const& opposite = TetrahedronKey<true>::GetOppositeFace()[i];
                TriangleKey<false> tkey(tetra->V[opposite[0]], tetra->V[opposite[1]], tetra->V[opposite[2]]);
                Triangle* face = nullptr;
                auto titer = mTMap.find(tkey);
                if (titer == mTMap.end())
                {
                    // This is the first time the face is encountered.
                    std::unique_ptr<Triangle> newFace = mTCreator(tetra->V[opposite[0]],
                        tetra->V[opposite[1]], tetra->V[opposite[2]]);
                    face = newFace.get();
                    mTMap[tkey] = std::move(newFace);

                    // Update the face and tetrahedron.
                    face->S[0] = tetra;
                    tetra->T[i] = face;
                }
                else
                {
                    // This is the second time the face is encountered. With
                    // a correct implementation, the GTL_RUNTIME_ASSERT should
                    // not trigger.
                    face = titer->second.get();
                    GTL_RUNTIME_ASSERT(
                        face != nullptr,
                        "Expecting a face.");

                    // Update the face.
                    if (face->S[1])
                    {
                        if (mThrowOnNonmanifoldInsertion)
                        {
                            GTL_RUNTIME_ERROR(
                                "Attempt to create nonmanifold mesh.");
                        }
                        else
                        {
                            return nullptr;
                        }
                    }
                    face->S[1] = tetra;

                    // Update the adjacent tetrahedra. With a correct
                    // implementation, the GTL_RUNTIME_ASSERT should not
                    // trigger.
                    auto adjacent = face->S[0];
                    GTL_RUNTIME_ASSERT(
                        adjacent != nullptr,
                        "Expecting a tetrahedron.");

                    for (std::size_t j = 0; j < 4; ++j)
                    {
                        if (adjacent->T[j] == face)
                        {
                            adjacent->S[j] = tetra;
                            break;
                        }
                    }

                    // Update the tetrahedron.
                    tetra->T[i] = face;
                    tetra->S[i] = adjacent;
                }
            }

            mSMap[skey] = std::move(newTetra);
            return tetra;
        }

        // If <v0,v1,v2,v3> is in the mesh, it is removed and 'true' is
        // returned; otherwise, <v0,v1,v2,v3> is not in the mesh and 'false'
        // is returned.
        virtual bool Remove(std::size_t v0, std::size_t v1, std::size_t v2, std::size_t v3)
        {
            TetrahedronKey<true> skey(v0, v1, v2, v3);
            auto siter = mSMap.find(skey);
            if (siter == mSMap.end())
            {
                // The tetrahedron does not exist.
                return false;
            }

            // Get the tetrahedron.
            Tetrahedron* tetra = siter->second.get();

            // Remove the faces and update adjacent tetrahedra if necessary.
            for (std::size_t i = 0; i < 4; ++i)
            {
                // Inform the faces the tetrahedron is being deleted. With a
                // correct implementation, the GTL_RUNTIME_ASSERT should not
                // trigger.
                auto face = tetra->T[i];
                GTL_RUNTIME_ASSERT(
                    face != nullptr,
                    "Expecting a face.");

                if (face->S[0] == tetra)
                {
                    // One-tetrahedron faces always have pointer at index
                    // zero.
                    face->S[0] = face->S[1];
                    face->S[1] = nullptr;
                }
                else if (face->S[1] == tetra)
                {
                    face->S[1] = nullptr;
                }
                else
                {
                    GTL_RUNTIME_ERROR(
                        "Expecting an adjacent tetrahedron.");
                }

                // Remove the face if you have the last reference to it.
                if (!face->S[0] && !face->S[1])
                {
                    TriangleKey<false> tkey(face->V[0], face->V[1], face->V[2]);
                    mTMap.erase(tkey);
                }

                // Inform adjacent tetrahedra the tetrahedron is being
                // deleted.
                auto adjacent = tetra->S[i];
                if (adjacent)
                {
                    for (std::size_t j = 0; j < 4; ++j)
                    {
                        if (adjacent->S[j] == tetra)
                        {
                            adjacent->S[j] = nullptr;
                            break;
                        }
                    }
                }
            }

            mSMap.erase(skey);
            return true;
        }

        // Destroy the triangles and tetrahedra to obtain an empty mesh.
        virtual void Clear()
        {
            mTMap.clear();
            mSMap.clear();
        }

        // A manifold mesh is closed if each face is shared twice. A closed
        // mesh is not necessarily oriented.
        bool IsClosed() const
        {
            for (auto const& element : mTMap)
            {
                Triangle* tri = element.second.get();
                if (!tri->S[0] || !tri->S[1])
                {
                    return false;
                }
            }
            return true;
        }

    protected:
        // The triangle data and default triangle creation.
        static std::unique_ptr<Triangle> CreateTriangle(std::size_t v0, std::size_t v1, std::size_t v2)
        {
            return std::make_unique<Triangle>(v0, v1, v2);
        }

        // The tetrahedron data and default tetrahedron creation.
        static std::unique_ptr<Tetrahedron> CreateTetrahedron(std::size_t v0, std::size_t v1, std::size_t v2, std::size_t v3)
        {
            return std::make_unique<Tetrahedron>(v0, v1, v2, v3);
        }

        TCreator mTCreator;
        TMap mTMap;
        SCreator mSCreator;
        SMap mSMap;
        bool mThrowOnNonmanifoldInsertion;

    private:
        friend class UnitTestDynamicTSManifoldMesh;
    };
}
