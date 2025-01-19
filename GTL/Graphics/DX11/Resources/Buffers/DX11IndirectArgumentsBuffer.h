// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#pragma once

#include <GTL/Graphics/Resources/Buffers/IndirectArgumentsBuffer.h>
#include <GTL/Graphics/DX11/Resources/Buffers/DX11Buffer.h>

namespace gtl
{
    class DX11IndirectArgumentsBuffer : public DX11Buffer
    {
    public:
        // Construction.
        virtual ~DX11IndirectArgumentsBuffer() = default;
        DX11IndirectArgumentsBuffer(ID3D11Device* device, IndirectArgumentsBuffer const* iabuffer);
        static std::shared_ptr<GEObject> Create(void* device, GraphicsObject const* object);

        // Member access.
        inline IndirectArgumentsBuffer* GetIndirectArgumentsBuffer() const
        {
            return static_cast<IndirectArgumentsBuffer*>(mGTObject);
        }
    };
}
