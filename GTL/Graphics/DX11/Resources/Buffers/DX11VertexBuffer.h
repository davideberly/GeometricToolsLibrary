// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#pragma once

#include <GTL/Graphics/Resources/Buffers/VertexBuffer.h>
#include <GTL/Graphics/DX11/Resources/Buffers/DX11Buffer.h>

namespace gtl
{
    class DX11VertexBuffer : public DX11Buffer
    {
    public:
        // Construction.
        virtual ~DX11VertexBuffer() = default;
        DX11VertexBuffer(ID3D11Device* device, VertexBuffer const* vbuffer);
        static std::shared_ptr<GEObject> Create(void* device, GraphicsObject const* object);

        // Member access.
        inline VertexBuffer* GetVertexBuffer() const
        {
            return static_cast<VertexBuffer*>(mGTObject);
        }

        // Drawing support.
        void Enable(ID3D11DeviceContext* context);
        void Disable(ID3D11DeviceContext* context);
    };
}
