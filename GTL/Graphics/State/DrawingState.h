// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#pragma once

#include <GTL/Graphics/Base/GraphicsObject.h>

namespace gtl
{
    class DrawingState : public GraphicsObject
    {
    protected:
        // Abstract base class for grouping state classes.  This supports
        // simplification and reduction of member functions in the graphics
        // engine code.
        DrawingState();
    };
}
