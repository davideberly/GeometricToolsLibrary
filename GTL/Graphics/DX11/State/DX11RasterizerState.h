// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#pragma once

#include <GTL/Graphics/State/RasterizerState.h>
#include <GTL/Graphics/DX11/State/DX11DrawingState.h>

namespace gtl
{
    class DX11RasterizerState : public DX11DrawingState
    {
    public:
        // Construction.
        virtual ~DX11RasterizerState() = default;
        DX11RasterizerState(ID3D11Device* device, RasterizerState const* rasterizerState);
        static std::shared_ptr<GEObject> Create(void* device, GraphicsObject const* object);

        // Member access.
        inline RasterizerState* GetRasterizerState()
        {
            return static_cast<RasterizerState*>(mGTObject);
        }

        inline ID3D11RasterizerState* GetDXRasterizerState()
        {
            return static_cast<ID3D11RasterizerState*>(mDXObject);
        }

        // Enable the rasterizer state.
        void Enable(ID3D11DeviceContext* context);

    private:
        // Conversions from GTL values to DX11 values.
        static D3D11_FILL_MODE const msFillMode[];
        static D3D11_CULL_MODE const msCullMode[];
    };
}
