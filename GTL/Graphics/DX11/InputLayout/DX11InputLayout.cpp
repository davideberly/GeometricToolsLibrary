// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#include <GTLGraphicsDX11PCH.h>
#include <GTL/Utility/Exceptions.h>
#include <GTL/Graphics/DX11/InputLayout/DX11InputLayout.h>
using namespace gtl;

DX11InputLayout::~DX11InputLayout()
{
    DX11::FinalRelease(mLayout);
}

DX11InputLayout::DX11InputLayout(ID3D11Device* device,
    VertexBuffer const* vbuffer, Shader const* vshader)
    :
    mLayout(nullptr),
    mNumElements(0)
{
    GTL_ARGUMENT_ASSERT(
        vbuffer != nullptr && vshader != nullptr,
        "Invalid inputs.");

    std::memset(&mElements[0], 0, VAConstant::MAX_ATTRIBUTES*sizeof(mElements[0]));

    VertexFormat const& format = vbuffer->GetFormat();
    mNumElements = format.GetNumAttributes();
    for (int32_t i = 0; i < mNumElements; ++i)
    {
        VASemantic semantic{};
        DFType type{};
        uint32_t unit{}, offset{};
        format.GetAttribute(i, semantic, type, unit, offset);

        D3D11_INPUT_ELEMENT_DESC& element = mElements[i];
        element.SemanticName = msSemantic[semantic];
        element.SemanticIndex = unit;
        element.Format = static_cast<DXGI_FORMAT>(type);
        element.InputSlot = 0;  // TODO: Streams not yet supported.
        element.AlignedByteOffset = offset;
        element.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        element.InstanceDataStepRate = 0;
    }

    auto const& compiledCode = vshader->GetCompiledCode();
    DX11Log(device->CreateInputLayout(mElements, (UINT)mNumElements,
        &compiledCode[0], compiledCode.size(), &mLayout));
}

void DX11InputLayout::Enable(ID3D11DeviceContext* context)
{
    if (mLayout)
    {
        context->IASetInputLayout(mLayout);
    }
}

void DX11InputLayout::Disable(ID3D11DeviceContext* context)
{
    if (mLayout)
    {
        // TODO: Verify that mLayout is the active input layout.
        context->IASetInputLayout(nullptr);
    }
}

HRESULT DX11InputLayout::SetName(std::string const& name)
{
    mName = name;
    return DX11::SetPrivateName(mLayout, mName);
}


char const* DX11InputLayout::msSemantic[VASemantic::NUM_SEMANTICS] =
{
    "",
    "POSITION",
    "BLENDWEIGHT",
    "BLENDINDICES",
    "NORMAL",
    "PSIZE",
    "TEXCOORD",
    "TANGENT",
    "BINORMAL",
    "TESSFACTOR",
    "POSITIONT",
    "COLOR",
    "FOG",
    "DEPTH",
    "SAMPLE"
};
