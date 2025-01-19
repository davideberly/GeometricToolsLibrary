// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#pragma once

#include <GTL/Graphics/Resources/Textures/Texture1Array.h>
#include <GTL/Graphics/DX11/Resources/Textures/DX11TextureArray.h>

namespace gtl
{
    class DX11Texture1Array : public DX11TextureArray
    {
    public:
        // Construction and destruction.
        virtual ~DX11Texture1Array() = default;
        DX11Texture1Array(ID3D11Device* device, Texture1Array const* textureArray);
        static std::shared_ptr<GEObject> Create(void* device, GraphicsObject const* object);

        // Member access.
        inline Texture1Array* GetTextureArray() const
        {
            return static_cast<Texture1Array*>(mGTObject);
        }

        inline ID3D11Texture1D* GetDXTextureArray() const
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
