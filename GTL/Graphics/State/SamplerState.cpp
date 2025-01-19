// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#include <GTLGraphicsPCH.h>
#include <GTL/Graphics/State/SamplerState.h>
using namespace gtl;

SamplerState::SamplerState()
    :
    filter(Filter::MIN_P_MAG_P_MIP_P),
    mode{ Mode::CLAMP, Mode::CLAMP, Mode::CLAMP },
    mipLODBias(0.0f),
    maxAnisotropy(1),
    comparison(Comparison::NEVER),
    borderColor{ 1.0f, 1.0f, 1.0f, 1.0f },
    minLOD(-std::numeric_limits<float>::max()),
    maxLOD(std::numeric_limits<float>::max())
{
    mType = GT_SAMPLER_STATE;
}
