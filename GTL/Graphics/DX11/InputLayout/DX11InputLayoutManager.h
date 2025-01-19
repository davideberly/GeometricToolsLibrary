// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#pragma once

#include <GTL/Graphics/Base/GEInputLayoutManager.h>
#include <GTL/Graphics/DX11/InputLayout/DX11InputLayout.h>
#include <map>
#include <mutex>

namespace gtl
{
    class DX11InputLayoutManager : public GEInputLayoutManager
    {
    public:
        // Construction and destruction.
        DX11InputLayoutManager()
            :
            mMap{},
            mMutex{}
        {
        }

        virtual ~DX11InputLayoutManager();

        // Management functions.  The Unbind(vbuffer) removes all pairs that
        // involve vbuffer.  The Unbind(vshader) removes all pairs that
        // involve vshader.
        DX11InputLayout* Bind(ID3D11Device* device, VertexBuffer const* vbuffer, Shader const* vshader);
        virtual bool Unbind(VertexBuffer const* vbuffer) override;
        virtual bool Unbind(Shader const* vshader) override;
        virtual void UnbindAll() override;
        virtual bool HasElements() const override;

    private:
        typedef std::pair<VertexBuffer const*, Shader const*> VBSPair;
        std::map<VBSPair, std::shared_ptr<DX11InputLayout>> mMap;
        mutable std::mutex mMutex;
    };
}
