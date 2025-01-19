// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#include <GTLGraphicsDX11PCH.h>
#include <GTL/Utility/Exceptions.h>
#include <GTL/Graphics/DX11/State/DX11RasterizerState.h>
using namespace gtl;

DX11RasterizerState::DX11RasterizerState(ID3D11Device* device, RasterizerState const* rasterizerState)
    :
    DX11DrawingState(rasterizerState)
{
    // Specify the rasterizer state description.
    D3D11_RASTERIZER_DESC desc;
    desc.FillMode = msFillMode[rasterizerState->fill];
    desc.CullMode = msCullMode[rasterizerState->cull];
    desc.FrontCounterClockwise = (rasterizerState->frontCCW ? TRUE : FALSE);
    desc.DepthBias = rasterizerState->depthBias;
    desc.DepthBiasClamp = rasterizerState->depthBiasClamp;
    desc.SlopeScaledDepthBias = rasterizerState->slopeScaledDepthBias;
    desc.DepthClipEnable = (rasterizerState->enableDepthClip ? TRUE : FALSE);
    desc.ScissorEnable = (rasterizerState->enableScissor ? TRUE : FALSE);
    desc.MultisampleEnable = (rasterizerState->enableMultisample ? TRUE : FALSE);
    desc.AntialiasedLineEnable = (rasterizerState->enableAntialiasedLine ? TRUE : FALSE);

    // Create the rasterizer state.
    ID3D11RasterizerState* state = nullptr;
    DX11Log(device->CreateRasterizerState(&desc, &state));
    mDXObject = state;
}

std::shared_ptr<GEObject> DX11RasterizerState::Create(void* device, GraphicsObject const* object)
{
    GTL_RUNTIME_ASSERT(
        object->GetType() == GT_RASTERIZER_STATE,
        "Invalid object type.");

    return std::make_shared<DX11RasterizerState>(
        reinterpret_cast<ID3D11Device*>(device),
        static_cast<RasterizerState const*>(object));
}

void DX11RasterizerState::Enable(ID3D11DeviceContext* context)
{
    context->RSSetState(GetDXRasterizerState());
}


D3D11_FILL_MODE const DX11RasterizerState::msFillMode[] =
{
    D3D11_FILL_SOLID,
    D3D11_FILL_WIREFRAME
};

D3D11_CULL_MODE const DX11RasterizerState::msCullMode[] =
{
    D3D11_CULL_NONE,
    D3D11_CULL_FRONT,
    D3D11_CULL_BACK
};
