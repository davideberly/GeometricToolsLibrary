// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.10

#pragma once

#include <GTL/Utility/Multiarray.h>
#include <array>
#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace gtl
{
    template <typename PixelType>
    class Image2 : public Multiarray<PixelType, true>
    {
    public:
        // The inheritance does not use virtual destructors, so you cannot
        // delete Image2 polymorphically via its base-class type.
        Image2() = default;
        ~Image2() = default;

        Image2(std::size_t bound0, std::size_t bound1)
            :
            Multiarray<PixelType, true>{ bound0, bound1 }
        {
        }

        // In the following discussion, u and v are in {-1,1}. Given a pixel
        // (x,y), the 4-connected neighbors have relative offsets (u,0) and
        // (0,v). The 8-connected neighbors include the 4-connected neighbors
        // and have additional relative offsets (u,v). The corner neighbors
        // have relative offsets (0,0), (1,0), (0,1), and (1,1) in that order.
        // The full neighborhood is the set of 3x3 pixels centered at (x,y).
        // Neighborhood relative offsets must be signed. The signed type
        // depends on whether std::size_t is std::uint64_t or std::uint32_t.
        using SignedType = std::conditional<sizeof(std::size_t) == 8, std::int64_t, std::int32_t>::type;

        // The neighborhoods can be accessed as 1-dimensional indices using
        // these functions. The first four functions provide 1-dimensional
        // indices relative to any pixel location; these depend only on the
        // image dimensions. The last four functions provide 1-dimensional
        // indices for the actual pixels in the neighborhood; no clamping is
        // used when (x,y) is on the boundary.
        void GetNeighborhood(std::array<SignedType, 4>& offset) const
        {
            SignedType const dim0 = static_cast<SignedType>(this->size(0));
            offset[0] = -1;        // (x-1,y)
            offset[1] = +1;        // (x+1,y)
            offset[2] = -dim0;     // (x,y-1)
            offset[3] = +dim0;     // (x,y+1)
        }

        void GetNeighborhood(std::array<SignedType, 8>& offset) const
        {
            SignedType const dim0 = static_cast<SignedType>(this->size(0));
            offset[0] = -1;            // (x-1,y)
            offset[1] = +1;            // (x+1,y)
            offset[2] = -dim0;         // (x,y-1)
            offset[3] = +dim0;         // (x,y+1)
            offset[4] = -1 - dim0;     // (x-1,y-1)
            offset[5] = +1 - dim0;     // (x+1,y-1)
            offset[6] = -1 + dim0;     // (x-1,y+1)
            offset[7] = +1 + dim0;     // (x+1,y+1)
        }

        void GetCorners(std::array<SignedType, 4>& offset) const
        {
            SignedType const dim0 = static_cast<SignedType>(this->size(0));
            offset[0] = 0;         // (x,y)
            offset[1] = 1;         // (x+1,y)
            offset[2] = dim0;      // (x,y+1)
            offset[3] = dim0 + 1;  // (x+1,y+1)
        }

        void GetFull(std::array<SignedType, 9>& offset) const
        {
            SignedType const dim0 = static_cast<SignedType>(this->size(0));
            offset[0] = -1 - dim0;     // (x-1,y-1)
            offset[1] = -dim0;         // (x,y-1)
            offset[2] = +1 - dim0;     // (x+1,y-1)
            offset[3] = -1;            // (x-1,y)
            offset[4] = 0;             // (x,y)
            offset[5] = +1;            // (x+1,y)
            offset[6] = -1 + dim0;     // (x-1,y+1)
            offset[7] = +dim0;         // (x,y+1)
            offset[8] = +1 + dim0;     // (x+1,y+1)
        }

        void GetNeighborhood(std::size_t x, std::size_t y, std::array<std::size_t, 4>& nbr) const
        {
            std::size_t center = this->index(x, y);
            std::array<SignedType, 4> offset{};
            GetNeighborhood(offset);
            for (std::size_t i = 0; i < 4; ++i)
            {
                nbr[i] = center + offset[i];
            }
        }

        void GetNeighborhood(std::size_t x, std::size_t y, std::array<std::size_t, 8>& nbr) const
        {
            std::size_t center = this->index(x, y);
            std::array<SignedType, 8> offset{};
            GetNeighborhood(offset);
            for (std::size_t i = 0; i < 8; ++i)
            {
                nbr[i] = center + offset[i];
            }
        }

        void GetCorners(std::size_t x, std::size_t y, std::array<std::size_t, 4>& nbr) const
        {
            std::size_t center = this->index(x, y);
            std::array<SignedType, 4> offset{};
            GetCorners(offset);
            for (std::size_t i = 0; i < 4; ++i)
            {
                nbr[i] = center + offset[i];
            }
        }

        void GetFull(std::size_t x, std::size_t y, std::array<std::size_t, 9>& nbr) const
        {
            std::size_t center = this->index(x, y);
            std::array<SignedType, 9> offset{};
            GetFull(offset);
            for (std::size_t i = 0; i < 9; ++i)
            {
                nbr[i] = center + offset[i];
            }
        }

        // The neighborhoods can be accessed as 2-tuples using these
        // functions. The first four functions provide 2-tuples relative to
        // any pixel location; these depend only on the image dimensions. The
        // last four functions provide 2-tuples for the actual pixels in the
        // neighborhood; no clamping is used when (x,y) is on the boundary.
        void GetNeighborhood(std::array<std::array<SignedType, 2>, 4>& offset) const
        {
            offset[0] = { { -1, 0 } };
            offset[1] = { { +1, 0 } };
            offset[2] = { { 0, -1 } };
            offset[3] = { { 0, +1 } };
        }

        void GetNeighborhood(std::array<std::array<SignedType, 2>, 8>& offset) const
        {
            offset[0] = { { -1, -1 } };
            offset[1] = { { 0, -1 } };
            offset[2] = { { +1, -1 } };
            offset[3] = { { -1, 0 } };
            offset[4] = { { +1, 0 } };
            offset[5] = { { -1, +1 } };
            offset[6] = { { 0, +1 } };
            offset[7] = { { +1, +1 } };
        }

        void GetCorners(std::array<std::array<SignedType, 2>, 4>& offset) const
        {
            offset[0] = { { 0, 0 } };
            offset[1] = { { 1, 0 } };
            offset[2] = { { 0, 1 } };
            offset[3] = { { 1, 1 } };
        }

        void GetFull(std::array<std::array<SignedType, 2>, 9>& offset) const
        {
            offset[0] = { { -1, -1 } };
            offset[1] = { { 0, -1 } };
            offset[2] = { { +1, -1 } };
            offset[3] = { { -1, 0 } };
            offset[4] = { { 0, 0 } };
            offset[5] = { { +1, 0 } };
            offset[6] = { { -1, +1 } };
            offset[7] = { { 0, +1 } };
            offset[8] = { { +1, +1 } };
        }

        void GetNeighborhood(std::size_t x, std::size_t y, std::array<std::array<std::size_t, 2>, 4>& nbr) const
        {
            std::array<std::array<SignedType, 2>, 4> offset{};
            GetNeighborhood(offset);
            for (std::size_t i = 0; i < 4; ++i)
            {
                nbr[i][0] = x + offset[i][0];
                nbr[i][1] = y + offset[i][1];
            }
        }

        void GetNeighborhood(std::size_t x, std::size_t y, std::array<std::array<std::size_t, 2>, 8>& nbr) const
        {
            std::array<std::array<SignedType, 2>, 8> offset{};
            GetNeighborhood(offset);
            for (std::size_t i = 0; i < 8; ++i)
            {
                nbr[i][0] = x + offset[i][0];
                nbr[i][1] = y + offset[i][1];
            }
        }

        void GetCorners(std::size_t x, std::size_t y, std::array<std::array<std::size_t, 2>, 4>& nbr) const
        {
            std::array<std::array<SignedType, 2>, 4> offset{};
            GetCorners(offset);
            for (std::size_t i = 0; i < 4; ++i)
            {
                nbr[i][0] = x + offset[i][0];
                nbr[i][1] = y + offset[i][1];
            }
        }

        void GetFull(std::size_t x, std::size_t y, std::array<std::array<std::size_t, 2>, 9>& nbr) const
        {
            std::array<std::array<SignedType, 2>, 9> offset{};
            GetFull(offset);
            for (std::size_t i = 0; i < 9; ++i)
            {
                nbr[i][0] = x + offset[i][0];
                nbr[i][1] = y + offset[i][1];
            }
        }

    private:
        friend class UnitTestImage2;
    };
}
