// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#include <GTLGraphicsPCH.h>
#include <GTL/Graphics/SceneGraph/Hierarchy/Light.h>
using namespace gtl;

Light::Light(bool isPerspective, bool isDepthRangeZeroOne)
    :
    ViewVolume(isPerspective, isDepthRangeZeroOne)
{
}
