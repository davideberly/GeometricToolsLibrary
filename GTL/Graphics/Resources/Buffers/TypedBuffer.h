// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#pragma once

// TypedBuffer is currently supported only in the DirectX graphics engine.

#include <GTL/Graphics/Resources/Buffers/Buffer.h>
#include <cstdint>

namespace gtl
{
    class TypedBuffer : public Buffer
    {
    public:
        // Abstract base class.
        virtual ~TypedBuffer() = default;
    protected:
        TypedBuffer(uint32_t numElements, size_t elementSize, bool createStorage = true);
    };
}
