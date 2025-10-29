// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.10.27

#pragma once

// The samples are (x[i],y[i],z[i],w[i]) for 0 <= i < S. Think of w as a
// function of x, y and z, say w = f(x,y,z). The function fits the samples
// with a polynomial of degree d0 in x, degree d1 in y and degree d2 in z,
// say
//   w = sum_{i=0}^{d0} sum_{j=0}^{d1} sum_{k=0}^{d2} c[i][j][k]*x^i*y^j*z^k
// The method is a least-squares fitting algorithm. The mParameters stores
// c[i][j][k] = mParameters[i+(d0+1)*(j+(d1+1)*k)] for a total of
// (d0+1)*(d1+1)*(d2+1) coefficients. The observation type is std::array<T,4>,
// which represents a 4-tuple (x,y,z,w).
//
// WARNING. The fitting algorithm for polynomial terms
//   (1,x,x^2,...,x^d0), (1,y,y^2,...,y^d1), (1,z,z^2,...,z^d2)
// is known to be nonrobust for large degrees and for large magnitude data.
// One alternative is to use orthogonal polynomials
//   (f[0](x),...,f[d0](x)), (g[0](y),...,g[d1](y)), (h[0](z),...,h[d2](z))
// and apply the least-squares algorithm to these. Another alternative is to
// transform
//   (x',y',z',w') = ((x-xcen)/rng, (y-ycen)/rng, (z-zcen)/rng, w/rng)
// where xmin = min(x[i]), xmax = max(x[i]), xcen = (xmin+xmax)/2,
// ymin = min(y[i]), ymax = max(y[i]), ycen = (ymin+ymax)/2, zmin = min(z[i]),
// zmax = max(z[i]), zcen = (zmin+zmax)/2, and
// rng = max(xmax-xmin,ymax-ymin,zmax-zmin). Fit the (x',y',z',w') points,
//   w' = sum_{i=0}^{d0} sum_{j=0}^{d1} sum_{k=0}^{d2} c'[i][j][k] *
//          (x')^i*(y')^j*(z')^k
// The original polynomial is evaluated as
//   w = rng * sum_{i=0}^{d0} sum_{j=0}^{d1} sum_{k=0}^{d2} c'[i][j][k] *
//       ((x-xcen)/rng)^i * ((y-ycen)/rng)^j * ((z-zcen)/rng)^k

#include <GTL/Mathematics/Algebra/Polynomial.h>
#include <GTL/Mathematics/MatrixAnalysis/LinearSystem.h>
#include <array>
#include <cstddef>
#include <utility>
#include <vector>

namespace gtl
{
    template <typename T>
    class ApprPolynomial4
    {
    public:
        static bool Fit(std::size_t xDegree, std::size_t yDegree, std::size_t zDegree,
            std::vector<std::array<T, 4>> const& observations,
            Polynomial<T, 3>& polynomial, std::array<T, 2>& xExtreme,
            std::array<T, 2>& yExtreme, std::array<T, 2>& zExtreme,
            std::array<T, 2>& wExtreme)
        {
            // Compute the powers of x, y and z.
            std::size_t const twoXDegree = 2 * xDegree;
            std::size_t const twoYDegree = 2 * yDegree;
            std::size_t const twoZDegree = 2 * zDegree;
            Matrix<T> xPower(observations.size(), twoXDegree + 1);
            Matrix<T> yPower(observations.size(), twoYDegree + 1);
            Matrix<T> zPower(observations.size(), twoZDegree + 1);
            xExtreme[0] = observations[0][0];
            xExtreme[1] = xExtreme[0];
            yExtreme[0] = observations[0][1];
            yExtreme[1] = yExtreme[0];
            zExtreme[0] = observations[0][2];
            zExtreme[1] = zExtreme[0];
            wExtreme[0] = observations[0][3];
            wExtreme[1] = wExtreme[0];
            for (std::size_t s = 0; s < observations.size(); ++s)
            {
                T const& x = observations[s][0];
                T const& y = observations[s][1];
                T const& z = observations[s][2];
                T const& w = observations[s][3];

                if (x < xExtreme[0])
                {
                    xExtreme[0] = x;
                }
                else if (x > xExtreme[1])
                {
                    xExtreme[1] = x;
                }

                if (y < yExtreme[0])
                {
                    yExtreme[0] = y;
                }
                else if (y > yExtreme[1])
                {
                    yExtreme[1] = y;
                }

                if (z < zExtreme[0])
                {
                    zExtreme[0] = z;
                }
                else if (z > zExtreme[1])
                {
                    zExtreme[1] = z;
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
                for (std::size_t j0 = 0, j1 = 1; j1 <= twoXDegree; j0 = j1++)
                {
                    xPower(s, j1) = x * xPower(s, j0);
                }

                yPower(s, 0) = C_<T>(1);
                for (std::size_t j0 = 0, j1 = 1; j1 <= twoYDegree; j0 = j1++)
                {
                    yPower(s, j1) = y * yPower(s, j0);
                }

                zPower(s, 0) = C_<T>(1);
                for (std::size_t j0 = 0, j1 = 1; j1 <= twoZDegree; j0 = j1++)
                {
                    zPower(s, j1) = z * zPower(s, j0);
                }
            }

            // Matrix A is the Vandermonde matrix and vector B is the
            // right-hand side of the linear system A*X = B.
            std::size_t const xDegreeP1 = xDegree + 1;
            std::size_t const yDegreeP1 = yDegree + 1;
            std::size_t const zDegreeP1 = zDegree + 1;
            std::size_t const numCoefficients = xDegreeP1 * yDegreeP1 * zDegreeP1;
            Matrix<T> A(numCoefficients, numCoefficients);
            Vector<T> B(numCoefficients);
            for (std::size_t k0 = 0; k0 <= zDegree; ++k0)
            {
                for (std::size_t j0 = 0; j0 <= yDegree; ++j0)
                {
                    for (std::size_t i0 = 0; i0 <= xDegree; ++i0)
                    {
                        T sum = C_<T>(0);
                        std::size_t n0 = i0 + xDegreeP1 * (j0 + yDegreeP1 * k0);
                        for (std::size_t s = 0; s < observations.size(); ++s)
                        {
                            T const& w = observations[s][3];
                            sum += w * xPower(s, i0) * yPower(s, j0) * zPower(s, k0);
                        }

                        B[n0] = sum;

                        for (std::size_t k1 = 0; k1 <= zDegree; ++k1)
                        {
                            for (std::size_t j1 = 0; j1 <= yDegree; ++j1)
                            {
                                for (std::size_t i1 = 0; i1 <= xDegree; ++i1)
                                {
                                    sum = C_<T>(0);
                                    std::size_t n1 = i1 + xDegreeP1 * (j1 + yDegreeP1 * k1);
                                    for (std::size_t s = 0; s < observations.size(); ++s)
                                    {
                                        sum += xPower(s, i0 + i1) * yPower(s, j0 + j1) * zPower(s, k0 + k1);
                                    }

                                    A(n0, n1) = sum;
                                }
                            }
                        }
                    }
                }
            }

            // Solve for the polynomial coefficients.
            Vector<T> coefficient(numCoefficients);
            if (LinearSystem<T>::Solve(A, B, coefficient))
            {
                polynomial.SetDegree(zDegree);
                for (std::size_t s = 0, i = 0; s <= zDegree; ++s)
                {
                    auto& pxy = polynomial[s];
                    pxy.SetDegree(yDegree);
                    for (std::size_t r = 0; r <= yDegree; ++r)
                    {
                        auto& px = pxy[r];
                        px.SetDegree(xDegree);
                        for (std::size_t c = 0; c <= xDegree; ++c, ++i)
                        {
                            px[c] = std::move(coefficient[i]);
                        }
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
        friend class UnitTestApprPolynomial4;
    };
}
