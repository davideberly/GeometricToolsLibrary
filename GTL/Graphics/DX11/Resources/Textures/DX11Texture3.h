// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#pragma once

#include <GTL/Graphics/Resources/Textures/Texture3.h>
#include <GTL/Graphics/DX11/Resources/Textures/DX11TextureSingle.h>

namespace gtl
{
    class DX11Texture3 : public DX11TextureSingle
    {
    public:
        // Construction and destruction.
        virtual ~DX11Texture3() = default;
        DX11Texture3(ID3D11Device* device, Texture3 const* texture);
        static std::shared_ptr<GEObject> Create(void* device, GraphicsObject const* object);

        // Member access.
        inline Texture3* GetTexture() const
        {
            return static_cast<Texture3*>(mGTObject);
        }

        inline ID3D11Texture3D* GetDXTexture() const
        {
            return static_cast<ID3D11Texture3D*>(mDXObject);
        }

    private:
        // Support for construction.
        void CreateStaging(ID3D11Device* device, D3D11_TEXTURE3D_DESC const& tx);
        void CreateSRView(ID3D11Device* device, D3D11_TEXTURE3D_DESC const& tx);
        void CreateUAView(ID3D11Device* device, D3D11_TEXTURE3D_DESC const& tx);
    };
}
