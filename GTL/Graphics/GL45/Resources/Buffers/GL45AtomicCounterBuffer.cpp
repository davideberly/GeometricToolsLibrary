// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.1.19

#include <GTLGraphicsGL45PCH.h>
#include <GTL/Graphics/GL45/Resources/Buffers/GL45AtomicCounterBuffer.h>
using namespace gtl;

GL45AtomicCounterBuffer::GL45AtomicCounterBuffer(RawBuffer const* cbuffer)
    :
    GL45Buffer(cbuffer, GL_ATOMIC_COUNTER_BUFFER)
{
    Initialize();
}

std::shared_ptr<GEObject> GL45AtomicCounterBuffer::Create(void*, GraphicsObject const* object)
{
    if (object->GetType() == GT_RAW_BUFFER)
    {
        return std::make_shared<GL45AtomicCounterBuffer>(
            static_cast<RawBuffer const*>(object));
    }

    GTL_RUNTIME_ERROR("Invalid object type.");
}

void GL45AtomicCounterBuffer::AttachToUnit(GLint atomicCounterBufferUnit)
{
    glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, atomicCounterBufferUnit, mGLHandle);
}
