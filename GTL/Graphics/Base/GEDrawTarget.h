// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.19

#pragma once

namespace gtl
{
    class DrawTarget;

    class GEDrawTarget
    {
    public:
        // Abstract base class.
        virtual ~GEDrawTarget();
    protected:
        GEDrawTarget(DrawTarget const* gtTarget);

    public:
        // Member access.
        inline DrawTarget* GetDrawTarget() const
        {
            return mTarget;
        }

    protected:
        DrawTarget* mTarget;
    };
}
