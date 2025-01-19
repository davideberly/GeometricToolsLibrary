// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#pragma once

#include <GTL/Graphics/Resources/Textures/Texture2.h>
#include <cstdint>

namespace gtl
{
    class TextureRT : public Texture2
    {
    public:
        // Construction for render targets.
        TextureRT(uint32_t format, uint32_t width, uint32_t height,
            bool hasMipmaps = false, bool createStorage = true);
    };
}
