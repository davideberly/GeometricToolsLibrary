// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#pragma once

#include <GTL/Graphics/Resources/Buffers/Buffer.h>
#include <GTL/Graphics/Resources/Buffers/VertexFormat.h>
#include <cstdint>

namespace gtl
{
    class StructuredBuffer;

    class VertexBuffer : public Buffer
    {
    public:
        // This constructor is for standard usage where the vertex buffer is
        // used by the rasterizer to provide vertices to the vertex shader.
        VertexBuffer(VertexFormat const& vformat, uint32_t numVertices,
            bool createStorage = true);

        // This constructor is used for vertex-id-based drawing where the
        // vertices are read from a structured buffer resource in the vertex
        // shader.  The input 'sbuffer' must be nonnull and its number of
        // vertices is copied to 'this' number of vertices.
        VertexBuffer(VertexFormat const& vformat,
            std::shared_ptr<StructuredBuffer> const& sbuffer);

        // This constructor is used for vertex-id-based drawing that does not
        // require vertices; for example, the shader itself can generate the
        // positions from the identifiers.
        VertexBuffer(uint32_t numVertices);

        // Member access.  The function StandardUsage() returns 'true' when
        // the first constructor is used or 'false' when the second
        // constructor is used.
        inline VertexFormat const& GetFormat() const
        {
            return mVFormat;
        }

        inline std::shared_ptr<StructuredBuffer> const& GetSBuffer() const
        {
            return mSBuffer;
        }

        inline bool StandardUsage() const
        {
            return mVFormat.GetNumAttributes() != 0 && mSBuffer == nullptr;
        }

        // Get pointers to attribute data if it exists for the specified
        // semantic and unit.  Also, you request that the attribute be one of
        // a list of required types (OR-ed bit flags).  If you do not care
        // about the type, pass DF_UNKNOWN for the required input.  If the
        // request fails, a null pointer is returned.
        char* GetChannel(VASemantic semantic, uint32_t unit,
            std::set<DFType> const& requiredTypes);

    protected:
        VertexFormat mVFormat;

        // Valid when the second constructor is used.
        std::shared_ptr<StructuredBuffer> mSBuffer;
    };
}
