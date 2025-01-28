// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.28

#pragma once

// The samples are (x[i],w[i]) for 0 <= i < S. Think of w as a function of
// x, say w = f(x). The function fits the samples with a polynomial of
// degree d, say w = sum_{i=0}^d c[i]*x^i. The method is a least-squares
// fitting algorithm. The observation type is std::array<T,2>, which
// represents a 2-tuple (x,w).
//
// WARNING. The fitting algorithm for polynomial terms
//   (1,x,x^2,...,x^d)
// is known to be nonrobust for large degrees and for large magnitude data.
// One alternative is to use orthogonal polynomials
//   (f[0](x),...,f[d](x))
// and apply the least-squares algorithm to these. Another alternative is to
// transform
//   (x',w') = ((x-xcen)/rng, w/rng)
// where xmin = min(x[i]), xmax = max(x[i]), xcen = (xmin+xmax)/2, and
// rng = xmax-xmin. Fit the (x',w') points,
//   w' = sum_{i=0}^d c'[i]*(x')^i.
// The original polynomial is evaluated as
//   w = rng*sum_{i=0}^d c'[i]*((x-xcen)/rng)^i

#include <GTL/Mathematics/Algebra/Polynomial.h>
#include <GTL/Mathematics/MatrixAnalysis/LinearSystem.h>
#include <array>
#include <cstddef>
#include <utility>
#include <vector>

namespace gtl
{
    template <typename T>
    class ApprPolynomial2
    {
    public:
        static bool Fit(std::size_t degree,
            std::vector<std::array<T, 2>> const& observations,
            Polynomial<T, 1>& polynomial, std::array<T, 2>& xExtreme,
            std::array<T, 2>& wExtreme)
        {
            // Compute the powers of x.
            std::size_t const twoDegree = 2 * degree;
            Matrix<T> xPower(observations.size(), twoDegree + 1);
            xExtreme[0] = observations[0][0];
            xExtreme[1] = xExtreme[0];
            wExtreme[0] = observations[0][1];
            wExtreme[1] = wExtreme[0];
            for (std::size_t s = 0; s < observations.size(); ++s)
            {
                T const& x = observations[s][0];
                T const& w = observations[s][1];

                if (x < xExtreme[0])
                {
                    xExtreme[0] = x;
                }
                else if (x > xExtreme[1])
                {
                    xExtreme[1] = x;
                }

                if (w < wExtreme[0])
                {
                    wExtreme[0] = w;
                }
                else if (w > wExtreme[1])
                {
                    wExtreme[1] = w;
                }

                xPower(s, 0) = C_<T>(1);
                for (std::size_t j0 = 0, j1 = 1; j1 <= twoDegree; j0 = j1++)
                {
                    xPower(s, j1) = x * xPower(s, j0);
                }
            }

            // Matrix A is the Vandermonde matrix and vector B is the
            // right-hand side of the linear system A*X = B.
            std::size_t const numCoefficients = degree + 1;
            Matrix<T> A(numCoefficients, numCoefficients);
            Vector<T> B(numCoefficients);
            for (std::size_t j0 = 0; j0 <= degree; ++j0)
            {
                T sum = C_<T>(0);
                for (std::size_t s = 0; s < observations.size(); ++s)
                {
                    T const& w = observations[s][1];
                    sum += w * xPower(s, j0);
                }

                B[j0] = sum;

                for (std::size_t j1 = 0; j1 <= degree; ++j1)
                {
                    sum = C_<T>(0);
                    for (std::size_t s = 0; s < observations.size(); ++s)
                    {
                        sum += xPower(s, j0 + j1);
                    }

                    A(j0, j1) = sum;
                }
            }

            // Solve for the polynomial coefficients.
            Vector<T> coefficient(numCoefficients);
            if (LinearSystem<T>::Solve(A, B, coefficient))
            {
                polynomial.SetDegree(degree);
                for (std::size_t i = 0; i <= degree; ++i)
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
        friend class UnitTestApprPolynomial2;
    };
}
