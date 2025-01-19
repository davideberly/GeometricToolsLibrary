// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#pragma once

#include <GTL/Graphics/Base/GEDrawTarget.h>
#include <GTL/Graphics/DX11/Resources/Textures/DX11TextureDS.h>
#include <GTL/Graphics/DX11/Resources/Textures/DX11TextureRT.h>

namespace gtl
{
    class DX11DrawTarget : public GEDrawTarget
    {
    public:
        // Construction and destruction.
        virtual ~DX11DrawTarget() = default;
        DX11DrawTarget(DrawTarget const* target,
            std::vector<DX11TextureRT*>& rtTextures, DX11TextureDS* dsTexture);

        static std::shared_ptr<GEDrawTarget> Create(DrawTarget const* target,
            std::vector<GEObject*>& rtTextures, GEObject* dsTexture);

        // Member access.
        inline DX11TextureRT* GetRTTexture(uint32_t i) const
        {
            return mRTTextures[i];
        }

        inline DX11TextureDS* GetDSTexture() const
        {
            return mDSTexture;
        }

        // Used in the Renderer::Draw function.
        void Enable(ID3D11DeviceContext* context);
        void Disable(ID3D11DeviceContext* context);

    private:
        std::vector<DX11TextureRT*> mRTTextures;
        DX11TextureDS* mDSTexture;

        // Convenient storage for enable/disable of targets.
        std::vector<ID3D11RenderTargetView*> mRTViews;
        ID3D11DepthStencilView* mDSView;

        // Temporary storage during enable/disable of targets.
        D3D11_VIEWPORT mSaveViewport;
        std::vector<ID3D11RenderTargetView*> mSaveRTViews;
        ID3D11DepthStencilView* mSaveDSView;
    };
}
