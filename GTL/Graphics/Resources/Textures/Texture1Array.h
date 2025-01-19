// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#pragma once

#include <GTL/Graphics/Resources/Textures/TextureArray.h>
#include <cstdint>

namespace gtl
{
    class Texture1Array : public TextureArray
    {
    public:
        // Construction.
        Texture1Array(uint32_t numItems, uint32_t format, uint32_t length,
            bool hasMipmaps = false, bool createStorage = true);

        // Texture dimensions.
        inline uint32_t GetLength() const
        {
            return TextureArray::GetDimension(0);
        }
    };
}
