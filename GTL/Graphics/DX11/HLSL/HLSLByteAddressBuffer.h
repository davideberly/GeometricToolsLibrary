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
    class HLSLByteAddressBuffer : public HLSLResource
    {
    public:
        // Construction and destruction.
        virtual ~HLSLByteAddressBuffer() = default;

        HLSLByteAddressBuffer(D3D_SHADER_INPUT_BIND_DESC const& desc);
        HLSLByteAddressBuffer(D3D_SHADER_INPUT_BIND_DESC const& desc, uint32_t index);

        // Member access.
        inline bool IsGpuWritable() const
        {
            return mGpuWritable;
        }

    private:
        bool mGpuWritable;
    };
}
