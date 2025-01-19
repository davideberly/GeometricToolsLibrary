// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#include <GTLGraphicsPCH.h>
#include <GTL/Graphics/Resources/Buffers/IndirectArgumentsBuffer.h>
using namespace gtl;

IndirectArgumentsBuffer::IndirectArgumentsBuffer(uint32_t numElements, bool createStorage)
    :
    Buffer(numElements, 4, createStorage)
{
    mType = GT_INDIRECT_ARGUMENTS_BUFFER;
}
