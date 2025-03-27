// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

// A hyperellipsoid has center K; axis directions U[0] through U[N-1], all
// unit-length vectors; and extents e[0] through e[N-1], all positive numbers.
// A point X = K + sum_{d=0}^{N-1} y[d]*U[d] is on the hyperellipsoid whenever
// sum_{d=0}^{N-1} (y[d]/e[d])^2 = 1. An algebraic representation for the
// hyperellipsoid is (X-K)^T * M * (X-K) = 1, where M is the NxN symmetric
// matrix M = sum_{d=0}^{N-1} U[d]*U[d]^T/e[d]^2, where the superscript T
// denotes transpose. Observe that U[i]*U[i]^T is a matrix, not a scalar dot
// product. The hyperellipsoid is also represented by a quadratic equation
// 0 = C + B^T*X + X^T*A*X, where C is a scalar, B is an Nx1 vector and A is
// an NxN symmetric matrix with positive eigenvalues. The coefficients can be
// stored from lowest degree to highest degree,
//   C = k[0]
//   B = k[1], ..., k[N]
//   A = k[N+1], ..., k[(N+1)(N+2)/2 - 1]
// where the A-coefficients are the upper-triangular elements of A listed in
// row-major order. For N = 2, X = (x[0],x[1]) and
//   0 = k[0] +
//       k[1]*x[0] + k[2]*x[1] + 
//       k[3]*x[0]*x[0] + k[4]*x[0]*x[1]
//                      + k[5]*x[1]*x[1]
// For N = 3, X = (x[0],x[1],x[2]) and
//   0 = k[0] +
//       k[1]*x[0] + k[2]*x[1] + k[3]*x[2] +
//       k[4]*x[0]*x[0] + k[5]*x[0]*x[1] + k[6]*x[0]*x[2] +
//                      + k[7]*x[1]*x[1] + k[8]*x[1]*x[2] +
//                                       + k[9]*x[2]*x[2]
// This equation can be factored to the form (X-K)^T * M * (X-K) = 1, where
// K = -A^{-1}*B/2, M = A/(B^T*A^{-1}*B/4-C).

#include <GTL/Mathematics/MatrixAnalysis/GaussianElimination.h>
#include <GTL/Mathematics/MatrixAnalysis/SymmetricEigensolver.h>
#include <array>
#include <cmath>
#include <cstddef>

namespace gtl
{
    template <typename T, std::size_t N>
    class Hyperellipsoid
    {
    public:
        Hyperellipsoid()
            :
            center{},
            axis{},
            extent{}
        {
        }

        Hyperellipsoid(Vector<T, N> const& inCenter,
            std::array<Vector<T, N>, N> const inAxis,
            Vector<T, N> const& inExtent)
            :
            center(inCenter),
            axis(inAxis),
            extent(inExtent)
        {
        }

        // Compute M = sum_{d=0}^{N-1} U[d]*U[d]^T/e[d]^2.
        void GetM(Matrix<T, N, N>& M) const
        {
            MakeZero(M);
            for (std::size_t d = 0; d < N; ++d)
            {
                Vector<T, N> ratio = axis[d] / extent[d];
                M += OuterProduct(ratio, ratio);
            }
        }

        // Compute M^{-1} = sum_{d=0}^{N-1} U[d]*U[d]^T*e[d]^2.
        void GetMInverse(Matrix<T, N, N>& MInverse) const
        {
            MakeZero(MInverse);
            for (std::size_t d = 0; d < N; ++d)
            {
                Vector<T, N> product = axis[d] * extent[d];
                MInverse += OuterProduct(product, product);
            }
        }

        // Construct the coefficients in the quadratic equation that represents
        // the hyperellipsoid.
        void ToCoefficients(std::array<T, (N + 1) * (N + 2) / 2>& coeff) const
        {
            std::size_t constexpr numCoefficients = (N + 1) * (N + 2) / 2;
            Matrix<T, N, N> A{};
            Vector<T, N> B{};
            T C = C_<T>(0);
            ToCoefficients(A, B, C);
            Convert(A, B, C, coeff);

            // Arrange for one of the coefficients of the quadratic terms
            // to be 1.
            std::size_t quadIndex = numCoefficients - 1;
            std::size_t maxIndex = quadIndex;
            T maxValue = std::fabs(coeff[quadIndex]);
#if defined(GTL_USE_MSWINDOWS)
            // NOTE: When N = 2, MSVS 2019 16.7.5 generates:
            //   warning C6294: Ill-defined for-loop: initial condition does
            //   not satisfy test. Loop body not executed.
            // Not executing the loop is the correct behavior for N = 2.
#pragma warning(disable : 6294)
#endif
            for (std::size_t d = 2; d < N; ++d)
            {
                quadIndex -= d;
                T absValue = std::fabs(coeff[quadIndex]);
                if (absValue > maxValue)
                {
                    maxIndex = quadIndex;
                    maxValue = absValue;
                }
            }
#if defined(GTL_USE_MSWINDOWS)
#pragma warning(default : 6294)
#endif

            T invMaxValue = C_<T>(1) / maxValue;
            for (std::size_t i = 0; i < numCoefficients; ++i)
            {
                if (i != maxIndex)
                {
                    coeff[i] *= invMaxValue;
                }
                else
                {
                    coeff[i] = C_<T>(1);
                }
            }
        }

        void ToCoefficients(Matrix<T, N, N>& A, Vector<T, N>& B, T& C) const
        {
            GetM(A);
            Vector<T, N> product = A * center;
            B = -C_<T>(2) * product;
            C = Dot(center, product) - C_<T>(1);
        }

        // Construct C, U[i], and e[i] from the equation. The return value is
        // 'true' if and only if the input coefficients represent a
        // hyperellipsoid. If the function returns 'false', the hyperellipsoid
        // data members are undefined.
        bool FromCoefficients(std::array<T, (N + 1) * (N + 2) / 2> const& coeff)
        {
            Matrix<T, N, N> A{};
            Vector<T, N> B{};
            T C = C_<T>(0);
            Convert(coeff, A, B, C);
            return FromCoefficients(A, B, C);
        }

        bool FromCoefficients(Matrix<T, N, N> const& A, Vector<T, N> const& B, T C)
        {
            // Compute the center K = -A^{-1}*B/2.
            T determinant = C_<T>(0);
            Matrix<T, N, N> invA = Inverse(A, &determinant);
            if (determinant == C_<T>(0))
            {
                return false;
            }

            center = -C_<T>(1, 2) * (invA * B);

            // Compute B^T*A^{-1}*B/4 - C = K^T*A*K - C = -K^T*B/2 - C.
            T rightSide = -C_<T>(1, 2) * Dot(center, B) - C;
            if (rightSide == C_<T>(0))
            {
                return false;
            }

            // Compute M = A/(K^T*A*K - C).
            T invRightSide = C_<T>(1) / rightSide;
            Matrix<T, N, N> M = invRightSide * A;

            // Factor into M = R*D*R^T. M is symmetric, so it does not matter
            // whether the matrix is stored in row-major or column-major
            // order; they are equivalent. The output R, however, is in
            // row-major order.
            SymmetricEigensolver<T> es{};
            std::size_t constexpr maxIterations = 32;
            es(N, M.data(), maxIterations);

            Matrix<T, N, N> rotation{}, diagonal{};
            for (std::size_t i = 0; i < N; ++i)
            {
                diagonal(i, i) = es.GetEigenvalue(i);
                rotation.SetCol(i, es.GetEigenvector(i));
            }

            for (std::size_t d = 0; d < N; ++d)
            {
                T eigenvalue = diagonal(d, d);
                if (eigenvalue <= C_<T>(0))
                {
                    return false;
                }

                extent[d] = C_<T>(1) / std::sqrt(eigenvalue);
                axis[d] = rotation.GetCol(d);
            }

            return true;
        }

        // The ellipse is Q(X) = (X-C)^T*M*(X-C) - 1. Compute Q(X) for any
        // point X. If Q(X) < 0, the point is inside the hyperellipsoid.
        // If Q(X) = 0, the point is on the hyperellipsoid. If Q(X) > 0,
        // the point is outside the hyperellipsoid.
        T GetLevelValue(Vector<T, N> const& point) const
        {
            Matrix<T, N, N> M{};
            for (std::size_t i = 0; i < N; ++i)
            {
                T axisSqrLength = Dot(axis[i], axis[i]);
                T extentSqrLength = extent[i] * extent[i];
                M += OuterProduct(axis[i], axis[i]) / (extentSqrLength * axisSqrLength);
            }

            Vector<T, N> diff = point - center;
            T levelValue = Dot(diff, M * diff) - C_<T>(1);
            return levelValue;
        }

        // Public member access.
        Vector<T, N> center;
        std::array<Vector<T, N>, N> axis;
        Vector<T, N> extent;

    private:
        static void Convert(std::array<T, (N + 1) * (N + 2) / 2> const& coeff,
            Matrix<T, N, N>& A, Vector<T, N>& B, T& C)
        {
            std::size_t i = 0;
            C = coeff[i++];

            for (std::size_t j = 0; j < N; ++j)
            {
                B[j] = coeff[i++];
            }

            for (std::size_t r = 0; r < N; ++r)
            {
                for (std::size_t c = 0; c < r; ++c)
                {
                    A(r, c) = A(c, r);
                }

#if defined(GTL_USE_MSWINDOWS)
                // For N = 2, Visual Studio generates
                //   warning C28020: The expression
                //   '0 <= _Param_(1)&&_Param(1)<=6-1' is not true at this
                //   call.
                // A similar warning occurs for N = 3 (upper bound is 10-1).
                // The warning is incorrect.
                //
                // When r = N-1, i = (N+1)*(N+2)/2 - 1 which corresponds to
                // the last element of coeff[]. The assignment is valid. After
                // the assignment, i is incremented and now out of range for
                // coeff[]. However, the loop after the assignment starts at
                // c = N and the loop body is not executed, after which the
                // r-loop terminates.
#pragma warning(disable : 28020)
#endif
                A(r, r) = coeff[i++];
#if defined(GTL_USE_MSWINDOWS)
#pragma warning(default : 28020)
#endif
                for (std::size_t c = r + 1; c < N; ++c)
                {
                    A(r, c) = coeff[i++] * C_<T>(1, 2);
                }
            }
        }

        static void Convert(Matrix<T, N, N> const& A, Vector<T, N> const& B,
            T C, std::array<T, (N + 1) * (N + 2) / 2>& coeff)
        {
            std::size_t i = 0;
            coeff[i++] = C;

            for (std::size_t j = 0; j < N; ++j)
            {
                coeff[i++] = B[j];
            }

            for (std::size_t r = 0; r < N; ++r)
            {
#if defined(GTL_USE_MSWINDOWS)
                // NOTE: MSVS 2019 16.7.5 generates for N = 2:
                //   warning C28020: The expression
                //   '0 <= _Param_(1)&&_Param(1)<=6-1' is not true at this
                //   call.
                // A similar warning occurs for N = 3 (upper bound is 10-1).
                // The warning is incorrect.
                //
                // When the r-loop is entered the first time, i = N+1. The
                // code below increments i 1+N-(r+1) times. The last time
                // this code is reached, i has been incremented
                // S = sum_{k=2}^{N} k times, so its value is i = N+1+S
                // = (N+1)*(N+2)/2 - 1. At this time, the first assignment
                // below is valid. After the assignment, i is incremented and
                // now out of range for coeff[]. However, the loop after the
                // assignment starts at c = N and the loop body is not
                // executed, after which the r-loop terminates.
#pragma warning(disable : 28020)
#endif
                coeff[i++] = A(r, r);
#if defined(GTL_USE_MSWINDOWS)
#pragma warning(default : 28020)
#endif
                for (std::size_t c = r + 1; c < N; ++c)
                {
                    coeff[i++] = A(r, c) * C_<T>(2);
                }
            }
        }

    private:
        friend class UnitTestHyperellipsoid;
    };

    // Comparisons to support sorted containers.
    template <typename T, std::size_t N>
    bool operator==(Hyperellipsoid<T, N> const& hyperellipsoid0, Hyperellipsoid<T, N> const& hyperellipsoid1)
    {
        return hyperellipsoid0.center == hyperellipsoid1.center
            && hyperellipsoid0.axis == hyperellipsoid1.axis
            && hyperellipsoid0.extent == hyperellipsoid1.extent;
    }

    template <typename T, std::size_t N>
    bool operator!=(Hyperellipsoid<T, N> const& hyperellipsoid0, Hyperellipsoid<T, N> const& hyperellipsoid1)
    {
        return !operator==(hyperellipsoid0, hyperellipsoid1);
    }

    template <typename T, std::size_t N>
    bool operator<(Hyperellipsoid<T, N> const& hyperellipsoid0, Hyperellipsoid<T, N> const& hyperellipsoid1)
    {
        if (hyperellipsoid0.center < hyperellipsoid1.center)
        {
            return true;
        }

        if (hyperellipsoid0.center > hyperellipsoid1.center)
        {
            return false;
        }

        if (hyperellipsoid0.axis < hyperellipsoid1.axis)
        {
            return true;
        }

        if (hyperellipsoid0.axis > hyperellipsoid1.axis)
        {
            return false;
        }

        return hyperellipsoid0.extent < hyperellipsoid1.extent;
    }

    template <typename T, std::size_t N>
    bool operator<=(Hyperellipsoid<T, N> const& hyperellipsoid0, Hyperellipsoid<T, N> const& hyperellipsoid1)
    {
        return !operator<(hyperellipsoid1, hyperellipsoid0);
    }

    template <typename T, std::size_t N>
    bool operator>(Hyperellipsoid<T, N> const& hyperellipsoid0, Hyperellipsoid<T, N> const& hyperellipsoid1)
    {
        return operator<(hyperellipsoid1, hyperellipsoid0);
    }

    template <typename T, std::size_t N>
    bool operator>=(Hyperellipsoid<T, N> const& hyperellipsoid0, Hyperellipsoid<T, N> const& hyperellipsoid1)
    {
        return !operator<(hyperellipsoid0, hyperellipsoid1);
    }
}
