// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#pragma once

#include <GTL/Graphics/Resources/Buffers/ConstantBuffer.h>
#include <GTL/Graphics/DX11/Resources/Buffers/DX11Buffer.h>

namespace gtl
{
    class DX11ConstantBuffer : public DX11Buffer
    {
    public:
        // Construction.
        virtual ~DX11ConstantBuffer() = default;
        DX11ConstantBuffer(ID3D11Device* device, ConstantBuffer const* cbuffer);
        static std::shared_ptr<GEObject> Create(void* device, GraphicsObject const* object);

        // Member access.
        inline ConstantBuffer* GetConstantBuffer() const
        {
            return static_cast<ConstantBuffer*>(mGTObject);
        }
    };
}
