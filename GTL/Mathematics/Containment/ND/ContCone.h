// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.13

#pragma once

#include <GTL/Mathematics/Primitives/ND/Cone.h>

namespace gtl
{
    template <typename T, std::size_t N>
    class ContCone
    {
    public:
        // Test for containment of a point by a cone.
        static bool InContainer(Vector<T, N> const& point, Cone<T, N> const& cone)
        {
            Vector<T, N> diff = point - cone.vertex;
            T h = Dot(cone.direction, diff);
            return cone.HeightInRange(h) && h * h >= cone.cosAngleSqr * Dot(diff, diff);
        }

    private:
        friend class UnitTestContCone;
    };
}
