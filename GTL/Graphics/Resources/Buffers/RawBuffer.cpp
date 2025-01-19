// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#include <GTLGraphicsPCH.h>
#include <GTL/Graphics/Resources/Buffers/RawBuffer.h>
using namespace gtl;

RawBuffer::RawBuffer(uint32_t numElements, bool createStorage)
    :
    Buffer(numElements, 4, createStorage)
{
    mType = GT_RAW_BUFFER;
}
