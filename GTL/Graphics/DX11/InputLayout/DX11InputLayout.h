// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#pragma once

#include <GTL/Graphics/Shaders/Shader.h>
#include <GTL/Graphics/Resources/Buffers/VertexBuffer.h>
#include <GTL/Graphics/DX11/Engine/DX11.h>

namespace gtl
{
    class DX11InputLayout
    {
    public:
        // Construction and destruction.
        ~DX11InputLayout();
        DX11InputLayout(ID3D11Device* device, VertexBuffer const* vbuffer, Shader const* vshader);

        // Support for drawing geometric primitives.
        void Enable(ID3D11DeviceContext* context);
        void Disable(ID3D11DeviceContext* context);

        // Support for the DX11 debug layer; see comments in the file
        // DX11GraphicsObject.h about usage.
        HRESULT SetName(std::string const& name);

        inline std::string const& GetName() const
        {
            return mName;
        }

    private:
        ID3D11InputLayout* mLayout;
        int32_t mNumElements;
        D3D11_INPUT_ELEMENT_DESC mElements[VAConstant::MAX_ATTRIBUTES];
        std::string mName;

        // Conversions from GTL values to DX11 values.
        static char const* msSemantic[VASemantic::NUM_SEMANTICS];
    };
}
