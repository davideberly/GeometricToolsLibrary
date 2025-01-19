// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#include <GTLGraphicsPCH.h>
#include <GTL/Mathematics/Arithmetic/Constants.h>
#include <GTL/Graphics/Effects/Lighting.h>
using namespace gtl;

Lighting::Lighting()
    :
    ambient{ 1.0f, 1.0f, 1.0f, 1.0f },
    diffuse{ 1.0f, 1.0f, 1.0f, 1.0f },
    specular{ 1.0f, 1.0f, 1.0f, 1.0f },
    spotCutoff{ C_PI_DIV_2<float>, 0.0f, 1.0f, 1.0f },
    attenuation{ 1.0f, 0.0f, 0.0f, 1.0f }
{
}
