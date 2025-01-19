// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// Version: 0.0.2025.1.19

#pragma once

#include <GTL/Graphics/Resources/Buffers/VertexBuffer.h>
#include <GTL/Graphics/GL45/Resources/Buffers/GL45Buffer.h>

namespace gtl
{
    class GL45VertexBuffer : public GL45Buffer
    {
    public:
        // Construction.
        virtual ~GL45VertexBuffer() = default;
        GL45VertexBuffer(VertexBuffer const* vbuffer);
        static std::shared_ptr<GEObject> Create(void* unused, GraphicsObject const* object);

        // Member access.
        inline VertexBuffer* GetVertexBuffer() const
        {
            return static_cast<VertexBuffer*>(mGTObject);
        }

        // TODO: Drawing support?  Currently, the enable/disable is in the
        // GL45InputLayout class, which assumes OpenGL 4.5 or later.  What if
        // the application machine does not have OpenGL 4.5?  Fall back to the
        // glBindBuffer paradigm?
    };
}
