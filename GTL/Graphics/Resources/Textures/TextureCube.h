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
    class TextureCube : public TextureArray
    {
    public:
        // Construction.  Cube maps must be square; the 'length' parameter is
        // the shared value for width and height.
        TextureCube(uint32_t format, uint32_t length, bool hasMipmaps = false,
            bool createStorage = true);

        // The texture width and height are the same value.
        inline uint32_t GetLength() const
        {
            return TextureArray::GetDimension(0);
        }
    };
}
