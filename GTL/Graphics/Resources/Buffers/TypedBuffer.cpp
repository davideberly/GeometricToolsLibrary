// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#include <GTLGraphicsPCH.h>
#include <GTL/Graphics/Resources/Buffers/TypedBuffer.h>
using namespace gtl;

TypedBuffer::TypedBuffer(uint32_t numElements, size_t elementSize, bool createStorage)
    :
    Buffer(numElements, elementSize, createStorage)
{
    mType = GT_TYPED_BUFFER;
}
