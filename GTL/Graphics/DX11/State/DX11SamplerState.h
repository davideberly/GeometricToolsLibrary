// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#pragma once

#include <GTL/Graphics/State/SamplerState.h>
#include <GTL/Graphics/DX11/State/DX11DrawingState.h>

namespace gtl
{
    class DX11SamplerState : public DX11DrawingState
    {
    public:
        // Construction.
        virtual ~DX11SamplerState() = default;
        DX11SamplerState(ID3D11Device* device, SamplerState const* samplerState);
        static std::shared_ptr<GEObject> Create(void* device, GraphicsObject const* object);

        // Member access.
        inline SamplerState* GetSamplerState()
        {
            return static_cast<SamplerState*>(mGTObject);
        }

        inline ID3D11SamplerState* GetDXSamplerState()
        {
            return static_cast<ID3D11SamplerState*>(mDXObject);
        }

    private:
        // Conversions from GTL values to DX11 values.
        static D3D11_FILTER const msFilter[];
        static D3D11_TEXTURE_ADDRESS_MODE const msMode[];
        static D3D11_COMPARISON_FUNC const msComparison[];
    };
}
