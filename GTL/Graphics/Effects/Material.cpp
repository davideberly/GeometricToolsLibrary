// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#include <GTLGraphicsPCH.h>
#include <GTL/Graphics/Effects/Material.h>
using namespace gtl;

Material::Material()
    :
    emissive{ 0.0f, 0.0f, 0.0f, 1.0f },
    ambient{ 0.0f, 0.0f, 0.0f, 1.0f },
    diffuse{ 0.0f, 0.0f, 0.0f, 1.0f },
    specular{ 0.0f, 0.0f, 0.0f, 1.0f }
{
}
