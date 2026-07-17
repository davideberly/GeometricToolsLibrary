// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.17

#pragma once

// Conformally map a 2-dimensional manifold mesh with the topology of a sphere
// to a sphere. The algorithm is an implementation of the one in the paper
//    S.Haker, S.Angenent, A.Tannenbaum, R.Kikinis, G.Sapiro and M.Halle.
//    Conformal surface parameterization for texture mapping,
//    IEEE Transactions on Visualization and Computer Graphics,
//    Volume 6, Number 2, pages 181–189, 2000
// The paper is available at https://ieeexplore.ieee.org/document/856998 but
// is not freely downloadable.

#include <GTL/Mathematics/Algebra/Polynomial.h>
#include <GTL/Mathematics/Algebra/Vector.h>
#include <GTL/Mathematics/Meshes/DynamicETManifoldMesh.h>
#include <GTL/Mathematics/MatrixAnalysis/LinearSystem.h>
#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <vector>

namespace gtl
{
    template <typename T>
    class ConformalMapGenusZero
    {
    public:
        // The input mesh should be a closed, manifold surface that has the
        // topology of a sphere (genus 0 surface).
        ConformalMapGenusZero()
            :
            mPlaneCoordinates{},
            mMinPlaneCoordinate{},
            mMaxPlaneCoordinate{},
            mSphereCoordinates{},
            mSphereRadius(C_<T>(0))
        {
        }

        ~ConformalMapGenusZero() = default;

        // The returned 'bool' value is 'true' whenever the conjugate gradient
        // algorithm converged. Even if it did not, the results might still
        // be acceptable.
        bool operator()(std::size_t numPositions, Vector3<T> const* positions,
            std::size_t numTriangles, std::size_t const* indices, std::size_t punctureTriangle)
        {
            GTL_ARGUMENT_ASSERT(
                numPositions >= 3 && positions != nullptr &&
                numTriangles >= 1 && indices != nullptr && punctureTriangle < numTriangles,
                "Invalid input.");

            bool converged = true;
            mPlaneCoordinates.resize(numPositions);
            mSphereCoordinates.resize(numPositions);

            // Construct an edge-triangle representation of the mesh.
            DynamicETManifoldMesh mesh{};
            std::size_t const* currentIndex = indices;
            for (std::size_t t = 0; t < numTriangles; ++t)
            {
                std::size_t v0 = *currentIndex++;
                std::size_t v1 = *currentIndex++;
                std::size_t v2 = *currentIndex++;
                mesh.Insert(v0, v1, v2);
            }
            auto const& emap = mesh.GetEdges();

            // Construct the nondiagonal entries of the sparse matrix A.
            typename LinearSystem<T>::SparseMatrix A{};
            for (auto const& element : emap)
            {
                std::size_t v0 = element.first[0];
                std::size_t v1 = element.first[1];

                T value = C_<T>(0);
                for (std::size_t j = 0; j < 2; ++j)
                {
                    auto triangle = element.second->T[j];
                    for (std::size_t i = 0; i < 3; ++i)
                    {
                        std::size_t v2 = triangle->V[i];
                        if (v2 != v0 && v2 != v1)
                        {
                            Vector3<T> E0 = positions[v0] - positions[v2];
                            Vector3<T> E1 = positions[v1] - positions[v2];
                            value += Dot(E0, E1) / Length(Cross(E0, E1));
                        }
                    }
                }

                value *= C_<T>(-1, 2);

                std::array<std::size_t, 2> lookup = { v0, v1 };
                A[lookup] = value;
            }

            // Construct the diagonal entries of the sparse matrix A.
            std::vector<T> tmp(numPositions, C_<T>(0));
            for (auto const& element : A)
            {
                tmp[element.first[0]] -= element.second;
                tmp[element.first[1]] -= element.second;
            }
            for (std::size_t i = 0; i < numPositions; ++i)
            {
                std::array<std::size_t, 2> lookup = { i, i };
                A[lookup] = tmp[i];
            }
            GTL_RUNTIME_ASSERT(
                numPositions + emap.size() == A.size(),
                "Mismatched sizes.");

            // Construct the sparse column vector B.
            currentIndex = &indices[3 * punctureTriangle];
            std::size_t v0 = *currentIndex++;
            std::size_t v1 = *currentIndex++;
            std::size_t v2 = *currentIndex++;
            Vector3<T> V0 = positions[v0];
            Vector3<T> V1 = positions[v1];
            Vector3<T> V2 = positions[v2];
            Vector3<T> E10 = V1 - V0;
            Vector3<T> E20 = V2 - V0;
            Vector3<T> E12 = V1 - V2;
            Vector3<T> normal = Cross(E20, E10);
            T len10 = Length(E10);
            T invLen10 = C_<T>(1) / len10;
            T twoArea = Length(normal);
            T invLenNormal = C_<T>(1) / twoArea;
            T invProd = invLen10 * invLenNormal;
            T re0 = -invLen10;
            T im0 = invProd * Dot(E12, E10);
            T re1 = invLen10;
            T im1 = invProd * Dot(E20, E10);
            T re2 = C_<T>(0);
            T im2 = -len10 * invLenNormal;

            // Solve the sparse system for the real parts.
            std::size_t constexpr maxIterations = 1024;
            T const tolerance = 1e-06f;
            std::fill(tmp.begin(), tmp.end(), C_<T>(0));
            tmp[v0] = re0;
            tmp[v1] = re1;
            tmp[v2] = re2;
            std::vector<T> result(numPositions);
            std::size_t iterations = LinearSystem<T>::SolveSymmetricCG(numPositions, A,
                tmp.data(), result.data(), maxIterations, tolerance);
            if (iterations >= maxIterations)
            {
                converged = false;
            }
            for (std::size_t i = 0; i < numPositions; ++i)
            {
                mPlaneCoordinates[i][0] = result[i];
            }

            // Solve the sparse system for the imaginary parts.
            std::fill(tmp.begin(), tmp.end(), C_<T>(0));
            tmp[v0] = -im0;
            tmp[v1] = -im1;
            tmp[v2] = -im2;
            iterations = LinearSystem<T>::SolveSymmetricCG(numPositions, A,
                tmp.data(), result.data(), maxIterations, tolerance);
            if (iterations >= maxIterations)
            {
                converged = false;
            }
            for (std::size_t i = 0; i < numPositions; ++i)
            {
                mPlaneCoordinates[i][1] = result[i];
            }

            // Scale to [-1,1]^2 for numerical conditioning in later steps.
            T fmin = mPlaneCoordinates[0][0], fmax = fmin;
            for (std::size_t i = 0; i < numPositions; i++)
            {
                if (mPlaneCoordinates[i][0] < fmin)
                {
                    fmin = mPlaneCoordinates[i][0];
                }
                else if (mPlaneCoordinates[i][0] > fmax)
                {
                    fmax = mPlaneCoordinates[i][0];
                }
                if (mPlaneCoordinates[i][1] < fmin)
                {
                    fmin = mPlaneCoordinates[i][1];
                }
                else if (mPlaneCoordinates[i][1] > fmax)
                {
                    fmax = mPlaneCoordinates[i][1];
                }
            }
            T halfRange = C_<T>(1, 2) * (fmax - fmin);
            T invHalfRange = C_<T>(1) / halfRange;
            for (std::size_t i = 0; i < numPositions; ++i)
            {
                mPlaneCoordinates[i][0] = C_<T>(-1) + invHalfRange * (mPlaneCoordinates[i][0] - fmin);
                mPlaneCoordinates[i][1] = C_<T>(-1) + invHalfRange * (mPlaneCoordinates[i][1] - fmin);
            }

            // Map the plane coordinates to the sphere using inverse
            // stereographic projection. The main issue is selecting a
            // translation in (x,y) and a radius of the projection sphere.
            // Both factors strongly influence the final result.

            // Use the average as the south pole. The points tend to be
            // clustered approximately in the middle of the conformally
            // mapped punctured triangle, so the average is a good choice
            // to place the pole.
            Vector2<T> origin{};  // zero vector
            for (std::size_t i = 0; i < numPositions; ++i)
            {
                origin += mPlaneCoordinates[i];
            }
            origin /= static_cast<T>(numPositions);
            for (std::size_t i = 0; i < numPositions; ++i)
            {
                mPlaneCoordinates[i] -= origin;
            }

            mMinPlaneCoordinate = mPlaneCoordinates[0];
            mMaxPlaneCoordinate = mPlaneCoordinates[0];
            for (std::size_t i = 1; i < numPositions; ++i)
            {
                if (mPlaneCoordinates[i][0] < mMinPlaneCoordinate[0])
                {
                    mMinPlaneCoordinate[0] = mPlaneCoordinates[i][0];
                }
                else if (mPlaneCoordinates[i][0] > mMaxPlaneCoordinate[0])
                {
                    mMaxPlaneCoordinate[0] = mPlaneCoordinates[i][0];
                }

                if (mPlaneCoordinates[i][1] < mMinPlaneCoordinate[1])
                {
                    mMinPlaneCoordinate[1] = mPlaneCoordinates[i][1];
                }
                else if (mPlaneCoordinates[i][1] > mMaxPlaneCoordinate[1])
                {
                    mMaxPlaneCoordinate[1] = mPlaneCoordinates[i][1];
                }
            }

            // Select the radius of the sphere so that the projected punctured
            // triangle has an area whose fraction of total spherical area is
            // the same fraction as the area of the punctured triangle to the
            // total area of the original triangle mesh.
            T twoTotalArea = C_<T>(0);
            currentIndex = indices;
            for (std::size_t t = 0; t < numTriangles; ++t)
            {
                V0 = positions[*currentIndex++];
                V1 = positions[*currentIndex++];
                V2 = positions[*currentIndex++];
                Vector3<T> E0 = V1 - V0;
                Vector3<T> E1 = V2 - V0;
                twoTotalArea += Length(Cross(E0, E1));
            }
            ComputeSphereRadius(v0, v1, v2, twoArea / twoTotalArea);
            T sqrSphereRadius = mSphereRadius * mSphereRadius;

            // Inverse stereographic projection to obtain sphere coordinates.
            // The sphere is centered at the origin and has radius 1.
            for (std::size_t i = 0; i < numPositions; i++)
            {
                T rSqr = Dot(mPlaneCoordinates[i], mPlaneCoordinates[i]);
                T mult = C_<T>(1) / (rSqr + sqrSphereRadius);
                T x = C_<T>(2) * mult * sqrSphereRadius * mPlaneCoordinates[i][0];
                T y = C_<T>(2) * mult * sqrSphereRadius * mPlaneCoordinates[i][1];
                T z = mult * mSphereRadius * (rSqr - sqrSphereRadius);
                mSphereCoordinates[i] = Vector3<T>{ x, y, z } / mSphereRadius;
            }

            return converged;
        }

        bool operator()(std::vector<Vector3<T>> const& positions,
            std::vector<std::size_t> const& indices, std::size_t punctureTriangle)
        {
            GTL_ARGUMENT_ASSERT(
                positions.size() >= 3 && indices.size() >= 3 &&
                (indices.size() % 3) == 0 && punctureTriangle < indices.size() / 3,
                "Invalid input.");

            return operator()(positions.size(), positions.data(), indices.size() / 3,
                indices.data(), punctureTriangle);
        }

        // Conformal mapping of mesh to plane. The array of coordinates has a
        // one-to-one correspondence with the input vertex array.
        inline std::vector<Vector2<T>> const& GetPlaneCoordinates() const
        {
            return mPlaneCoordinates;
        }

        inline Vector2<T> const& GetMinPlaneCoordinate() const
        {
            return mMinPlaneCoordinate;
        }

        inline Vector2<T> const& GetMaxPlaneCoordinate() const
        {
            return mMaxPlaneCoordinate;
        }

        // Conformal mapping of mesh to sphere (centered at origin). The array
        // of coordinates has a one-to-one correspondence with the input vertex
        // array.
        inline std::vector<Vector3<T>> const& GetSphereCoordinates() const
        {
            return mSphereCoordinates;
        }

        inline T const& GetSphereRadius() const
        {
            return mSphereRadius;
        }

    private:
        void ComputeSphereRadius(std::size_t v0, std::size_t v1, std::size_t v2, T const& areaFraction)
        {
            Vector2<T> V0 = mPlaneCoordinates[v0];
            Vector2<T> V1 = mPlaneCoordinates[v1];
            Vector2<T> V2 = mPlaneCoordinates[v2];

            T r0Sqr = Dot(V0, V0);
            T r1Sqr = Dot(V1, V1);
            T r2Sqr = Dot(V2, V2);
            T diffR10 = r1Sqr - r0Sqr;
            T diffR20 = r2Sqr - r0Sqr;
            T diffX10 = V1[0] - V0[0];
            T diffY10 = V1[1] - V0[1];
            T diffX20 = V2[0] - V0[0];
            T diffY20 = V2[1] - V0[1];
            T diffRX10 = V1[0] * r0Sqr - V0[0] * r1Sqr;
            T diffRY10 = V1[1] * r0Sqr - V0[1] * r1Sqr;
            T diffRX20 = V2[0] * r0Sqr - V0[0] * r2Sqr;
            T diffRY20 = V2[1] * r0Sqr - V0[1] * r2Sqr;

            T c0 = diffR20 * diffRY10 - diffR10 * diffRY20;
            T c1 = diffR20 * diffY10 - diffR10 * diffY20;
            T d0 = diffR10 * diffRX20 - diffR20 * diffRX10;
            T d1 = diffR10 * diffX20 - diffR20 * diffX10;
            T e0 = diffRX10 * diffRY20 - diffRX20 * diffRY10;
            T e1 = diffRX10 * diffY20 - diffRX20 * diffY10;
            T e2 = diffX10 * diffY20 - diffX20 * diffY10;

            Polynomial1<T> poly0(6);
            poly0[0] = C_<T>(0);
            poly0[1] = C_<T>(0);
            poly0[2] = e0 * e0;
            poly0[3] = c0 * c0 + d0 * d0 + C_<T>(2) * e0 * e1;
            poly0[4] = C_<T>(2) * (c0 * c1 + d0 * d1 + e0 * e1) + e1 * e1;
            poly0[5] = c1 * c1 + d1 * d1 + C_<T>(2) * e1 * e2;
            poly0[6] = e2 * e2;

            Polynomial1<T> qpoly0(1), qpoly1(1), qpoly2(1);
            qpoly0[0] = r0Sqr;
            qpoly0[1] = C_<T>(1);
            qpoly1[0] = r1Sqr;
            qpoly1[1] = C_<T>(1);
            qpoly2[0] = r2Sqr;
            qpoly2[1] = C_<T>(1);

            T tmp = areaFraction * C_TWO_PI<T>;
            T amp = tmp * tmp;

            Polynomial1<T> poly1 = amp * qpoly0;
            poly1 = poly1 * qpoly0;
            poly1 = poly1 * qpoly0;
            poly1 = poly1 * qpoly0;
            poly1 = poly1 * qpoly1;
            poly1 = poly1 * qpoly1;
            poly1 = poly1 * qpoly2;
            poly1 = poly1 * qpoly2;

            Polynomial1<T> poly2 = poly1 - poly0;
            GTL_RUNTIME_ASSERT(
                poly2.GetDegree() <= 8,
                "Expecting degree no larger than 8.");

            // Bound a root near zero and apply bisection to find t.
            T tmin = C_<T>(0), fmin = poly2(tmin);
            T tmax = C_<T>(1), fmax = poly2(tmax);
            GTL_RUNTIME_ASSERT(
                fmin > C_<T>(0) && fmax < C_<T>(0),
                "Expecting opposite-signed extremes.");

            // Determine the number of iterations to get 'digits' of accuracy.
            std::size_t constexpr digits = 6;
            T tmp0 = std::log(tmax - tmin);
            T tmp1 = static_cast<T>(digits) * C_LN_10<T>;
            T arg = (tmp0 + tmp1) * C_INV_LN_2<T>;
            std::size_t maxIterations = static_cast<std::size_t>(arg + C_<T>(1, 2));
            T tmid = C_<T>(0), fmid{};
            for (std::size_t i = 0; i < maxIterations; ++i)
            {
                tmid = C_<T>(1, 2) * (tmin + tmax);
                fmid = poly2(tmid);
                T product = fmid * fmin;
                if (product < C_<T>(0))
                {
                    tmax = tmid;
                    fmax = fmid;
                }
                else
                {
                    tmin = tmid;
                    fmin = fmid;
                }
            }

            mSphereRadius = std::sqrt(tmid);
        }

        // Conformal mapping to a plane. The plane's (px,py) points
        // correspond to the mesh's (mx,my,mz) points.
        std::vector<Vector2<T>> mPlaneCoordinates;
        Vector2<T> mMinPlaneCoordinate, mMaxPlaneCoordinate;

        // Conformal mapping to a sphere. The sphere's (sx,sy,sz) points
        // correspond to the mesh's (mx,my,mz) points.
        std::vector<Vector3<T>> mSphereCoordinates;
        T mSphereRadius;

    private:
        friend class UnitTestConformalMapGenusZero;
    };
}
