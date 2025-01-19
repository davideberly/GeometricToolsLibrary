// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#pragma once

#include <GTL/Graphics/State/BlendState.h>
#include <GTL/Graphics/DX11/State/DX11DrawingState.h>

namespace gtl
{
    class DX11BlendState : public DX11DrawingState
    {
    public:
        // Construction.
        virtual ~DX11BlendState() = default;
        DX11BlendState(ID3D11Device* device, BlendState const* blendState);
        static std::shared_ptr<GEObject> Create(void* device, GraphicsObject const* object);

        // Member access.
        inline BlendState* GetBlendState()
        {
            return static_cast<BlendState*>(mGTObject);
        }

        inline ID3D11BlendState* GetDXBlendState()
        {
            return static_cast<ID3D11BlendState*>(mDXObject);
        }

        // Enable the blend state.
        void Enable(ID3D11DeviceContext* context);

    private:
        // Conversions from GTL values to DX11 values.
        static D3D11_BLEND const msMode[];
        static D3D11_BLEND_OP const msOperation[];
    };
}
