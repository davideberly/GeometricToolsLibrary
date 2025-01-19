// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#include <GTLGraphicsPCH.h>
#include <GTL/Graphics/Resources/Textures/Texture2Array.h>
using namespace gtl;

Texture2Array::Texture2Array(uint32_t numItems, uint32_t format,
    uint32_t width, uint32_t height, bool hasMipmaps,
    bool createStorage)
    :
    TextureArray(numItems, format, 2, width, height, 1, hasMipmaps, createStorage)
{
    mType = GT_TEXTURE2_ARRAY;
}
