// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.19

#include <GTLGraphicsPCH.h>
#include <GTL/Graphics/Base/GEDrawTarget.h>
using namespace gtl;

GEDrawTarget::~GEDrawTarget()
{
}

GEDrawTarget::GEDrawTarget(DrawTarget const* gtTarget)
    :
    mTarget(const_cast<DrawTarget*>(gtTarget))  // conceptual constness
{
}
