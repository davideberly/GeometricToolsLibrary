// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.03.27

#pragma once

#include <GTL/Utility/Exceptions.h>
#include <GTL/Utility/Multiarray.h>
#include <array>
#include <cstddef>
#include <cstdint>
#include <string>

namespace gtl
{
    template <typename PixelType>
    class Image2 : public Multiarray<PixelType, true>
    {
    public:
        // Construction and destruction.
        Image2() = default;
        virtual ~Image2() = default;

        Image2(std::size_t dimension0, std::size_t dimension1)
            :
            Multiarray<PixelType, true>({ dimension0, dimension1 })
        {
        }

        // Use default copy and move semantics.
        Image2(Image2 const&) = default;
        Image2& operator=(Image2 const&) = default;
        Image2(Image2&&) noexcept = default;
        Image2& operator=(Image2&& image) noexcept = default;

        // Support for resizing the image. All pixel data is lost by this
        // operation.
        void resize(std::size_t dimension0, std::size_t dimension1)
        {
            Multiarray<PixelType, true>::resize({ dimension0, dimension1 });
        }

        // Get the relative offsets for a neighborhood, accessed as a
        // 1-dimensional array. The indices are relative to any pixel
        // location.
        void GetNeighborhood(std::array<std::int64_t, 4>& nbr) const
        {
            std::int64_t dim0 = static_cast<std::int64_t>(this->size(0));
            nbr[0] = -1;    // (x-1,y)
            nbr[1] = +1;    // (x+1,y)
            nbr[2] = -dim0; // (x,y-1)
            nbr[3] = +dim0; // (x,y+1)
        }

        void GetNeighborhood(std::array<std::int64_t, 8>& nbr) const
        {
            std::int64_t dim0 = static_cast<std::int64_t>(this->size(0));
            nbr[0] = -1;        // (x-1,y)
            nbr[1] = +1;        // (x+1,y)
            nbr[2] = -dim0;     // (x,y-1)
            nbr[3] = +dim0;     // (x,y+1)
            nbr[4] = -1 - dim0; // (x-1,y-1)
            nbr[5] = +1 - dim0; // (x+1,y-1)
            nbr[6] = -1 + dim0; // (x-1,y+1)
            nbr[7] = +1 + dim0; // (x+1,y+1)
        }

        void GetNeighborhood(std::array<std::int64_t, 9>& nbr) const
        {
            std::int64_t dim0 = static_cast<std::int64_t>(this->size(0));
            nbr[0] = -1 - dim0; // (x-1,y-1)
            nbr[1] = -dim0;     // (x,y-1)
            nbr[2] = +1 - dim0; // (x+1,y-1)
            nbr[3] = -1;        // (x-1,y)
            nbr[4] = 0;         // (x,y)
            nbr[5] = +1;        // (x+1,y)
            nbr[6] = -1 + dim0; // (x-1,y+1)
            nbr[7] = +dim0;     // (x,y+1)
            nbr[8] = +1 + dim0; // (x+1,y+1)
        }

        // Get the relative offsets for a neighborhood, accessed as a
        // 2-dimensional array. The 2-tuples are relative to any pixel
        // location.
        void GetNeighborhood(std::array<std::array<std::int64_t, 2>, 4>& nbr) const
        {
            nbr[0] = { { -1, 0 } };
            nbr[1] = { { +1, 0 } };
            nbr[2] = { { 0, -1 } };
            nbr[3] = { { 0, +1 } };
        }

        void GetNeighborhood(std::array<std::array<std::int64_t, 2>, 8>& nbr) const
        {
            nbr[0] = { { -1, -1 } };
            nbr[1] = { { 0, -1 } };
            nbr[2] = { { +1, -1 } };
            nbr[3] = { { -1, 0 } };
            nbr[4] = { { +1, 0 } };
            nbr[5] = { { -1, +1 } };
            nbr[6] = { { 0, +1 } };
            nbr[7] = { { +1, +1 } };
        }

        void GetNeighborhood(std::array<std::array<std::int64_t, 2>, 9>& nbr) const
        {
            nbr[0] = { { -1, -1 } };
            nbr[1] = { { 0, -1 } };
            nbr[2] = { { +1, -1 } };
            nbr[3] = { { -1, 0 } };
            nbr[4] = { { 0, 0 } };
            nbr[5] = { { +1, 0 } };
            nbr[6] = { { -1, +1 } };
            nbr[7] = { { 0, +1 } };
            nbr[8] = { { +1, +1 } };
        }

        // Get the locations for a neighborhood of (x,y), accessed as a
        // 1-dimensional array. The input (x,y) is required to be strictly
        // inside the image. If you require neighborhoods of boundary pixels,
        // you must computer your own or use GetNeighborhood(...) and extract
        // the relevant elements.
        inline void GetNeighborhood(std::size_t x, std::size_t y,
            std::array<std::size_t, 4>& nbr) const
        {
            GetNeighborhood1D(x, y, nbr);
        }

        inline void GetNeighborhood(std::size_t x, std::size_t y,
            std::array<std::size_t, 8>& nbr) const
        {
            GetNeighborhood1D(x, y, nbr);
        }

        inline void GetNeighborhood(std::size_t x, std::size_t y,
            std::array<std::size_t, 9>& nbr) const
        {
            GetNeighborhood1D(x, y, nbr);
        }

        // Get the locations for a neighborhood of (x,y), accessed as a
        // 2-dimensional array. The input (x,y) is required to be strictly
        // inside the image. If you require neighborhoods of boundary pixels,
        // you must computer your own or use GetNeighborhood(...) and extract
        // the relevant elements.
        void GetNeighborhood(std::size_t x, std::size_t y,
            std::array<std::array<std::size_t, 2>, 4>& nbr) const
        {
            GetNeighborhood2D(x, y, nbr);
        }

        void GetNeighborhood(std::size_t x, std::size_t y,
            std::array<std::array<std::size_t, 2>, 8>& nbr) const
        {
            GetNeighborhood2D(x, y, nbr);
        }

        void GetNeighborhood(std::size_t x, std::size_t y,
            std::array<std::array<std::size_t, 2>, 9>& nbr) const
        {
            GetNeighborhood2D(x, y, nbr);
        }

    protected:
        template <std::size_t n>
        void GetNeighborhood1D(std::size_t x, std::size_t y,
            std::array<std::size_t, n>& nbr) const
        {
            auto const& dim = this->mSizes;
            GTL_OUTOFRANGE_ASSERT(
                1 <= x && x + 1 <= dim[0] && 1 <= y && y + 1 <= dim[1],
                "Invalid (" + std::to_string(x) + "," + std::to_string(y) + ").");

            std::int64_t index = static_cast<std::int64_t>(x + dim[0] * y);
            std::array<std::int64_t, n> inbr{};
            GetNeighborhood(inbr);
            for (std::size_t i = 0; i < n; ++i)
            {
                nbr[i] = static_cast<std::size_t>(index + inbr[i]);
            }
        }

        template <std::size_t n>
        void GetNeighborhood2D(std::size_t x, std::size_t y,
            std::array<std::array<std::size_t, 2>, n>& nbr) const
        {
            auto const& dim = this->mSizes;
            GTL_OUTOFRANGE_ASSERT(
                1 <= x && x + 1 <= dim[0] && 1 <= y && y + 1 <= dim[1],
                "Invalid (" + std::to_string(x) + "," + std::to_string(y) + ").");

            std::int64_t ix = static_cast<std::int64_t>(x);
            std::int64_t iy = static_cast<std::int64_t>(y);

            std::array<std::array<std::int64_t, 2>, n> inbr{};
            GetNeighborhood(inbr);
            for (std::size_t i = 0; i < n; ++i)
            {
                nbr[i][0] = static_cast<std::size_t>(ix + inbr[i][0]);
                nbr[i][1] = static_cast<std::size_t>(iy + inbr[i][1]);
            }
        }

    private:
        friend class UnitTestImage2;
    };
}
