// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#include <GTLGraphicsDX11PCH.h>
#include <GTL/Graphics/DX11/Engine/DX11GraphicsObject.h>
using namespace gtl;

DX11GraphicsObject::~DX11GraphicsObject()
{
    if (mGTObject && mGTObject->IsDrawingState())
    {
        // Sampler, blend, depth-stencil, and rasterizer states have only a
        // finite number of possibilities in DX11.  If you create a state
        // whose settings duplicate one already in existence, DX11 gives you
        // a pointer to the existing one, incrementing the reference count
        // internally.  GTL does not track the duplicates, so we cannot
        // assert that the reference count is zero.
        DX11::SafeRelease(mDXObject);
    }
    else
    {
        DX11::FinalRelease(mDXObject);
    }
}

DX11GraphicsObject::DX11GraphicsObject(GraphicsObject const* gtObject)
    :
    GEObject(gtObject),
    mDXObject(nullptr)
{
}

void DX11GraphicsObject::SetName(std::string const& name)
{
    mName = name;
    DX11Log(DX11::SetPrivateName(mDXObject, mName));
}
