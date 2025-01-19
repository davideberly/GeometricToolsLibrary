// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#pragma once

#include <GTL/Graphics/Resources/Textures/TextureRT.h>
#include <GTL/Graphics/DX11/Resources/Textures/DX11Texture2.h>

namespace gtl
{
    class DX11TextureRT : public DX11Texture2
    {
    public:
        // Construction and destruction.
        virtual ~DX11TextureRT();
        DX11TextureRT(ID3D11Device* device, TextureRT const* texture);
        DX11TextureRT(ID3D11Device* device, DX11TextureRT const* dxSharedTexture);
        static std::shared_ptr<GEObject> Create(void* device, GraphicsObject const* object);

        // Member access.
        inline ID3D11RenderTargetView* GetRTView() const
        {
            return mRTView;
        }

        // Support for the DX11 debug layer; see comments in the file
        // DX11GraphicsObject.h about usage.
        virtual void SetName(std::string const& name);

    private:
        // Support for construction.
        void CreateRTView(ID3D11Device* device, D3D11_TEXTURE2D_DESC const& tx);

        ID3D11RenderTargetView* mRTView;
    };
}
