// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.12

#pragma once

// Documentation for this class is
// https://www.geometrictools.com/Documentation/GTLUtility.pdf#HashCombine

#include <cstddef>
#include <functional>

namespace gtl
{
    template <typename T>
    inline void HashCombine(std::size_t& seed, T const& value)
    {
        seed ^= std::hash<T>()(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    // Functions to create a hash value using a seed.
    template <typename T>
    inline void HashValue(std::size_t& seed, T const& value)
    {
        HashCombine(seed, value);
    }

    template <typename T, typename... Tail>
    inline void HashValue(std::size_t& seed, T const& value, Tail const&... arguments)
    {
        HashCombine(seed, value);
        HashValue(seed, arguments...);
    }

    // Functions to create a hash value from a list of arguments.
    template <typename... Tail>
    inline std::size_t HashValue(Tail const&... arguments)
    {
        std::size_t seed = 0;
        HashValue(seed, arguments...);
        return seed;
    }
}
