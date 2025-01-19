// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#pragma once

#include <GTL/Graphics/Resources/Buffers/StructuredBuffer.h>
#include <GTL/Graphics/DX11/Resources/Buffers/DX11Buffer.h>

namespace gtl
{
    class DX11StructuredBuffer : public DX11Buffer
    {
    public:
        // Construction and destruction.
        virtual ~DX11StructuredBuffer();
        DX11StructuredBuffer(ID3D11Device* device, StructuredBuffer const* sbuffer);
        static std::shared_ptr<GEObject> Create(void* device, GraphicsObject const* object);

    public:
        // Member access.
        inline StructuredBuffer* GetStructuredBuffer() const
        {
            return static_cast<StructuredBuffer*>(mGTObject);
        }

        inline ID3D11ShaderResourceView* GetSRView() const
        {
            return mSRView;
        }

        inline ID3D11UnorderedAccessView* GetUAView() const
        {
            return mUAView;
        }

        inline ID3D11Buffer* GetCounterStagingBuffer() const
        {
            return mCounterStaging;
        }

        // Copy of data between CPU and GPU.
        virtual bool CopyGpuToCpu(ID3D11DeviceContext* context);

        // The number of active elements is read from GPU and stored in the
        // StructuredBuffer object  (of type CT_APPEND_CONSUME or CT_COUNTER).
        // The result can be accessed from this object via its member function
        // GetNumActiveElements().
        bool GetNumActiveElements(ID3D11DeviceContext* context);

        // Support for the DX11 debug layer; see comments in the file
        // DX11GraphicsObject.h about usage.
        virtual void SetName(std::string const& name);

    private:
        // Support for construction.
        void CreateSRView(ID3D11Device* device);
        void CreateUAView(ID3D11Device* device);
        void CreateCounterStaging(ID3D11Device* device);

        ID3D11ShaderResourceView* mSRView;
        ID3D11UnorderedAccessView* mUAView;
        ID3D11Buffer* mCounterStaging;

        // Mapping from StructuredBuffer::CounterType to
        // D3D11_BUFFER_UAV_FLAG.
        static uint32_t const msUAVFlag[];
    };
}
