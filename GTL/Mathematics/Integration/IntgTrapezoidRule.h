// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

// An implementation of the Trapezoid Rule for integration. It is a simple
// algorithm, but slow to converge as the number of samples is increased. The
// number of samples needs to be two or larger.

#include <GTL/Mathematics/Arithmetic/Constants.h>
#include <GTL/Utility/Exceptions.h>
#include <cstddef>
#include <functional>

namespace gtl
{
    template <typename T>
    class IntgTrapezoidRule
    {
    public:
        static T Integrate(std::size_t numSamples, T const& a, T const& b,
            std::function<T(T)> const& integrand)
        {
            GTL_ARGUMENT_ASSERT(
                numSamples >= 2,
                "At least 2 samples are required.");

            T h = (b - a) / static_cast<T>(numSamples - 1);
            T result = C_<T>(1, 2) * (integrand(a) + integrand(b));
            for (std::size_t i = 1; i + 2 <= numSamples; ++i)
            {
                result += integrand(a + static_cast<T>(i) * h);
            }
            result *= h;
            return result;
        }

    private:
        friend class UnitTestIntgTrapezoidRule;
    };
}
