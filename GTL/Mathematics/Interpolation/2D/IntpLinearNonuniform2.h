// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.10.10

#pragma once

// Linear interpolation of a mesh of triangles whose vertices are of the form
// (x,y,f(x,y)). Such a mesh is obtained by Delaunay triangulation. The
// domain samples are (x[i],y[i]), where i is the index of the planar mesh
// vertices. The function samples are F[i], which represent f(x[i],y[i]).

#include <GTL/Mathematics/Meshes/PlanarMesh.h>
#include <array>
#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <vector>

namespace gtl
{
    template <typename T>
    class IntpLinearNonuniform2
    {
    public:
        // If 'meshIsConvex' is set to 'true', the 'numThreads' parameter is
        // ignored because the interpolator does an efficient linear walk
        // through the planar mesh. If 'meshIsConvex' is set to 'false', the
        // interpolator uses an exhaustive search of the triangles, so
        // multithreading can improve the performance when there is a large
        // number of triangles. In this case, set 'numThreads' to a positive
        // number.
        IntpLinearNonuniform2(PlanarMesh<T> const& mesh, std::vector<T> const& F,
            bool meshIsConvex, std::size_t numThreads)
            :
            mMesh(mesh),
            mF(F),
            mMeshIsConvex(meshIsConvex),
            mNumThreads(numThreads),
            mLastVisited(PlanarMesh<T>::invalid)
        {
            static_assert(
                std::is_floating_point<T>::value,
                "The input type must be a floating-point type.");

            GTL_ARGUMENT_ASSERT(
                F.size() == mesh.GetPositions().size(),
                "The number of F-samples must equal the number of mesh positions.");
        }

        // Disallow copy semantics and move semantics.
        IntpLinearNonuniform2(IntpLinearNonuniform2 const&) = delete;
        IntpLinearNonuniform2& operator=(IntpLinearNonuniform2 const&) = delete;
        IntpLinearNonuniform2(IntpLinearNonuniform2&&) = delete;
        IntpLinearNonuniform2& operator=(IntpLinearNonuniform2&&) = delete;

        // The return value is 'true' if and only if the input point P is in
        // the planar mesh of the input vertices, in which case the
        // interpolation is valid.
        bool Evaluate(Vector2<T> const& P, T& F) const
        {
            if (mLastVisited == PlanarMesh<T>::invalid)
            {
                mLastVisited = 0;
            }

            if (mMeshIsConvex)
            {
                mLastVisited = mMesh.GetContainingTriangleConvex(P, mLastVisited);
            }
            else
            {
                mLastVisited = mMesh.GetContainingTriangleNotConvex(P, mNumThreads);
            }

            if (mLastVisited == PlanarMesh<T>::invalid)
            {
                // The point is outside the triangulation.
                return false;
            }

            // Get the barycentric coordinates of P with respect to the
            // triangle, P = b0*V0 + b1*V1 + b2*V2, where b0 + b1 + b2 = 1.
            std::array<T, 3> bary{};
            if (!mMesh.GetBarycentrics(mLastVisited, P, bary))
            {
                // The triangle is degenerate. Report a failure to
                // interpolate.
                return false;
            }

            // Get the triangle indices and compute the result as a barycentric
            // combination of function values.
            std::array<std::size_t, 3> indices{ 0, 0, 0 };
            mMesh.GetIndices(mLastVisited, indices);
            F = bary[0] * mF[indices[0]] + bary[1] * mF[indices[1]] + bary[2] * mF[indices[2]];
            return true;
        }

    private:
        // Constructor inputs.
        PlanarMesh<T> const& mMesh;
        std::vector<T> mF;
        bool mMeshIsConvex;
        std::size_t mNumThreads;

        // Keep track of the last triangle visited during an interpolation.
        mutable std::size_t mLastVisited;

    private:
        friend class UnitTestIntpLinearNonuniform2;
    };
}
