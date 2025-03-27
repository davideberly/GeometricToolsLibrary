// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

#include <GTL/Mathematics/Primitives/ND/Hyperellipsoid.h>

namespace gtl
{
    // Template alias to expose Ellipsoid3<T>. The name is more descriptive
    // than Hyperellipsoid3<T>.
    template <typename T> using Ellipsoid3 = Hyperellipsoid<T, 3>;
}
