// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#pragma once

#include <GTL/Graphics/State/DrawingState.h>
#include <cstdint>

namespace gtl
{
    class RasterizerState : public DrawingState
    {
    public:
        enum Fill
        {
            SOLID,
            WIREFRAME
        };

        enum Cull
        {
            NONE,
            FRONT,
            BACK
        };

        RasterizerState();

        // Member access.  The members are intended to be write-once before
        // you create an associated graphics state.
        Fill fill;                  // default: SOLID
        Cull cull;                  // default: BACK
        bool frontCCW;              // default: true
        int32_t depthBias;          // default: 0
        float depthBiasClamp;       // default: 0
        float slopeScaledDepthBias; // default: 0
        bool enableDepthClip;       // default: true
        bool enableScissor;         // default: false
        bool enableMultisample;     // default: false
        bool enableAntialiasedLine; // default: false
    };
}
