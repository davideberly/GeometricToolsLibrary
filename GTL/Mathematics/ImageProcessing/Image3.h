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
    class Image3 : public Multiarray<PixelType, true>
    {
    public:
        // The inheritance does not use virtual destructors, so you cannot
        // delete Image2 polymorphically via its base-class type.
        Image3() = default;
        ~Image3() = default;

        Image3(std::size_t bound0, std::size_t bound1, std::size_t bound2)
            :
            Multiarray<PixelType, true>{ bound0, bound1, bound2 }
        {
        }

        // In the following discussion, u, v and w are in {-1,1}. Given a
        // voxel (x,y,z), the 6-connected neighbors have relative offsets
        // (u,0,0), (0,v,0), and (0,0,w). The 18-connected neighbors include
        // the 6-connected neighbors and have additional relative offsets
        // (u,v,0), (u,0,w), and (0,v,w). The 26-connected neighbors include
        // the 18-connected neighbors and have additional relative offsets
        // (u,v,w). The corner neighbors have offsets (0,0,0), (1,0,0),
        // (0,1,0), (1,1,0), (0,0,1), (1,0,1), (0,1,1), and (1,1,1) in that
        // order. The full neighborhood is the set of 3x3x3 pixels centered
        // at (x,y,z).
        using SignedType = std::conditional<sizeof(std::size_t) == 8, std::int64_t, std::int32_t>::type;

        // The neighborhoods can be accessed as 1-dimensional indices using
        // these functions. The first five functions provide 1-dimensional
        // indices relative to any voxel location; these depend only on the
        // image dimensions. The last five functions provide 1-dimensional
        // indices for the actual voxels in the neighborhood; no clamping is
        // used when (x,y,z) is on the boundary.
        void GetNeighborhood(std::array<SignedType, 6>& offset) const
        {
            SignedType dim0 = static_cast<SignedType>(this->size(0));
            SignedType dim01 = static_cast<SignedType>(this->size(0) * this->size(1));
            offset[0] = -1;        // (x-1,y,z)
            offset[1] = +1;        // (x+1,y,z)
            offset[2] = -dim0;     // (x,y-1,z)
            offset[3] = +dim0;     // (x,y+1,z)
            offset[4] = -dim01;    // (x,y,z-1)
            offset[5] = +dim01;    // (x,y,z+1)
        }

        void GetNeighborhood(std::array<SignedType, 18>& offset) const
        {
            SignedType dim0 = static_cast<SignedType>(this->size(0));
            SignedType dim01 = static_cast<SignedType>(this->size(0) * this->size(1));
            offset[0] = -1;                // (x-1,y,z)
            offset[1] = +1;                // (x+1,y,z)
            offset[2] = -dim0;             // (x,y-1,z)
            offset[3] = +dim0;             // (x,y+1,z)
            offset[4] = -dim01;            // (x,y,z-1)
            offset[5] = +dim01;            // (x,y,z+1)
            offset[6] = -1 - dim0;         // (x-1,y-1,z)
            offset[7] = +1 - dim0;         // (x+1,y-1,z)
            offset[8] = -1 + dim0;         // (x-1,y+1,z)
            offset[9] = +1 + dim0;         // (x+1,y+1,z)
            offset[10] = -1 + dim01;       // (x-1,y,z+1)
            offset[11] = +1 + dim01;       // (x+1,y,z+1)
            offset[12] = -dim0 + dim01;    // (x,y-1,z+1)
            offset[13] = +dim0 + dim01;    // (x,y+1,z+1)
            offset[14] = -1 - dim01;       // (x-1,y,z-1)
            offset[15] = +1 - dim01;       // (x+1,y,z-1)
            offset[16] = -dim0 - dim01;    // (x,y-1,z-1)
            offset[17] = +dim0 - dim01;    // (x,y+1,z-1)
        }

        void GetNeighborhood(std::array<SignedType, 26>& offset) const
        {
            SignedType dim0 = static_cast<SignedType>(this->size(0));
            SignedType dim01 = static_cast<SignedType>(this->size(0) * this->size(1));
            offset[0] = -1;                    // (x-1,y,z)
            offset[1] = +1;                    // (x+1,y,z)
            offset[2] = -dim0;                 // (x,y-1,z)
            offset[3] = +dim0;                 // (x,y+1,z)
            offset[4] = -dim01;                // (x,y,z-1)
            offset[5] = +dim01;                // (x,y,z+1)
            offset[6] = -1 - dim0;             // (x-1,y-1,z)
            offset[7] = +1 - dim0;             // (x+1,y-1,z)
            offset[8] = -1 + dim0;             // (x-1,y+1,z)
            offset[9] = +1 + dim0;             // (x+1,y+1,z)
            offset[10] = -1 + dim01;           // (x-1,y,z+1)
            offset[11] = +1 + dim01;           // (x+1,y,z+1)
            offset[12] = -dim0 + dim01;        // (x,y-1,z+1)
            offset[13] = +dim0 + dim01;        // (x,y+1,z+1)
            offset[14] = -1 - dim01;           // (x-1,y,z-1)
            offset[15] = +1 - dim01;           // (x+1,y,z-1)
            offset[16] = -dim0 - dim01;        // (x,y-1,z-1)
            offset[17] = +dim0 - dim01;        // (x,y+1,z-1)
            offset[18] = -1 - dim0 - dim01;    // (x-1,y-1,z-1)
            offset[19] = +1 - dim0 - dim01;    // (x+1,y-1,z-1)
            offset[20] = -1 + dim0 - dim01;    // (x-1,y+1,z-1)
            offset[21] = +1 + dim0 - dim01;    // (x+1,y+1,z-1)
            offset[22] = -1 - dim0 + dim01;    // (x-1,y-1,z+1)
            offset[23] = +1 - dim0 + dim01;    // (x+1,y-1,z+1)
            offset[24] = -1 + dim0 + dim01;    // (x-1,y+1,z+1)
            offset[25] = +1 + dim0 + dim01;    // (x+1,y+1,z+1)
        }

        void GetCorners(std::array<SignedType, 8>& offset) const
        {
            SignedType dim0 = static_cast<SignedType>(this->size(0));
            SignedType dim01 = static_cast<SignedType>(this->size(0) * this->size(1));
            offset[0] = 0;                 // (x,y,z)
            offset[1] = 1;                 // (x+1,y,z)
            offset[2] = dim0;              // (x,y+1,z)
            offset[3] = dim0 + 1;          // (x+1,y+1,z)
            offset[4] = dim01;             // (x,y,z+1)
            offset[5] = dim01 + 1;         // (x+1,y,z+1)
            offset[6] = dim01 + dim0;      // (x,y+1,z+1)
            offset[7] = dim01 + dim0 + 1;  // (x+1,y+1,z+1)
        }

        void GetFull(std::array<SignedType, 27>& offset) const
        {
            SignedType dim0 = static_cast<SignedType>(this->size(0));
            SignedType dim01 = static_cast<SignedType>(this->size(0) * this->size(1));
            offset[0] = -1 - dim0 - dim01;     // (x-1,y-1,z-1)
            offset[1] = -dim0 - dim01;         // (x,  y-1,z-1)
            offset[2] = +1 - dim0 - dim01;     // (x+1,y-1,z-1)
            offset[3] = -1 - dim01;            // (x-1,y,  z-1)
            offset[4] = -dim01;                // (x,  y,  z-1)
            offset[5] = +1 - dim01;            // (x+1,y,  z-1)
            offset[6] = -1 + dim0 - dim01;     // (x-1,y+1,z-1)
            offset[7] = +dim0 - dim01;         // (x,  y+1,z-1)
            offset[8] = +1 + dim0 - dim01;     // (x+1,y+1,z-1)
            offset[9] = -1 - dim0;             // (x-1,y-1,z)
            offset[10] = -dim0;                // (x,  y-1,z)
            offset[11] = +1 - dim0;            // (x+1,y-1,z)
            offset[12] = -1;                   // (x-1,y,  z)
            offset[13] = 0;                    // (x,  y,  z)
            offset[14] = +1;                   // (x+1,y,  z)
            offset[15] = -1 + dim0;            // (x-1,y+1,z)
            offset[16] = +dim0;                // (x,  y+1,z)
            offset[17] = +1 + dim0;            // (x+1,y+1,z)
            offset[18] = -1 - dim0 + dim01;    // (x-1,y-1,z+1)
            offset[19] = -dim0 + dim01;        // (x,  y-1,z+1)
            offset[20] = +1 - dim0 + dim01;    // (x+1,y-1,z+1)
            offset[21] = -1 + dim01;           // (x-1,y,  z+1)
            offset[22] = +dim01;               // (x,  y,  z+1)
            offset[23] = +1 + dim01;           // (x+1,y,  z+1)
            offset[24] = -1 + dim0 + dim01;    // (x-1,y+1,z+1)
            offset[25] = +dim0 + dim01;        // (x,  y+1,z+1)
            offset[26] = +1 + dim0 + dim01;    // (x+1,y+1,z+1)
        }

        void GetNeighborhood(std::size_t x, std::size_t y, std::size_t z, std::array<std::size_t, 6>& nbr) const
        {
            std::size_t center = this->index(x, y, z);
            std::array<SignedType, 6> offset{};
            GetNeighborhood(offset);
            for (std::size_t i = 0; i < 6; ++i)
            {
                nbr[i] = center + offset[i];
            }
        }

        void GetNeighborhood(std::size_t x, std::size_t y, std::size_t z, std::array<std::size_t, 18>& nbr) const
        {
            std::size_t center = this->index(x, y, z);
            std::array<SignedType, 18> offset{};
            GetNeighborhood(offset);
            for (std::size_t i = 0; i < 18; ++i)
            {
                nbr[i] = center + offset[i];
            }
        }

        void GetNeighborhood(std::size_t x, std::size_t y, std::size_t z, std::array<std::size_t, 26>& nbr) const
        {
            std::size_t center = this->index(x, y, z);
            std::array<SignedType, 26> offset{};
            GetNeighborhood(offset);
            for (std::size_t i = 0; i < 26; ++i)
            {
                nbr[i] = center + offset[i];
            }
        }

        void GetCorners(std::size_t x, std::size_t y, std::size_t z, std::array<std::size_t, 8>& nbr) const
        {
            std::size_t center = this->index(x, y, z);
            std::array<SignedType, 8> offset{};
            GetCorners(offset);
            for (std::size_t i = 0; i < 8; ++i)
            {
                nbr[i] = center + offset[i];
            }
        }

        void GetFull(std::size_t x, std::size_t y, std::size_t z, std::array<std::size_t, 27>& nbr) const
        {
            std::size_t center = this->index(x, y, z);
            std::array<SignedType, 27> offset{};
            GetFull(offset);
            for (std::size_t i = 0; i < 27; ++i)
            {
                nbr[i] = center + offset[i];
            }
        }

        // The neighborhoods can be accessed as 3-tuples using these
        // functions.  The first five functions provide 3-tuples relative to
        // any voxel location; these depend only on the image dimensions.  The
        // last five functions provide 3-tuples for the actual voxels in the
        // neighborhood; no clamping is used when (x,y,z) is on the boundary.
        void GetNeighborhood(std::array<std::array<SignedType, 3>, 6>& offset) const
        {
            offset[0] = { { -1, 0, 0 } };
            offset[1] = { { +1, 0, 0 } };
            offset[2] = { { 0, -1, 0 } };
            offset[3] = { { 0, +1, 0 } };
            offset[4] = { { 0, 0, -1 } };
            offset[5] = { { 0, 0, +1 } };
        }

        void GetNeighborhood(std::array<std::array<SignedType, 3>, 18>& offset) const
        {
            offset[0] = { { -1, 0, 0 } };
            offset[1] = { { +1, 0, 0 } };
            offset[2] = { { 0, -1, 0 } };
            offset[3] = { { 0, +1, 0 } };
            offset[4] = { { 0, 0, -1 } };
            offset[5] = { { 0, 0, +1 } };
            offset[6] = { { -1, -1, 0 } };
            offset[7] = { { +1, -1, 0 } };
            offset[8] = { { -1, +1, 0 } };
            offset[9] = { { +1, +1, 0 } };
            offset[10] = { { -1, 0, +1 } };
            offset[11] = { { +1, 0, +1 } };
            offset[12] = { { 0, -1, +1 } };
            offset[13] = { { 0, +1, +1 } };
            offset[14] = { { -1, 0, -1 } };
            offset[15] = { { +1, 0, -1 } };
            offset[16] = { { 0, -1, -1 } };
            offset[17] = { { 0, +1, -1 } };
        }

        void GetNeighborhood(std::array<std::array<SignedType, 3>, 26>& offset) const
        {
            offset[0] = { { -1, 0, 0 } };
            offset[1] = { { +1, 0, 0 } };
            offset[2] = { { 0, -1, 0 } };
            offset[3] = { { 0, +1, 0 } };
            offset[4] = { { 0, 0, -1 } };
            offset[5] = { { 0, 0, +1 } };
            offset[6] = { { -1, -1, 0 } };
            offset[7] = { { +1, -1, 0 } };
            offset[8] = { { -1, +1, 0 } };
            offset[9] = { { +1, +1, 0 } };
            offset[10] = { { -1, 0, +1 } };
            offset[11] = { { +1, 0, +1 } };
            offset[12] = { { 0, -1, +1 } };
            offset[13] = { { 0, +1, +1 } };
            offset[14] = { { -1, 0, -1 } };
            offset[15] = { { +1, 0, -1 } };
            offset[16] = { { 0, -1, -1 } };
            offset[17] = { { 0, +1, -1 } };
            offset[18] = { { -1, -1, -1 } };
            offset[19] = { { +1, -1, -1 } };
            offset[20] = { { -1, +1, -1 } };
            offset[21] = { { +1, +1, -1 } };
            offset[22] = { { -1, -1, +1 } };
            offset[23] = { { +1, -1, +1 } };
            offset[24] = { { -1, +1, +1 } };
            offset[25] = { { +1, +1, +1 } };
        }

        void GetCorners(std::array<std::array<SignedType, 3>, 8>& offset) const
        {
            offset[0] = { { 0, 0, 0 } };
            offset[1] = { { 1, 0, 0 } };
            offset[2] = { { 0, 1, 0 } };
            offset[3] = { { 1, 1, 0 } };
            offset[4] = { { 0, 0, 1 } };
            offset[5] = { { 1, 0, 1 } };
            offset[6] = { { 0, 1, 1 } };
            offset[7] = { { 1, 1, 1 } };
        }

        void GetFull(std::array<std::array<SignedType, 3>, 27>& offset) const
        {
            offset[0] = { { -1, -1, -1 } };
            offset[1] = { { 0, -1, -1 } };
            offset[2] = { { +1, -1, -1 } };
            offset[3] = { { -1, 0, -1 } };
            offset[4] = { { 0, 0, -1 } };
            offset[5] = { { +1, 0, -1 } };
            offset[6] = { { -1, +1, -1 } };
            offset[7] = { { 0, +1, -1 } };
            offset[8] = { { +1, +1, -1 } };
            offset[9] = { { -1, -1, 0 } };
            offset[10] = { { 0, -1, 0 } };
            offset[11] = { { +1, -1, 0 } };
            offset[12] = { { -1, 0, 0 } };
            offset[13] = { { 0, 0, 0 } };
            offset[14] = { { +1, 0, 0 } };
            offset[15] = { { -1, +1, 0 } };
            offset[16] = { { 0, +1, 0 } };
            offset[17] = { { +1, +1, 0 } };
            offset[18] = { { -1, -1, +1 } };
            offset[19] = { { 0, -1, +1 } };
            offset[20] = { { +1, -1, +1 } };
            offset[21] = { { -1, 0, +1 } };
            offset[22] = { { 0, 0, +1 } };
            offset[23] = { { +1, 0, +1 } };
            offset[24] = { { -1, +1, +1 } };
            offset[25] = { { 0, +1, +1 } };
            offset[26] = { { +1, +1, +1 } };
        }

        void GetNeighborhood(std::size_t x, std::size_t y, std::size_t z, std::array<std::array<std::size_t, 3>, 6>& nbr) const
        {
            std::array<std::array<SignedType, 3>, 6> offset{};
            GetNeighborhood(offset);
            for (std::size_t i = 0; i < 6; ++i)
            {
                nbr[i][0] = x + offset[i][0];
                nbr[i][1] = y + offset[i][1];
                nbr[i][2] = z + offset[i][2];
            }
        }

        void GetNeighborhood(std::size_t x, std::size_t y, std::size_t z, std::array<std::array<std::size_t, 3>, 18>& nbr) const
        {
            std::array<std::array<SignedType, 3>, 18> offset{};
            GetNeighborhood(offset);
            for (std::size_t i = 0; i < 18; ++i)
            {
                nbr[i][0] = x + offset[i][0];
                nbr[i][1] = y + offset[i][1];
                nbr[i][2] = z + offset[i][2];
            }
        }

        void GetNeighborhood(std::size_t x, std::size_t y, std::size_t z, std::array<std::array<std::size_t, 3>, 26>& nbr) const
        {
            std::array<std::array<SignedType, 3>, 26> offset{};
            GetNeighborhood(offset);
            for (std::size_t i = 0; i < 26; ++i)
            {
                nbr[i][0] = x + offset[i][0];
                nbr[i][1] = y + offset[i][1];
                nbr[i][2] = z + offset[i][2];
            }
        }

        void GetCorners(std::size_t x, std::size_t y, std::size_t z, std::array<std::array<std::size_t, 3>, 8>& nbr) const
        {
            std::array<std::array<SignedType, 3>, 8> offset{};
            GetCorners(offset);
            for (std::size_t i = 0; i < 8; ++i)
            {
                nbr[i][0] = x + offset[i][0];
                nbr[i][1] = y + offset[i][1];
                nbr[i][2] = z + offset[i][2];
            }
        }

        void GetFull(std::size_t x, std::size_t y, std::size_t z, std::array<std::array<std::size_t, 3>, 27>& nbr) const
        {
            std::array<std::array<SignedType, 3>, 27> offset{};
            GetFull(offset);
            for (std::size_t i = 0; i < 27; ++i)
            {
                nbr[i][0] = x + offset[i][0];
                nbr[i][1] = y + offset[i][1];
                nbr[i][2] = z + offset[i][2];
            }
        }

    private:
        friend class UnitTestImage3;
    };
}
