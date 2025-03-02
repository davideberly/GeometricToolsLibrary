// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.03.01

#pragma once

// Class UIntegerFP32 is designed to support fixed-precision arithmetic using
// BSNumber and BSRational. It is not a general-purpose class for arithmetic
// of unsigned integers. The template parameter N is the number of 32-bit
// words required to store the precision for the desired computations where
// the maximum number of bits is 32*N.
//
// The constructors and the copy and move operations do not initialize fully
// the mBits array. In particular, there are no initializations of the form
// mBits{} in the constructors. By adding the brace, mBits elements are set
// to 0 via a memset operation. For N large (such as MinimumVolumeBox3) and
// having a significant number of constructor calls, the CPU time spent in
// memset is enormous.
//
// To collect statistics on how large the UIntegerFP32 storage becomes when
// using it for the UInteger of BSNumber, add the preprocessor symbol
// GTL_COLLECT_UINTEGERFP32_STATISTICS to the global defines passed to the
// compiler.
// 
// If you use this feature, you must define gsUIntegerFP32MaxBlocks somewhere
// in your code. After a sequence of BSNumber operations, look at
// gsUIntegerFP32MaxBlocks in the debugger watch window. 

#include <GTL/Mathematics/Arithmetic/UIntegerALU32.h>
#include <GTL/Utility/Exceptions.h>
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <istream>
#include <ostream>
#include <utility>

#if defined(GTL_COLLECT_UINTEGERFP32_STATISTICS)
#include <GTL/Utility/AtomicMinMax.h>
namespace gtl
{
    extern std::atomic<std::size_t> gsUIntegerFP32MaxBlocks;
}
#endif

namespace gtl
{
    template <std::size_t N>
    class UIntegerFP32
    {
    public:
#if defined(GTL_USE_MSWINDOWS)
        // Disable the warning:
        //   warning C26495: Variable 'gte::UIntegerFP32<2561>::mBits' is
        //   uninitialized. Always initialize a member variable (type.6).
        // See the NOTE above about performance problems when mBits is
        // initialized in the constructors.
#pragma warning(disable : 26495)
#endif
        // Construction and destruction.
        UIntegerFP32()
            :
            mNumBits(0),
            mNumBlocks(0)
        {
            static_assert(
                N >= 1,
                "Invalid size N.");
        }

        UIntegerFP32(std::uint32_t number)
            :
            mNumBits(0),
            mNumBlocks(0)
        {
            static_assert(
                N >= 1,
                "Invalid size N.");

            if (number > 0)
            {
                std::uint32_t const first = BitHacks::GetLeadingBit(number);
                std::uint32_t const last = BitHacks::GetTrailingBit(number);
                mNumBits = static_cast<std::size_t>(first - last) + 1;
                mNumBlocks = 1;
                mBits[0] = (number >> last);
            }
            else
            {
                mNumBits = 0;
                mNumBlocks = 0;
            }

#if defined(GTL_COLLECT_UINTEGERFP32_STATISTICS)
            AtomicMax(gsUIntegerFP32MaxBlocks, mNumBlocks);
#endif
        }

        UIntegerFP32(std::uint64_t number)
            :
            mNumBits(0),
            mNumBlocks(0)
        {
            static_assert(
                N >= 2,
                "N not large enough to store 64-bit integers.");

            if (number > 0)
            {
                std::uint32_t const first = BitHacks::GetLeadingBit(number);
                std::uint32_t const last = BitHacks::GetTrailingBit(number);
                number >>= last;
                std::size_t const numBitsM1 = static_cast<std::size_t>(first - last);
                mNumBits = numBitsM1 + 1;
                mNumBlocks = 1 + numBitsM1 / 32;
                mBits[0] = static_cast<std::uint32_t>(number & 0x00000000FFFFFFFFull);
                if (mNumBlocks > 1)
                {
                    mBits[1] = static_cast<std::uint32_t>((number >> 32) & 0x00000000FFFFFFFFull);
                }
            }
            else
            {
                mNumBits = 0;
                mNumBlocks = 0;
            }

#if defined(GTL_COLLECT_UINTEGERFP32_STATISTICS)
            AtomicMax(gsUIntegerFP32MaxBlocks, mNumBlocks);
#endif
        }

        ~UIntegerFP32() = default;

        // Copy semantics. Only other.mNumBlocks elements are copied for
        // performance.
        UIntegerFP32(UIntegerFP32 const& other)
            :
            mNumBits(0),
            mNumBlocks(0)
        {
            static_assert(
                N >= 1,
                "Invalid size N.");

            *this = other;
        }

#if defined(GTL_USE_MSWINDOWS)
#pragma warning(default : 28020)
#endif

        UIntegerFP32& operator=(UIntegerFP32 const& other)
        {
            mNumBits = other.mNumBits;
            mNumBlocks = other.mNumBlocks;
            std::copy(other.mBits.begin(), other.mBits.begin() + other.mNumBlocks, mBits.begin());
            return *this;
        }

        // Copy from UIntegerFP32<NSource> to UIntegerFP32<N> as long as
        // NSource < N.
        template <std::size_t NSource>
        UIntegerFP32& operator=(UIntegerFP32<NSource> const& source)
        {
            static_assert(
                NSource < N,
                "The source dimension must be smaller than the target dimension.");

            mNumBits = source.GetNumBits();
            mNumBlocks = source.GetNumBlocks();
            auto const& srcBits = source.GetBits();
            std::copy(srcBits.begin(), srcBits.end(), mBits.begin());
            return *this;
        }

        // Move semantics. The std::move of std::array is a copy; that is,
        // there is no pointer stealing. Only other.mNumBlocks elements are
        // copied for performance. The source object has its number of bits
        // and number of blocks set to 0 to give the appearance that the data
        // data was actually stolen.
        UIntegerFP32(UIntegerFP32&& number) noexcept
            :
            mNumBits(0),
            mNumBlocks(0)
        {
            static_assert(
                N >= 1,
                "Invalid size N.");

            *this = std::move(number);
        }

        UIntegerFP32& operator=(UIntegerFP32&& other) noexcept
        {
            mNumBits = other.mNumBits;
            mNumBlocks = other.mNumBlocks;
            std::copy(other.mBits.begin(), other.mBits.begin() + other.mNumBlocks, mBits.begin());
            other.mNumBits = 0;
            other.mNumBlocks = 0;
            return *this;
        }

        // Member access.
        void SetNumBits(std::size_t numBits)
        {
            if (numBits > 0)
            {
                mNumBits = numBits;
                std::size_t const numBitsM1 = numBits - 1;
                mNumBlocks = 1 + numBitsM1 / 32;
            }
            else
            {
                mNumBits = 0;
                mNumBlocks = 0;
            }

#if defined(GTL_COLLECT_UINTEGERFP32_STATISTICS)
            AtomicMax(gsUIntegerFP32MaxBlocks, mNumBlocks);
#endif

            // N not large enough to store the number of bits.
            GTL_ARGUMENT_ASSERT(
                mNumBlocks <= N,
                "N not large enough to store requested bits.");
        }

        inline std::size_t GetNumBits() const
        {
            return mNumBits;
        }

        inline std::array<std::uint32_t, N> const& GetBits() const
        {
            return mBits;
        }

        inline std::array<std::uint32_t, N>& GetBits()
        {
            return mBits;
        }

        inline std::size_t GetNumBlocks() const
        {
            return mNumBlocks;
        }

        inline static std::size_t GetMaxNumBlocks()
        {
            return N;
        }

        inline void SetBack(std::uint32_t value)
        {
            GTL_RUNTIME_ASSERT(
                mNumBlocks > 0,
                "Cannot call SetBack on an empty mBits array.");

            std::size_t last = mNumBlocks - 1;
            mBits[last] = value;
        }

        inline std::uint32_t GetBack() const
        {
            GTL_RUNTIME_ASSERT(
                mNumBlocks > 0,
                "Cannot call SetBack on an empty mBits array.");

            std::size_t last = mNumBlocks - 1;
            return mBits[last];
        }

        inline void SetAllBitsToZero()
        {
            std::fill(mBits.begin(), mBits.end(), 0u);
        }

        // Disk input/output. The fstream objects should be created using
        // std::ios::binary. The return value is 'true' iff the operation is
        // successful.
        bool Write(std::ostream& output) const
        {
            if (output.write((char const*)& mNumBits, sizeof(mNumBits)).bad())
            {
                return false;
            }

            if (output.write((char const*)& mNumBlocks, sizeof(mNumBlocks)).bad())
            {
                return false;
            }

            return output.write((char const*)& mBits[0], mNumBlocks * sizeof(mBits[0])).good();
        }

        bool Read(std::istream& input)
        {
            if (input.read((char*)& mNumBits, sizeof(mNumBits)).bad())
            {
                return false;
            }

            if (input.read((char*)&mNumBlocks, sizeof(mNumBlocks)).bad())
            {
                return false;
            }

            return input.read((char*)& mBits[0], mNumBlocks * sizeof(mBits[0])).good();
        }

    private:
        std::size_t mNumBits, mNumBlocks;
        std::array<std::uint32_t, N> mBits;

    private:
        friend class UnitTestUIntegerFP32;
    };

    // Comparisons.
    template <std::size_t N>
    inline bool operator==(UIntegerFP32<N> const& n0, UIntegerFP32<N> const& n1)
    {
        return UIntegerALU32<UIntegerFP32<N>>::Equal(n0, n1);
    }

    template <std::size_t N>
    inline bool operator!=(UIntegerFP32<N> const& n0, UIntegerFP32<N> const& n1)
    {
        return UIntegerALU32<UIntegerFP32<N>>::NotEqual(n0, n1);
    }

    template <std::size_t N>
    inline bool operator<(UIntegerFP32<N> const& n0, UIntegerFP32<N> const& n1)
    {
        return UIntegerALU32<UIntegerFP32<N>>::LessThan(n0, n1);
    }

    template <std::size_t N>
    inline bool operator<=(UIntegerFP32<N> const& n0, UIntegerFP32<N> const& n1)
    {
        return UIntegerALU32<UIntegerFP32<N>>::LessThanOrEqual(n0, n1);
    }

    template <std::size_t N>
    inline bool operator>(UIntegerFP32<N> const& n0, UIntegerFP32<N> const& n1)
    {
        return UIntegerALU32<UIntegerFP32<N>>::GreaterThan(n0, n1);
    }

    template <std::size_t N>
    inline bool operator>=(UIntegerFP32<N> const& n0, UIntegerFP32<N> const& n1)
    {
        return UIntegerALU32<UIntegerFP32<N>>::GreaterThanOrEqual(n0, n1);
    }
}
