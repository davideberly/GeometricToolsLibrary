// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#pragma once

// RawBuffer is currently supported only in the DirectX graphics engine.

#include <GTL/Graphics/Resources/Buffers/Buffer.h>
#include <cstdint>

namespace gtl
{
    class RawBuffer : public Buffer
    {
    public:
        // Construction.  The element size is always 4 bytes.
        RawBuffer(uint32_t numElements, bool createStorage = true);

    public:
        // For use by the Shader class for storing reflection information.
        static int32_t const shaderDataLookup = 3;
    };
}
