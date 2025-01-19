// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#include <GTLGraphicsPCH.h>
#include <GTL/Graphics/Resources/Textures/Texture3.h>
using namespace gtl;

Texture3::Texture3(uint32_t format, uint32_t width, uint32_t height,
    uint32_t thickness, bool hasMipmaps, bool createStorage)
    :
    TextureSingle(format, 3, width, height, thickness, hasMipmaps, createStorage)
{
    mType = GT_TEXTURE3;
}
