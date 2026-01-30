// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.10.29

#pragma once

// The samples are (x[i],y[i],w[i]) for 0 <= i < S. Think of w as a function
// of x and y, say w = f(x,y). The function fits the samples with a polynomial
// of degree d0 in x and degree d1 in y, 
//   w = sum_{i=0}^{d0} sum_{j=0}^{d1} c[i][j]*x^i*y^j
// The method is a least-squares fitting algorithm. The observation type is
// std::array<T,2>, which represents a 3-tuple (x,y,w).
//
// WARNING. The fitting algorithm for polynomial terms
//   (1,x,x^2,...,x^d0), (1,y,y^2,...,y^d1)
// is known to be nonrobust for large degrees and for large magnitude data.
// One alternative is to use orthogonal polynomials
//   (f[0](x),...,f[d0](x)), (g[0](y),...,g[d1](y))
// and apply the least-squares algorithm to these. Another alternative is to
// transform
//   (x',y',w') = ((x-xcen)/rng, (y-ycen)/rng, w/rng)
// where xmin = min(x[i]), xmax = max(x[i]), xcen = (xmin+xmax)/2,
// ymin = min(y[i]), ymax = max(y[i]), ycen = (ymin+ymax)/2, and
// rng = max(xmax-xmin,ymax-ymin). Fit the (x',y',w') points,
//   w' = sum_{i=0}^{d0} sum_{j=0}^{d1} c'[i][j]*(x')^i*(y')^j
// The original polynomial is evaluated as
//   w = rng * sum_{i=0}^{d0} sum_{j=0}^{d1} c'[i][j] *
//       ((x-xcen)/rng)^i * ((y-ycen)/rng)^j

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
        static bool Fit(
            std::size_t xDegree, std::size_t yDegree,
            std::vector<std::array<T, 3>> const& observations,
            Polynomial<T, 2>& polynomial)
        {
            // Compute the powers of x and y.
            std::size_t const twoXDegree = 2 * xDegree;
            std::size_t const twoYDegree = 2 * yDegree;
            Matrix<T> xPower(observations.size(), twoXDegree + 1);
            Matrix<T> yPower(observations.size(), twoYDegree + 1);
            for (std::size_t s = 0; s < observations.size(); ++s)
            {
                T const& x = observations[s][0];
                T const& y = observations[s][1];

                xPower(s, 0) = C_<T>(1);
                for (std::size_t j0 = 0, j1 = 1; j1 <= twoXDegree; j0 = j1++)
                {
                    xPower(s, j1) = x * xPower(s, j0);
                }

                yPower(s, 0) = C_<T>(1);
                for (std::size_t j0 = 0, j1 = 1; j1 <= twoYDegree; j0 = j1++)
                {
                    yPower(s, j1) = y * yPower(s, j0);
                }
            }

            // Matrix A is the Vandermonde matrix and vector B is the
            // right-hand side of the linear system A*X = B.
            std::size_t const xDegreeP1 = xDegree + 1;
            std::size_t const yDegreeP1 = yDegree + 1;
            std::size_t const numCoefficients = xDegreeP1 * yDegreeP1;
            Matrix<T> A(numCoefficients, numCoefficients);
            Vector<T> B(numCoefficients);
            for (std::size_t j0 = 0; j0 <= yDegree; ++j0)
            {
                for (std::size_t i0 = 0; i0 <= xDegree; ++i0)
                {
                    T sum = C_<T>(0);
                    std::size_t k0 = i0 + xDegreeP1 * j0;
                    for (std::size_t s = 0; s < observations.size(); ++s)
                    {
                        T const& w = observations[s][2];
                        sum += w * xPower(s, i0) * yPower(s, j0);
                    }

                    B[k0] = sum;

                    for (std::size_t j1 = 0; j1 <= yDegree; ++j1)
                    {
                        for (std::size_t i1 = 0; i1 <= xDegree; ++i1)
                        {
                            sum = C_<T>(0);
                            std::size_t k1 = i1 + xDegreeP1 * j1;
                            for (std::size_t s = 0; s < observations.size(); ++s)
                            {
                                sum += xPower(s, i0 + i1) * yPower(s, j0 + j1);
                            }

                            A(k0, k1) = sum;
                        }
                    }
                }
            }

            // Solve for the polynomial coefficients.
            Vector<T> coefficient(numCoefficients);
            if (LinearSystem<T>::Solve(A, B, coefficient))
            {
                polynomial.SetDegree(yDegree);
                for (std::size_t r = 0, i = 0; r <= yDegree; ++r)
                {
                    auto& px = polynomial[r];
                    px.SetDegree(xDegree);
                    for (std::size_t c = 0; c <= xDegree; ++c, ++i)
                    {
                        px[c] = std::move(coefficient[i]);
                    }
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
        friend class UnitTestApprPolynomial3;
    };
}
