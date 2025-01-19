// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// Version: 0.0.2025.1.19

#pragma once

#include <GTL/Graphics/State/DrawingState.h>
#include <GTL/Graphics/GL45/Engine/GL45GraphicsObject.h>

namespace gtl
{
    class GL45DrawingState : public GL45GraphicsObject
    {
    public:
        // Abstract base class.
        virtual ~GL45DrawingState() = default;
    protected:
        GL45DrawingState(DrawingState const* gtState);
    };
}
