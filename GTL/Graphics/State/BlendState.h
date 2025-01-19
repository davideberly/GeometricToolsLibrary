// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#pragma once

#include <GTL/Mathematics/Algebra/Vector.h>
#include <GTL/Graphics/State/DrawingState.h>
#include <cstdint>

namespace gtl
{
    class BlendState : public DrawingState
    {
    public:
        enum Mode
        {
            ZERO,
            ONE,
            SRC_COLOR,
            INV_SRC_COLOR,
            SRC_ALPHA,
            INV_SRC_ALPHA,
            DEST_ALPHA,
            INV_DEST_ALPHA,
            DEST_COLOR,
            INV_DEST_COLOR,
            SRC_ALPHA_SAT,
            FACTOR,
            INV_FACTOR,
            SRC1_COLOR,
            INV_SRC1_COLOR,
            SRC1_ALPHA,
            INV_SRC1_ALPHA
        };

        enum Operation
        {
            ADD,
            SUBTRACT,
            REV_SUBTRACT,
            MIN,
            MAX
        };

        enum ColorWrite
        {
            ENABLE_RED = 1,
            ENABLE_GREEN = 2,
            ENABLE_BLUE = 4,
            ENABLE_ALPHA = 8,
            ENABLE_ALL = 15
        };

        static size_t constexpr NUM_TARGETS = 8;

        struct Target
        {
            Target()
                :
                enable(false),
                srcColor(Mode::ONE),
                dstColor(Mode::ZERO),
                opColor(Operation::ADD),
                srcAlpha(Mode::ONE),
                dstAlpha(Mode::ZERO),
                opAlpha(Operation::ADD),
                mask(ColorWrite::ENABLE_ALL)
            {
            }

            bool enable;
            uint32_t srcColor;
            uint32_t dstColor;
            uint32_t opColor;
            uint32_t srcAlpha;
            uint32_t dstAlpha;
            uint32_t opAlpha;
            uint8_t mask;
        };

        BlendState();

        // Member access.  The members are intended to be write-once before
        // you create an associated graphics state.
        bool enableAlphaToCoverage;             // default: false
        bool enableIndependentBlend;            // default: false
        std::array<Target, NUM_TARGETS> target;
        Vector4<float> blendColor;              // default: (0,0,0,0)
        uint32_t sampleMask;                    // default: 0xFFFFFFFF
    };
}
