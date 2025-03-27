// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

#include <GTL/Mathematics/Primitives/ND/Hyperellipsoid.h>
#include <cmath>
#include <cstddef>

namespace gtl
{
    // Orthogonally project a hyperellipsoid onto a line. The projection
    // interval is [smin,smax] and corresponds to the line segment P + s * D,
    // where smin <= s <= smax.
    template <typename T, std::size_t N>
    void OrthogonalProject(Hyperellipsoid<T, N> const& hyperellipsoid,
        Vector<T, N> const& origin, Vector<T, N> const& direction, T& sMin, T& sMax)
    {
        // The center of the projection interval.
        T sCenter = Dot(direction, hyperellipsoid.center - origin);

        // Squared radius of projection interval.
        T sSqrRadius = C_<T>(0);
        for (std::size_t i = 0; i < N; ++i)
        {
            T dot = Dot(direction, hyperellipsoid.axis[i]);
            T increment = hyperellipsoid.extent[i] * dot;
            sSqrRadius += increment * increment;
        }

        T sRadius = std::sqrt(sSqrRadius);

        sMin = sCenter - sRadius;
        sMax = sCenter + sRadius;
    }
}
