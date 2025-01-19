// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#include <GTLGraphicsPCH.h>
#include <GTL/Graphics/Resources/Textures/Texture1.h>
using namespace gtl;

Texture1::Texture1(uint32_t format, uint32_t length, bool hasMipmaps, bool createStorage)
    :
    TextureSingle(format, 1, length, 1, 1, hasMipmaps, createStorage)
{
    mType = GT_TEXTURE1;
}
