// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

#include <GTL/Mathematics/RootFinders/PolynomialRoot.h>
#include <type_traits>

// Compute the real-valued root of a linear polynomial with real-valued
// coefficients. The general linear polynomial is g(x) = g0 + g1 * x and the
// monic linear polynomial is m(x) = m0 + x.

namespace gtl
{
    template <typename T>
    class RootsLinear
    {
    public:
        // Solve for the roots using a mixture of rational arithmetic and
        // floating-point arithmetic. The roots[] array must have at least 1
        // element. The returned std::size_t is the number of valid roots in the
        // roots[] array (0 or 1).
        using Rational = BSRational<UIntegerAP32>;

        // Solve the general polynomial g0 + g1*x = 0.
        static std::size_t Solve(
            T const& g0, T const& g1,
            PolynomialRoot<T>* roots)
        {
            static_assert(
                std::is_floating_point<T>::value || std::is_same<T, Rational>::value,
                "Type T must be 'float', 'double', or 'Rational' ");

            // Test whether the degree is smaller than 2.
            T const zero = static_cast<T>(0);
            if (g1 == zero)
            {
                // The solution set is either all real-valued x (g0 = 0) or no
                // solution (g0 != 0). In either case, report no roots.
                return 0;
            }

            // Test for zero-valued roots.
            if (g0 == zero)
            {
                roots[0] = { zero, 1 };
                return 1;
            }

            // At this time g0 and g1 are not zero.
            roots[0] = { -g0 / g1, 1 };
            return 1;
        }

        // Solve the monic polynomial m0 + x = 0.
        static std::size_t Solve(
            T const& m0,
            PolynomialRoot<T>* roots)
        {
            static_assert(
                std::is_floating_point<T>::value || std::is_same<T, Rational>::value,
                "Type T must be 'float', 'double', or 'Rational' ");

            roots[0] = { -m0, 1 };
            return 1;
        }
    };
}
