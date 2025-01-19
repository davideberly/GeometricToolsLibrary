// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#pragma once

#include <GTL/Mathematics/Algebra/Vector.h>

namespace gtl
{
    struct LightCameraGeometry
    {
        // Construction.
        LightCameraGeometry();

        Vector4<float> lightModelPosition;      // default: (0,0,0,1)
        Vector4<float> lightModelDirection;     // default: (0,0,-1,0)
        Vector4<float> lightModelUp;            // default: (0,1,0,0)
        Vector4<float> lightModelRight;         // default: (1,0,0,0)

        Vector4<float> cameraModelPosition;     // default: (0,0,0,1)
        Vector4<float> cameraModelDirection;    // default: (0,0,-1,0)
        Vector4<float> cameraModelUp;           // default: (0,1,0,0)
        Vector4<float> cameraModelRight;        // default: (1,0,0,0)
    };
}
