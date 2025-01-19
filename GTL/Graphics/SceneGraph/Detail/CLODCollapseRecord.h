// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#pragma once

#include <cstdint>
#include <vector>

namespace gtl
{
    class CLODCollapseRecord
    {
    public:
        CLODCollapseRecord(int32_t inVKeep = -1, int32_t inVThrow = -1,
            int32_t inNumVertices = 0, int32_t inNumTriangles = 0)
            :
            vKeep(inVKeep),
            vThrow(inVThrow),
            numVertices(inNumVertices),
            numTriangles(inNumTriangles),
            indices{}
        {
        }

        // Edge <VKeep,VThrow> collapses so that VThrow is replaced by VKeep.
        int32_t vKeep, vThrow;

        // The number of vertices after the edge collapse.
        int32_t numVertices;

        // The number of triangles after the edge collapse.
        int32_t numTriangles;

        // The array of indices in [0..numTriangles-1] that contain vThrow.
        std::vector<int32_t> indices;
    };
}
