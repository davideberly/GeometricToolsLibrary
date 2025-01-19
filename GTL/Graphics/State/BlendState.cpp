// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#include <GTLGraphicsPCH.h>
#include <GTL/Graphics/State/BlendState.h>
using namespace gtl;

BlendState::BlendState()
    :
    enableAlphaToCoverage(false),
    enableIndependentBlend(false),
    target{},
    blendColor{ 0.0f, 0.0f, 0.0f, 0.0f },
    sampleMask(0xFFFFFFFFu)
{
    mType = GT_BLEND_STATE;
}
