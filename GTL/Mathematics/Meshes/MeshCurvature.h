// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.15

#pragma once

// The MeshCurvature class estimates principal curvatures and principal
// directions at the vertices of a manifold triangle mesh. The algorithm
// is described in
// https://www.geometrictools.com/Documentation/MeshDifferentialGeometry.pdf

#include <GTL/Mathematics/Algebra/Vector.h>
#include <GTL/Mathematics/MatrixAnalysis/GaussianElimination.h>
#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <vector>

namespace gtl
{
    template <typename T>
    class MeshCurvature
    {
    public:
        MeshCurvature()
            :
            mNormals{},
            mMinCurvatures{},
            mMaxCurvatures{},
            mMinDirections{},
            mMaxDirections{}
        {
        }

        // The input to operator() is a triangle mesh with the specified
        // vertex buffer and index buffer. The number of elements of
        // 'indices' must be a multiple of 3, each triple of indices
        // (3*t, 3*t+1, 3*t+2) representing the triangle with vertices
        // (vertices[3*t], vertices[3*t+1], vertices[3*t+2]). The singularity
        // threshold is a small nonnegative number. It is used to characterize
        // whether the DWTrn matrix is singular. In theory, set the threshold
        // to zero. In practice you might have to set this to a small positive
        // number.
        void operator()(std::vector<Vector3<T>> const& vertices,
            std::vector<std::size_t> const& indices, T const& singularityThreshold)
        {
            GTL_ARGUMENT_ASSERT(
                vertices.size() > 0,
                "Mesh must have a positive number of vertices.");

            GTL_ARGUMENT_ASSERT(
                indices.size() > 0 && (indices.size() % 3) == 0,
                "Mesh must have a positive number of 3-tuples of indices.");

            std::size_t const numVertices = vertices.size();
            mNormals.resize(numVertices);
            mMinCurvatures.resize(numVertices);
            mMaxCurvatures.resize(numVertices);
            mMinDirections.resize(numVertices);
            mMaxDirections.resize(numVertices);

            // Compute the normal vectors for the vertices as an area-weighted
            // sum of the triangles sharing a vertex.
            std::size_t const numTriangles = indices.size() / 3;
            Vector3<T> vzero{};
            std::fill(mNormals.begin(), mNormals.end(), vzero);
            for (std::size_t t = 0, i = 0; t < numTriangles; ++t)
            {
                // Get the vertex indices.
                std::size_t v0 = indices[i++];
                std::size_t v1 = indices[i++];
                std::size_t v2 = indices[i++];

                // Compute the normal (length provides a weighted sum).
                Vector3<T> edge1 = vertices[v1] - vertices[v0];
                Vector3<T> edge2 = vertices[v2] - vertices[v0];
                Vector3<T> triangleNormal = Cross(edge1, edge2);

                mNormals[v0] += triangleNormal;
                mNormals[v1] += triangleNormal;
                mNormals[v2] += triangleNormal;
            }
            for (std::size_t i = 0; i < numVertices; ++i)
            {
                Normalize(mNormals[i]);
            }

            // Compute the matrix of normal derivatives.
            Matrix3x3<T> mzero{};
            std::vector<Matrix3x3<T>> DNormal(numVertices, mzero);
            std::vector<Matrix3x3<T>> WWTrn(numVertices, mzero);
            std::vector<Matrix3x3<T>> DWTrn(numVertices, mzero);
            std::vector<std::size_t> DWTrnZero(numVertices, static_cast<std::size_t>(0));

            for (std::size_t t = 0, i = 0; t < numTriangles; ++t)
            {
                // Get the vertex indices.
                std::array<std::size_t, 3> v{};
                v[0] = indices[i++];
                v[1] = indices[i++];
                v[2] = indices[i++];

                for (std::size_t j = 0; j < 3; j++)
                {
                    std::size_t v0 = v[j];
                    std::size_t v1 = v[(j + 1) % 3];
                    std::size_t v2 = v[(j + 2) % 3];

                    // Compute the edge direction from vertex v0 to vertex v1,
                    // project it to the tangent plane of vertex v0 and
                    // compute the difference of adjacent normals.
                    Vector3<T> E = vertices[v1] - vertices[v0];
                    Vector3<T> W = E - Dot(E, mNormals[v0]) * mNormals[v0];
                    Vector3<T> D = mNormals[v1] - mNormals[v0];
                    for (std::size_t row = 0; row < 3; ++row)
                    {
                        for (std::size_t col = 0; col < 3; ++col)
                        {
                            WWTrn[v0](row, col) += W[row] * W[col];
                            DWTrn[v0](row, col) += D[row] * W[col];
                        }
                    }

                    // Compute the edge direction from vertex v0 to vertex v2,
                    // project it to the tangent plane of vertex v0 and
                    // compute the difference of adjacent normals.
                    E = vertices[v2] - vertices[v0];
                    W = E - Dot(E, mNormals[v0]) * mNormals[v0];
                    D = mNormals[v2] - mNormals[v0];
                    for (std::size_t row = 0; row < 3; ++row)
                    {
                        for (std::size_t col = 0; col < 3; ++col)
                        {
                            WWTrn[v0](row, col) += W[row] * W[col];
                            DWTrn[v0](row, col) += D[row] * W[col];
                        }
                    }
                }
            }

            // Add in N*N^T to W*W^T for numerical stability. In theory 0*0^T
            // is added to D*W^T, but of course no update is needed in the
            // implementation. Compute the matrix of normal derivatives.
            for (std::size_t i = 0; i < numVertices; ++i)
            {
                for (std::size_t row = 0; row < 3; ++row)
                {
                    for (std::size_t col = 0; col < 3; ++col)
                    {
                        WWTrn[i](row, col) = C_<T>(1, 2) * WWTrn[i](row, col) +
                            mNormals[i][row] * mNormals[i][col];
                        DWTrn[i](row, col) *= C_<T>(1, 2);
                    }
                }

                // Compute the max-abs entry of D*W^T.  If this entry is
                // (nearly) zero, flag the DNormal matrix as singular.
                T maxAbs = C_<T>(0);
                for (std::size_t row = 0; row < 3; ++row)
                {
                    for (std::size_t col = 0; col < 3; ++col)
                    {
                        T absEntry = std::fabs(DWTrn[i](row, col));
                        if (absEntry > maxAbs)
                        {
                            maxAbs = absEntry;
                        }
                    }
                }
                if (maxAbs < singularityThreshold)
                {
                    DWTrnZero[i] = 1;
                }

                DNormal[i] = DWTrn[i] * Inverse(WWTrn[i]);
            }

            // If N is a unit-length normal at a vertex, let U and V be
            // unit-length tangents so that {U, V, N} is an orthonormal set.
            // Define the matrix J = [U | V], a 3-by-2 matrix whose columns
            // are U and V. Define J^T to be the transpose of J, a 2-by-3
            // matrix. Let dN/dX denote the matrix of first-order derivatives
            // of the normal vector field. The shape matrix is
            //   S = (J^T * J)^{-1} * J^T * dN/dX * J = J^T * dN/dX * J
            // where the superscript of -1 denotes the inverse; the formula
            // allows for J to be created from non-perpendicular vectors. The
            // matrix S is 2-by-2. The principal curvatures are the
            // eigenvalues of S. If k is a principal curvature and W is the
            // 2-by-1 eigenvector corresponding to it, then S*W = k*W (by
            // definition). The corresponding 3-by-1 tangent vector at the
            // vertex is a principal direction for k and is J*W.
            for (std::size_t i = 0; i < numVertices; ++i)
            {
                // Compute U and V given N.
                Vector3<T> normal = mNormals[i], U, V;
                ComputeOrthonormalBasis(1, normal, U, V);

                if (DWTrnZero[i] > 0)
                {
                    // At a locally planar point.
                    mMinCurvatures[i] = C_<T>(0);
                    mMaxCurvatures[i] = C_<T>(0);
                    mMinDirections[i] = U;
                    mMaxDirections[i] = V;
                    continue;
                }

                // Compute S = J^T * dN/dX * J. In theory S is symmetric, but
                // because dN/dX is estimated, we must ensure that the
                // computed S is symmetric.
                T s00 = Dot(U, DNormal[i] * U);
                T s01 = Dot(U, DNormal[i] * V);
                T s10 = Dot(V, DNormal[i] * U);
                T s11 = Dot(V, DNormal[i] * V);
                T avr = C_<T>(1, 2) * (s01 + s10);
                Matrix2x2<T> S{ { s00, avr }, { avr, s11 } };

                // Compute the eigenvalues of S (min and max curvatures).
                T trace = S(0, 0) + S(1, 1);
                T det = S(0, 0) * S(1, 1) - S(0, 1) * S(1, 0);
                T discr = trace * trace - C_<T>(4) * det;
                T rootDiscr = std::sqrt(std::max(discr, C_<T>(0)));
                mMinCurvatures[i] = C_<T>(1, 2) * (trace - rootDiscr);
                mMaxCurvatures[i] = C_<T>(1, 2) * (trace + rootDiscr);

                // Compute the eigenvectors of S.
                Vector2<T> W0{ S(0, 1), mMinCurvatures[i] - S(0, 0) };
                Vector2<T> W1{ mMinCurvatures[i] - S(1, 1), S(1, 0) };
                if (Dot(W0, W0) >= Dot(W1, W1))
                {
                    Normalize(W0);
                    mMinDirections[i] = W0[0] * U + W0[1] * V;
                }
                else
                {
                    Normalize(W1);
                    mMinDirections[i] = W1[0] * U + W1[1] * V;
                }

                W0 = Vector2<T>{ S(0, 1), mMaxCurvatures[i] - S(0, 0) };
                W1 = Vector2<T>{ mMaxCurvatures[i] - S(1, 1), S(1, 0) };
                if (Dot(W0, W0) >= Dot(W1, W1))
                {
                    Normalize(W0);
                    mMaxDirections[i] = W0[0] * U + W0[1] * V;
                }
                else
                {
                    Normalize(W1);
                    mMaxDirections[i] = W1[0] * U + W1[1] * V;
                }
            }
        }

        inline std::vector<Vector3<T>> const& GetNormals() const
        {
            return mNormals;
        }

        inline std::vector<T> const& GetMinCurvatures() const
        {
            return mMinCurvatures;
        }

        inline std::vector<T> const& GetMaxCurvatures() const
        {
            return mMaxCurvatures;
        }

        inline std::vector<Vector3<T>> const& GetMinDirections() const
        {
            return mMinDirections;
        }

        inline std::vector<Vector3<T>> const& GetMaxDirections() const
        {
            return mMaxDirections;
        }

    private:
        std::vector<Vector3<T>> mNormals;
        std::vector<T> mMinCurvatures;
        std::vector<T> mMaxCurvatures;
        std::vector<Vector3<T>> mMinDirections;
        std::vector<Vector3<T>> mMaxDirections;

    private:
        friend class UnitTestMeshCurvature;
    };
}
