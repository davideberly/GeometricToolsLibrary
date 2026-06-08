// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.06.08

#pragma once

// Fit a polynomial to data, where the polynomial terms have user-specified
// powers. Let N be the number of independent variables X = (x[0],...,x[N-1]).
// The polynomial is
//   y = sum_{j=0}^{m-1} c[j] * prod_{i=0}^{N-1} x[i]^{d[j][i]}
// Each N-tuple of degrees D[j] = (d[j][0], ..., d[j][N-1]) must be unique;
// that is, D[j0] != D[j1] for j0 != j1. The method is a least-squares fitting
// algorithm. The observation type is std::array<T, N+1>, which represents an
// (N+1)-tuple (x[0],...,x[N-1],y).
//
// WARNING. The fitting algorithm for polynomial terms is known to be
// nonrobust for large degrees and for large magnitude data. One alternative
// is to use orthogonal polynomials and apply the least-squares algorithm to
// these. Another alternative is to transform the X-values to the hypercube
// [-1,1]^N by subtracting the center of their axis-aligned bounding hypercube
// and then uniformly scaling (X,y) so that the hypercube tightly contains
// the X-values.
//
// The fitting algorithm is described in
//   https://www.geometrictools.com/Documentation/PolynomialLeastSquares.pdf

#include <GTL/Mathematics/MatrixAnalysis/LinearSystem.h>
#include <algorithm>
#include <array>
#include <cstddef>
#include <utility>
#include <vector>

namespace gtl
{
    template <typename T, std::size_t N>
    class ApprPolynomial
    {
    public:
        using Multiindex = std::array<std::size_t, N>;
        using Observation = std::array<T, N + 1>;

        ApprPolynomial()
            :
            mMultiindices{},
            mObservations{},
            mMaxMultiindex{},
            mTwoMaxMultiindexP1{},
            mPowers{},
            mMaxMaxMultiindex(0),
            mInterpolationProduct{}
        {
        }

        // The index of 'polynomial' is an index into the multiindices. The T
        // of 'polynomial' is the coefficient of the polynomial corresponding\
        // to that index.
        bool Fit(
            std::vector<Multiindex> const& multiindices,
            std::vector<Observation> const& observations,
            std::vector<T>& polynomial)
        {
            GTL_ARGUMENT_ASSERT(
                multiindices.size() > 0 && observations.size() > 0,
                "Invalid input.");

            mMultiindices = &multiindices;
            mObservations = &observations;
            polynomial.resize(multiindices.size());

            // Determine which powers need to be computed.
            ComputeMaxMultiindex();

            // Compute powers of T(j_m,k).
            ComputePowers();

            Matrix<T> A{};
            Vector<T> B{};
            ComputeLinearSystemComponents(A, B);

            return SolveLinearSystem(A, B, polynomial);
        }

        T Evaluate(
            std::array<T, N> const& x,
            std::vector<T> const& polynomial)
        {
            T output = C_<T>(0);
            for (std::size_t i = 0; i < mMultiindices->size(); ++i)
            {
                output += ComputeInterpolationTerm(x, polynomial[i], (*mMultiindices)[i]);
            }
            return output;
        }

    private:
        void ComputeMaxMultiindex()
        {
            mMaxMultiindex.fill(0);
            for (auto const& multiindex : *mMultiindices)
            {
                for (std::size_t i = 0; i < N; ++i)
                {
                    mMaxMultiindex[i] = std::max(multiindex[i], mMaxMultiindex[i]);
                    mTwoMaxMultiindexP1[i] = 2 * mMaxMultiindex[i] + 1;
                }
            }

            for (std::size_t i = 0; i < N; ++i)
            {
                GTL_ARGUMENT_ASSERT(
                    mMaxMultiindex[i] > 0,
                    "Multiindices must reference all N variables.");
            }

            mMaxMaxMultiindex = *std::max_element(mMaxMultiindex.begin(), mMaxMultiindex.end());
            mInterpolationProduct.resize(mMaxMaxMultiindex + 1, 0);
        }

        void ComputePowers()
        {
            // Compute the maximum storage for powers, which occurs for the
            // unconstrained least-squares fit.
            for (std::size_t i = 0; i < N; ++i)
            {
                mPowers[i].resize(mObservations->size(), mTwoMaxMultiindexP1[i]);
            }

            for (std::size_t k = 0; k < mObservations->size(); ++k)
            {
                for (std::size_t i = 0; i < N; ++i)
                {
                    T const& variable = (*mObservations)[k][i];
                    mPowers[i](k, 0) = C_<T>(1);
                    for (std::size_t j0 = 0, j1 = 1; j1 < mTwoMaxMultiindexP1[i]; j0 = j1++)
                    {
                        mPowers[i](k, j1) = variable * mPowers[i](k, j0);
                    }
                }
            }
        }

        T ComputeInterpolationTerm(
            std::array<T, N> const& x,
            T const& coefficient,
            Multiindex const& multiindex)
        {
            T term = coefficient;
            for (std::size_t i = 0; i < N; ++i)
            {
                // Powers of x for which a subset is used to compute the
                // interpolation term.
                mInterpolationProduct[0] = C_<T>(1);
                for (std::size_t j0 = 0, j1 = 1; j1 <= mMaxMultiindex[i]; j0 = j1++)
                {
                    mInterpolationProduct[j1] = x[i] * mInterpolationProduct[j0];
                }
                term *= mInterpolationProduct[multiindex[i]];
            }
            return term;
        }

        T ComputeObservationTerm(
            Multiindex const& multiindex,
            std::size_t k)
        {
            T term = C_<T>(1);
            for (std::size_t i = 0; i < N; ++i)
            {
                term *= mPowers[i](k, multiindex[i]);
            }
            return term;
        }

        void ComputeLinearSystemComponents(
            Matrix<T>& A,
            Vector<T>& B)
        {
            A.resize(mMultiindices->size(), mMultiindices->size());
            B.resize(mMultiindices->size());

            std::vector<T> rTerms(mObservations->size());
            for (std::size_t k = 0; k < mObservations->size(); ++k)
            {
                // Compute B.
                for (std::size_t r = 0; r < mMultiindices->size(); ++r)
                {
                    Multiindex const& jr = (*mMultiindices)[r];
                    rTerms[r] = ComputeObservationTerm(jr, k);
                    B[r] += (*mObservations)[k][N] * rTerms[r];
                }

                // Compute A.
                for (std::size_t m = 0; m < mMultiindices->size(); ++m)
                {
                    Multiindex const& jm = (*mMultiindices)[m];
                    for (std::size_t r = m; r < mMultiindices->size(); ++r)
                    {
                        A(m, r) += ComputeObservationTerm(jm, k) * rTerms[r];
                    }

                    // Fill in the lower-left portion of A for a symmetric matrix.
                    for (std::size_t r = 0; r < m; ++r)
                    {
                        A(m, r) = A(r, m);
                    }
                }
            }
        }

        static bool SolveLinearSystem(
            Matrix<T> const& A,
            Vector<T> const& B,
            std::vector<T>& polynomial)
        {
            Vector<T> coefficients(polynomial.size());
            if (LinearSystem<T>::Solve(A, B, coefficients))
            {
                for (std::size_t i = 0; i < polynomial.size(); ++i)
                {
                    polynomial[i] = coefficients[i];
                }
                return true;
            }
            else
            {
                std::fill(polynomial.begin(), polynomial.end(),C_<T>(0));
                return false;
            }
        }

        std::vector<Multiindex> const* mMultiindices;
        std::vector<Observation> const* mObservations;
        Multiindex mMaxMultiindex;
        Multiindex mTwoMaxMultiindexP1;
        std::array<Matrix<T>, N> mPowers;
        std::size_t mMaxMaxMultiindex;
        std::vector<T> mInterpolationProduct;

    private:
        friend class UnitTestApprPolynomial;
    };
}
