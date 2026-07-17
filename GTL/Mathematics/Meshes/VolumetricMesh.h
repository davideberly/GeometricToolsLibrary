// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.15

#pragma once

// The volumetric mesh class is convenient for many applications involving
// searches for tetrahedra containing a specified point. The type T must be
// 'float' or 'double'. The input tetrahedra are all counterclockwise in the
// mesh; see Tetrahedron3.h for a description of what that means. Set the
// number of threads to a positive number if you want the underlying
// StaticVTSManifoldMesh to use multithreading.
//
// GetContainingTetrahedron* functions use a blend of interval arithmetic and
// exact rational arithmetic to correctly determine containment.
// 
// GetBarycentrics uses rational arithmetic to compute the exact coordinates
// but then rounds to the nearest T-value.

#include <GTL/Mathematics/Geometry/3D/ExactToPlane3.h>
#include <GTL/Mathematics/Geometry/3D/ExactToTetrahedron3.h>
#include <GTL/Mathematics/Meshes/StaticVTSManifoldMesh.h>
#include <GTL/Mathematics/Algebra/Vector.h>
#include <array>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

namespace gtl
{
    template <typename T>
    class VolumetricMesh
    {
    public:
        static std::size_t constexpr invalid = std::numeric_limits<std::size_t>::max();

        VolumetricMesh()
            :
            mPositions{},
            mMesh{},
            mETPQuery{},
            mETTQuery{}
        {
            static_assert(
                std::is_floating_point<T>::value,
                "The input type must be a floating-point type.");
        }

        // The inputs must represent a manifold mesh of tetrahedra in space.
        // The 'positions' must have at least 4 elements. The 'tetrahedra'
        // must have a positive number of elements; the tetrahedra indices are
        // lookups into the 'positions' array. Each index i must satisfy
        // i < positions.size(). The tetrahedra must be counterclockwise
        // ordered in space. Set 'numThreads' to a positive number if you want
        // the underlying StaticVETManifoldMesh to use multithreading.
        void Create(std::vector<Vector3<T>> const& positions,
            std::vector<std::array<std::size_t, 4>> const& tetrahedra,
            std::size_t numThreads)
        {
            GTL_ARGUMENT_ASSERT(
                positions.size() >= 4 && tetrahedra.size() > 0,
                "Invalid number of points or tetrahedra.");

            mPositions = positions;
            mMesh.Create(positions.size(), tetrahedra, numThreads);
        }

        virtual ~VolumetricMesh() = default;

        // Mesh information.
        inline std::vector<Vector3<T>> const& GetPositions() const
        {
            return mPositions;
        }

        inline std::vector<std::array<std::size_t, 4>> const& GetTetrahedra() const
        {
            return mMesh.GetTetrahedra();
        }

        inline std::vector<std::array<std::size_t, 4>> const& GetAdjacents() const
        {
            return mMesh.GetAdjacents();
        }

        inline StaticVTSManifoldMesh const& GetMesh() const
        {
            return mMesh;
        }

        // Get the vertex positions for tetrahedron t.
        void GetPositions(std::size_t t, std::array<Vector3<T>, 4>& positions) const
        {
            auto const& source = mMesh.GetTetrahedra();
            GTL_ARGUMENT_ASSERT(
                t < source.size(),
                "Invalid tetrahedron index.");

            auto const& tetrahedron = source[t];
            for (std::size_t i = 0; i < 4; ++i)
            {
                positions[i] = mPositions[tetrahedron[i]];
            }
        }

        // Get the indices for tetrahedron t.
        void GetIndices(std::size_t t, std::array<std::size_t, 4>& indices) const
        {
            auto const& source = mMesh.GetTetrahedra();
            GTL_ARGUMENT_ASSERT(
                t < source.size(),
                "Invalid tetrahedron index.");

            indices = source[t];
        }

        // Get the triangles adjacent to triangle t. If there is no adjacent
        // triangle for an edge, its indices are 'VolumetricMesh<T>::invalid'.
        void GetAdjacents(std::size_t t, std::array<std::size_t, 4>& adjacents) const
        {
            auto const& source = mMesh.GetAdjacents();
            GTL_ARGUMENT_ASSERT(
                t < source.size(),
                "Invalid tetrahedron index.");

            adjacents = source[t];
        }

        // The caller is responsible for determining whether the mesh is
        // convex. For example, this is the case if the mesh comes from a
        // Delaunay tetrahedralization. The search for the containing triangle
        // use a linear walk. The tetrahedra edges are used as binary
        // separating planes. If the mesh is not convex, the function can
        // return 'VolumetricMesh<T>::invalid' because the walk exited the
        // mesh. However, the point might be in the (nonconvex) mesh.
        std::size_t GetContainingTetrahedronConvex(Vector3<T> const& P,
            std::size_t initialTetrahedronIndex) const
        {
            auto const& tetrahedra = mMesh.GetTetrahedra();
            auto const& adjacents = mMesh.GetAdjacents();
            std::array<std::array<std::size_t, 4>, 4> const lookups =
            { {
                { 0, 1, 3, 2 },
                { 1, 0, 2, 3 },
                { 2, 0, 3, 1 },
                { 3, 0, 1, 2 }
            } };

            GTL_ARGUMENT_ASSERT(
                initialTetrahedronIndex < tetrahedra.size(),
                "The initial tetrahedron index is larger than the number of tetrahedra.");

            std::size_t tetrahedronIndex = initialTetrahedronIndex;
            for (std::size_t t = 0; t < tetrahedra.size(); ++t)
            {
                auto const& index = tetrahedra[tetrahedronIndex];
                auto const& adjacent = adjacents[tetrahedronIndex];

                std::size_t i{};
                for (i = 0; i < 4; ++i)
                {
                    std::size_t j0 = lookups[i][0];
                    std::size_t j1 = lookups[i][1];
                    std::size_t j2 = lookups[i][2];
                    std::size_t j3 = lookups[i][3];
                    auto const& V0 = mPositions[index[j1]];
                    auto const& V1 = mPositions[index[j2]];
                    auto const& V2 = mPositions[index[j3]];
                    if (mETPQuery(P, V0, V1, V2) <= 0)
                    {
                        tetrahedronIndex = adjacent[j0];
                        if (tetrahedronIndex == invalid)
                        {
                            return invalid;
                        }
                        break;
                    }
                }
                if (i < 4)
                {
                    continue;
                }

                return tetrahedronIndex;
            }

            return invalid;
        }

        // The caller is responsible for determining whether the mesh is not
        // convex. The search is exhaustive over all tetrahedra but uses
        // multithreading to help with performance.
        std::size_t GetContainingTetrahedronNotConvex(Vector3<T> const& P,
            std::size_t numThreads) const
        {
            // Use an exhaustive search. For performance, the search is
            // multithreaded.
            auto const& tetrahedra = mMesh.GetTetrahedra();

            if (numThreads > 1)
            {
                // Compute on multiple threads.
                mETTQuery.resize(numThreads);
                std::size_t numTetrahedraPerThread = tetrahedra.size() / numThreads;
                std::vector<std::size_t> tmin(numThreads), tsup(numThreads);
                for (std::size_t i = 0; i < numThreads; ++i)
                {
                    tmin[i] = i * numTetrahedraPerThread;
                    tsup[i] = (i + 1) * numTetrahedraPerThread;
                }
                tsup.back() = tetrahedra.size();

                // The foundTetrahedron variable technically should be atomic,
                // because each thread reads it and then possibly sets it.
                // For now it is not atomic. If one thread sets it to 'true',
                // other threads might read 'false', the state before the
                // memory update occurs, but eventually it will read 'true' or
                // complete its batch of tests.
                bool foundTetrahedron = false;
                std::vector<std::pair<std::size_t, std::int32_t>> tetraSignPairs(
                    numThreads, std::make_pair(invalid, 1));

                std::vector<std::thread> process(numThreads);
                for (std::size_t i = 0; i < numThreads; ++i)
                {
                    process[i] = std::thread(
                        [this, i, &tmin, &tsup, &tetrahedra, &P, &tetraSignPairs, &foundTetrahedron]()
                        {
                            for (std::size_t t = tmin[i]; t < tsup[i]; ++t)
                            {
                                if (foundTetrahedron)
                                {
                                    // See the comments before the declaration of
                                    // foundTetrahedron.
                                    return;
                                }

                                auto const& index = tetrahedra[t];
                                auto const& V0 = mPositions[index[0]];
                                auto const& V1 = mPositions[index[1]];
                                auto const& V2 = mPositions[index[2]];
                                auto const& V3 = mPositions[index[3]];
                                std::int32_t sign = mETTQuery[i](P, V0, V1, V2, V3);
                                if (sign <= 0)
                                {
                                    tetraSignPairs[i] = std::make_pair(t, sign);

                                    // See the comments before the declaration of
                                    // foundTetrahedron.
                                    foundTetrahedron = true;
                                    return;
                                }
                            }
                        });
                }

                for (std::size_t i = 0; i < numThreads; ++i)
                {
                    process[i].join();
                }

                if (foundTetrahedron)
                {
                    for (std::size_t i = 0; i < numThreads; ++i)
                    {
                        if (tetraSignPairs[i].second <= 0)
                        {
                            return tetraSignPairs[i].first;
                        }
                    }
                }
                return invalid;
            }
            else
            {
                // Compute on the main thread.
                mETTQuery.resize(1);
                for (std::size_t t = 0; t < tetrahedra.size(); ++t)
                {
                    auto const& index = tetrahedra[t];
                    auto const& V0 = mPositions[index[0]];
                    auto const& V1 = mPositions[index[1]];
                    auto const& V2 = mPositions[index[2]];
                    auto const& V3 = mPositions[index[3]];
                    std::int32_t sign = mETTQuery[0](P, V0, V1, V2, V3);
                    if (sign <= 0)
                    {
                        return t;
                    }
                }
                return invalid;
            }
        }

        // Rational arithmetic is used to compute the coordinates exactly. The
        // values are rounded to the nearest T-values (T is 'float' or
        // 'double').
        bool GetBarycentrics(std::size_t t, Vector3<T> const& P, std::array<T, 4>& bary) const
        {
            std::array<Vector3<T>, 4> V{};
            GetPositions(t, V);

            Vector3<Rational> rP{ P[0], P[1], P[2] };
            std::array<Vector3<Rational>, 4> rV{};
            for (std::size_t i = 0; i < 4; ++i)
            {
                for (std::size_t j = 0; j < 3; ++j)
                {
                    rV[i][j] = V[i][j];
                }
            }

            std::array<Vector3<Rational>, 4> rDiff =
            {
                rV[0] - rV[3],
                rV[1] - rV[3],
                rV[2] - rV[3],
                rP - rV[3]
            };

            Rational rDet = DotCross(rDiff[0], rDiff[1], rDiff[2]);
            if (rDet.GetSign() != 0)
            {
                std::array<Rational, 4> rBary{};
                rBary[0] = DotCross(rDiff[3], rDiff[1], rDiff[2]) / rDet;
                rBary[1] = DotCross(rDiff[3], rDiff[2], rDiff[0]) / rDet;
                rBary[2] = DotCross(rDiff[3], rDiff[0], rDiff[1]) / rDet;
                rBary[3] = C_<Rational>(1) - rBary[0] - rBary[1] - rBary[2];
                for (std::size_t i = 0; i < 4; ++i)
                {
                    bary[i] = rBary[i];
                }
                return true;
            }
            else
            {
                bary.fill(C_<T>(0));
                return false;
            }
        }

    protected:
        // Input points. A copy is made to avoid object-lifetime issues for
        // an object whose points are passed via a pointer.
        std::vector<Vector3<T>> mPositions;

        // Support for computing a manifold mesh from the constructor inputs.
        StaticVTSManifoldMesh mMesh;

        // Support for GetContainingTetrahedron*.
        mutable ExactToPlane3<T> mETPQuery;
        mutable std::vector<ExactToTetrahedron3<T>> mETTQuery;

        // Support for GetBarycentrics.
        static std::size_t constexpr numWords = (std::is_same<T, float>::value ? 1302 : 9838);
        using Rational = BSRational<UIntegerFP32<numWords>>;

    private:
        friend class UnitTestVolumetricMesh;
    };
}
