// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.1.19

#pragma once

namespace gtl
{
    class Shader;
    class VertexBuffer;

    class GEInputLayoutManager
    {
    public:
        // Abstract base interface.
        virtual ~GEInputLayoutManager() = default;
        GEInputLayoutManager() = default;

        virtual bool Unbind(VertexBuffer const* vbuffer) = 0;
        virtual bool Unbind(Shader const* vshader) = 0;
        virtual void UnbindAll() = 0;
        virtual bool HasElements() const = 0;
    };
}
