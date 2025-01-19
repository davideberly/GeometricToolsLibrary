// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// Version: 0.0.2025.1.19

#include <GTLGraphicsGL45PCH.h>
#include <GTL/Graphics/GL45/Resources/Buffers/GL45VertexBuffer.h>
using namespace gtl;

GL45VertexBuffer::GL45VertexBuffer(VertexBuffer const* vbuffer)
    :
    GL45Buffer(vbuffer, GL_ARRAY_BUFFER)
{
    Initialize();
}

std::shared_ptr<GEObject> GL45VertexBuffer::Create(void*, GraphicsObject const* object)
{
    if (object->GetType() == GT_VERTEX_BUFFER)
    {
        return std::make_shared<GL45VertexBuffer>(
            static_cast<VertexBuffer const*>(object));
    }

    GTL_RUNTIME_ERROR("Invalid object type.");
}
