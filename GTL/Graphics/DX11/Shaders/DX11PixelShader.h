// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#pragma once

#include <GTL/Graphics/Shaders/Shader.h>
#include <GTL/Graphics/DX11/Shaders/DX11Shader.h>

namespace gtl
{
    class DX11PixelShader : public DX11Shader
    {
    public:
        // Construction and destruction.
        virtual ~DX11PixelShader() = default;
        DX11PixelShader(ID3D11Device* device, Shader const* shader);
        static std::shared_ptr<GEObject> Create(void* device, GraphicsObject const* object);

        // Overrides to hide DX11 functions that have "PS" embedded in their
        // names.

        // Calls to ID3D11DeviceContext::PSSetShader.
        virtual void Enable(ID3D11DeviceContext* context) override;
        virtual void Disable(ID3D11DeviceContext* context) override;

        // Calls to ID3D11DeviceContext::PSSetConstantBuffers.
        virtual void EnableCBuffer(ID3D11DeviceContext* context,
            uint32_t bindPoint, ID3D11Buffer* buffer) override;
        virtual void DisableCBuffer(ID3D11DeviceContext* context,
            uint32_t bindPoint) override;

        // Calls to ID3D11DeviceContext::PSSetShaderResources.
        virtual void EnableSRView(ID3D11DeviceContext* context,
            uint32_t bindPoint, ID3D11ShaderResourceView* srView) override;
        virtual void DisableSRView(ID3D11DeviceContext* context,
            uint32_t bindPoint) override;

        // Calls to ID3D11DeviceContext::PSSetUnorderedAccessViews.
        virtual void EnableUAView(ID3D11DeviceContext* context,
            uint32_t bindPoint, ID3D11UnorderedAccessView* uaView,
            uint32_t initialCount) override;
        virtual void DisableUAView(ID3D11DeviceContext* context,
            uint32_t bindPoint) override;

        // Calls to ID3D11DeviceContext::PSSetSamplers.
        virtual void EnableSampler(ID3D11DeviceContext* context,
            uint32_t bindPoint, ID3D11SamplerState* state) override;
        virtual void DisableSampler(ID3D11DeviceContext* context,
            uint32_t bindPoint) override;
    };
}
