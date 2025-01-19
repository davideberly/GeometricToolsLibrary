// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#pragma once

#include <GTL/Graphics/Resources/Textures/TextureArray.h>
#include <cstdint>

namespace gtl
{
    class Texture2Array : public TextureArray
    {
    public:
        // Construction.
        Texture2Array(uint32_t numItems, uint32_t format, uint32_t width,
            uint32_t height, bool hasMipmaps = false, bool createStorage = true);

        // Texture dimensions.
        inline uint32_t GetWidth() const
        {
            return TextureArray::GetDimension(0);
        }

        inline uint32_t GetHeight() const
        {
            return TextureArray::GetDimension(1);
        }
    };
}
