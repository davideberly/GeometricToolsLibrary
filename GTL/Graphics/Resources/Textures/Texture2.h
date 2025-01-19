// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#pragma once

#include <GTL/Graphics/Resources/Textures/TextureSingle.h>
#include <cstdint>

namespace gtl
{
    class Texture2 : public TextureSingle
    {
    public:
        // Construction.
        Texture2(uint32_t format, uint32_t width, uint32_t height,
            bool hasMipmaps = false, bool createStorage = true);

        // Texture dimensions.
        inline uint32_t GetWidth() const
        {
            return TextureSingle::GetDimension(0);
        }

        inline uint32_t GetHeight() const
        {
            return TextureSingle::GetDimension(1);
        }

        // If you intend to share this texture among graphics engine objects,
        // call this function before binding the texture to the engine.
        // Currently, shared textures are supported only by the DX graphics
        // engine.
        inline void MakeShared()
        {
            // Shared textures are required to be GPU writable.
            mUsage = Usage::SHADER_OUTPUT;
            mShared = true;
        }

        inline bool IsShared() const
        {
            return mShared;
        }

    protected:
        bool mShared;
    };
}
