// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#pragma once

#include <cstdint>

namespace gtl
{
    // Types of primitives supported by index buffers.
    enum IPType
    {
        IP_NONE = 0xFFFFFFFF,
        IP_POLYPOINT = 0x00000001,
        IP_POLYSEGMENT_DISJOINT = 0x00000002,
        IP_POLYSEGMENT_CONTIGUOUS = 0x00000004,
        IP_TRIMESH = 0x00000008,
        IP_TRISTRIP = 0x00000010,
        IP_POLYSEGMENT_DISJOINT_ADJ = 0x00000020,
        IP_POLYSEGMENT_CONTIGUOUS_ADJ = 0x00000040,
        IP_TRIMESH_ADJ = 0x00000080,
        IP_TRISTRIP_ADJ = 0x00000100,

        IP_HAS_POINTS = IP_POLYPOINT,

        IP_HAS_SEGMENTS =
            IP_POLYSEGMENT_DISJOINT |
            IP_POLYSEGMENT_CONTIGUOUS |
            IP_POLYSEGMENT_DISJOINT_ADJ |
            IP_POLYSEGMENT_CONTIGUOUS_ADJ,

        IP_HAS_TRIANGLES =
            IP_TRIMESH |
            IP_TRISTRIP |
            IP_TRIMESH_ADJ |
            IP_TRISTRIP_ADJ,

        // POLYPOINT through TRISTRIP_ADJ
        IP_NUM_TYPES = 9
    };
}
