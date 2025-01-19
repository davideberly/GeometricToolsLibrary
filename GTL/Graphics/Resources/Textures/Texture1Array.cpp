// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#include <GTLGraphicsPCH.h>
#include <GTL/Graphics/Resources/Textures/Texture1Array.h>
using namespace gtl;

Texture1Array::Texture1Array(uint32_t numItems, uint32_t format,
    uint32_t length, bool hasMipmaps, bool createStorage)
    :
    TextureArray(numItems, format, 1, length, 1, 1, hasMipmaps, createStorage)
{
    mType = GT_TEXTURE1_ARRAY;
}
