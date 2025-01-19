// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#pragma once

#include <GTL/Graphics/DX11/HLSL/HLSLResource.h>
#include <cstdint>

namespace gtl
{
    class HLSLStructuredBuffer : public HLSLResource
    {
    public:
        enum Type
        {
            SBT_INVALID,
            SBT_BASIC,
            SBT_APPEND,
            SBT_CONSUME,
            SBT_COUNTER
        };

        // Construction and destruction.
        virtual ~HLSLStructuredBuffer() = default;

        HLSLStructuredBuffer(D3D_SHADER_INPUT_BIND_DESC const& desc);
        HLSLStructuredBuffer(D3D_SHADER_INPUT_BIND_DESC const& desc, uint32_t index);

        // Member access.
        inline HLSLStructuredBuffer::Type GetType() const
        {
            return mType;
        }

        inline bool IsGpuWritable() const
        {
            return mGpuWritable;
        }

    private:
        void Initialize(D3D_SHADER_INPUT_BIND_DESC const& desc);

        Type mType;
        bool mGpuWritable;
    };
}
