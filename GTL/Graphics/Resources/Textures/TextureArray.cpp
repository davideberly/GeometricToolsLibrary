// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#include <GTLGraphicsPCH.h>
#include <GTL/Graphics/Resources/Textures/TextureArray.h>
using namespace gtl;

TextureArray::TextureArray(uint32_t numItems, uint32_t format,
    uint32_t numDimensions, uint32_t dim0, uint32_t dim1,
    uint32_t dim2, bool hasMipmaps, bool createStorage)
    :
    Texture(numItems, format, numDimensions, dim0, dim1, dim2, hasMipmaps, createStorage)
{
    mType = GT_TEXTURE_ARRAY;
}
