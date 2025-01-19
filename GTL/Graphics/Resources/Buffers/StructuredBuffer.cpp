// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#include <GTLGraphicsPCH.h>
#include <GTL/Graphics/Resources/Buffers/StructuredBuffer.h>
using namespace gtl;

StructuredBuffer::StructuredBuffer(uint32_t numElements, size_t elementSize, bool createStorage)
    :
    Buffer(numElements, elementSize, createStorage),
    mCounterType(CounterType::NONE),
    mKeepInternalCount(false)
{
    mType = GT_STRUCTURED_BUFFER;
}
