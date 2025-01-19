// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#include <GTLGraphicsDX11PCH.h>
#include <GTL/Utility/Exceptions.h>
#include <GTL/Graphics/DX11/Resources/DX11Resource.h>
using namespace gtl;

DX11Resource::~DX11Resource()
{
    DX11::FinalRelease(mStaging);
}

DX11Resource::DX11Resource(Resource const* gtResource)
    :
    DX11GraphicsObject(gtResource),
    mStaging(nullptr)
{
    // Derived classes must create the staging resource, because DX11 does
    // not have a generic description structure that could be used here
    // otherwise.
}

D3D11_MAPPED_SUBRESOURCE DX11Resource::MapForWrite(ID3D11DeviceContext* context, uint32_t sri)
{
    D3D11_MAPPED_SUBRESOURCE mapped;
    DX11Log(context->Map(GetDXResource(), sri, D3D11_MAP_WRITE_DISCARD, 0, &mapped));
    return mapped;
}

void DX11Resource::Unmap(ID3D11DeviceContext* context, uint32_t sri)
{
    context->Unmap(GetDXResource(), sri);
}

void DX11Resource::SetName(std::string const& name)
{
    DX11GraphicsObject::SetName(name);
    DX11Log(DX11::SetPrivateName(mStaging, name));
}

void DX11Resource::PreparedForCopy(D3D11_CPU_ACCESS_FLAG access) const
{
    // Verify existence of objects.
    GTL_RUNTIME_ASSERT(
        mDXObject != nullptr,
        "DX object does not exist.");

    GTL_RUNTIME_ASSERT(
        mStaging != nullptr,
        "Staging object does not exist.");

    // Verify the copy type.
    GTL_RUNTIME_ASSERT(
        (msStagingAccess[GetResource()->GetCopy()] & access) != 0,
        "Invalid copy type.");
}


UINT const DX11Resource::msStagingAccess[] =
{
    D3D11_CPU_ACCESS_NONE,          // COPY_NONE
    D3D11_CPU_ACCESS_WRITE,         // COPY_CPU_TO_STAGING
    D3D11_CPU_ACCESS_READ,          // COPY_STAGING_TO_CPU
    D3D11_CPU_ACCESS_READ_WRITE     // COPY_BIDIRECTIONAL
};
