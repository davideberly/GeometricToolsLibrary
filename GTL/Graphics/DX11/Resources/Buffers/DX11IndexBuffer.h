// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#pragma once

#include <GTL/Graphics/Resources/Buffers/IndexBuffer.h>
#include <GTL/Graphics/DX11/Resources/Buffers/DX11Buffer.h>

namespace gtl
{
    class DX11IndexBuffer : public DX11Buffer
    {
    public:
        // Construction.
        virtual ~DX11IndexBuffer() = default;
        DX11IndexBuffer(ID3D11Device* device, IndexBuffer const* vbuffer);
        static std::shared_ptr<GEObject> Create(void* device, GraphicsObject const* object);

        // Member access.
        inline IndexBuffer* GetIndexBuffer() const
        {
            return static_cast<IndexBuffer*>(mGTObject);
        }

        // Drawing support.
        void Enable(ID3D11DeviceContext* context);
        void Disable(ID3D11DeviceContext* context);

    private:
        DXGI_FORMAT mFormat;
    };
}
