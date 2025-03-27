// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

#include <GTL/Mathematics/Primitives/2D/Circle2.h>
#include <GTL/Mathematics/MatrixAnalysis/LinearSystem.h>
#include <cmath>

namespace gtl
{
    // All functions return 'true' if the circle has been constructed, 'false'
    // otherwise (input points are linearly dependent).

    // Circle circumscribing triangle.
    template <typename T>
    bool Circumscribe(Vector2<T> const& v0, Vector2<T> const& v1,
        Vector2<T> const& v2, Circle2<T>& circle)
    {
        Vector2<T> e10 = v1 - v0;
        Vector2<T> e20 = v2 - v0;
        Matrix2x2<T> A{ { e10[0], e10[1] }, { e20[0], e20[1] } };
        Vector2<T> B{ C_<T>(1, 2) * Dot(e10, e10), C_<T>(1, 2) * Dot(e20, e20) };
        Vector2<T> solution{};
        if (LinearSystem<T>::Solve(A, B, solution))
        {
            circle.center = v0 + solution;
            circle.radius = Length(solution);
            return true;
        }
        return false;
    }

    // Circle inscribing triangle.
    template <typename T>
    bool Inscribe(Vector2<T> const& v0, Vector2<T> const& v1,
        Vector2<T> const& v2, Circle2<T>& circle)
    {
        Vector2<T> d10 = v1 - v0;
        Vector2<T> d20 = v2 - v0;
        Vector2<T> d21 = v2 - v1;
        T len10 = Length(d10);
        T len20 = Length(d20);
        T len21 = Length(d21);
        T perimeter = len10 + len20 + len21;
        if (perimeter > C_<T>(0))
        {
            len10 /= perimeter;
            len20 /= perimeter;
            len21 /= perimeter;
            circle.center = len21 * v0 + len20 * v1 + len10 * v2;
            circle.radius = std::fabs(DotPerp(d10, d20)) / perimeter;
            return circle.radius > C_<T>(0);
        }
        return false;
    }
}
