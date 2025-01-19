// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#include <GTLGraphicsPCH.h>
#include <GTL/Graphics/State/RasterizerState.h>
using namespace gtl;

RasterizerState::RasterizerState()
    :
    fill(Fill::SOLID),
    cull(Cull::BACK),
    frontCCW(true),
    depthBias(0),
    depthBiasClamp(0.0f),
    slopeScaledDepthBias(0.0f),
    enableDepthClip(true),
    enableScissor(false),
    enableMultisample(false),
    enableAntialiasedLine(false)
{
    mType = GT_RASTERIZER_STATE;
}
