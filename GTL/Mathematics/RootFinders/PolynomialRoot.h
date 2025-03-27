// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

// Support for computing roots of polynomials of degrees 1, 2, 3, or 4.

#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <type_traits>

namespace gtl
{
    template <typename T>
    struct PolynomialRoot
    {
        PolynomialRoot()
            :
            x(C_<T>(0)),
            m(0)
        {
        }

        PolynomialRoot(T const& inX, std::size_t inM)
            :
            x(inX),
            m(inM)
        {
        }

        bool operator==(PolynomialRoot& other) const
        {
            return x == other.x;
        }

        bool operator<(PolynomialRoot& other) const
        {
            return x < other.x;
        }

        // x is the root estimate)and m is the multiplicity of x. The
        // object is invalid when m is 0.
        T x;
        std::size_t m;
    };

    // Compute a tight interval [xMin,xMax] for a root to the polynomial F(x).
    // The inputs signFMin and signFMax are in {-1,1} and are the theoretical
    // signs of F(xMin) and F(xMax) for the initial xMin and xMax. They are
    // required to have opposite signs. Bisection is performed using
    // floating-point arithmetic for speed.
    template <typename T>
    static void PolynomialRootBisect(std::function<T(T)> const& F,
        std::int32_t signFMin, std::int32_t signFMax, T& xMin, T& xMax)
    {
        static_assert(
            std::is_floating_point<T>::value,
            "Type T must be 'float' or 'double'");

        T const zero = static_cast<T>(0);
        T fMin = F(xMin);
        std::int32_t trueSignFMin = (fMin > zero ? +1 : (fMin < zero ? -1 : 0));
        if (trueSignFMin == signFMin)
        {
            T fMax = F(xMax);
            std::int32_t trueSignFMax = (fMax > zero ? +1 : (fMax < zero ? -1 : 0));
            if (trueSignFMax == signFMax)
            {
                // The signs are correct for bisection. The iteration
                // algorithm terminates when the function value at the
                // midpoint is 0. Or it terminates when the midpoint of
                // the current interval equals one of the interval
                // endpoints, at which time the interval endpoints are
                // consecutive floating-point numbers. The upper bound
                // maxBisections is sufficiently large to ensure ensure
                // the loop terminates, but the typical number of
                // iterations is much smaller.
                std::size_t constexpr maxBisections = 4096;
                for (std::size_t iteration = 1; iteration < maxBisections; ++iteration)
                {
                    T x = static_cast<T>(0.5) * (xMin + xMax);
                    T f = F(x);

                    if (x == xMin || x == xMax)
                    {
                        // The floating-point numbers xMin and xMax are
                        // consecutive in which case subdivision cannot
                        // produce a floating-point number between them.
                        // Return the bounding interval to the caller for
                        // further processing.
                        return;
                    }

                    std::int32_t signF = (f > zero ? 1 : (f < zero ? -1 : 0));
                    if (signF == 0)
                    {
                        // The function is exactly zero and a root is found.
                        xMin = x;
                        xMax = x;
                        return;
                    }

                    // Update the correct endpoint to the midpoint.
                    if (signF == signFMin)
                    {
                        xMin = x;
                    }
                    else // signF == signFMax
                    {
                        xMax = x;
                    }
                }
            }
            else
            {
                // Floating-point rounding errors prevent the correct
                // classification of the multiplicity of roots.
                xMin = xMax;
            }
        }
        else
        {
            // Floating-point rounding errors prevent the correct
            // classification of the multiplicity of roots.
            xMax = xMin;
        }
    }
}
