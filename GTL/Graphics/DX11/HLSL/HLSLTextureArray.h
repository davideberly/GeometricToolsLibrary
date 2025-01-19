// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#pragma once

#include <GTL/Graphics/DX11/HLSL/HLSLResource.h>

namespace gtl
{
    class HLSLTextureArray : public HLSLResource
    {
    public:
        // Construction and destruction.
        virtual ~HLSLTextureArray() = default;

        HLSLTextureArray(D3D_SHADER_INPUT_BIND_DESC const& desc);
        HLSLTextureArray(D3D_SHADER_INPUT_BIND_DESC const& desc, uint32_t index);

        // Member access.
        inline uint32_t GetNumComponents() const
        {
            return mNumComponents;
        }

        inline uint32_t GetNumDimensions() const
        {
            return mNumDimensions;
        }

        inline bool IsGpuWritable() const
        {
            return mGpuWritable;
        }

    private:
        void Initialize(D3D_SHADER_INPUT_BIND_DESC const& desc);

        uint32_t mNumComponents;
        uint32_t mNumDimensions;
        bool mGpuWritable;
    };
}
