// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#pragma once

#include <GTL/Graphics/Resources/Textures/Texture.h>
#include <cstdint>

namespace gtl
{
    class TextureSingle : public Texture
    {
    protected:
        // Abstract base class.
        TextureSingle(uint32_t format, uint32_t numDimensions,
            uint32_t dim0, uint32_t dim1, uint32_t dim2,
            bool hasMipmaps, bool createStorage);

    public:
        // Mipmap information.
        inline uint32_t GetOffsetFor(uint32_t level) const
        {
            return Texture::GetOffsetFor(0, level);
        }

        inline char const* GetDataFor(uint32_t level) const
        {
            return Texture::GetDataFor(0, level);
        }

        inline char* GetDataFor(uint32_t level)
        {
            return Texture::GetDataFor(0, level);
        }

        template <typename T>
        inline T const* GetFor(uint32_t level) const
        {
            return Texture::GetFor<T>(0, level);
        }

        template <typename T>
        inline T* GetFor(uint32_t level)
        {
            return Texture::GetFor<T>(0, level);
        }

    public:
        // For use by the Shader class for storing reflection information.
        static int32_t const shaderDataLookup = 4;
    };
}
