// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.14

#pragma once

// Linear interpolation of a mesh of tetrahedra whose vertices are of the form
// (x,y,z,f(x,y,z)). Such a mesh is obtained by Delaunay tetrahedralization.
// The domain samples are (x[i],y[i],z[i]), where i is the index of the
// volumetric mesh vertices. The function samples are F[i], which represent
// f(x[i],y[i],z[i])

#include <GTL/Mathematics/Meshes/VolumetricMesh.h>
#include <array>
#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <vector>

namespace gtl
{
    template <typename T>
    class IntpLinearNonuniform3
    {
    public:
        // If 'meshIsConvex' is set to 'true', the 'numThreads' parameter is
        // ignored because the interpolator does an efficient linear walk
        // through the volumetric mesh. If 'meshIsConvex' is set to 'false',
        // the interpolator uses an exhaustive search of the tetrahedra, so
        // multithreading can improve the performance when there is a large
        // number of tetrahedra. In this case, set 'numThreads' to a positive
        // number.
        IntpLinearNonuniform3(VolumetricMesh<T> const& mesh, std::vector<T> const& F,
            bool meshIsConvex, std::size_t numThreads)
            :
            mMesh(mesh),
            mF(F),
            mMeshIsConvex(meshIsConvex),
            mNumThreads(numThreads),
            mLastVisited(VolumetricMesh<T>::invalid)
        {
            static_assert(
                std::is_floating_point<T>::value,
                "The input type must be a floating-point type.");

            GTL_ARGUMENT_ASSERT(
                F.size() == mesh.GetPositions().size(),
                "The number of F-samples must equal the number of mesh positions.");
        }

        // Disallow copy semantics and move semantics.
        IntpLinearNonuniform3(IntpLinearNonuniform3 const&) = delete;
        IntpLinearNonuniform3& operator=(IntpLinearNonuniform3 const&) = delete;
        IntpLinearNonuniform3(IntpLinearNonuniform3&&) = delete;
        IntpLinearNonuniform3& operator=(IntpLinearNonuniform3&&) = delete;

        // The return value is 'true' if and only if the input point P is in
        // the volumetric mesh of the input vertices, in which case the
        // interpolation is valid.
        bool Evaluate(Vector3<T> const& P, T& F) const
        {
            if (mLastVisited == VolumetricMesh<T>::invalid)
            {
                mLastVisited = 0;
            }

            if (mMeshIsConvex)
            {
                mLastVisited = mMesh.GetContainingTetrahedronConvex(P, mLastVisited);
            }
            else
            {
                mLastVisited = mMesh.GetContainingTetrahedronNotConvex(P, mNumThreads);
            }

            if (mLastVisited == VolumetricMesh<T>::invalid)
            {
                // The point is outside the triangulation.
                return false;
            }

            // Get the barycentric coordinates of P with respect to the
            // tetrahedron, P = b0*V0 + b1*V1 + b2*V2 + b3*V3, where
            // b0 + b1 + b2 + b3 = 1.
            std::array<T, 4> bary{};
            if (!mMesh.GetBarycentrics(mLastVisited, P, bary))
            {
                // The triangle is degenerate. Report a failure to
                // interpolate.
                return false;
            }

            // Get the tetrahedron indices and compute the result as a
            // barycentric combination of function values.
            std::array<std::size_t, 4> indices{ 0, 0, 0, 0 };
            mMesh.GetIndices(mLastVisited, indices);
            F = bary[0] * mF[indices[0]] + bary[1] * mF[indices[1]] +
                bary[2] * mF[indices[2]] + bary[3] * mF[indices[3]];
            return true;
        }

    private:
        // Constructor inputs.
        VolumetricMesh<T> const& mMesh;
        std::vector<T> mF;
        bool mMeshIsConvex;
        std::size_t mNumThreads;

        // Keep track of the last tetrahedron visited during an interpolation.
        mutable std::size_t mLastVisited;
    };
}
