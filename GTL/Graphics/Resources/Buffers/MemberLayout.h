// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace gtl
{
    // Support for generation of lookup tables for constant buffers and
    // texture buffers.  Given the name of a member of a buffer, get the
    // offset into the buffer memory where the member lives.  The layout
    // is specific to the shading language (HLSL, GLSL).
    struct MemberLayout
    {
        MemberLayout()
            :
            name(""),
            offset(0),
            numElements(0)
        {
        }

        std::string name;
        uint32_t offset;
        uint32_t numElements;
    };

    typedef std::vector<MemberLayout> BufferLayout;
}
