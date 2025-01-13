// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Arithmetic/BitHacks.h>
#include <random>
using namespace gtl;

namespace gtl
{
    class UnitTestBitHacks
    {
    public:
        UnitTestBitHacks();
    private:
        void TestPopulationCount();
        void TestGetNumLeadingZeros();
        void TestGetNumTrailingZeros();
        void TestGetLeadingBit();
        void TestTrailingBit();
        void TestIsPowerOfTwo();
        void TestLog2OfPowerOfTwo();
        void TestRoundUpToPowerOfTwo();
        void TestRoundDownToPowerOfTwo();

        std::default_random_engine mDRE;
        std::uniform_int_distribution<std::uint32_t> mURD32;
        std::uniform_int_distribution<std::uint64_t> mURD64;
    };
}

UnitTestBitHacks::UnitTestBitHacks()
    :
    mURD32(0, std::numeric_limits<std::uint32_t>::max()),
    mURD64(0, std::numeric_limits<std::uint64_t>::max())
{
    UTInformation("Mathematics/Arithmetic/BitHacks");

    TestPopulationCount();
    TestGetNumLeadingZeros();
    TestGetNumTrailingZeros();
    TestGetLeadingBit();
    TestTrailingBit();
    TestIsPowerOfTwo();
    TestLog2OfPowerOfTwo();
    TestRoundUpToPowerOfTwo();
    TestRoundDownToPowerOfTwo();
}

void UnitTestBitHacks::TestPopulationCount()
{
    std::uint32_t actualCount, popCount, bit;

    std::uint32_t constexpr umax32 = std::numeric_limits<std::uint32_t>::max();
    for (std::uint32_t k = 0; k < 1024; ++k)
    {
        std::uint32_t i = mURD32(mDRE);
        std::uint32_t mask = 1;
        actualCount = 0;
        for (bit = 0; bit < 32; ++bit, mask <<= 1)
        {
            actualCount += ((i & mask) != 0 ? 1 : 0);
        }
        popCount = BitHacks::PopulationCount(i);
        UTAssert(popCount == actualCount,
            "PopulationCount failed: i = " + std::to_string(i));
    }
    popCount = BitHacks::PopulationCount(umax32);
    UTAssert(popCount == 32,
        "PopulationCount failed: i = " + std::to_string(umax32));

    std::uint64_t constexpr umax64 = std::numeric_limits<std::uint64_t>::max();
    for (std::uint32_t k = 0; k < 1024; ++k)
    {
        std::uint64_t i = mURD64(mDRE);
        std::uint64_t mask = 1ull;
        actualCount = 0;
        for (bit = 0; bit < 64; ++bit, mask <<= 1)
        {
            actualCount += ((i & mask) != 0 ? 1 : 0);
        }
        popCount = BitHacks::PopulationCount(i);
        UTAssert(popCount == actualCount,
            "PopulationCount failed: i = " + std::to_string(i));
    }
    popCount = BitHacks::PopulationCount(umax64);
    UTAssert(popCount == 64,
        "PopulationCount failed: i = " + std::to_string(umax64));
}

void UnitTestBitHacks::TestGetNumLeadingZeros()
{
    std::uint32_t actualCount, numLeading, bit;
    std::uint32_t constexpr umax32 = std::numeric_limits<std::uint32_t>::max();
    for (std::uint32_t k = 0; k < 1024; ++k)
    {
        std::uint32_t i = mURD32(mDRE);
        std::uint32_t j, mask = 0x80000000u;
        actualCount = 0;
        for (j = 0, bit = 31; j < 32; ++j, --bit, mask >>= 1)
        {
            if ((i & mask) == 0)
            {
                ++actualCount;
            }
            else
            {
                break;
            }
        }
        numLeading = BitHacks::GetNumLeadingZeroBits(i);
        UTAssert(numLeading == actualCount,
            "GetNumLeadingZeroBits failed: i = " + std::to_string(i));
    }
    numLeading = BitHacks::GetNumLeadingZeroBits(umax32);
    UTAssert(numLeading == 0,
        "GetNumLeadingZeroBits failed: i = " + std::to_string(umax32));

    std::uint64_t constexpr umax64 = std::numeric_limits<std::uint64_t>::max();
    for (std::uint32_t k = 0; k < 1024; ++k)
    {
        std::uint64_t i = mURD64(mDRE);
        std::uint64_t mask = 0x8000000000000000ull;
        std::uint32_t j;
        actualCount = 0;
        for (j = 0, bit = 63; j < 64; ++j, --bit, mask >>= 1)
        {
            if ((i & mask) == 0)
            {
                ++actualCount;
            }
            else
            {
                break;
            }
        }
        numLeading = BitHacks::GetNumLeadingZeroBits(i);
        UTAssert(numLeading == actualCount,
            "GetNumLeadingZeroBits count failed: i = " + std::to_string(i));
    }
    numLeading = BitHacks::GetNumLeadingZeroBits(umax64);
    UTAssert(numLeading == 0,
        "GetNumLeadingZeroBits failed: i = " + std::to_string(umax64));
}

void UnitTestBitHacks::TestGetNumTrailingZeros()
{
    std::uint32_t actualCount, numTrailing, bit;
    std::uint32_t constexpr umax32 = std::numeric_limits<std::uint32_t>::max();
    for (std::uint32_t k = 0; k < 1024; ++k)
    {
        std::uint32_t i = mURD32(mDRE);
        std::uint32_t mask = 0x00000001u;
        actualCount = 0;
        for (bit = 0; bit < 32; ++bit, mask <<= 1)
        {
            if ((i & mask) == 0)
            {
                ++actualCount;
            }
            else
            {
                break;
            }
        }
        numTrailing = BitHacks::GetNumTrailingZeroBits(i);
        UTAssert(numTrailing == actualCount,
            "GetNumTrailingZeroBits failed: i = " + std::to_string(i));
    }
    numTrailing = BitHacks::GetNumTrailingZeroBits(umax32);
    UTAssert(numTrailing == 0,
        "GetNumTrailingZeroBits failed: i = " + std::to_string(umax32));

    std::uint64_t constexpr umax64 = std::numeric_limits<std::uint64_t>::max();
    for (std::uint32_t k = 0; k < 1024; ++k)
    {
        std::uint64_t i = mURD64(mDRE);
        std::uint64_t mask = 0x0000000000000001ull;
        actualCount = 0;
        for (bit = 0; bit < 64; ++bit, mask <<= 1)
        {
            if ((i & mask) == 0)
            {
                ++actualCount;
            }
            else
            {
                break;
            }
        }
        numTrailing = BitHacks::GetNumTrailingZeroBits(i);
        UTAssert(numTrailing == actualCount,
            "GetNumTrailingZeroBits failed: i = " + std::to_string(i));
    }
    numTrailing = BitHacks::GetNumTrailingZeroBits(umax64);
    UTAssert(numTrailing == 0,
        "GetNumTrailingZeroBits failed: i = " + std::to_string(umax64));
}

void UnitTestBitHacks::TestGetLeadingBit()
{
    std::uint32_t actualLeading, leading, bit;

    std::uint32_t constexpr umax32 = std::numeric_limits<std::uint32_t>::max();
    for (std::uint32_t k = 0; k < 1024; ++k)
    {
        std::uint32_t i = mURD32(mDRE);
        std::uint32_t j, mask = 0x80000000u;
        actualLeading = 0;
        for (j = 0, bit = 31; j < 32; ++j, --bit, mask >>= 1)
        {
            if ((i & mask) != 0)
            {
                actualLeading = bit;
                break;
            }
        }
        leading = BitHacks::GetLeadingBit(i);
        UTAssert(leading == actualLeading,
            "GetLeadingBit failed: i = " + std::to_string(i));
    }
    leading = BitHacks::GetLeadingBit(umax32);
    UTAssert(leading == 31,
        "GetLeadingBit failed: i = " + std::to_string(umax32));

    std::uint64_t constexpr umax64 = std::numeric_limits<std::uint64_t>::max();
    for (std::uint32_t k = 0; k < 1024; ++k)
    {
        std::uint64_t i = mURD64(mDRE);
        std::uint64_t mask = 0x8000000000000000ull;
        std::uint32_t j;
        actualLeading = 0;
        for (j = 0, bit = 63; j < 64; ++j, --bit, mask >>= 1)
        {
            if ((i & mask) != 0)
            {
                actualLeading = bit;
                break;
            }
        }
        leading = BitHacks::GetLeadingBit(i);
        UTAssert(leading == actualLeading,
            "GetLeadingBit failed: i = " + std::to_string(i));
    }
    leading = BitHacks::GetLeadingBit(umax64);
    UTAssert(leading == 63,
        "GetLeadingBit failed: i = " + std::to_string(umax64));
}

void UnitTestBitHacks::TestTrailingBit()
{
    std::uint32_t actualTrailing, trailing, bit;

    std::uint32_t constexpr umax32 = std::numeric_limits<std::uint32_t>::max();
    for (std::uint32_t k = 0; k < 1024; ++k)
    {
        std::uint32_t i = mURD32(mDRE);
        std::uint32_t mask = 0x00000001u;
        actualTrailing = umax32;
        for (bit = 0; bit < 32; ++bit, mask <<= 1)
        {
            if ((i & mask) != 0)
            {
                actualTrailing = bit;
                break;
            }
        }
        trailing = BitHacks::GetTrailingBit(i);
        UTAssert(trailing == actualTrailing,
            "GetTrailingBit failed: i = " + std::to_string(i));
    }
    trailing = BitHacks::GetTrailingBit(umax32);
    UTAssert(trailing == 0,
        "GetTrailingBit failed: i = " + std::to_string(umax32));

    std::uint64_t constexpr umax64 = std::numeric_limits<std::uint64_t>::max();
    for (std::uint32_t k = 0; k < 1024; ++k)
    {
        std::uint64_t i = mURD64(mDRE);
        std::uint64_t mask = 0x0000000000000001ull;
        actualTrailing = 0;
        for (bit = 0; bit < 64; ++bit, mask <<= 1)
        {
            if ((i & mask) != 0)
            {
                actualTrailing = bit;
                break;
            }
        }
        trailing = BitHacks::GetTrailingBit(i);
        UTAssert(trailing == actualTrailing,
            "GetTrailingBit failed: i = " + std::to_string(i));
    }
    trailing = BitHacks::GetTrailingBit(umax64);
    UTAssert(trailing == 0,
        "GetTrailingBit failed: i = " + std::to_string(umax64));
}

void UnitTestBitHacks::TestIsPowerOfTwo()
{
    std::uint32_t constexpr umax32 = std::numeric_limits<std::uint32_t>::max();
    for (std::uint32_t k = 0; k < 1024; ++k)
    {
        std::uint32_t i = mURD32(mDRE);
        bool isPowerOfTwo = BitHacks::IsPowerOfTwo(i);
        bool hasSingleOne = (BitHacks::PopulationCount(i) == 1);
        UTAssert(isPowerOfTwo == hasSingleOne,
            "Population count IsPowerOfTwo: i = " + std::to_string(i));
    }
    UTAssert(BitHacks::IsPowerOfTwo(umax32) == false,
        "Population count failed: i = " + std::to_string(umax32));
}

void UnitTestBitHacks::TestLog2OfPowerOfTwo()
{
    std::uint32_t mask32 = 1;
    for (std::uint32_t bit = 0; bit < 32; ++bit, mask32 <<= 1)
    {
        std::uint32_t log2 = BitHacks::Log2OfPowerOfTwo(mask32);
        UTAssert(log2 == bit, "Log2OfPowerOfTwo failed.");
    }

    std::uint64_t mask64 = 1;
    for (std::uint32_t bit = 0; bit < 64; ++bit, mask64 <<= 1)
    {
        std::uint32_t log2 = BitHacks::Log2OfPowerOfTwo(mask64);
        UTAssert(log2 == bit, "Log2OfPowerOfTwo failed.");
    }
}

void UnitTestBitHacks::TestRoundUpToPowerOfTwo()
{
    std::array<std::uint32_t, 2> result32;
    for (std::uint32_t k = 0; k < 1024; ++k)
    {
        std::uint32_t i = mURD32(mDRE);
        result32 = BitHacks::RoundUpToPowerOfTwo(i);
        if ((i & 0x80000000u) == 0)
        {
            std::uint32_t leading = BitHacks::GetLeadingBit(i);
            std::uint32_t roundup = (1 << (leading + 1));
            UTAssert(result32[0] == roundup || result32[0] == i,
                "RoundUpToPowerOfTwo failed: i = " + std::to_string(i));
        }
        else
        {
            if (i != 0x80000000u)
            {
                UTAssert(result32[0] == 0 && result32[1] == 1,
                    "RoundUpToPowerOfTwo failed: i = " + std::to_string(i));
            }
            else
            {
                UTAssert(i == result32[0],
                    "RoundUpToPowerOfTwo failed: i = " + std::to_string(i));
            }
        }
    }

    std::array<std::uint64_t, 2> result64;
    std::uint64_t i;
    for (std::uint32_t k = 0; k < 1024; ++k)
    {
        i = mURD64(mDRE);
        result64 = BitHacks::RoundUpToPowerOfTwo(i);
        if ((i & 0x8000000000000000ull) == 0)
        {
            std::uint32_t leading = BitHacks::GetLeadingBit(i);
            std::uint64_t roundup = (1ull << (leading + 1));
            UTAssert(result64[0] == roundup || result64[0] == i,
                "RoundUpToPowerOfTwo failed: i = " + std::to_string(i));
        }
        else
        {
            if (i != 0x8000000000000000u)
            {
                UTAssert(result64[0] == 0 && result64[1] == 1,
                    "RoundUpToPowerOfTwo failed: i = " + std::to_string(i));
            }
            else
            {
                UTAssert(i == result64[0],
                    "RoundUpToPowerOfTwo failed: i = " + std::to_string(i));
            }
        }
    }

    i = 0x8000000000000000ull;
    result64 = BitHacks::RoundUpToPowerOfTwo(i);
    UTAssert(result64[0] == i, "RoundUpToPowerOfTwo failed: i = " + std::to_string(i));

    i = 0x8012340000000000ull;
    result64 = BitHacks::RoundUpToPowerOfTwo(i);
    UTAssert(result64[0] == 0 && result64[1] == 1, "RoundUpToPowerOfTwo failed: i = " + std::to_string(i));

    i = 0xFFFFFFFFFFFFFFFFull;
    result64 = BitHacks::RoundUpToPowerOfTwo(i);
    UTAssert(result64[0] == 0 && result64[1] == 1, "RoundUpToPowerOfTwo failed: i = " + std::to_string(i));
}

void UnitTestBitHacks::TestRoundDownToPowerOfTwo()
{
    std::uint32_t result32;
    for (std::uint32_t k = 0; k < 1024; ++k)
    {
        std::uint32_t i = mURD32(mDRE);
        result32 = BitHacks::RoundDownToPowerOfTwo(i);
        UTAssert(BitHacks::IsPowerOfTwo(result32) && i >= result32,
            "RoundDownToPowerOfTwo failed: i = " + std::to_string(i));
        if ((i & 0x80000000u) == 0)
        {
            std::uint32_t shift = (result32 << 1);
            UTAssert(shift > i,
                "RoundDownToPowerOfTwo failed: i = " + std::to_string(i));
        }
    }

    std::uint64_t result64;
    for (std::uint32_t k = 0; k < 1024; ++k)
    {
        std::uint64_t i = mURD64(mDRE);
        result64 = BitHacks::RoundDownToPowerOfTwo(i);
        UTAssert(BitHacks::IsPowerOfTwo(result64) && i >= result64,
            "RoundDownToPowerOfTwo failed: i = " + std::to_string(i));
        if ((i & 0x8000000000000000u) == 0)
        {
            std::uint64_t shift = (result64 << 1);
            UTAssert(shift > i,
                "RoundDownToPowerOfTwo failed: i = " + std::to_string(i));
        }
    }
}

#else

#include <GTL/Mathematics/Arithmetic/BitHacks.h>

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(BitHacks)
