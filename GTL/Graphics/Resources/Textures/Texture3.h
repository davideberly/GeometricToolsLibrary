// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#pragma once

#include <GTL/Graphics/Resources/Textures/TextureSingle.h>
#include <cstdint>

namespace gtl
{
    class Texture3 : public TextureSingle
    {
    public:
        // Construction.
        Texture3(uint32_t format, uint32_t width, uint32_t height,
            uint32_t thickness, bool hasMipmaps = false, bool createStorage = true);

        // Texture dimensions.
        inline uint32_t GetWidth() const
        {
            return TextureSingle::GetDimension(0);
        }

        inline uint32_t GetHeight() const
        {
            return TextureSingle::GetDimension(1);
        }

        inline uint32_t GetThickness() const
        {
            return TextureSingle::GetDimension(2);
        }
    };
}
