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
    class Image3 : public Multiarray<PixelType, true>
    {
    public:
        // Construction and destruction.
        Image3() = default;
        virtual ~Image3() = default;

        Image3(std::size_t dimension0, std::size_t dimension1, std::size_t dimension2)
            :
            Multiarray<PixelType, true>({ dimension0, dimension1, dimension2 })
        {
        }

        // Use default copy and move semantics.
        Image3(Image3 const&) = default;
        Image3& operator=(Image3 const&) = default;
        Image3(Image3&&) noexcept = default;
        Image3& operator=(Image3&& image) noexcept = default;

        // Support for resizing the image. All pixel data is lost by this
        // operation.
        void resize(std::size_t dimension0, std::size_t dimension1, std::size_t dimension2)
        {
            Multiarray<PixelType, true>::resize({ dimension0, dimension1, dimension2 });
        }

        // Get the relative offsets for a neighborhood, accessed as a
        // 1-dimensional array. The indices are relative to any pixel
        // location.
        void GetNeighborhood(std::array<std::int64_t, 6>& nbr) const
        {
            std::int64_t dim0 = static_cast<std::int64_t>(this->size(0));
            std::int64_t dim01 = static_cast<std::int64_t>(this->size(0) * this->size(1));
            nbr[0] = -1;        // (x-1,y,z)
            nbr[1] = +1;        // (x+1,y,z)
            nbr[2] = -dim0;     // (x,y-1,z)
            nbr[3] = +dim0;     // (x,y+1,z)
            nbr[4] = -dim01;    // (x,y,z-1)
            nbr[5] = +dim01;    // (x,y,z+1)
        }

        void GetNeighborhood(std::array<std::int64_t, 18>& nbr) const
        {
            std::int64_t dim0 = static_cast<std::int64_t>(this->size(0));
            std::int64_t dim01 = static_cast<std::int64_t>(this->size(0) * this->size(1));
            nbr[0] = -1;                // (x-1,y,z)
            nbr[1] = +1;                // (x+1,y,z)
            nbr[2] = -dim0;             // (x,y-1,z)
            nbr[3] = +dim0;             // (x,y+1,z)
            nbr[4] = -dim01;            // (x,y,z-1)
            nbr[5] = +dim01;            // (x,y,z+1)
            nbr[6] = -1 - dim0;         // (x-1,y-1,z)
            nbr[7] = +1 - dim0;         // (x+1,y-1,z)
            nbr[8] = -1 + dim0;         // (x-1,y+1,z)
            nbr[9] = +1 + dim0;         // (x+1,y+1,z)
            nbr[10] = -1 + dim01;       // (x-1,y,z+1)
            nbr[11] = +1 + dim01;       // (x+1,y,z+1)
            nbr[12] = -dim0 + dim01;    // (x,y-1,z+1)
            nbr[13] = +dim0 + dim01;    // (x,y+1,z+1)
            nbr[14] = -1 - dim01;       // (x-1,y,z-1)
            nbr[15] = +1 - dim01;       // (x+1,y,z-1)
            nbr[16] = -dim0 - dim01;    // (x,y-1,z-1)
            nbr[17] = +dim0 - dim01;    // (x,y+1,z-1)
        }

        void GetNeighborhood(std::array<std::int64_t, 26>& nbr) const
        {
            std::int64_t dim0 = static_cast<std::int64_t>(this->size(0));
            std::int64_t dim01 = static_cast<std::int64_t>(this->size(0) * this->size(1));
            nbr[0] = -1;                    // (x-1,y,z)
            nbr[1] = +1;                    // (x+1,y,z)
            nbr[2] = -dim0;                 // (x,y-1,z)
            nbr[3] = +dim0;                 // (x,y+1,z)
            nbr[4] = -dim01;                // (x,y,z-1)
            nbr[5] = +dim01;                // (x,y,z+1)
            nbr[6] = -1 - dim0;             // (x-1,y-1,z)
            nbr[7] = +1 - dim0;             // (x+1,y-1,z)
            nbr[8] = -1 + dim0;             // (x-1,y+1,z)
            nbr[9] = +1 + dim0;             // (x+1,y+1,z)
            nbr[10] = -1 + dim01;           // (x-1,y,z+1)
            nbr[11] = +1 + dim01;           // (x+1,y,z+1)
            nbr[12] = -dim0 + dim01;        // (x,y-1,z+1)
            nbr[13] = +dim0 + dim01;        // (x,y+1,z+1)
            nbr[14] = -1 - dim01;           // (x-1,y,z-1)
            nbr[15] = +1 - dim01;           // (x+1,y,z-1)
            nbr[16] = -dim0 - dim01;        // (x,y-1,z-1)
            nbr[17] = +dim0 - dim01;        // (x,y+1,z-1)
            nbr[18] = -1 - dim0 - dim01;    // (x-1,y-1,z-1)
            nbr[19] = +1 - dim0 - dim01;    // (x+1,y-1,z-1)
            nbr[20] = -1 + dim0 - dim01;    // (x-1,y+1,z-1)
            nbr[21] = +1 + dim0 - dim01;    // (x+1,y+1,z-1)
            nbr[22] = -1 - dim0 + dim01;    // (x-1,y-1,z+1)
            nbr[23] = +1 - dim0 + dim01;    // (x+1,y-1,z+1)
            nbr[24] = -1 + dim0 + dim01;    // (x-1,y+1,z+1)
            nbr[25] = +1 + dim0 + dim01;    // (x+1,y+1,z+1)
        }

        void GetNeighborhood(std::array<std::int64_t, 27>& nbr) const
        {
            std::int64_t dim0 = static_cast<std::int64_t>(this->size(0));
            std::int64_t dim01 = static_cast<std::int64_t>(this->size(0) * this->size(1));
            nbr[0] = -1 - dim0 - dim01;     // (x-1,y-1,z-1)
            nbr[1] = -dim0 - dim01;         // (x,  y-1,z-1)
            nbr[2] = +1 - dim0 - dim01;     // (x+1,y-1,z-1)
            nbr[3] = -1 - dim01;            // (x-1,y,  z-1)
            nbr[4] = -dim01;                // (x,  y,  z-1)
            nbr[5] = +1 - dim01;            // (x+1,y,  z-1)
            nbr[6] = -1 + dim0 - dim01;     // (x-1,y+1,z-1)
            nbr[7] = +dim0 - dim01;         // (x,  y+1,z-1)
            nbr[8] = +1 + dim0 - dim01;     // (x+1,y+1,z-1)
            nbr[9] = -1 - dim0;             // (x-1,y-1,z)
            nbr[10] = -dim0;                // (x,  y-1,z)
            nbr[11] = +1 - dim0;            // (x+1,y-1,z)
            nbr[12] = -1;                   // (x-1,y,  z)
            nbr[13] = 0;                    // (x,  y,  z)
            nbr[14] = +1;                   // (x+1,y,  z)
            nbr[15] = -1 + dim0;            // (x-1,y+1,z)
            nbr[16] = +dim0;                // (x,  y+1,z)
            nbr[17] = +1 + dim0;            // (x+1,y+1,z)
            nbr[18] = -1 - dim0 + dim01;    // (x-1,y-1,z+1)
            nbr[19] = -dim0 + dim01;        // (x,  y-1,z+1)
            nbr[20] = +1 - dim0 + dim01;    // (x+1,y-1,z+1)
            nbr[21] = -1 + dim01;           // (x-1,y,  z+1)
            nbr[22] = +dim01;               // (x,  y,  z+1)
            nbr[23] = +1 + dim01;           // (x+1,y,  z+1)
            nbr[24] = -1 + dim0 + dim01;    // (x-1,y+1,z+1)
            nbr[25] = +dim0 + dim01;        // (x,  y+1,z+1)
            nbr[26] = +1 + dim0 + dim01;    // (x+1,y+1,z+1)
        }

        // Get the relative offsets for a neighborhood, accessed as a
        // 3-dimensional array. The 3-tuples are relative to any pixel
        // location.
        void GetNeighborhood(std::array<std::array<std::int64_t, 3>, 6>& nbr) const
        {
            nbr[0] = { { -1, 0, 0 } };
            nbr[1] = { { +1, 0, 0 } };
            nbr[2] = { { 0, -1, 0 } };
            nbr[3] = { { 0, +1, 0 } };
            nbr[4] = { { 0, 0, -1 } };
            nbr[5] = { { 0, 0, +1 } };
        }

        void GetNeighborhood(std::array<std::array<std::int64_t, 3>, 18>& nbr) const
        {
            nbr[0] = { { -1, 0, 0 } };
            nbr[1] = { { +1, 0, 0 } };
            nbr[2] = { { 0, -1, 0 } };
            nbr[3] = { { 0, +1, 0 } };
            nbr[4] = { { 0, 0, -1 } };
            nbr[5] = { { 0, 0, +1 } };
            nbr[6] = { { -1, -1, 0 } };
            nbr[7] = { { +1, -1, 0 } };
            nbr[8] = { { -1, +1, 0 } };
            nbr[9] = { { +1, +1, 0 } };
            nbr[10] = { { -1, 0, +1 } };
            nbr[11] = { { +1, 0, +1 } };
            nbr[12] = { { 0, -1, +1 } };
            nbr[13] = { { 0, +1, +1 } };
            nbr[14] = { { -1, 0, -1 } };
            nbr[15] = { { +1, 0, -1 } };
            nbr[16] = { { 0, -1, -1 } };
            nbr[17] = { { 0, +1, -1 } };
        }

        void GetNeighborhood(std::array<std::array<std::int64_t, 3>, 26>& nbr) const
        {
            nbr[0] = { { -1, 0, 0 } };
            nbr[1] = { { +1, 0, 0 } };
            nbr[2] = { { 0, -1, 0 } };
            nbr[3] = { { 0, +1, 0 } };
            nbr[4] = { { 0, 0, -1 } };
            nbr[5] = { { 0, 0, +1 } };
            nbr[6] = { { -1, -1, 0 } };
            nbr[7] = { { +1, -1, 0 } };
            nbr[8] = { { -1, +1, 0 } };
            nbr[9] = { { +1, +1, 0 } };
            nbr[10] = { { -1, 0, +1 } };
            nbr[11] = { { +1, 0, +1 } };
            nbr[12] = { { 0, -1, +1 } };
            nbr[13] = { { 0, +1, +1 } };
            nbr[14] = { { -1, 0, -1 } };
            nbr[15] = { { +1, 0, -1 } };
            nbr[16] = { { 0, -1, -1 } };
            nbr[17] = { { 0, +1, -1 } };
            nbr[18] = { { -1, -1, -1 } };
            nbr[19] = { { +1, -1, -1 } };
            nbr[20] = { { -1, +1, -1 } };
            nbr[21] = { { +1, +1, -1 } };
            nbr[22] = { { -1, -1, +1 } };
            nbr[23] = { { +1, -1, +1 } };
            nbr[24] = { { -1, +1, +1 } };
            nbr[25] = { { +1, +1, +1 } };
        }

        void GetNeighborhood(std::array<std::array<std::int64_t, 3>, 27>& nbr) const
        {
            nbr[0] = { { -1, -1, -1 } };
            nbr[1] = { { 0, -1, -1 } };
            nbr[2] = { { +1, -1, -1 } };
            nbr[3] = { { -1, 0, -1 } };
            nbr[4] = { { 0, 0, -1 } };
            nbr[5] = { { +1, 0, -1 } };
            nbr[6] = { { -1, +1, -1 } };
            nbr[7] = { { 0, +1, -1 } };
            nbr[8] = { { +1, +1, -1 } };
            nbr[9] = { { -1, -1, 0 } };
            nbr[10] = { { 0, -1, 0 } };
            nbr[11] = { { +1, -1, 0 } };
            nbr[12] = { { -1, 0, 0 } };
            nbr[13] = { { 0, 0, 0 } };
            nbr[14] = { { +1, 0, 0 } };
            nbr[15] = { { -1, +1, 0 } };
            nbr[16] = { { 0, +1, 0 } };
            nbr[17] = { { +1, +1, 0 } };
            nbr[18] = { { -1, -1, +1 } };
            nbr[19] = { { 0, -1, +1 } };
            nbr[20] = { { +1, -1, +1 } };
            nbr[21] = { { -1, 0, +1 } };
            nbr[22] = { { 0, 0, +1 } };
            nbr[23] = { { +1, 0, +1 } };
            nbr[24] = { { -1, +1, +1 } };
            nbr[25] = { { 0, +1, +1 } };
            nbr[26] = { { +1, +1, +1 } };
        }

        // Get the locations for a neighborhood of (x,y,z), accessed as a
        // 1-dimensional array. The input (x,y,z) is required to be strictly
        // inside the image. If you require neighborhoods of boundary pixels,
        // you must computer your own or use GetNeighborhood(...) and extract
        // the relevant elements.
        void GetNeighborhood(std::size_t x, std::size_t y, std::size_t z,
            std::array<std::size_t, 6>& nbr) const
        {
            GetNeighborhood1D(x, y, z, nbr);
        }

        void GetNeighborhood(std::size_t x, std::size_t y, std::size_t z,
            std::array<std::size_t, 18>& nbr) const
        {
            GetNeighborhood1D(x, y, z, nbr);
        }

        void GetNeighborhood(std::size_t x, std::size_t y, std::size_t z,
            std::array<std::size_t, 26>& nbr) const
        {
            GetNeighborhood1D(x, y, z, nbr);
        }

        void GetNeighborhood(std::size_t x, std::size_t y, std::size_t z,
            std::array<std::size_t, 27>& nbr) const
        {
            GetNeighborhood1D(x, y, z, nbr);
        }

        // Get the locations for a neighborhood of (x,y,z), accessed as a
        // 3-dimensional array. The input (x,y,z) is required to be strictly
        // inside the image. If you require neighborhoods of boundary pixels,
        // you must computer your own or use GetNeighborhood(...) and extract
        // the relevant elements.
        void GetNeighborhood(std::size_t x, std::size_t y, std::size_t z,
            std::array<std::array<std::size_t, 3>, 6>& nbr) const
        {
            GetNeighborhood3D(x, y, z, nbr);
        }

        void GetNeighborhood(std::size_t x, std::size_t y, std::size_t z,
            std::array<std::array<std::size_t, 3>, 18>& nbr) const
        {
            GetNeighborhood3D(x, y, z, nbr);
        }

        void GetNeighborhood(int32_t x, int32_t y, int32_t z,
            std::array<std::array<std::size_t, 3>, 26>& nbr) const
        {
            GetNeighborhood3D(x, y, z, nbr);
        }

        void GetNeighborhood(int32_t x, int32_t y, int32_t z,
            std::array<std::array<std::size_t, 3>, 27>& nbr) const
        {
            GetNeighborhood3D(x, y, z, nbr);
        }

    protected:
        template <std::size_t n>
        void GetNeighborhood1D(std::size_t x, std::size_t y, std::size_t z,
            std::array<std::size_t, n>& nbr) const
        {
            auto const& dim = this->mSizes;
            GTL_OUTOFRANGE_ASSERT(
                1 <= x && x + 1 <= dim[0] && 1 <= y && y + 1 <= dim[1] && 1 <= z && z + 1 <= dim[2],
                "Invalid (" + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z) + ").");

            std::int64_t index = static_cast<std::int64_t>(x + dim[0] * (y + dim[1] * z));
            std::array<std::int64_t, 6> inbr{};
            GetNeighborhood(inbr);
            for (std::size_t i = 0; i < 6; ++i)
            {
                nbr[i] = static_cast<std::size_t>(index + inbr[i]);
            }
        }

        template <std::size_t n>
        void GetNeighborhood3D(std::size_t x, std::size_t y, std::size_t z,
            std::array<std::array<std::size_t, 3>, n>& nbr) const
        {
            auto const& dim = this->mSizes;
            GTL_OUTOFRANGE_ASSERT(
                1 <= x && x + 1 <= dim[0] && 1 <= y && y + 1 <= dim[1] && 1 <= z && z + 1 <= dim[2],
                "Invalid (" + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z) + ").");

            std::int64_t ix = static_cast<std::int64_t>(x);
            std::int64_t iy = static_cast<std::int64_t>(y);
            std::int64_t iz = static_cast<std::int64_t>(z);

            std::array<std::array<std::int64_t, 3>, n> inbr{};
            GetNeighborhood(inbr);
            for (std::size_t i = 0; i < n; ++i)
            {
                nbr[i][0] = static_cast<std::size_t>(ix + inbr[i][0]);
                nbr[i][1] = static_cast<std::size_t>(iy + inbr[i][1]);
                nbr[i][2] = static_cast<std::size_t>(iz + inbr[i][2]);
            }
        }

    private:
        friend class UnitTestImage3;
    };
}
