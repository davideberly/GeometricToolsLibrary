// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#include <GTLGraphicsPCH.h>
#include <GTL/Graphics/Resources/Textures/TextureCubeArray.h>
using namespace gtl;

TextureCubeArray::TextureCubeArray(uint32_t numCubes, uint32_t format,
    uint32_t length, bool hasMipmaps, bool createStorage)
    :
    TextureArray(cubeFaceCount* numCubes, format, 2, length, length, 1, hasMipmaps, createStorage),
    mNumCubes(numCubes)
{
    mType = GT_TEXTURE_CUBE_ARRAY;
}
