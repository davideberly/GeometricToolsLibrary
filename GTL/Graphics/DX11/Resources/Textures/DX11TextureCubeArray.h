// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#pragma once

#include <GTL/Graphics/Resources/Textures/TextureCubeArray.h>
#include <GTL/Graphics/DX11/Resources/Textures/DX11TextureArray.h>

namespace gtl
{
    class DX11TextureCubeArray : public DX11TextureArray
    {
    public:
        // Construction and destruction.
        virtual ~DX11TextureCubeArray() = default;
        DX11TextureCubeArray(ID3D11Device* device, TextureCubeArray const* textureCubeArray);
        static std::shared_ptr<GEObject> Create(void* device, GraphicsObject const* object);

        // Member access.
        inline TextureCubeArray* GetTextureArray() const
        {
            return static_cast<TextureCubeArray*>(mGTObject);
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
