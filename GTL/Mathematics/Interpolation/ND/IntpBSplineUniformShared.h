// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.03.23

#pragma once

#include <GTL/Utility/Exceptions.h>
#include <GTL/Mathematics/Algebra/Polynomial.h>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <vector>

namespace gtl
{
    template <typename T>
    class IntpBSplineUniformShared
    {
    public:
        // Support for caching the intermediate tensor product of control
        // points with the blending matrices. A precached container has all
        // elements precomputed before any Evaluate(...) calls. The 'bool'
        // flags are all set to 'true'. A cached container fills the elements
        // on demand. The 'bool' flags are initially 'false', indicating the
        // EvalType component has not yet been computed. After it is computed
        // and stored, the flag is set to 'true'.
        static std::uint32_t constexpr NO_CACHING = 0;
        static std::uint32_t constexpr PRE_CACHING = 1;
        static std::uint32_t constexpr ON_DEMAND_CACHING = 2;
        static std::uint32_t constexpr NUM_CACHING_MODES = 3;

    protected:
        static void ComputeBlendingMatrix(std::size_t degree, std::vector<T>& A)
        {
            GTL_ARGUMENT_ASSERT(
                degree >= 1,
                "The degree must be positive.");

            T const zero = C_<T>(0);
            T const one = C_<T>(1);
            T const negOne = C_<T>(-1);

            std::size_t const degreeP1 = degree + 1;
            A.resize(degreeP1 * degreeP1);

            // P_{0,0}(s)
            std::vector<Polynomial1<T>> P(degreeP1);
            P[0][0] = one;

            // L0 = s/j
            Polynomial1<T> L0(1);
            L0[0] = zero;

            // L1(s) = (j + 1 - s)/j
            Polynomial1<T> L1(1);

            // s-1 is used in computing translated P_{j-1,k-1}(s-1)
            Polynomial1<T> sm1 = { negOne, one };

            // Compute
            //   P_{j,k}(s) = L0(s)*P_{j-1,k}(s) + L1(s)*P_{j-1,k-1}(s-1)
            // for 0 <= k <= j where 1 <= j <= degree. When k = 0,
            // P_{j-1,-1}(s) = 0, so P_{j,0}(s) = L0(s)*P_{j-1,0}(s). When
            // k = j, P_{j-1,j}(s) = 0, so P_{j,j}(s) = L1(s)*P_{j-1,j-1}(s).
            // The polynomials at level j-1 are currently stored in P[0]
            // through P[j-1]. The polynomials at level j are computed and
            // stored in P[0] through P[j]; that is, they are computed in
            // place to reduce memory usage and copying. This requires
            // computing P[k] (level j) from P[k] (level j-1) and P[k-1]
            // (level j-1), which means we have to process k = j down to
            // k = 0.
            for (std::size_t j = 1; j <= degree; ++j)
            {
                T invJ = one / static_cast<T>(j);
                L0[1] = invJ;
                L1[0] = one + invJ;
                L1[1] = -invJ;

                for (std::size_t c = 0, k = j; c <= j; ++c, --k)
                {
                    Polynomial1<T> result = { zero };

                    if (k > 0)
                    {
                        result += L1 * GetTranslation(P[k - 1], one);
                    }

                    if (k < j)
                    {
                        result += L0 * P[k];
                    }

                    P[k] = result;
                }
            }

            // Compute Q_{d,k}(s) = P_{d,k}(s + k).
            std::vector<Polynomial1<T>> Q(degreeP1);
            for (std::size_t k = 0; k <= degree; ++k)
            {
                T translate = static_cast<T>(k);
                Q[k] = GetTranslation(P[k], -translate);
            }

            // Extract the matrix A from the Q-polynomials. Row r of A
            // contains the coefficients of Q_{d,d-r}(s).
            for (std::size_t k = 0, row = degree; k <= degree; ++k, --row)
            {
                for (std::size_t col = 0; col <= degree; ++col)
                {
                    A[col + degreeP1 * row] = Q[k][col];
                }
            }
        }

        // Compute the coefficients for the derivative polynomial terms.
        static void ComputeDCoefficients(std::size_t degree, std::vector<T>& dCoefficients,
            std::vector<std::size_t>& ellMax)
        {
            std::size_t numDCoefficients = (degree + 1) * (degree + 2) / 2;
            dCoefficients.resize(numDCoefficients);
            for (std::size_t i = 0; i < numDCoefficients; ++i)
            {
                dCoefficients[i] = 1;
            }

            for (std::size_t order = 1, col0 = 0, col1 = degree + 1; order <= degree; ++order)
            {
                ++col0;
                for (std::size_t c = order, m = 1; c <= degree; ++c, ++m, ++col0, ++col1)
                {
                    dCoefficients[col1] = dCoefficients[col0] * static_cast<T>(m);
                }
            }

            ellMax.resize(degree + 1);
            ellMax[0] = degree;
            for (std::size_t i0 = 0, i1 = 1; i1 <= degree; i0 = i1++)
            {
                ellMax[i1] = ellMax[i0] + degree - i0;
            }
        }

        // Compute powers of ds/dt.
        static void ComputePowers(std::size_t degree, std::size_t numControls,
            T const& tmin, T const& tmax, std::vector<T>& powerDSDT)
        {
            T dsdt = (static_cast<T>(numControls) - static_cast<T>(degree)) / (tmax - tmin);
            powerDSDT.resize(degree + 1);
            powerDSDT[0] = static_cast<T>(1);
            powerDSDT[1] = dsdt;
            for (std::size_t i = 2, im1 = 1; i <= degree; ++i, ++im1)
            {
                powerDSDT[i] = powerDSDT[im1] * dsdt;
            }
        }

        // Determine the interval [index,index+1) corresponding to the
        // specified value of t and compute u in that interval.
        static void GetKey(T const& t, T const& tmin, T const& tmax, T const& dsdt,
            std::size_t numControls, std::size_t degree, std::size_t& index, T& u)
        {
            // Compute s - d = ((c + 1 - d)/(c + 1))(t + 1/2), the index for
            // which d + index <= s < d + index + 1. Let u = s - d - index so
            // that 0 <= u < 1.
            if (t > tmin)
            {
                if (t < tmax)
                {
                    T smd = dsdt * (t - tmin);
                    index = static_cast<std::size_t>(std::floor(smd));
                    u = smd - static_cast<T>(index);
                }
                else
                {
                    // In the evaluation, s = c + 1 - d and i = c - d. This
                    // causes s-d-i to be 1 in G_c(c+1-d). Effectively, the
                    // selection of i extends the s-domain [d,c+1) to its
                    // support [d,c+1].
                    index = numControls - 1 - degree;
                    u = static_cast<T>(1);
                }
            }
            else
            {
                index = 0;
                u = static_cast<T>(0);
            }
        }

    private:
        friend class UnitTestBSplineUniformShared;
    };
}
