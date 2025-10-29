// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.10.27

#pragma once

// Fit a polynomial to data, where the polynomial terms have specially chosen
// powers. Let N be the number of independent variables X = (x[0],...,x[N-1]).
// The polynomial is
//   y = sum_{j=0}^{m-1} c[j] * prod_{i=0}^{N-1} x[i]^{d[j][i]}
// Each m-tuple of degrees D[j] = (d[j][0], ..., d[j][N-1]) must be unique;
// that is, D[j0] != D[j1] for j0 != j1. A least-squares fitting algorithm is
// used, but the input data is first mapped to (x',y') in [-1,1]^{N+1} for
// numerical robustness.

#include <GTL/Mathematics/MatrixAnalysis/LinearSystem.h>
#include <algorithm>
#include <array>
#include <cstddef>
#include <vector>

namespace gtl
{
    template <typename T, std::size_t N>
    class ApprPolynomialSpecial
    {
    public:
        class Polynomial
        {
        public:
            Polynomial()
                :
                mDegrees{},
                mCoefficients{},
                mDomain{},
                mScales{},
                mPowers{}
            {
                for (std::size_t i = 0; i <= N; ++i)
                {
                    mDomain[i] = { C_<T>(0), C_<T>(0) };
                    mScales[i] = C_<T>(0);
                }
            }

            // Support for polynomial evaluation. The coefficients were
            // generated for the samples mapped to [-1,1]^{N+1}. The
            // operator()(...) function must transform X to X' in [-1,1]^N,
            // compute y' in [-1,1] and then transform y' to y.
            T operator()(std::array<T, N> const& inX) const
            {
                // Transform X to X' in [-1,1]^N.
                std::array<T, N> x{};
                for (std::size_t i = 0; i < N; ++i)
                {
                    x[i] = -C_<T>(1) + C_<T>(2) * mScales[i] * (inX[i] - mDomain[i][0]);
                }

                // Compute powers of x[i] from 1 through x[i]^{d[i][m-1]}.
                // TODO: Some of these powers are unused, but the choice
                // depends on the input degrees. Provide a mechanism to
                // compute only the powers that are needed?
                for (std::size_t i = 0; i < N; ++i)
                {
                    auto& powers = mPowers[i];
                    std::size_t const kSup = powers.size();
                    powers[0] = C_<T>(1);
                    for (std::size_t k0 = 0, k1 = 1; k1 < kSup; k0 = k1++)
                    {
                        powers[k1] = powers[k0] * x[i];
                    }
                }

                // y = sum_{j=0}^{m-1} c[j] * prod_{i=0}^{N-1} x[i]^{d[i][j]}
                T y = C_<T>(0);
                for (std::size_t j = 0; j < mCoefficients.size(); ++j)
                {
                    T term = mCoefficients[j];
                    for (std::size_t i = 0; i < N; ++i)
                    {
                        term *= mPowers[i][mDegrees[j][i]];
                    }
                    y += term;
                }

                // Transform y' from [-1,1] back to the original space.
                y = C_<T>(1, 2) * (y + C_<T>(1)) / mScales[N] + mDomain[N][0];
                return y;
            }

            inline std::vector<std::array<std::size_t, N>> const& GetDegrees() const
            {
                return mDegrees;
            }

            // The input index must satisfy 0 <= i < N.
            inline std::array<std::size_t, N> const& GetDegrees(std::size_t i) const
            {
                return mDegrees[i];
            }

            inline std::array<std::array<T, 2>, N + 1> const& GetDomain() const
            {
                return mDomain;
            }

            // The input index must satisfy 0 <= i <= N.
            inline std::array<T, 2> const& GetDomain(std::size_t i) const
            {
                return mDomain[i];
            }

            inline std::vector<T> const& GetCoefficients() const
            {
                return mCoefficients;
            }

        private:
            // Allow the approximator to set the members of Polynomial without
            // having to expose them in public scope.
            friend class ApprPolynomialSpecial<T, N>;

            // The mDegrees and mCoefficients have the same size.
            std::vector<std::array<std::size_t, N>> mDegrees;
            std::vector<T> mCoefficients;
            std::array<std::array<T, 2>, N + 1> mDomain;
            std::array<T, N + 1> mScales;

            // Storage for powers of the independent variables. The mutable
            // tag allows the operator()(...) to have conceptual constness.
            mutable std::array<std::vector<T>, N> mPowers;
        };

        static bool Fit(
            std::vector<std::array<T, N + 1>> const& observations,
            std::vector<std::array<std::size_t, N>> const& degrees,
            Polynomial& polynomial)
        {
            GTL_ARGUMENT_ASSERT(
                observations.size() > 0 && degrees.size() > 0,
                "Invalid input.");

            std::size_t const numCoefficients = degrees.size();
            polynomial = Polynomial{};
            polynomial.mDegrees = degrees;
            polynomial.mCoefficients.resize(numCoefficients);
            std::fill(polynomial.mCoefficients.begin(), polynomial.mCoefficients.end(), C_<T>(0));

            // Powers of x[i] are computed up to twice the degree when
            // constructing the fitted polynomial. Powers of x[i] are
            // computed up to the degree for the evaluation of the fitted
            // polynomial. Allocate the maximum space for the powers.
            for (std::size_t i = 0; i < N; ++i)
            {
                std::size_t maxPower = 0;
                for (std::size_t j = 0; j < numCoefficients; ++j)
                {
                    maxPower = std::max(maxPower, degrees[j][i]);
                }
                polynomial.mPowers[i].resize(2 * maxPower + 1);
            }

            // Transform the observations to [-1,1]^{N+1} for numerical
            // robustness.
            std::vector<std::array<T, N + 1>> transformed{};
            Transform(observations, transformed, polynomial);

            // Fit the transformed data using a least-squares algorithm.
            return DoLeastSquares(transformed, polynomial);
        }

    private:
        // Transform (X,y) to (X',y') in [-1,1]^{N+1}.
        static void Transform(
            std::vector<std::array<T, N + 1>> const& observations,
            std::vector<std::array<T, N + 1>>& transformed,
            Polynomial& polynomial)
        {
            transformed.resize(observations.size());

            std::array<T, N + 1> omin = observations[0];
            std::array<T, N + 1> omax = omin;
            for (std::size_t s = 1; s < observations.size(); ++s)
            {
                auto const& observation = observations[s];
                for (std::size_t i = 0; i <= N; ++i)
                {
                    if (observation[i] < omin[i])
                    {
                        omin[i] = observation[i];
                    }
                    else if (observation[i] > omax[i])
                    {
                        omax[i] = observation[i];
                    }
                }
            }

            for (std::size_t i = 0; i <= N; ++i)
            {
                polynomial.mDomain[i][0] = omin[i];
                polynomial.mDomain[i][1] = omax[i];
                polynomial.mScales[i] = C_<T>(1) / (omax[i] - omin[i]);
            }

            for (std::size_t s = 0; s < observations.size(); ++s)
            {
                auto const& observation = observations[s];
                for (std::size_t i = 0; i <= N; ++i)
                {
                    transformed[s][i] =
                        -C_<T>(1) + C_<T>(2) * polynomial.mScales[i] * (observation[i] - omin[i]);
                }
            }
        }

        // The least-squares fitting algorithm for the transformed data.
        static bool DoLeastSquares(std::vector<std::array<T, N + 1>> const& transformed,
            Polynomial& polynomial)
        {
            // Set up a linear system A*C = B, where C are the polynomial
            // coefficients.
            std::size_t size = polynomial.mCoefficients.size();
            Matrix<T> A(size, size);  // zero matrix
            Vector<T> B(size);  // zero vector

            for (auto const& x : transformed)
            {
                // Compute powers of x[i], 0 <= i < N. The y-value is x[N].
                for (std::size_t i = 0; i < N; ++i)
                {
                    auto& powers = polynomial.mPowers[i];
                    std::size_t const kSup = powers.size();
                    powers[0] = C_<T>(1);
                    for (std::size_t k0 = 0, k1 = 1; k1 < kSup; k0 = k1++)
                    {
                        powers[k1] = powers[k0] * x[i];
                    }
                }

                for (std::size_t row = 0; row < size; ++row)
                {
                    auto const& degreesRow = polynomial.mDegrees[row];

                    // Update the upper-triangular portion of the symmetric
                    // matrix.
                    T term{};
                    for (std::size_t col = row; col < size; ++col)
                    {
                        auto const& degreesCol = polynomial.mDegrees[col];

                        term = C_<T>(1);
                        for (std::size_t i = 0; i < N; ++i)
                        {
                            auto const& powers = polynomial.mPowers[i];
                            term *= powers[degreesRow[i] + degreesCol[i]];
                        }
                        A(row, col) += term;
                    }

                    // Update the right-hand side of the system.
                    term = x[N];
                    for (std::size_t i = 0; i < N; ++i)
                    {
                        auto const& powers = polynomial.mPowers[i];
                        term *= powers[degreesRow[i]];
                    }
                    B[row] += term;
                }
            }

            // Copy the upper-triangular portion of the symmetric matrix to
            // the lower-triangular portion.
            for (std::size_t row = 0; row < size; ++row)
            {
                for (std::size_t col = 0; col < row; ++col)
                {
                    A(row, col) = A(col, row);
                }
            }

            // Precondition by normalizing the sums.
            T const tNumObservations = static_cast<T>(transformed.size());
            A /= tNumObservations;
            B /= tNumObservations;

            // Solve for the polynomial coefficients.
            Vector<T> coefficients{};
            if (LinearSystem<T>::Solve(A, B, coefficients))
            {
                for (std::size_t i = 0; i < size; ++i)
                {
                    polynomial.mCoefficients[i] = coefficients[i];
                }
                return true;
            }
            else
            {
                std::fill(polynomial.mCoefficients.begin(),
                    polynomial.mCoefficients.end(), C_<T>(0));
                return false;
            }
        }

    private:
        friend class UnitTestApprPolynomialSpecial;
    };
}
