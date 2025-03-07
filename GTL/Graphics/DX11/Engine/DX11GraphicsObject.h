// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#pragma once

#include <GTL/Graphics/Base/GEObject.h>
#include <GTL/Graphics/DX11/Engine/DX11.h>

namespace gtl
{
    class DX11GraphicsObject : public GEObject
    {
    public:
        // Abstract base class.
        virtual ~DX11GraphicsObject();
    protected:
        DX11GraphicsObject(GraphicsObject const* gtObject);

    public:
        // Member access.
        inline ID3D11DeviceChild* GetDXDeviceChild() const
        {
            return mDXObject;
        }

        // Support for the DX11 debug layer.  Set the name if you want to have
        // ID3D11DeviceChild destruction messages show your name rather than
        // "<unnamed>".  The typical usage is
        //   auto texture = std::make_shared<Texture2>(...);
        //   engine->Bind(texture)->SetName("MyTexture");
        // The virtual override is used to allow derived classes to use the
        // same name for associated resources.
        virtual void SetName(std::string const& name) override;

    protected:
        ID3D11DeviceChild* mDXObject;
    };
}
