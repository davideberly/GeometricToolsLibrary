// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.28

#pragma once

// The planar mesh class is convenient for many applications involving
// searches for triangles containing a specified point. The type T must be
// 'float' or 'double'. The input triangles are all counterclockwise in the
// mesh. Set the number of threads to a positive number if you want the
// underlying StaticVETManifoldMesh to use multithreading.
//
// GetContainingTriangle* functions use a blend of interval arithmetic and
// exact rational arithmetic to correctly determine containment.
// 
// GetBarycentrics uses rational arithmetic to compute the exact coordinates
// but then rounds to the nearest T-value.

#include <GTL/Mathematics/Geometry/2D/ExactToLine2.h>
#include <GTL/Mathematics/Geometry/2D/ExactToTriangle2.h>
#include <GTL/Mathematics/Meshes/StaticVETManifoldMesh.h>
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
    class PlanarMesh
    {
    public:
        static std::size_t constexpr invalid = std::numeric_limits<std::size_t>::max();

        PlanarMesh()
            :
            mPositions{},
            mMesh{},
            mETLQuery{},
            mETTQuery{}
        {
            static_assert(
                std::is_floating_point<T>::value,
                "The input type must be a floating-point type.");
        }

        PlanarMesh(std::vector<Vector2<T>> const& positions,
            std::vector<std::array<std::size_t, 3>> const& triangles,
            std::size_t numThreads)
        {
            Create(positions, triangles, numThreads);
        }

        // The inputs must represent a manifold mesh of triangles in the
        // plane. The 'positions' must have at least 3 elements. The
        // 'triangles' must have a positive number of elements; the
        // triangle indices are lookups into the 'positions' array. Each
        // index i must satisfy i < positions.size(). The triangles must
        // be counterclockwise ordered in the plane. Set 'numThreads' to
        // a positive number if you want the underlying StaticVETManifoldMesh
        // to use multithreading.
        void Create(std::vector<Vector2<T>> const& positions,
            std::vector<std::array<std::size_t, 3>> const& triangles,
            std::size_t numThreads)
        {
            GTL_ARGUMENT_ASSERT(
                positions.size() >= 3 && triangles.size() > 0,
                "Invalid number of points or triangles.");

            mPositions = positions;
            mMesh.Create(positions.size(), triangles, numThreads);
        }

        virtual ~PlanarMesh() = default;

        // Mesh information.
        inline std::vector<Vector2<T>> const& GetPositions() const
        {
            return mPositions;
        }

        inline std::vector<std::array<std::size_t, 3>> const& GetTriangles() const
        {
            return mMesh.GetTriangles();
        }

        inline std::vector<std::array<std::size_t, 3>> const& GetAdjacents() const
        {
            return mMesh.GetAdjacents();
        }

        inline StaticVETManifoldMesh const& GetMesh() const
        {
            return mMesh;
        }

        // Get the vertex positions for triangle t.
        void GetPositions(std::size_t t, std::array<Vector2<T>, 3>& positions) const
        {
            auto const& source = mMesh.GetTriangles();
            GTL_ARGUMENT_ASSERT(
                t < source.size(),
                "Invalid triangle index.");

            auto const& triangle = source[t];
            for (std::size_t i = 0; i < 3; ++i)
            {
                positions[i] = mPositions[triangle[i]];
            }
        }

        // Get the indices for triangle t.
        void GetIndices(std::size_t t, std::array<std::size_t, 3>& indices) const
        {
            auto const& source = mMesh.GetTriangles();
            GTL_ARGUMENT_ASSERT(
                t < source.size(),
                "Invalid triangle index.");

            indices = source[t];
        }

        // Get the triangles adjacent to triangle t. If there is no adjacent
        // triangle for an edge, its indices are 'PlanarMesh<T>::invalid'.
        void GetAdjacents(std::size_t t, std::array<std::size_t, 3>& adjacents) const
        {
            auto const& source = mMesh.GetAdjacents();
            GTL_ARGUMENT_ASSERT(
                t < source.size(),
                "Invalid triangle index.");

            adjacents = source[t];
        }

        // The caller is responsible for determining whether the mesh is
        // convex. For example, this is the case if the mesh comes from a
        // Delaunay triangulation. The search for the containing triangle
        // use a linear walk. The triangle edges are used as binary
        // separating lines. If the mesh is not convex, the function can
        // return 'PlanarMesh<T>::invalid' because the walk exited the mesh.
        // However, the point might be in the (nonconvex) mesh.
        std::size_t GetContainingTriangleConvex(Vector2<T> const& P,
            std::size_t initialTriangleIndex) const
        {
            auto const& triangles = mMesh.GetTriangles();
            auto const& adjacents = mMesh.GetAdjacents();

            GTL_ARGUMENT_ASSERT(
                initialTriangleIndex < triangles.size(),
                "The initial triangle index is larger than the number of triangles.");

            std::size_t triangleIndex = initialTriangleIndex;
            for (std::size_t t = 0; t < triangles.size(); ++t)
            {
                auto const& triangle = triangles[triangleIndex];
                auto const& adjacent = adjacents[triangleIndex];

                if (mETLQuery(P, mPositions[triangle[1]], mPositions[triangle[2]]) > 0)
                {
                    triangleIndex = adjacent[0];
                    if (triangleIndex == invalid)
                    {
                        return invalid;
                    }
                    continue;
                }

                if (mETLQuery(P, mPositions[triangle[2]], mPositions[triangle[0]]) > 0)
                {
                    triangleIndex = adjacent[1];
                    if (triangleIndex == invalid)
                    {
                        return invalid;
                    }
                    continue;
                }

                if (mETLQuery(P, mPositions[triangle[0]], mPositions[triangle[1]]) > 0)
                {
                    triangleIndex = adjacent[2];
                    if (triangleIndex == invalid)
                    {
                        return invalid;
                    }
                    continue;
                }

                return triangleIndex;
            }

            return invalid;
        }

        // The caller is responsible for determining whether the mesh is not
        // convex. The search is exhaustive over all triangles but uses
        // multithreading to help with performance.
        std::size_t GetContainingTriangleNotConvex(Vector2<T> const& P,
            std::size_t numThreads) const
        {
            // Use an exhaustive search. For performance, the search is
            // multithreaded.
            auto const& triangles = mMesh.GetTriangles();

            if (numThreads > 1)
            {
                // Compute on multiple threads.
                mETTQuery.resize(numThreads);
                std::size_t numTrianglesPerThread = triangles.size() / numThreads;
                std::vector<std::size_t> tmin(numThreads), tsup(numThreads);
                for (std::size_t i = 0; i < numThreads; ++i)
                {
                    tmin[i] = i * numTrianglesPerThread;
                    tsup[i] = (i + 1) * numTrianglesPerThread;
                }
                tsup.back() = triangles.size();

                // The foundTriangle variable technically should be atomic,
                // because each thread reads it and then possibly sets it.
                // For now it is not atomic. If one thread sets it to 'true',
                // other threads might read 'false', the state before the
                // memory update occurs, but eventually it will read 'true' or
                // complete its batch of tests.
                bool foundTriangle = false;
                std::vector<std::pair<std::size_t, std::int32_t>> triSignPairs(
                    numThreads, std::make_pair(invalid, 1));

                std::vector<std::thread> process(numThreads);
                for (std::size_t i = 0; i < numThreads; ++i)
                {
                    process[i] = std::thread(
                        [this, i, &tmin, &tsup, &triangles, &P, &triSignPairs, &foundTriangle]()
                    {
                        for (std::size_t t = tmin[i]; t < tsup[i]; ++t)
                        {
                            if (foundTriangle)
                            {
                                // See the comments before the declaration of
                                // foundTriangle.
                                return;
                            }

                            auto const& index = triangles[t];
                            auto const& V0 = mPositions[index[0]];
                            auto const& V1 = mPositions[index[1]];
                            auto const& V2 = mPositions[index[2]];
                            std::int32_t sign = mETTQuery[0](P, V0, V1, V2);
                            if (sign <= 0)
                            {
                                triSignPairs[i] = std::make_pair(t, sign);

                                // See the comments before the declaration of
                                // foundTriangle.
                                foundTriangle = true;
                                return;
                            }
                        }
                    });
                }

                for (std::size_t i = 0; i < numThreads; ++i)
                {
                    process[i].join();
                }

                if (foundTriangle)
                {
                    for (std::size_t i = 0; i < numThreads; ++i)
                    {
                        if (triSignPairs[i].second <= 0)
                        {
                            return triSignPairs[i].first;
                        }
                    }
                }
                return invalid;
            }
            else
            {
                // Compute on the main thread.
                mETTQuery.resize(1);
                for (std::size_t t = 0; t < triangles.size(); ++t)
                {
                    auto const& index = triangles[t];
                    auto const& V0 = mPositions[index[0]];
                    auto const& V1 = mPositions[index[1]];
                    auto const& V2 = mPositions[index[2]];
                    std::int32_t sign = mETTQuery[0](P, V0, V1, V2);
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
        bool GetBarycentrics(std::size_t t, Vector2<T> const& P, std::array<T, 3>& bary) const
        {
            std::array<Vector2<T>, 3> V{};
            GetPositions(t, V);

            Vector2<Rational> rP{ P[0], P[1] };
            std::array<Vector2<Rational>, 3> rV{};
            for (std::size_t i = 0; i < 3; ++i)
            {
                for (std::size_t j = 0; j < 2; ++j)
                {
                    rV[i][j] = V[i][j];
                }
            }

            std::array<Vector2<Rational>, 3> rDiff =
            {
                rV[0] - rV[2],
                rV[1] - rV[2],
                rP - rV[2]
            };

            Rational rDet = DotPerp(rDiff[0], rDiff[1]);
            if (rDet.GetSign() != 0)
            {
                std::array<Rational, 3> rBary{};
                rBary[0] = DotPerp(rDiff[2], rDiff[1]) / rDet;
                rBary[1] = DotPerp(rDiff[0], rDiff[2]) / rDet;
                rBary[2] = C_<Rational>(1) - rBary[0] - rBary[1];
                for (std::size_t i = 0; i < 3; ++i)
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
        // The vertex positions for the triangles. A copy is made of the
        // input positions, allowing the PlanarMesh object to have a lifetime
        // longer than that of those input positions.
        std::vector<Vector2<T>> mPositions;

        // Support for computing a manifold mesh from the constructor inputs.
        StaticVETManifoldMesh mMesh;

        // Support for GetContainingTriangle*.
        mutable ExactToLine2<T> mETLQuery;
        mutable std::vector<ExactToTriangle2<T>> mETTQuery;

        // Support for GetBarycentrics.
        static std::size_t constexpr numWords = (std::is_same<T, float>::value ? 278 : 2100);
        using Rational = BSRational<UIntegerFP32<numWords>>;

    private:
        friend class UnitTestPlanarMesh;
};
}
