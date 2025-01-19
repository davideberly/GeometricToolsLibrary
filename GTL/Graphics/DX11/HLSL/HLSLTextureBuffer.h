// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#pragma once

#include <GTL/Graphics/DX11/HLSL/HLSLBaseBuffer.h>

namespace gtl
{
    class HLSLTextureBuffer : public HLSLBaseBuffer
    {
    public:
        // Construction and destruction.
        virtual ~HLSLTextureBuffer() = default;

        HLSLTextureBuffer(D3D_SHADER_INPUT_BIND_DESC const& desc,
            uint32_t numBytes, std::vector<Member> const& members);

        HLSLTextureBuffer(D3D_SHADER_INPUT_BIND_DESC const& desc,
            uint32_t index, uint32_t numBytes,
            std::vector<Member> const& members);
    };
}
