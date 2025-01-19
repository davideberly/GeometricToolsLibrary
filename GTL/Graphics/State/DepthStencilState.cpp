// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#include <GTLGraphicsPCH.h>
#include <GTL/Graphics/State/DepthStencilState.h>
using namespace gtl;

DepthStencilState::DepthStencilState()
    :
    depthEnable(true),
    writeMask(WriteMask::ALL),
    comparison(Comparison::LESS_EQUAL),
    stencilEnable(false),
    stencilReadMask(0xFF),
    stencilWriteMask(0xFF),
    reference(0)
{
    mType = GT_DEPTH_STENCIL_STATE;
}
