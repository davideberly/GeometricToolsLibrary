// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#include <GTLGraphicsDX11PCH.h>
#include <GTL/Utility/Exceptions.h>
#include <GTL/Graphics/DX11/Resources/Buffers/DX11IndirectArgumentsBuffer.h>
using namespace gtl;

DX11IndirectArgumentsBuffer::DX11IndirectArgumentsBuffer(ID3D11Device* device, IndirectArgumentsBuffer const* iabuffer)
    :
    DX11Buffer(iabuffer)
{
    // Specify the counter buffer description.
    D3D11_BUFFER_DESC desc;
    desc.ByteWidth = iabuffer->GetNumBytes();
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_NONE;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_NONE;
    desc.MiscFlags = D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS;
    desc.StructureByteStride = 0;

    // Create the counter buffer.
    ID3D11Buffer* buffer = nullptr;
    if (iabuffer->GetData())
    {
        D3D11_SUBRESOURCE_DATA data;
        data.pSysMem = iabuffer->GetData();
        data.SysMemPitch = 0;
        data.SysMemSlicePitch = 0;
        DX11Log(device->CreateBuffer(&desc, &data, &buffer));
    }
    else
    {
        DX11Log(device->CreateBuffer(&desc, nullptr, &buffer));
    }
    mDXObject = buffer;
}

std::shared_ptr<GEObject> DX11IndirectArgumentsBuffer::Create(void* device, GraphicsObject const* object)
{
    GTL_RUNTIME_ASSERT(
        object->GetType() == GT_INDIRECT_ARGUMENTS_BUFFER,
        "Invalid object type.");

    return std::make_shared<DX11IndirectArgumentsBuffer>(
        reinterpret_cast<ID3D11Device*>(device),
        static_cast<IndirectArgumentsBuffer const*>(object));
}
