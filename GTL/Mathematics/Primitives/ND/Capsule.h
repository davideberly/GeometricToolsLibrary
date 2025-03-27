// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

// A sphere-swept segment is the set of points that are equidistant from a
// segment, the common distance called the radius.

#include <GTL/Mathematics/Primitives/ND/Segment.h>
#include <cstddef>

namespace gtl
{
    template <typename T, std::size_t N>
    class Capsule
    {
    public:
        Capsule()
            :
            segment{},
            radius(C_<T>(0))
        {
        }

        Capsule(Segment<T, N> const& inSegment, T const& inRadius)
            :
            segment(inSegment),
            radius(inRadius)
        {
        }

        Segment<T, N> segment;
        T radius;

    private:
        friend class UnitTestCapsule;
    };

    // Comparisons to support sorted containers.
    template <typename T, std::size_t N>
    bool operator==(Capsule<T, N> const& sss0, Capsule<T, N> const& sss1)
    {
        return sss0.segment == sss1.segment
            && sss0.radius == sss1.radius;
    }

    template <typename T, std::size_t N>
    bool operator!=(Capsule<T, N> const& sss0, Capsule<T, N> const& sss1)
    {
        return !operator==(sss0, sss1);
    }

    template <typename T, std::size_t N>
    bool operator<(Capsule<T, N> const& sss0, Capsule<T, N> const& sss1)
    {
        if (sss0.segment < sss1.segment)
        {
            return true;
        }

        if (sss0.segment > sss1.segment)
        {
            return false;
        }

        return sss0.radius < sss1.radius;
    }

    template <typename T, std::size_t N>
    bool operator<=(Capsule<T, N> const& sss0, Capsule<T, N> const& sss1)
    {
        return !operator<(sss1, sss0);
    }

    template <typename T, std::size_t N>
    bool operator>(Capsule<T, N> const& sss0, Capsule<T, N> const& sss1)
    {
        return operator<(sss1, sss0);
    }

    template <typename T, std::size_t N>
    bool operator>=(Capsule<T, N> const& sss0, Capsule<T, N> const& sss1)
    {
        return !operator<(sss0, sss1);
    }

    // Template alias for convenience.
    template <typename T> using Capsule3 = Capsule<T, 3>;
}
