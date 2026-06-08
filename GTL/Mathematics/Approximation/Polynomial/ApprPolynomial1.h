// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.06.08

#pragma once

// The samples are (x[i],w[i]) for 0 <= i < S. Think of w as a function of
// x, say w = f(x). The function fits the samples with a polynomial of
// degree d, say w = sum_{i=0}^d c[i]*x^i. The method is a least-squares
// fitting algorithm. The observation type is std::array<T,2>, which
// represents a 2-tuple (x,w).
//
// WARNING. The fitting algorithm for polynomial terms is known to be
// nonrobust for large degrees and for large magnitude data. One alternative
// is to use orthogonal polynomials and apply the least-squares algorithm to
// these. Another alternative is to transform the x-values to the square
// [-1,1] by subtracting the center of the their axis-aligned bounding
// interval and then uniformly scaling (x,w) so that the interval tightly
// contains the x-values.
//
// The fitting algorithm is described in
//   https://www.geometrictools.com/Documentation/PolynomialLeastSquares.pdf

#include <GTL/Mathematics/Algebra/Polynomial.h>
#include <GTL/Mathematics/MatrixAnalysis/LinearSystem.h>
#include <array>
#include <cstddef>
#include <utility>
#include <vector>

namespace gtl
{
    template <typename T>
    class ApprPolynomial1
    {
    public:
        static bool Fit(
            std::size_t xDegree,
            std::vector<std::array<T, 2>> const& observations,
            bool validateInput,
            Polynomial<T, 1>& polynomial)
        {
            if (validateInput)
            {
                ValidateInput(xDegree, observations);
            }

            Matrix<T> xPower{};
            ComputePowers(xDegree, observations,
                xPower);

            Matrix<T> A{};
            Vector<T> B{};
            ComputeLinearSystemComponents(xDegree, observations,
                xPower, A, B);

            return SolveLinearSystem(xDegree,
                A, B, polynomial);
        }

    private:
        static void ValidateInput(
            std::size_t xDegree,
            std::vector<std::array<T, 2>> const& observations)
        {
            GTL_ARGUMENT_ASSERT(
                xDegree > 0 && observations.size() > 0,
                "Invalid input.");
        }

        // Compute the powers of x.
        static void ComputePowers(
            std::size_t xDegree,
            std::vector<std::array<T, 2>> const& observations,
            Matrix<T>& xPower)
        {
            std::size_t twoXDegreeP1 = 2 * xDegree + 1;
            xPower.resize(observations.size(), twoXDegreeP1);
            for (std::size_t s = 0; s < observations.size(); ++s)
            {
                T const& x = observations[s][0];

                xPower(s, 0) = C_<T>(1);
                for (std::size_t j0 = 0, j1 = 1; j1 < twoXDegreeP1; j0 = j1++)
                {
                    xPower(s, j1) = x * xPower(s, j0);
                }
            }
        }

        // Matrix A is the Vandermonde matrix and vector B is the
        // right-hand side of the linear system A*X = B.
        static void ComputeLinearSystemComponents(
            std::size_t xDegree,
            std::vector<std::array<T, 2>> const& observations,
            Matrix<T>const & xPower,
            Matrix<T>& A,
            Vector<T>& B)
        {
            std::size_t xDegreeP1 = xDegree + 1;
            A.resize(xDegreeP1, xDegreeP1);
            B.resize(xDegreeP1);
            for (std::size_t j0 = 0; j0 < xDegreeP1; ++j0)
            {
                T sum = C_<T>(0);
                for (std::size_t s = 0; s < observations.size(); ++s)
                {
                    T const& w = observations[s][1];
                    sum += w * xPower(s, j0);
                }

                B[j0] = sum;

                for (std::size_t j1 = 0; j1 < xDegreeP1; ++j1)
                {
                    sum = C_<T>(0);
                    for (std::size_t s = 0; s < observations.size(); ++s)
                    {
                        sum += xPower(s, j0 + j1);
                    }

                    A(j0, j1) = sum;
                }
            }
        }

        // Solve for the polynomial coefficients.
        static bool SolveLinearSystem(
            std::size_t xDegree,
            Matrix<T> const& A,
            Vector<T> const& B,
            Polynomial<T, 1>& polynomial)
        {
            std::size_t xDegreeP1 = xDegree + 1;
            Vector<T> coefficient(xDegreeP1);
            if (LinearSystem<T>::Solve(A, B, coefficient))
            {
                polynomial.SetDegree(xDegree);
                for (std::size_t i = 0; i < xDegreeP1; ++i)
                {
                    polynomial[i] = std::move(coefficient[i]);
                }
                return true;
            }
            else
            {
                polynomial = C_<T>(0);
                return false;
            }
        }

    private:
        friend class UnitTestApprPolynomial1;
    };
}
