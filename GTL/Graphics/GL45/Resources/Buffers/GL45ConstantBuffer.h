// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// Version: 0.0.2025.1.19

#pragma once

#include <GTL/Graphics/Resources/Buffers/ConstantBuffer.h>
#include <GTL/Graphics/GL45/Resources/Buffers/GL45Buffer.h>

namespace gtl
{
    class GL45ConstantBuffer : public GL45Buffer
    {
    public:
        // Construction.
        virtual ~GL45ConstantBuffer() = default;
        GL45ConstantBuffer(ConstantBuffer const* cbuffer);
        static std::shared_ptr<GEObject> Create(void* unused, GraphicsObject const* object);

        // Member access.
        inline ConstantBuffer* GetConstantBuffer() const
        {
            return static_cast<ConstantBuffer*>(mGTObject);
        }

        // Bind the constant buffer data to the specified uniform buffer unit.
        void AttachToUnit(GLint uniformBufferUnit);
    };
}
