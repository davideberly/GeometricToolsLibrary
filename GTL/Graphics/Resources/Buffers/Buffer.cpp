// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#include <GTLGraphicsPCH.h>
#include <GTL/Graphics/Resources/Buffers/Buffer.h>
using namespace gtl;

Buffer::Buffer(uint32_t numElements, size_t elementSize, bool createStorage)
    :
    Resource(numElements, elementSize, createStorage)
{
    mType = GT_BUFFER;
}
