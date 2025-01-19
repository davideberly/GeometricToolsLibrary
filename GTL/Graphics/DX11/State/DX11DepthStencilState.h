// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#pragma once

#include <GTL/Graphics/State/DepthStencilState.h>
#include <GTL/Graphics/DX11/State/DX11DrawingState.h>

namespace gtl
{
    class DX11DepthStencilState : public DX11DrawingState
    {
    public:
        // Construction.
        virtual ~DX11DepthStencilState() = default;
        DX11DepthStencilState(ID3D11Device* device, DepthStencilState const* depthStencilState);
        static std::shared_ptr<GEObject> Create(void* device, GraphicsObject const* object);

        // Member access.
        inline DepthStencilState* GetDepthStencilState()
        {
            return static_cast<DepthStencilState*>(mGTObject);
        }

        inline ID3D11DepthStencilState* GetDXDepthStencilState()
        {
            return static_cast<ID3D11DepthStencilState*>(mDXObject);
        }

        // Enable the depth-stencil state.
        void Enable(ID3D11DeviceContext* context);

    private:
        // Conversions from GTL values to DX11 values.
        static D3D11_DEPTH_WRITE_MASK const msWriteMask[];
        static D3D11_COMPARISON_FUNC const msComparison[];
        static D3D11_STENCIL_OP const msOperation[];
    };
}
