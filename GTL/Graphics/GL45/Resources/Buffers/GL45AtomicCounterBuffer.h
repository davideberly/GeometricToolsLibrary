// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.1.19

#pragma once

#include <GTL/Graphics/Resources/Buffers/RawBuffer.h>
#include <GTL/Graphics/GL45/Resources/Buffers/GL45Buffer.h>

namespace gtl
{
    class GL45AtomicCounterBuffer : public GL45Buffer
    {
    public:
        // Construction.
        virtual ~GL45AtomicCounterBuffer() = default;
        GL45AtomicCounterBuffer(RawBuffer const* cbuffer);
        static std::shared_ptr<GEObject> Create(void* unused, GraphicsObject const* object);

        // Member access.
        inline RawBuffer* GetRawBuffer() const
        {
            return static_cast<RawBuffer*>(mGTObject);
        }

        // Bind the raw buffer data to the specified atomic counter buffer unit.
        void AttachToUnit(GLint atomicCounterBufferUnit);
    };
}
