// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#pragma once

#include <GTL/Graphics/Resources/Textures/Texture.h>
#include <GTL/Graphics/DX11/Resources/DX11Resource.h>
#include <cstdint>

namespace gtl
{
    class DX11Texture : public DX11Resource
    {
    public:
        // Abstract base class.
        virtual ~DX11Texture();
    protected:
        DX11Texture(Texture const* gtTexture);

    public:
        // Member access.
        inline Texture* GetTexture() const
        {
            return static_cast<Texture*>(mGTObject);
        }

        inline ID3D11ShaderResourceView* GetSRView() const
        {
            return mSRView;
        }

        inline ID3D11UnorderedAccessView* GetUAView() const
        {
            return mUAView;
        }

        // Copy of data between CPU and GPU.
        virtual bool Update(ID3D11DeviceContext* context, uint32_t sri) override;
        virtual bool Update(ID3D11DeviceContext* context) override;
        virtual bool CopyCpuToGpu(ID3D11DeviceContext* context, uint32_t sri) override;
        virtual bool CopyCpuToGpu(ID3D11DeviceContext* context) override;
        virtual bool CopyGpuToCpu(ID3D11DeviceContext* context, uint32_t sri) override;
        virtual bool CopyGpuToCpu(ID3D11DeviceContext* context) override;
        virtual void CopyGpuToGpu(ID3D11DeviceContext* context,
            ID3D11Resource* target, uint32_t sri) override;
        virtual void CopyGpuToGpu(ID3D11DeviceContext* context,
            ID3D11Resource* target) override;

        // Support for the DX11 debug layer; see comments in the file
        // DX11GraphicsObject.h about usage.
        virtual void SetName(std::string const& name) override;

    protected:
        // Support for copy of row-pitched and slice-pitched (noncontiguous)
        // texture memory.
        static void CopyPitched2(uint32_t numRows, uint32_t srcRowPitch,
            void const* srcData, uint32_t trgRowPitch, void* trgData);

        static void CopyPitched3(uint32_t numRows, uint32_t numSlices,
            uint32_t srcRowPitch, uint32_t srcSlicePitch,
            void const* srcData, uint32_t trgRowPitch,
            uint32_t trgSlicePitch, void* trgData);

        ID3D11ShaderResourceView* mSRView;
        ID3D11UnorderedAccessView* mUAView;
    };
}
