// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#pragma once

#include <GTL/Graphics/Resources/Textures/TextureSingle.h>
#include <GTL/Graphics/DX11/Resources/Textures/DX11Texture.h>

namespace gtl
{
    class DX11TextureSingle : public DX11Texture
    {
    public:
        // Abstract base class, a shim to distinguish between single textures
        // and texture arrays.
        virtual ~DX11TextureSingle() = default;
    protected:
        DX11TextureSingle(TextureSingle const* gtTextureSingle);

    public:
        // Member access.
        inline TextureSingle* GetTextureSingle() const
        {
            return static_cast<TextureSingle*>(mGTObject);
        }
    };
}
