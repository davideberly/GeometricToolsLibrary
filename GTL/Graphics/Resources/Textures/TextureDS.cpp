// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#include <GTLGraphicsPCH.h>
#include <GTL/Graphics/Resources/Textures/TextureDS.h>
using namespace gtl;

TextureDS::TextureDS(uint32_t format, uint32_t width, uint32_t height,
    bool createStorage)
    :
    Texture2(DataFormat::IsDepth(format) ? format : DF_D24_UNORM_S8_UINT,
        width, height, false, createStorage),
    mShaderInput(false)
{
    mType = GT_TEXTURE_DS;
}
