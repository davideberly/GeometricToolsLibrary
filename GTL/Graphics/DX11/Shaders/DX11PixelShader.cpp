// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#include <GTLGraphicsDX11PCH.h>
#include <GTL/Utility/Exceptions.h>
#include <GTL/Graphics/DX11/Shaders/DX11PixelShader.h>
using namespace gtl;

DX11PixelShader::DX11PixelShader(ID3D11Device* device, Shader const* shader)
    :
    DX11Shader(shader)
{
    std::vector<uint8_t> const& code = shader->GetCompiledCode();

    ID3D11ClassLinkage* linkage = nullptr;
    ID3D11PixelShader* dxShader = nullptr;
    DX11Log(device->CreatePixelShader(&code[0], code.size(), linkage, &dxShader));

    mDXObject = dxShader;
}

std::shared_ptr<GEObject> DX11PixelShader::Create(void* device, GraphicsObject const* object)
{
    GTL_RUNTIME_ASSERT(
        object->GetType() == GT_PIXEL_SHADER,
        "Invalid object type.");

    return std::make_shared<DX11PixelShader>(
        reinterpret_cast<ID3D11Device*>(device),
        static_cast<Shader const*>(object));
}

void DX11PixelShader::Enable(ID3D11DeviceContext* context)
{
    if (mDXObject)
    {
        ID3D11ClassInstance* instances[1] = { nullptr };
        UINT numInstances = 0;
        ID3D11PixelShader* dxShader = static_cast<ID3D11PixelShader*>(mDXObject);
        context->PSSetShader(dxShader, instances, numInstances);
    }
}

void DX11PixelShader::Disable(ID3D11DeviceContext* context)
{
    if (mDXObject)
    {
        ID3D11ClassInstance* instances[1] = { nullptr };
        UINT numInstances = 0;
        ID3D11PixelShader* dxShader = nullptr;
        context->PSSetShader(dxShader, instances, numInstances);
    }
}

void DX11PixelShader::EnableCBuffer(ID3D11DeviceContext* context,
    uint32_t bindPoint, ID3D11Buffer* buffer)
{
    if (mDXObject)
    {
        ID3D11Buffer* buffers[1] = { buffer };
        context->PSSetConstantBuffers(bindPoint, 1, buffers);
    }
}

void DX11PixelShader::DisableCBuffer(ID3D11DeviceContext* context,
    uint32_t bindPoint)
{
    if (mDXObject)
    {
        ID3D11Buffer* buffers[1] = { nullptr };
        context->PSSetConstantBuffers(bindPoint, 1, buffers);
    }
}

void DX11PixelShader::EnableSRView(ID3D11DeviceContext* context,
    uint32_t bindPoint, ID3D11ShaderResourceView* srView)
{
    if (mDXObject)
    {
        ID3D11ShaderResourceView* views[1] = { srView };
        context->PSSetShaderResources(bindPoint, 1, views);
    }
}

void DX11PixelShader::DisableSRView(ID3D11DeviceContext* context,
    uint32_t bindPoint)
{
    if (mDXObject)
    {
        ID3D11ShaderResourceView* views[1] = { nullptr };
        context->PSSetShaderResources(bindPoint, 1, views);
    }
}

void DX11PixelShader::EnableUAView(ID3D11DeviceContext* context,
    uint32_t bindPoint, ID3D11UnorderedAccessView* uaView,
    uint32_t initialCount)
{
    if (mDXObject)
    {
        ID3D11UnorderedAccessView* uaViews[1] = { uaView };
        uint32_t initialCounts[1] = { initialCount };
        context->OMSetRenderTargetsAndUnorderedAccessViews(
            D3D11_KEEP_RENDER_TARGETS_AND_DEPTH_STENCIL, nullptr, nullptr,
            bindPoint, 1, uaViews, initialCounts);
    }
}

void DX11PixelShader::DisableUAView(ID3D11DeviceContext* context,
    uint32_t bindPoint)
{
    if (mDXObject)
    {
        ID3D11UnorderedAccessView* uaViews[1] = { nullptr };
        uint32_t initialCounts[1] = { 0xFFFFFFFFu };
        context->OMSetRenderTargetsAndUnorderedAccessViews(
            D3D11_KEEP_RENDER_TARGETS_AND_DEPTH_STENCIL, nullptr, nullptr,
            bindPoint, 1, uaViews, initialCounts);
    }
}

void DX11PixelShader::EnableSampler(ID3D11DeviceContext* context,
    uint32_t bindPoint, ID3D11SamplerState* state)
{
    if (mDXObject)
    {
        ID3D11SamplerState* states[1] = { state };
        context->PSSetSamplers(bindPoint, 1, states);
    }
}

void DX11PixelShader::DisableSampler(ID3D11DeviceContext* context,
    uint32_t bindPoint)
{
    if (mDXObject)
    {
        ID3D11SamplerState* states[1] = { nullptr };
        context->PSSetSamplers(bindPoint, 1, states);
    }
}
