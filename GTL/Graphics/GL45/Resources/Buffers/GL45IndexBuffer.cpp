// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.1.19

#include <GTLGraphicsGL45PCH.h>
#include <GTL/Graphics/GL45/Resources/Buffers/GL45IndexBuffer.h>
using namespace gtl;

GL45IndexBuffer::GL45IndexBuffer(IndexBuffer const* ibuffer)
    :
    GL45Buffer(ibuffer, GL_ELEMENT_ARRAY_BUFFER)
{
    Initialize();
}

std::shared_ptr<GEObject> GL45IndexBuffer::Create(void*, GraphicsObject const* object)
{
    if (object->GetType() == GT_INDEX_BUFFER)
    {
        return std::make_shared<GL45IndexBuffer>(
            static_cast<IndexBuffer const*>(object));
    }

    GTL_RUNTIME_ERROR("Invalid object type.");
}

void GL45IndexBuffer::Enable()
{
    glBindBuffer(mType, mGLHandle);
}

void GL45IndexBuffer::Disable()
{
    glBindBuffer(mType, 0);
}
