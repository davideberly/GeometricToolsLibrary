// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// Version: 0.0.2025.1.19

#include <GTLGraphicsGL45PCH.h>
#include <GTL/Graphics/GL45/Resources/Buffers/GL45ConstantBuffer.h>
using namespace gtl;

GL45ConstantBuffer::GL45ConstantBuffer(ConstantBuffer const* cbuffer)
    :
    GL45Buffer(cbuffer, GL_UNIFORM_BUFFER)
{
    Initialize();
}

std::shared_ptr<GEObject> GL45ConstantBuffer::Create(void*, GraphicsObject const* object)
{
    if (object->GetType() == GT_CONSTANT_BUFFER)
    {
        return std::make_shared<GL45ConstantBuffer>(
            static_cast<ConstantBuffer const*>(object));
    }

    GTL_RUNTIME_ERROR("Invalid object type.");
}

void GL45ConstantBuffer::AttachToUnit(GLint uniformBufferUnit)
{
    glBindBufferBase(GL_UNIFORM_BUFFER, uniformBufferUnit, mGLHandle);
}
