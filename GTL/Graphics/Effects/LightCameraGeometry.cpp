// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#include <GTLGraphicsPCH.h>
#include <GTL/Graphics/Effects/LightCameraGeometry.h>
using namespace gtl;

LightCameraGeometry::LightCameraGeometry()
    :
    lightModelPosition{ 0.0f, 0.0f, 0.0f, 1.0f },
    lightModelDirection{ 0.0f, 0.0f, -1.0f, 0.0f },
    lightModelUp{ 0.0f, 1.0f, 0.0f, 0.0f },
    lightModelRight{ 1.0f, 0.0f, 0.0f, 0.0f },
    cameraModelPosition{ 0.0f, 0.0f, 0.0f, 1.0f },
    cameraModelDirection{ 0.0f, 0.0f, -1.0f, 0.0f },
    cameraModelUp{ 0.0f, 1.0f, 0.0f, 0.0f },
    cameraModelRight{ 1.0f, 0.0f, 0.0f, 0.0f }
{
}
