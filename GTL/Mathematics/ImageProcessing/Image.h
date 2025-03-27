// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.03.27

#pragma once

#include <GTL/Utility/Exceptions.h>
#include <GTL/Utility/Multiarray.h>
#include <cstddef>

namespace gtl
{
    template <typename PixelType>
    class Image : public Multiarray<PixelType, true>
    {
    public:
        // Construction and destruction.
        Image() = default;
        virtual ~Image() = default;

        Image(std::vector<std::size_t> const& dimensions)
            :
            Multiarray<PixelType, true>(dimensions)
        {
        }

        // Use default copy and move semantics.
        Image(Image const&) = default;
        Image& operator=(Image const&) = default;
        Image(Image&&) noexcept = default;
        Image& operator=(Image&& image) noexcept = default;

    private:
        friend class UnitTestImage;
    };
}
