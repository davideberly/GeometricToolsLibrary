// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.12

#pragma once

// Class UIntegerAP32 is designed to support arbitrary precision arithmetic
// using BSNumber and BSRational. It is not a general-purpose class for
// arithmetic of unsigned integers.
//
// To collect statistics on how large the UIntegerAP32 storage becomes when
// using it for the UInteger of BSNumber, add the preprocessor symbol
// GTL_COLLECT_UINTEGERAP32_STATISTICS to the global defines passed to the
// compiler.
// 
// If you use this feature, you must define gsUIntegerAP32MaxBlocks somewhere
// in your code. After a sequence of BSNumber operations, look at
// gsUIntegerAP32MaxBlocks in the debugger watch window. If the number is not
// too large, you might be safe in replacing UIntegerAP32 by UIntegerFP32<N>,
// where N is the value of gsUIntegerAP32MaxBlocks. This leads to faster code
// because you no longer have dynamic memory allocations and deallocations
// that occur regularly with std::vector<std::uint32_t> during BSNumber operations.
// A safer choice is to argue mathematically that the maximum size is bounded
// by N. This requires an analysis of how many bits of precision you need for
// the types of computation you perform. See class BSPrecision for code that
// allows you to compute maximum N.

#if defined(GTL_COLLECT_UINTEGERAP32_STATISTICS)
#include <GTL/Utility/AtomicMinMax.h>
namespace gtl
{
    extern std::atomic<std::size_t> gsUIntegerAP32MaxBlocks;
}
#endif

#include <GTL/Mathematics/Arithmetic/UIntegerALU32.h>
#include <GTL/Utility/Exceptions.h>
#include <algorithm>
#include <cstdint>
#include <istream>
#include <limits>
#include <ostream>
#include <utility>
#include <vector>

namespace gtl
{
    class UIntegerAP32
    {
    public:
        // Construction and destruction.
        UIntegerAP32()
            :
            mNumBits(0),
            mBits{}
        {
        }

        UIntegerAP32(std::uint32_t number)
            :
            mNumBits(0),
            mBits{}
        {
            if (number > 0)
            {
                std::uint32_t const first = BitHacks::GetLeadingBit(number);
                std::uint32_t const last = BitHacks::GetTrailingBit(number);
                mNumBits = static_cast<std::size_t>(first - last) + 1;
                mBits.resize(1);
                mBits[0] = (number >> last);
            }
            else
            {
                mNumBits = 0;
            }

#if defined(GTL_COLLECT_UINTEGERAP32_STATISTICS)
            AtomicMax(gsUIntegerAP32MaxBlocks, mBits.size());
#endif
        }

        UIntegerAP32(std::uint64_t number)
            :
            mNumBits(0),
            mBits{}
        {
            if (number > 0)
            {
                std::uint32_t const first = BitHacks::GetLeadingBit(number);
                std::uint32_t const last = BitHacks::GetTrailingBit(number);
                number >>= last;
                std::size_t const numBitsM1 = static_cast<std::size_t>(first - last);
                mNumBits = numBitsM1 + 1;
                std::size_t const numBlocks = 1 + numBitsM1 / 32;
                mBits.resize(numBlocks);
                mBits[0] = static_cast<std::uint32_t>(number & 0x00000000FFFFFFFFull);
                if (mBits.size() > 1)
                {
                    mBits[1] = static_cast<std::uint32_t>((number >> 32) & 0x00000000FFFFFFFFull);
                }
            }
            else
            {
                mNumBits = 0;
            }

#if defined(GTL_COLLECT_UINTEGERAP32_STATISTICS)
            AtomicMax(gsUIntegerAP32MaxBlocks, mBits.size());
#endif
        }

        ~UIntegerAP32() = default;

        // Copy semantics.
        UIntegerAP32(UIntegerAP32 const& other)
            :
            mNumBits(0),
            mBits{}
        {
            *this = other;
        }

        UIntegerAP32& operator=(UIntegerAP32 const& other)
        {
            mNumBits = other.mNumBits;
            mBits = other.mBits;
            return *this;
        }

        // Move semantics.
        UIntegerAP32(UIntegerAP32&& other) noexcept
            :
            mNumBits(0),
            mBits{}
        {
            *this = std::move(other);
        }

        UIntegerAP32& operator=(UIntegerAP32&& other) noexcept
        {
            mNumBits = other.mNumBits;
            mBits = std::move(other.mBits);
            other.mNumBits = 0;
            return *this;
        }

        // Member access.
        void SetNumBits(std::size_t numBits)
        {
            if (numBits > 0)
            {
                mNumBits = numBits;
                std::size_t const numBitsM1 = numBits - 1;
                std::size_t const numBlocks = 1 + numBitsM1 / 32;
                mBits.resize(numBlocks);
            }
            else
            {
                mNumBits = 0;
                mBits.clear();
            }

#if defined(GTL_COLLECT_UINTEGERAP32_STATISTICS)
            AtomicMax(gsUIntegerAP32MaxBlocks, mBits.size());
#endif
        }

        inline std::size_t GetNumBits() const
        {
            return mNumBits;
        }

        inline std::vector<std::uint32_t> const& GetBits() const
        {
            return mBits;
        }

        inline std::vector<std::uint32_t>& GetBits()
        {
            return mBits;
        }

        inline std::size_t GetNumBlocks() const
        {
            return mBits.size();
        }

        inline static std::size_t GetMaxNumBlocks()
        {
            return std::numeric_limits<std::size_t>::max();
        }

        inline void SetBack(std::uint32_t value)
        {
            GTL_RUNTIME_ASSERT(
                mBits.size() > 0,
                "Cannot call SetBack on an empty mBits array.");

            mBits.back() = value;
        }

        inline std::uint32_t GetBack() const
        {
            GTL_RUNTIME_ASSERT(
                mBits.size() > 0,
                "Cannot call SetBack on an empty mBits array.");

            return mBits.back();
        }

        inline void SetAllBitsToZero()
        {
            std::fill(mBits.begin(), mBits.end(), 0u);
        }

        // Stream binary output. The return value is the number of bytes
        // written if all 'output' operations succeed but 0 otherwise.
        std::size_t Write(std::ostream& output) const
        {
            std::size_t numWritten = 0;

            if (output.write((char const*)&mNumBits, sizeof(mNumBits)).bad())
            {
                return 0;
            }
            numWritten += sizeof(mNumBits);

            std::size_t numBlocks = mBits.size();
            if (output.write((char const*)&numBlocks, sizeof(numBlocks)).bad())
            {
                return 0;
            }
            numWritten += sizeof(numBlocks);

            if (output.write((char const*)&mBits[0], numBlocks * sizeof(mBits[0])).bad())
            {
                return 0;
            }
            numWritten += numBlocks * sizeof(mBits[0]);

            return numWritten;
        }

        // Stream binary input. The return value is the number of bytes
        // read if all 'output' operations succeed but 0 otherwise.
        std::size_t Read(std::istream& input)
        {
            std::size_t numRead = 0;

            if (input.read((char*)&mNumBits, sizeof(mNumBits)).bad())
            {
                return 0;
            }
            numRead += sizeof(mNumBits);

            std::size_t numBlocks = 0;
            if (input.read((char*)&numBlocks, sizeof(numBlocks)).bad())
            {
                return 0;
            }
            numRead += sizeof(numBlocks);

            mBits.resize(numBlocks);
            if (input.read((char*)&mBits[0], numBlocks * sizeof(mBits[0])).bad())
            {
                return 0;
            }
            numRead += numBlocks * sizeof(mBits[0]);

            return numRead;
        }

    private:
        std::size_t mNumBits;
        std::vector<std::uint32_t> mBits;

    private:
        friend class UnitTestUIntegerAP32;
    };

    // Comparisons.
    inline bool operator==(UIntegerAP32 const& n0, UIntegerAP32 const& n1)
    {
        return UIntegerALU32<UIntegerAP32>::Equal(n0, n1);
    }

    inline bool operator!=(UIntegerAP32 const& n0, UIntegerAP32 const& n1)
    {
        return UIntegerALU32<UIntegerAP32>::NotEqual(n0, n1);
    }

    inline bool operator<(UIntegerAP32 const& n0, UIntegerAP32 const& n1)
    {
        return UIntegerALU32<UIntegerAP32>::LessThan(n0, n1);
    }

    inline bool operator<=(UIntegerAP32 const& n0, UIntegerAP32 const& n1)
    {
        return UIntegerALU32<UIntegerAP32>::LessThanOrEqual(n0, n1);
    }

    inline bool operator>(UIntegerAP32 const& n0, UIntegerAP32 const& n1)
    {
        return UIntegerALU32<UIntegerAP32>::GreaterThan(n0, n1);
    }

    inline bool operator>=(UIntegerAP32 const& n0, UIntegerAP32 const& n1)
    {
        return UIntegerALU32<UIntegerAP32>::GreaterThanOrEqual(n0, n1);
    }
}
