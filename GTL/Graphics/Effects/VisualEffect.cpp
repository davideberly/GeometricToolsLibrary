// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#include <GTLGraphicsPCH.h>
#include <GTL/Graphics/Effects/VisualEffect.h>
using namespace gtl;

VisualEffect::VisualEffect()
{
    mPVWMatrixConstant = std::make_shared<ConstantBuffer>(sizeof(Matrix4x4<float>), true);
    SetPVWMatrix(Matrix4x4<float>::Identity());
}

VisualEffect::VisualEffect(std::shared_ptr<VisualProgram> const& program)
    :
    mProgram(program)
{
    mPVWMatrixConstant = std::make_shared<ConstantBuffer>(sizeof(Matrix4x4<float>), true);
    SetPVWMatrix(Matrix4x4<float>::Identity());
}

void VisualEffect::SetPVWMatrixConstant(std::shared_ptr<ConstantBuffer> const& buffer)
{
    mPVWMatrixConstant = buffer;
    SetPVWMatrix(Matrix4x4<float>::Identity());
}
