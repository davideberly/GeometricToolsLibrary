// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#pragma once

// IndirectArgumentsBuffer is currently supported only in the DirectX graphics
// engine.

#include <GTL/Graphics/Resources/Buffers/Buffer.h>
#include <cstdint>

namespace gtl
{
    class IndirectArgumentsBuffer : public Buffer
    {
    public:
        // Construction.  Each element is a 4-byte value.  For storing a
        // single set of input parameters to a draw call, the layout of
        // the buffer should be as shown next.  The parameters are in the
        // order expected by the Draw* call without the 'Indirect' suffix.
        //
        // DrawInstancedIndirect:
        //   UINT VertexCountPerInstance;
        //   UINT InstanceCount;
        //   UINT StartVertexLocation;
        //   UINT StartInstanceLocation;
        //
        // DrawIndexedInstancedIndirect:
        //   UINT IndexCountPerInstance;
        //   UINT InstanceCount;
        //   UINT StartIndexLocation;
        //   INT  BaseVertexLocation;
        //   UINT StartInstanceLocation;
        //
        // DispatchIndirect:
        //   UINT ThreadsGroupCountX;
        //   UINT ThreadsGroupCountY;
        //   UINT ThreadsGroupCountZ;

        IndirectArgumentsBuffer(uint32_t numElements, bool createStorage = true);
    };
}
