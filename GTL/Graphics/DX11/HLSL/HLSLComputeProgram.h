// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#pragma once

#include <GTL/Graphics/Shaders/ComputeProgram.h>

namespace gtl
{
    class HLSLComputeProgram : public ComputeProgram
    {
    public:
        // A simple stub to add HLSL as part of the program type.  This allows
        // polymorphism for the program factory classes, which in turn allows
        // us to hide the graphics-API-dependent program factory used by the
        // Window class (have a member mProgramFactory similar to mEngine that
        // is created according to the desired graphics API).
        virtual ~HLSLComputeProgram() = default;
        HLSLComputeProgram() = default;
    };
}
