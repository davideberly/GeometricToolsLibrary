// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#include <GTLGraphicsDX11PCH.h>
#include <GTL/Graphics/DX11/HLSL/HLSLTextureArray.h>
using namespace gtl;

HLSLTextureArray::HLSLTextureArray(D3D_SHADER_INPUT_BIND_DESC const& desc)
    :
    HLSLResource(desc, 0)
{
    Initialize(desc);
}

HLSLTextureArray::HLSLTextureArray(D3D_SHADER_INPUT_BIND_DESC const& desc,
    uint32_t index)
    :
    HLSLResource(desc, index, 0)
{
    Initialize(desc);
}

void HLSLTextureArray::Initialize(D3D_SHADER_INPUT_BIND_DESC const& desc)
{
    mNumComponents = ((desc.uFlags >> 2) + 1);

    switch (desc.Dimension)
    {
    case D3D_SRV_DIMENSION_TEXTURE1DARRAY:
        mNumDimensions = 1;
        break;
    case D3D_SRV_DIMENSION_TEXTURE2DARRAY:
    case D3D_SRV_DIMENSION_TEXTURE2DMSARRAY:
    case D3D_SRV_DIMENSION_TEXTURECUBE:
    case D3D_SRV_DIMENSION_TEXTURECUBEARRAY:
        mNumDimensions = 2;
        break;
    default:
        mNumDimensions = 0;
        break;
    }

    mGpuWritable = (desc.Type == D3D_SIT_UAV_RWTYPED);
}
