// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.1.19

#pragma once

#include <GTL/Graphics/State/SamplerState.h>
#include <GTL/Graphics/GL45/State/GL45DrawingState.h>

namespace gtl
{
    class GL45SamplerState : public GL45DrawingState
    {
    public:
        // Construction and destruction.
        virtual ~GL45SamplerState();
        GL45SamplerState(SamplerState const* samplerState);
        static std::shared_ptr<GEObject> Create(void* unused, GraphicsObject const* object);

        // Member access.
        inline SamplerState* GetSamplerState()
        {
            return static_cast<SamplerState*>(mGTObject);
        }

    private:
        // Conversions from GTEngine values to GL4 values.
        static GLint const msMode[];
    };
}
