// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.10.19

#pragma once

#include <cstddef>
#include <string>

namespace gtl
{
    struct VertexAttribute
    {
        // The 'semantic' string allows you to query for a specific vertex
        // attribute and use the 'source' and 'stride' to access the data
        // of the attribute. For example, you might use the semantics
        // "position" (px,py,pz), "normal" (nx,ny,nz), "tcoord" (texture
        // coordinates (u,v)), "dpdu" (derivative of position with respect
        // to u), or "dpdv" (derivative of position with respect to v) for
        // mesh vertices.
        //
        // The source pointer must be 4-byte aligned. The stride must be
        // positive and a multiple of 4. The pointer alignment constraint is
        // guaranteed on 32-bit and 64-bit architectures. The stride constraint
        // is reasonable given that geometric attributes are usually arrays of
        // 'float' or 'double'.

        VertexAttribute(std::string inSemantic = "", void* inSource = nullptr, std::size_t inStride = 0)
            :
            semantic(inSemantic),
            source(inSource),
            stride(inStride)
        {
        }

        std::string semantic;
        void* source;
        std::size_t stride;
    };
}
