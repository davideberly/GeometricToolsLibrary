// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#pragma once

#include <GTL/Graphics/State/DrawingState.h>
#include <GTL/Graphics/DX11/Engine/DX11GraphicsObject.h>

namespace gtl
{
    class DX11DrawingState : public DX11GraphicsObject
    {
    public:
        // Abstract base class.
        virtual ~DX11DrawingState() = default;
    protected:
        DX11DrawingState(DrawingState const* gtState);
    };
}
