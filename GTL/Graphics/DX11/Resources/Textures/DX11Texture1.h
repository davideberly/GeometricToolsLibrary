// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#pragma once

#include <GTL/Graphics/Resources/Textures/Texture1.h>
#include <GTL/Graphics/DX11/Resources/Textures/DX11TextureSingle.h>

namespace gtl
{
    class DX11Texture1 : public DX11TextureSingle
    {
    public:
        // Construction.
        DX11Texture1(ID3D11Device* device, Texture1 const* texture);
        static std::shared_ptr<GEObject> Create(void* device, GraphicsObject const* object);

        // Member access.
        inline Texture1* GetTexture() const
        {
            return static_cast<Texture1*>(mGTObject);
        }

        inline ID3D11Texture1D* GetDXTexture() const
        {
            return static_cast<ID3D11Texture1D*>(mDXObject);
        }

    private:
        // Support for construction.
        void CreateStaging(ID3D11Device* device, D3D11_TEXTURE1D_DESC const& tx);
        void CreateSRView(ID3D11Device* device, D3D11_TEXTURE1D_DESC const& tx);
        void CreateUAView(ID3D11Device* device, D3D11_TEXTURE1D_DESC const& tx);
    };
}
