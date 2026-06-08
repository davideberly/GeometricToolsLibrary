// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.06.08

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
// WARNING. The fitting algorithm for polynomial terms is known to be
// nonrobust for large degrees and for large magnitude data. One alternative
// is to use orthogonal polynomials and apply the least-squares algorithm to
// these. Another alternative is to transform the (x,y,z)-values to the
// cube [-1,1]^2 by subtracting the center of their axis-aligned bounding
// cube and then uniformly scaling (x,y,z,w) so that the cube tightly contains
// the (x,y,z)-values. 
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
    class ApprPolynomial3
    {
    public:
        static bool Fit(
            std::size_t xDegree,
            std::size_t yDegree,
            std::size_t zDegree,
            std::vector<std::array<T, 4>> const& observations,
            bool validateInput,
            Polynomial<T, 3>& polynomial)
        {
            if (validateInput)
            {
                ValidateInput(xDegree, yDegree, zDegree, observations);
            }

            Matrix<T> xPower{}, yPower{}, zPower{};
            ComputePowers(xDegree, yDegree, zDegree, observations,
                xPower, yPower, zPower);

            Matrix<T> A{};
            Vector<T> B{};
            ComputeLinearSystemComponents(xDegree, yDegree, zDegree, observations,
                xPower, yPower, zPower, A, B);

            return SolveLinearSystem(xDegree, yDegree, zDegree,
                A, B, polynomial);
        }

    private:
        static void ValidateInput(
            std::size_t xDegree,
            std::size_t yDegree,
            std::size_t zDegree,
            std::vector<std::array<T, 4>> const& observations)
        {
            GTL_ARGUMENT_ASSERT(
                xDegree > 0 && yDegree > 0 && zDegree > 0 && observations.size() > 0,
                "Invalid input.");
        }

        // Compute the powers of x, y and z.
        static void ComputePowers(
            std::size_t xDegree,
            std::size_t yDegree,
            std::size_t zDegree,
            std::vector<std::array<T, 4>> const& observations,
            Matrix<T>& xPower,
            Matrix<T>& yPower,
            Matrix<T>& zPower)
        {
            std::size_t twoXDegreeP1 = 2 * xDegree + 1;
            std::size_t twoYDegreeP1 = 2 * yDegree + 1;
            std::size_t twoZDegreeP1 = 2 * zDegree + 1;
            xPower.resize(observations.size(), twoXDegreeP1);
            yPower.resize(observations.size(), twoYDegreeP1);
            zPower.resize(observations.size(), twoZDegreeP1);
            for (std::size_t s = 0; s < observations.size(); ++s)
            {
                T const& x = observations[s][0];
                T const& y = observations[s][1];
                T const& z = observations[s][2];

                xPower(s, 0) = C_<T>(1);
                for (std::size_t j0 = 0, j1 = 1; j1 < twoXDegreeP1; j0 = j1++)
                {
                    xPower(s, j1) = x * xPower(s, j0);
                }

                yPower(s, 0) = C_<T>(1);
                for (std::size_t j0 = 0, j1 = 1; j1 < twoYDegreeP1; j0 = j1++)
                {
                    yPower(s, j1) = y * yPower(s, j0);
                }

                zPower(s, 0) = C_<T>(1);
                for (std::size_t j0 = 0, j1 = 1; j1 < twoZDegreeP1; j0 = j1++)
                {
                    zPower(s, j1) = z * zPower(s, j0);
                }
            }
        }

        // Matrix A is the Vandermonde matrix and vector B is the
        // right-hand side of the linear system A*X = B.
        static void ComputeLinearSystemComponents(
            std::size_t xDegree,
            std::size_t yDegree,
            std::size_t zDegree,
            std::vector<std::array<T, 4>> const& observations,
            Matrix<T> const& xPower,
            Matrix<T> const& yPower,
            Matrix<T> const& zPower,
            Matrix<T>& A,
            Vector<T>& B)
        {
            std::size_t xDegreeP1 = xDegree + 1;
            std::size_t yDegreeP1 = yDegree + 1;
            std::size_t zDegreeP1 = zDegree + 1;
            std::size_t numCoefficients = xDegreeP1 * yDegreeP1 * zDegreeP1;
            A.resize(numCoefficients, numCoefficients);
            B.resize(numCoefficients);
            for (std::size_t k0 = 0; k0 < zDegreeP1; ++k0)
            {
                for (std::size_t j0 = 0; j0 < yDegreeP1; ++j0)
                {
                    for (std::size_t i0 = 0; i0 < xDegreeP1; ++i0)
                    {
                        T sum = C_<T>(0);
                        std::size_t n0 = i0 + xDegreeP1 * (j0 + yDegreeP1 * k0);
                        for (std::size_t s = 0; s < observations.size(); ++s)
                        {
                            T const& w = observations[s][3];
                            sum += w * xPower(s, i0) * yPower(s, j0) * zPower(s, k0);
                        }

                        B[n0] = sum;

                        for (std::size_t k1 = 0; k1 < zDegreeP1; ++k1)
                        {
                            for (std::size_t j1 = 0; j1 < yDegreeP1; ++j1)
                            {
                                for (std::size_t i1 = 0; i1 < xDegreeP1; ++i1)
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
        }

        // Solve for the polynomial coefficients.
        static bool SolveLinearSystem(
            std::size_t xDegree,
            std::size_t yDegree,
            std::size_t zDegree,
            Matrix<T> const& A,
            Vector<T> const& B,
            Polynomial<T, 3>& polynomial)
        {
            std::size_t xDegreeP1 = xDegree + 1;
            std::size_t yDegreeP1 = yDegree + 1;
            std::size_t zDegreeP1 = zDegree + 1;
            std::size_t numCoefficients = xDegreeP1 * yDegreeP1 * zDegreeP1;
            Vector<T> coefficient(numCoefficients);
            if (LinearSystem<T>::Solve(A, B, coefficient))
            {
                polynomial.SetDegree(zDegree);
                for (std::size_t s = 0, i = 0; s < zDegreeP1; ++s)
                {
                    auto& pxy = polynomial[s];
                    pxy.SetDegree(yDegree);
                    for (std::size_t r = 0; r < yDegreeP1; ++r)
                    {
                        auto& px = pxy[r];
                        px.SetDegree(xDegree);
                        for (std::size_t c = 0; c < xDegreeP1; ++c, ++i)
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
        friend class UnitTestApprPolynomial3;
    };
}
