// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#include <GTLGraphicsDX11PCH.h>
#include <GTL/Utility/Exceptions.h>
#include <GTL/Graphics/DX11/Resources/Buffers/DX11ConstantBuffer.h>
using namespace gtl;

DX11ConstantBuffer::DX11ConstantBuffer(ID3D11Device* device, ConstantBuffer const* cbuffer)
    :
    DX11Buffer(cbuffer)
{
    // Specify the buffer description.
    D3D11_BUFFER_DESC desc;
    desc.ByteWidth = cbuffer->GetNumBytes();
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    desc.MiscFlags = D3D11_RESOURCE_MISC_NONE;
    desc.StructureByteStride = 0;
    uint32_t usage = cbuffer->GetUsage();
    if (usage == Resource::Usage::IMMUTABLE)
    {
        desc.Usage = D3D11_USAGE_IMMUTABLE;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_NONE;
    }
    else if (usage == Resource::Usage::DYNAMIC_UPDATE)
    {
        desc.Usage = D3D11_USAGE_DYNAMIC;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    }
    else
    {
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_NONE;
    }

    // Create the buffer.
    ID3D11Buffer* buffer = nullptr;
    if (cbuffer->GetData())
    {
        D3D11_SUBRESOURCE_DATA data;
        data.pSysMem = cbuffer->GetData();
        data.SysMemPitch = 0;
        data.SysMemSlicePitch = 0;
        DX11Log(device->CreateBuffer(&desc, &data, &buffer));
    }
    else
    {
        DX11Log(device->CreateBuffer(&desc, nullptr, &buffer));
    }
    mDXObject = buffer;

    // Create a staging buffer if requested.
    if (cbuffer->GetCopy() != Resource::Copy::NONE)
    {
        CreateStaging(device, desc);
    }
}

std::shared_ptr<GEObject> DX11ConstantBuffer::Create(void* device, GraphicsObject const* object)
{
    GTL_RUNTIME_ASSERT(
        object->GetType() == GT_CONSTANT_BUFFER,
        "Invalid object type.");

    return std::make_shared<DX11ConstantBuffer>(
        reinterpret_cast<ID3D11Device*>(device),
        static_cast<ConstantBuffer const*>(object));
}
