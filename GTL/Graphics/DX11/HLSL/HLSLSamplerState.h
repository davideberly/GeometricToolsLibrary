// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#pragma once

#include <GTL/Graphics/DX11/HLSL/HLSLResource.h>

namespace gtl
{
    class HLSLSamplerState : public HLSLResource
    {
    public:
        // Construction and destruction.
        virtual ~HLSLSamplerState() = default;

        HLSLSamplerState(D3D_SHADER_INPUT_BIND_DESC const& desc);
        HLSLSamplerState(D3D_SHADER_INPUT_BIND_DESC const& desc, uint32_t index);
    };
}
