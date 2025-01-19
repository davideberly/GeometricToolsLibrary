// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#pragma once

#include <GTL/Graphics/Resources/Textures/Texture2Array.h>
#include <GTL/Graphics/DX11/Resources/Textures/DX11TextureArray.h>

namespace gtl
{
    class DX11Texture2Array : public DX11TextureArray
    {
    public:
        // Construction and destruction.
        virtual ~DX11Texture2Array() = default;
        DX11Texture2Array(ID3D11Device* device, Texture2Array const* textureArray);
        static std::shared_ptr<GEObject> Create(void* device, GraphicsObject const* object);

        // Member access.
        inline Texture2Array* GetTextureArray() const
        {
            return static_cast<Texture2Array*>(mGTObject);
        }

        inline ID3D11Texture2D* GetDXTextureArray() const
        {
            return static_cast<ID3D11Texture2D*>(mDXObject);
        }

    private:
        // Support for construction.
        void CreateStaging(ID3D11Device* device, D3D11_TEXTURE2D_DESC const& tx);
        void CreateSRView(ID3D11Device* device, D3D11_TEXTURE2D_DESC const& tx);
        void CreateUAView(ID3D11Device* device, D3D11_TEXTURE2D_DESC const& tx);
    };
}
