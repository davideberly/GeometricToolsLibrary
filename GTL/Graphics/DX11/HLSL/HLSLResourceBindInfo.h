// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#pragma once

#include <GTL/Graphics/DX11/HLSL/HLSLBaseBuffer.h>

namespace gtl
{
    class HLSLResourceBindInfo : public HLSLBaseBuffer
    {
    public:
        // Construction and destruction.
        virtual ~HLSLResourceBindInfo() = default;

        HLSLResourceBindInfo(D3D_SHADER_INPUT_BIND_DESC const& desc,
            uint32_t numBytes, std::vector<Member> const& members);

        HLSLResourceBindInfo(D3D_SHADER_INPUT_BIND_DESC const& desc,
            uint32_t index, uint32_t numBytes,
            std::vector<Member> const& members);
    };
}
