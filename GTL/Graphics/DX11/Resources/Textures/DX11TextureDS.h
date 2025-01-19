// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#pragma once

#include <GTL/Graphics/Resources/Textures/TextureDS.h>
#include <GTL/Graphics/DX11/Resources/Textures/DX11Texture2.h>

namespace gtl
{
    class DX11TextureDS : public DX11Texture2
    {
    public:
        // Construction and destruction.
        virtual ~DX11TextureDS();
        DX11TextureDS(ID3D11Device* device, TextureDS const* texture);
        DX11TextureDS(ID3D11Device* device, DX11TextureDS const* dxSharedTexture);
        static std::shared_ptr<GEObject> Create(void* device, GraphicsObject const* object);

        // Member access.
        inline ID3D11DepthStencilView* GetDSView() const
        {
            return mDSView;
        }

        // Support for the DX11 debug layer; see comments in the file
        // DX11GraphicsObject.h about usage.
        virtual void SetName(std::string const& name);

    private:
        // Support for construction.
        void CreateDSView(ID3D11Device* device);
        void CreateDSSRView(ID3D11Device* device);
        DXGI_FORMAT GetDepthResourceFormat(DXGI_FORMAT depthFormat);
        DXGI_FORMAT GetDepthSRVFormat(DXGI_FORMAT depthFormat);

        ID3D11DepthStencilView* mDSView;
    };
}
