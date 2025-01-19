// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#pragma once

#include <GTL/Graphics/Resources/Buffers/RawBuffer.h>
#include <GTL/Graphics/DX11/Resources/Buffers/DX11Buffer.h>

namespace gtl
{
    class DX11RawBuffer : public DX11Buffer
    {
    public:
        // Construction and destruction.
        virtual ~DX11RawBuffer();
        DX11RawBuffer(ID3D11Device* device, RawBuffer const* rbuffer);
        static std::shared_ptr<GEObject> Create(void* device, GraphicsObject const* object);

        // Member access.
        inline RawBuffer* GetRawBuffer() const
        {
            return static_cast<RawBuffer*>(mGTObject);
        }

        inline ID3D11ShaderResourceView* GetSRView() const
        {
            return mSRView;
        }

        inline ID3D11UnorderedAccessView* GetUAView() const
        {
            return mUAView;
        }

        // Support for the DX11 debug layer; see comments in the file
        // DX11GraphicsObject.h about usage.
        virtual void SetName(std::string const& name);

    private:
        // Support for construction.
        void CreateSRView(ID3D11Device* device);
        void CreateUAView(ID3D11Device* device);

        ID3D11ShaderResourceView* mSRView;
        ID3D11UnorderedAccessView* mUAView;
    };
}
