// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#pragma once

// This class provides virtual functions for generic access to DX11 shader
// functions that have embedded in their names "VS", "GS", "PS", "CS",
// "DS", and "HS".  The prefix "XS" is generic, where X in {V,G,P,C,D,H}.

#include <GTL/Graphics/Shaders/Shader.h>
#include <GTL/Graphics/DX11/Engine/DX11GraphicsObject.h>

namespace gtl
{
    class DX11Shader : public DX11GraphicsObject
    {
    public:
        // Abstract base class.
        virtual ~DX11Shader() = default;
    protected:
        DX11Shader(Shader const* shader);

    public:
        // Calls to ID3D11DeviceContext::XSSetShader.
        virtual void Enable(ID3D11DeviceContext* context) = 0;
        virtual void Disable(ID3D11DeviceContext* context) = 0;

        // Calls to ID3D11DeviceContext::XSSetConstantBuffers.
        virtual void EnableCBuffer(ID3D11DeviceContext* context,
            uint32_t bindPoint, ID3D11Buffer* buffer) = 0;
        virtual void DisableCBuffer(ID3D11DeviceContext* context,
            uint32_t bindPoint) = 0;

        // Calls to ID3D11DeviceContext::XSSetShaderResources.
        virtual void EnableSRView(ID3D11DeviceContext* context,
            uint32_t bindPoint, ID3D11ShaderResourceView* srView) = 0;
        virtual void DisableSRView(ID3D11DeviceContext* context,
            uint32_t bindPoint) = 0;

        // Calls to ID3D11DeviceContext::XSSetUnorderedAccessViews.
        virtual void EnableUAView(ID3D11DeviceContext* context,
            uint32_t bindPoint, ID3D11UnorderedAccessView* uaView,
            uint32_t initialCount) = 0;
        virtual void DisableUAView(ID3D11DeviceContext* context,
            uint32_t bindPoint) = 0;

        // Calls to ID3D11DeviceContext::XSSetSamplers.
        virtual void EnableSampler(ID3D11DeviceContext* context,
            uint32_t bindPoint, ID3D11SamplerState* state) = 0;
        virtual void DisableSampler(ID3D11DeviceContext* context,
            uint32_t bindPoint) = 0;
    };
}
