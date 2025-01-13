// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Arithmetic/UIntegerAP32.h>
#include <GTL/Mathematics/Arithmetic/UIntegerFP32.h>
#include <GTL/Mathematics/Arithmetic/BSNumber.h>
#include <GTL/Utility/Timer.h>
#include <random>
using namespace gtl;

namespace gtl
{
    template class BSNumber<UIntegerAP32>;
    template class BSNumber<UIntegerFP32<8>>;
    using APNumber = BSNumber<UIntegerAP32>;
    using FPNumber = BSNumber<UIntegerFP32<8>>;
}

namespace gtl
{
    class UnitTestBSNumber
    {
    public:
        UnitTestBSNumber();

    private:
        template <typename Number> void Constructors();
        template <typename Number> void ConvertFloatToBSNumber();
        template <typename Number> void ConvertDoubleToBSNumber();
        template <typename Number> void ConvertBSNumberToFloat();
        template <typename Number> void ConvertBSNumberToDouble();
        template <typename Number> void ConvertBSNumberSpecifiedPrecision();
        template <typename Number> void EqualityComparisons();
        template <typename Number> void InequalityComparisons();
        template <typename Number> void UnaryOperators();
        template <typename Number> void Addition();
        template <typename Number> void Subtraction();
        template <typename Number> void Multiplication();

        template <typename Number> void Conversions();
        template <typename Number> void PositiveRoundNearest(float z, Number const& input);
        template <typename Number> void PositiveRoundUpward(float z, Number const& input);
        template <typename Number> void PositiveRoundDownward(float z, Number const& input);
        template <typename Number> void PositiveRoundTowardZero(float z, Number const& input);
        template <typename Number> void NegativeRoundNearest(float z, Number const& input);
        template <typename Number> void NegativeRoundUpward(float z, Number const& input);
        template <typename Number> void NegativeRoundDownward(float z, Number const& input);
        template <typename Number> void NegativeRoundTowardZero(float z, Number const& input);

        std::default_random_engine mDRE;
        std::uniform_int_distribution<std::uint32_t> mURD;
    };
}

UnitTestBSNumber::UnitTestBSNumber()
    :
    mURD(1u, (1u << 23))
{
    UTInformation("Mathematics/Arithmetic/BSNumber");

    Constructors<APNumber>();
    Constructors<FPNumber>();

    ConvertFloatToBSNumber<APNumber>();
    ConvertFloatToBSNumber<FPNumber>();

    ConvertDoubleToBSNumber<APNumber>();
    ConvertDoubleToBSNumber<FPNumber>();

    ConvertBSNumberToFloat<APNumber>();
    ConvertBSNumberToFloat<FPNumber>();

    ConvertBSNumberToDouble<APNumber>();
    ConvertBSNumberToDouble<FPNumber>();

    ConvertBSNumberSpecifiedPrecision<APNumber>();
    ConvertBSNumberSpecifiedPrecision<FPNumber>();

    EqualityComparisons<APNumber>();
    EqualityComparisons<FPNumber>();

    InequalityComparisons<APNumber>();
    InequalityComparisons<FPNumber>();

    UnaryOperators<APNumber>();
    UnaryOperators<FPNumber>();

    Addition<APNumber>();
    Addition<FPNumber>();

    Subtraction<APNumber>();
    Subtraction<FPNumber>();

    Multiplication<APNumber>();
    Multiplication<FPNumber>();

    Conversions<APNumber>();
    Conversions<FPNumber>();
}

template <typename Number>
void UnitTestBSNumber::Constructors()
{
    Number x;

    x = Number(0);
    UTAssert(x.GetSign() == 0, "Failed to construct 0.");

    x = Number(17);
    UTAssert(
        x.GetSign() == 1 &&
        x.GetBiasedExponent() == 0 &&
        x.GetUInteger().GetNumBits() == 5 &&
        x.GetUInteger().GetNumBlocks() == 1 &&
        x.GetUInteger().GetBits()[0] == 17,
        "Failed to construct 17.");

    x = Number(-34);
    UTAssert(
        x.GetSign() == -1 &&
        x.GetBiasedExponent() == 1 &&
        x.GetUInteger().GetNumBits() == 5 &&
        x.GetUInteger().GetNumBlocks() == 1 &&
        x.GetUInteger().GetBits()[0] == 17,
        "Failed to construct -34.");

    x = Number(0u);
    UTAssert(x.GetSign() == 0, "Failed to construct 0u.");

    x = Number(34u);
    UTAssert(
        x.GetSign() == 1 &&
        x.GetBiasedExponent() == 1 &&
        x.GetUInteger().GetNumBits() == 5 &&
        x.GetUInteger().GetNumBlocks() == 1 &&
        x.GetUInteger().GetBits()[0] == 17,
        "Failed to construct 34u.");

    x = Number(static_cast<std::int64_t>(0));
    UTAssert(x.GetSign() == 0, "Failed to construct 0ll.");

    x = Number(static_cast<std::int64_t>(17));
    UTAssert(
        x.GetSign() == 1 &&
        x.GetBiasedExponent() == 0 &&
        x.GetUInteger().GetNumBits() == 5 &&
        x.GetUInteger().GetNumBlocks() == 1 &&
        x.GetUInteger().GetBits()[0] == 17,
        "Failed to construct 17ll.");

    x = Number(static_cast<std::int64_t>(-34));
    UTAssert(
        x.GetSign() == -1 &&
        x.GetBiasedExponent() == 1 &&
        x.GetUInteger().GetNumBits() == 5 &&
        x.GetUInteger().GetNumBlocks() == 1 &&
        x.GetUInteger().GetBits()[0] == 17,
        "Failed to construct -34ll.");

    x = Number(static_cast<std::uint64_t>(0));
    UTAssert(x.GetSign() == 0, "Failed to construct 0ull.");

    x = Number(static_cast<std::uint64_t>(34));
    UTAssert(
        x.GetSign() == 1 &&
        x.GetBiasedExponent() == 1 &&
        x.GetUInteger().GetNumBits() == 5 &&
        x.GetUInteger().GetNumBlocks() == 1 &&
        x.GetUInteger().GetBits()[0] == 17,
        "Failed to construct 34ull.");

    x = Number("12345678901234567890123456789");
    UTAssert(
        x.GetSign() == 1 &&
        x.GetBiasedExponent() == 0 &&
        x.GetUInteger().GetNumBits() == 94 &&
        x.GetUInteger().GetNumBlocks() == 3 &&
        x.GetUInteger().GetBits()[0] == 0x6e398115u &&
        x.GetUInteger().GetBits()[1] == 0x46bec9b1u &&
        x.GetUInteger().GetBits()[2] == 0x27e41b32u,
        "Failed to construct from string.");

    Number y = x;
    UTAssert(
        y.GetSign() == 1 &&
        y.GetBiasedExponent() == 0 &&
        y.GetUInteger().GetNumBits() == 94 &&
        y.GetUInteger().GetNumBlocks() == 3 &&
        y.GetUInteger().GetBits()[0] == 0x6e398115u &&
        y.GetUInteger().GetBits()[1] == 0x46bec9b1u &&
        y.GetUInteger().GetBits()[2] == 0x27e41b32u,
        "Failed to construct from copy constructor.");

    Number z = std::move(x);
    UTAssert(
        z == y,
        "Failed to construct from move constructor.");
}

template <typename Number>
void UnitTestBSNumber::ConvertFloatToBSNumber()
{
    Number x;
    IEEEBinary32 y;

    // Convert +0.
    x = Number(0.0f);
    UTAssert(x.GetSign() == 0, "Failed to convert +0.0f to Number.");

    // Convert -0.
    x = Number(-0.0f);
    UTAssert(x.GetSign() == 0, "Failed to convert -0.0f to Number.");

    // Convert subnormals.
    // x = +0.00000000000000000000001 * 2^{-126}
    //   = +1.0 * 2{-149}
    //   = +1 * 2^{-149}, numBits = 1
    y.SetEncoding(0, 0, 0x00000001);
    x = Number(y.number);
    UTAssert(
        x.GetSign() == 1 &&
        x.GetBiasedExponent() == -149 &&
        x.GetUInteger().GetNumBits() == 1 &&
        x.GetUInteger().GetNumBlocks() == 1 &&
        x.GetUInteger().GetBits()[0] == 1,
        "Failed to convert subnormal 0x00000001.");

    // x = -0.00000101010101111001110 * 2^{-126}
    //   = -1.01010101111001110 * 2^{-132}
    //   = -10101010111100111 * 2^{-148}, numBits = 17
    //   = -0x000155E7 * 2^{-148}
    y.SetEncoding(1, 0, 0x0002ABCE);
    x = Number(y.number);
    UTAssert(
        x.GetSign() == -1 &&
        x.GetBiasedExponent() == -148 &&
        x.GetUInteger().GetNumBits() == 17 &&
        x.GetUInteger().GetNumBlocks() == 1 &&
        x.GetUInteger().GetBits()[0] == 0x000155E7,
        "Failed to convert subnormal 0x8002abce.");

    // Convert normals.
    // x = +0x40490FDB
    //   = +1.10010010000111111011011 * 2^{1}
    //   = +110010010000111111011011 * 2^{-22}, numBits = 24
    //   = +C90FDB * 2^{-22}
    x = Number(C_PI<float>);
    UTAssert(
        x.GetSign() == 1 &&
        x.GetBiasedExponent() == -22 &&
        x.GetUInteger().GetNumBits() == 24 &&
        x.GetUInteger().GetNumBlocks() == 1 &&
        x.GetUInteger().GetBits()[0] == 0x00C90FDB,
        "Failed to convert normal pi.");

    x = Number(16.0f);
    UTAssert(
        x.GetSign() == 1 &&
        x.GetBiasedExponent() == 4 &&
        x.GetUInteger().GetNumBits() == 1 &&
        x.GetUInteger().GetNumBlocks() == 1 &&
        x.GetUInteger().GetBits()[0] == 1,
        "Failed to convert normal 16.0f.");

#if !defined(GTL_THROW_ON_CONVERT_FROM_INFINITY_OR_NAN)
    // Convert infinities.
    y.encoding = IEEEBinary32::POS_INFINITY;
    x = Number(y.number);
    UTAssert(
        x.GetSign() == 1 &&
        x.GetBiasedExponent() == 128 &&
        x.GetUInteger().GetNumBits() == 1 &&
        x.GetUInteger().GetNumBlocks() == 1 &&
        x.GetUInteger().GetBits()[0] == 1,
        "Failed to convert +infinity.");

    y.encoding = IEEEBinary32::NEG_INFINITY;
    x = Number(y.number);
    UTAssert(
        x.GetSign() == -1 &&
        x.GetBiasedExponent() == 128 &&
        x.GetUInteger().GetNumBits() == 1 &&
        x.GetUInteger().GetNumBlocks() == 1 &&
        x.GetUInteger().GetBits()[0] == 1,
        "Failed to convert -infinity.");

    // Convert NaNs.
    y.SetEncoding(0, 255, 1);
    x = Number(y.number);
    UTAssert(x.GetSign() == 0, "Failed to convert NaN.");
#endif
}

template <typename Number>
void UnitTestBSNumber::ConvertDoubleToBSNumber()
{
    APNumber x;
    IEEEBinary64 y;

    // Convert +0.
    x = APNumber(0.0);
    UTAssert(x.GetSign() == 0, "Failed to convert +0.0 to APNumber.");

    // Convert -0.
    x = APNumber(-0.0);
    UTAssert(x.GetSign() == 0, "Failed to convert -0.0 to APNumber.");

    // Convert subnormals.
    // x = +0.0^{51}1 * 2^{-1022}
    //   = +1.0 * 2{-1074}
    //   = +1 * 2^{-1074}, numBits = 1
    y.SetEncoding(0, 0, 1);
    x = APNumber(y.number);
    UTAssert(
        x.GetSign() == 1 &&
        x.GetBiasedExponent() == -1074 &&
        x.GetUInteger().GetNumBits() == 1 &&
        x.GetUInteger().GetNumBlocks() >= 1 &&
        x.GetUInteger().GetBits()[0] == 1,
        "Failed to convert subnormal 1u64.");

    // x = -0.0001001000110100010101100111100010101011110011011110 * 2^{-1022}
    //   = -1.001000110100010101100111100010101011110011011110 * 2^{-1026}
    //   = -100100011010001010110011110001010101111001101111 * 2^{-1073}, numBits = 48
    //   = -0x000091A2B3C55E6F * 2^{-1073}
    y.SetEncoding(1, 0, static_cast<std::uint64_t>(0x00012345678ABCDEull));
    x = APNumber(y.number);
    UTAssert(
        x.GetSign() == -1 &&
        x.GetBiasedExponent() == -1073 &&
        x.GetUInteger().GetNumBits() == 48 &&
        x.GetUInteger().GetNumBlocks() == 2 &&
        x.GetUInteger().GetBits()[0] == 0xB3C55E6F &&
        x.GetUInteger().GetBits()[1] == 0x000091A2,
        "Failed to convert subnormal 0x80012345678ABCDE.");

    // Convert normals.
    // x = +0x400921FB54442D18
    //   = +1.921FB54442D18 * 2^{1}
    //   = +1.1001001000011111101101010100010001000010110100011000 * 2^{1}
    //   = +11001001000011111101101010100010001000010110100011 * 2^{-48}, numBits = 50
    //   = +C90FDB * 2^{-22}
    x = C_PI<APNumber>;
    UTAssert(
        x.GetSign() == 1 &&
        x.GetBiasedExponent() == -48 &&
        x.GetUInteger().GetNumBits() == 50 &&
        x.GetUInteger().GetNumBlocks() == 2 &&
        x.GetUInteger().GetBits()[0] == 0x6A8885A3 &&
        x.GetUInteger().GetBits()[1] == 0x0003243F,
        "Failed to convert normal pi.");

    x = APNumber(16.0);
    UTAssert(
        x.GetSign() == 1 &&
        x.GetBiasedExponent() == 4 &&
        x.GetUInteger().GetNumBits() == 1 &&
        x.GetUInteger().GetNumBlocks() >= 1 &&
        x.GetUInteger().GetBits()[0] == 1,
        "Failed to convert normal 16.0.");

#if !defined(GTL_THROW_ON_CONVERT_FROM_INFINITY_OR_NAN)
    // Convert infinities.
    y.encoding = IEEEBinary64::POS_INFINITY;
    x = APNumber(y.number);
    UTAssert(
        x.GetSign() == 1 &&
        x.GetBiasedExponent() == 1024 &&
        x.GetUInteger().GetNumBits() == 1 &&
        x.GetUInteger().GetNumBlocks() >= 1 &&
        x.GetUInteger().GetBits()[0] == 1,
        "Failed to convert +infinity.");

    y.encoding = IEEEBinary64::NEG_INFINITY;
    x = APNumber(y.number);
    UTAssert(
        x.GetSign() == -1 &&
        x.GetBiasedExponent() == 1024 &&
        x.GetUInteger().GetNumBits() == 1 &&
        x.GetUInteger().GetNumBlocks() >= 1 &&
        x.GetUInteger().GetBits()[0] == 1,
        "Failed to convert -infinity.");

    // Convert NaNs.
    y.SetEncoding(0, 2047, 1);
    x = APNumber(y.number);
    UTAssert(x.GetSign() == 0, "Failed to convert NaN.");
#endif
}

template <typename Number>
void UnitTestBSNumber::ConvertBSNumberToFloat()
{
    Number x{};
    IEEEBinary32 x32;
    IEEEBinary64 x64, x64prev, x64next;

    // The default constructor should set x to 0.
    UTAssert(x32.encoding == 0x00000000u,
        "Failed to convert +0.");

    // Convert from numbers smaller than minsubnormal.
    x64.number = std::ldexp(1.0, -150);
    x64prev.encoding = x64.GetNextDown();
    x64next.encoding = x64.GetNextUp();
    x = Number(x64.number);
    x32.number = static_cast<float>(x);
    UTAssert(x32.encoding == 0x00000000u,
        "Failed to convert 2^{-150}.");
    x = Number(x64prev.number);
    x32.number = static_cast<float>(x);
    UTAssert(x32.encoding == 0x00000000u,
        "Failed to convert 2^{-150}-epsilon.");
    x = Number(x64next.number);
    x32.number = static_cast<float>(x);
    UTAssert(x32.encoding == 0x00000001u,
        "Failed to convert 2^{-150}+epsilon.");

    // Convert from numbers smaller than minnormal. The "_" indicates the
    // implied binary point after ULPS.
    double const twoPowN149 = std::ldexp(1.0, -149);
    float fy;
    for (std::uint32_t k = 0; k < 1024; ++k)
    {
        std::uint32_t i = mURD(mDRE);
        double const di = static_cast<double>(i);
        double value = di * twoPowN149;
        fy = static_cast<float>(value);
        x = value;
        x32.number = static_cast<float>(x);
        UTAssert(x32.number == fy, "Failed to convert " + std::to_string(value));

        value = (di + 0.12345) * twoPowN149;
        fy = static_cast<float>(value);
        x = value;
        x32.number = static_cast<float>(x);
        UTAssert(x32.number == fy, "Failed to convert " + std::to_string(value));

        value = (di + 0.5) * twoPowN149;
        fy = static_cast<float>(value);
        x = value;
        x32.number = static_cast<float>(x);
        UTAssert(x32.number == fy, "Failed to convert " + std::to_string(value));

        value = (di + 0.56789) * twoPowN149;
        fy = static_cast<float>(value);
        x = value;
        x32.number = static_cast<float>(x);
        UTAssert(x32.number == fy, "Failed to convert " + std::to_string(value));
    }

    // Convert from numbers smaller than maxnormal. The "_" indicates the
    // implied binary point after ULPS.

    // 1.0 * 2^{-126} is an exact conversion
    x64.number = std::ldexp(1.0, -126);
    x = Number(x64.number);
    x32.number = static_cast<float>(x);
    UTAssert(x32.encoding == 0x00800000u,
        "Failed to convert 2^{-126}.");

    // 1.0^{22}1_01 * 2^{1}, round down
    // 02000005 * 2^{-24}, 26 bits
    x64.number = 2.0 * (1.0 + std::ldexp(1.0, -23) + std::ldexp(1.0, -25));
    x = Number(x64.number);
    x32.number = static_cast<float>(x);
    UTAssert(x32.encoding == 0x40000001u,
        "Failed to convert 2*(1+2^{-23}+2^{-25}).");

    // 1.0^{22}1_10 * 2^{1}, round up
    // 01000003 * 2^{-23}, 25 bits
    x64.number = 2.0 * (1.0 + std::ldexp(1.0, -23) + std::ldexp(1.0, -24));
    x = Number(x64.number);
    x32.number = static_cast<float>(x);
    UTAssert(x32.encoding == 0x40000002u,
        "Failed to convert 2*(1+2^{-23}+2^{-25}).");

    // 1.0^{22}1_11 * 2^{1}, round up
    // 02000007 * 2^{-24}, 26 bits
    x64.number = 2.0 * (1.0 + std::ldexp(1.0, -23) + std::ldexp(1.0, -24) + std::ldexp(1.0, -25));
    x = Number(x64.number);
    x32.number = static_cast<float>(x);
    UTAssert(x32.encoding == 0x40000002u,
        "Failed to convert 2*(1+2^{-23}+2^{-24}+2^{-25}).");

    // 1.0^{21}10_01 * 2^{1}, round down
    // 02000009 * 2^{-24}, 26 bits
    x64.number = 2.0 * (1.0 + std::ldexp(1.0, -22) + std::ldexp(1.0, -25));
    x = Number(x64.number);
    x32.number = static_cast<float>(x);
    UTAssert(x32.encoding == 0x40000002u,
        "Failed to convert 2*(1+2^{-22}+2^{-25}).");

    // 1.0^{21}10_10 * 2^{1}, round down
    // 01000005 * 2^{-23}, 25 bits
    x64.number = 2.0 * (1.0 + std::ldexp(1.0, -22) + std::ldexp(1.0, -24));
    x = Number(x64.number);
    x32.number = static_cast<float>(x);
    UTAssert(x32.encoding == 0x40000002u,
        "Failed to convert 2*(1+2^{-22}+2^{-24}).");

    // 1.0^{21}10_11 * 2^{1}, round up
    // 0200000b * 2^{-24}, 26 bits
    x64.number = 2.0 * (1.0 + std::ldexp(1.0, -22) + std::ldexp(1.0, -24) + std::ldexp(1.0, -25));
    x = Number(x64.number);
    x32.number = static_cast<float>(x);
    UTAssert(x32.encoding == 0x40000003u,
        "Failed to convert 2*(1+2^{-22}+2^{-24}+2^{-25}).");

    // 1.1^{23}_1 * 2^{1}, round up, carry out
    // 01ffffff * 2^{-24}, 25 bits
    x64.number = 2.0 * (2.0 - std::ldexp(1.0, -24));
    x = Number(x64.number);
    x32.number = static_cast<float>(x);
    UTAssert(x32.encoding == 0x40800000u,
        "Failed to convert 2*(2 - 2^{-24}).");

    // Convert from number larger or equal to MAX_NORMAL.
    // maxnor = 2^{128} - 2^{104}
    // inf = 2^{128}
    // avrmaxnorinf = 0.5 * (maxnor + inf)
    // mid0 = (maxnor + avrmaxnorinf) / 2 = 0.75 * maxnor + 0.25 * inf
    // mid1 = (avrmaxnorinf + inf) / 2 = 0.25 * maxnor + 0.75 * inf
    double inf = std::ldexp(1.0, 128);
    double maxnor = inf - std::ldexp(1.0, 104);
    double avrmaxnorinf = 0.5 * (maxnor + inf);
    double mid0 = 0.5 * (maxnor + avrmaxnorinf);
    double mid1 = 0.5 * (avrmaxnorinf + inf);

    // Rounds to maxnor.
    x = Number(maxnor);
    x32.number = static_cast<float>(x);
    UTAssert(x32.encoding == 0x7f7fffffu,
        "Failed to convert maxnor.");

    // Rounds to inf.
    x = Number(inf);
    x32.number = static_cast<float>(x);
    UTAssert(x32.encoding == 0x7f800000u,
        "Failed to convert number inf.");

    // Rounds to inf.
    x = Number(avrmaxnorinf);
    x32.number = static_cast<float>(x);
    UTAssert(x32.encoding == 0x7f800000u,
        "Failed to convert number avrmaxnorinf.");

    // Rounds to maxnor.
    x = Number(mid0);
    x32.number = static_cast<float>(x);
    UTAssert(x32.encoding == 0x7f7fffffu,
        "Failed to convert number mid0.");

    // Rounds to inf.
    x = Number(mid1);
    x32.number = static_cast<float>(x);
    UTAssert(x32.encoding == 0x7f800000u,
        "Failed to convert number mid1.");
}

template <typename Number>
void UnitTestBSNumber::ConvertBSNumberToDouble()
{
    Number const one(1), two(2), half(0.5);
    Number x, y;
    IEEEBinary64 x64;

    // Convert from numbers smaller than minsubnormal.
    x = std::ldexp(one, -1075) - std::ldexp(one, -1076);
    x64.number = static_cast<double>(x);
    UTAssert(x64.encoding == 0x0000000000000000ull,
        "Failed to convert 2^{-1075}.");

    x = std::ldexp(one, -1075) - std::ldexp(one, -1076);
    x64.number = static_cast<double>(x);
    UTAssert(x64.encoding == 0x0000000000000000ull,
        "Failed to convert 2^{-1075} - 2^{-1076}.");

    x = std::ldexp(one, -1075) + std::ldexp(one, -1076);
    x64.number = static_cast<double>(x);
    UTAssert(x64.encoding == 0x0000000000000001ull,
        "Failed to convert 2^{-1075} + 2^{-1076}.");

    // Convert from numbers smaller than minnormal. The "_" indicates the
    // implied binary point after ULPS.
    Timer timer;
    Number const twoPowN1074 = std::ldexp(one, -1074);
    Number const delta0(0.12345), delta1(0.56789);
    Number di, dip1, min, max;
    for (std::uint32_t k = 0; k < 1024; ++k)
    {
        std::uint32_t i = mURD(mDRE);
        di = Number(i);
        dip1 = Number(i + 1);
        min = di * twoPowN1074;
        max = dip1 * twoPowN1074;

        x = min;
        x64.number = static_cast<double>(x);
        y = Number(x64.number);
        UTAssert(y == min, "Failed to convert number to double: " + std::to_string(i));

        x = min + delta0 * twoPowN1074;
        x64.number = static_cast<double>(x);
        y = Number(x64.number);
        UTAssert(y == min, "Failed to convert number to double: " + std::to_string(i));

        x = min + half * twoPowN1074;
        x64.number = static_cast<double>(x);
        y = Number(x64.number);
        if (i & 1)
        {
            UTAssert(y == max, "Failed to convert number to double: " + std::to_string(i));
        }
        else
        {
            UTAssert(y == min, "Failed to convert number to double: " + std::to_string(i));
        }

        x = min + delta1 * twoPowN1074;
        x64.number = static_cast<double>(x);
        y = Number(x64.number);
        UTAssert(y == max, "Failed to convert number to double: " + std::to_string(i));
    }

    di = Number(static_cast<std::uint64_t>((1ull << 52) - 1ull));
    dip1 = Number(static_cast<std::uint64_t>(1ull << 52));
    min = di * twoPowN1074;
    max = dip1 * twoPowN1074;
    x = min;
    x64.number = static_cast<double>(x);
    y = Number(x64.number);
    UTAssert(y == min, "Failed to convert number to double: (1ull << 52)");

    x = min + delta0 * twoPowN1074;
    x64.number = static_cast<double>(x);
    y = Number(x64.number);
    UTAssert(y == min, "Failed to convert number to double: (1ull << 52)");

    x = min + half * twoPowN1074;
    x64.number = static_cast<double>(x);
    y = Number(x64.number);
    UTAssert(y == max, "Failed to convert number to double: (1ull << 52)");

    x = min + delta1 * twoPowN1074;
    x64.number = static_cast<double>(x);
    y = Number(x64.number);
    UTAssert(y == max, "Failed to convert number to double: (1ull << 52)");

    auto msecs = timer.GetMilliseconds();
    std::cout << "milliseconds = " << msecs << std::endl;

    // Convert from numbers smaller than maxnormal.  The "_" indicates the
    // implied binary point after ULPS.

    // 1.0 * 2^{-126} is an exact conversion
    x = std::ldexp(one, -1022);
    x64.number = static_cast<double>(x);
    UTAssert(x64.encoding == 0x0010000000000000ull,
        "Failed to convert 2^{-1022}.");

    // 1.0^{51}1_01 * 2^{1}, round down
    // 00400000 00000005 * 2^{-53}, 55 bits
    x = two * (one + std::ldexp(one, -52) + std::ldexp(one, -54));
    x64.number = static_cast<double>(x);
    UTAssert(x64.encoding == 0x4000000000000001ull,
        "Failed to convert 2*(1+2^{-52}+2^{-54}).");

    // 1.0^{51}1_10 * 2^{1}, round up
    // 00200000 0000003 * 2^{-52}, 54 bits
    x = two * (one + std::ldexp(one, -52) + std::ldexp(one, -53));
    x64.number = static_cast<double>(x);
    UTAssert(x64.encoding == 0x4000000000000002ull,
        "Failed to convert 2*(1+2^{-52}+2^{-53}).");

    // 1.0^{51}1_11 * 2^{1}, round up
    // 00400000 00000007  * 2^{-53}, 55 bits
    x = two * (one + std::ldexp(one, -52) + std::ldexp(one, -53) + std::ldexp(one, -54));
    x64.number = static_cast<double>(x);
    UTAssert(x64.encoding == 0x4000000000000002ull,
        "Failed to convert 2*(1+2^{-52}+2^{-53}+2^{-54}).");

    // 1.0^{50}10_01 * 2^{1}, round down
    // 00400000 00000009 * 2^{-53}, 55 bits
    x = two * (one + std::ldexp(one, -51) + std::ldexp(one, -54));
    x64.number = static_cast<double>(x);
    UTAssert(x64.encoding == 0x4000000000000002ull,
        "Failed to convert 2*(1+2^{-51}+2^{-53}).");

    // 1.0^{50}10_10 * 2^{1}, round down
    // 00200000 00000005  * 2^{-52}, 54 bits
    x = two * (one + std::ldexp(one, -51) + std::ldexp(one, -53));
    x64.number = static_cast<double>(x);
    UTAssert(x64.encoding == 0x4000000000000002ull,
        "Failed to convert 2*(1+2^{-51}+2^{-53}).");

    // 1.0^{51}10_11 * 2^{1}, round up
    // 00400000 0000000b * 2^{-53}, 55 bits
    x = two * (one + std::ldexp(one, -51) + std::ldexp(one, -53) + std::ldexp(one, -54));
    x64.number = static_cast<double>(x);
    UTAssert(x64.encoding == 0x4000000000000003ull,
        "Failed to convert 2*(1+2^{-51}+2^{-53}+2^{-54}).");

    // 1.1^{52}_1 * 2^{1}, round up, carry out
    // 003fffff ffffffff * 2^{-52}, 54 bits
    x = two * (two - std::ldexp(one, -53));
    x64.number = static_cast<double>(x);
    UTAssert(x64.encoding == 0x4010000000000000ull,
        "Failed to convert 2*(2 - 2^{-53}).");

    // Convert from number larger or equal to MAX_NORMAL.
    // maxnor = 2^{1024} - 2^{971}
    // inf = 2^{1024}
    // avrmaxnorinf = 0.5 * (maxnor + inf)
    // mid0 = (maxnor + avrmaxnorinf) / 2 = 0.75 * maxnor + 0.25 * inf
    // mid1 = (avrmaxnorinf + inf) / 2 = 0.25 * maxnor + 0.75 * inf
    Number inf = std::ldexp(one, 1024);
    Number maxnor = inf - std::ldexp(one, 971);
    Number avrmaxnorinf = half * (maxnor + inf);
    Number mid0 = half * (maxnor + avrmaxnorinf);
    Number mid1 = half * (avrmaxnorinf + inf);

    // Rounds to maxnor.
    x64.number = static_cast<double>(maxnor);
    UTAssert(x64.number == std::numeric_limits<double>::max(),
        "Failed to convert maxnor.");

    // Rounds to inf.
    x64.number = static_cast<double>(inf);
    UTAssert(x64.number == std::numeric_limits<double>::infinity(),
        "Failed to convert number inf.");

    // Rounds to inf.
    x64.number = static_cast<double>(avrmaxnorinf);
    UTAssert(x64.number == std::numeric_limits<double>::infinity(),
        "Failed to convert number avrmaxnorinf.");

    // Rounds to maxnor.
    x64.number = static_cast<double>(mid0);
    UTAssert(x64.number == std::numeric_limits<double>::max(),
        "Failed to convert number mid0.");

    // Rounds to inf.
    x = Number(mid1);
    x64.number = static_cast<double>(mid1);
    UTAssert(x64.number == std::numeric_limits<double>::infinity(),
        "Failed to convert number mid1.");
}

template <typename Number>
void UnitTestBSNumber::ConvertBSNumberSpecifiedPrecision()
{
    Number x(std::numeric_limits<double>::max());
    // x.biasedExponent = 971, x.exponent = 1023
    // x.numBits = 53, x.bits = 001fffff ffffffff
    // x = 1^[53] * 2^{971}
    //   = 1.1^[52] * 2^{1023} 
    UTAssert(
        x.GetBiasedExponent() == 971 &&
        x.GetExponent() == 1023 &&
        x.GetUInteger().GetNumBits() == 53 &&
        x.GetUInteger().GetNumBlocks() == 2 &&
        x.GetUInteger().GetBits()[0] == 0xffffffffu &&
        x.GetUInteger().GetBits()[1] == 0x001fffffu,
        "Incorrect representation.");

    Number xsqr = x * x;
    // xsqr.biasedExponent = 1942, xsqr.exponent = 2047
    // xsqr.numBits = 106, xsqr.bits = 000003ff ffffffff ffc00000 00000001
    // xsqr = 1^[52] 0^[53] 1^[1] * 2^{1942}
    //      = 1.1^[51] 0^[53] 1^[1] * 2^{2047}
    UTAssert(
        xsqr.GetBiasedExponent() == 1942 &&
        xsqr.GetExponent() == 2047 &&
        xsqr.GetUInteger().GetNumBits() == 106 &&
        xsqr.GetUInteger().GetNumBlocks() == 4 &&
        xsqr.GetUInteger().GetBits()[0] == 0x00000001u &&
        xsqr.GetUInteger().GetBits()[1] == 0xffc00000u &&
        xsqr.GetUInteger().GetBits()[2] == 0xffffffffu &&
        xsqr.GetUInteger().GetBits()[3] == 0x000003ffu,
        "Incorrect representation.");

    // The first example here leads first to a bit pattern that has
    // 1^[52] 0^[20] internally. This has to be shifted right to satisfy
    // the invariant that the UInteger part of y is a positive odd number.
    Number y{};
    xsqr.Convert(72, APRoundingMode::TO_NEAREST, y);
    // y.biasedExponent = 1996, y.exponent = 2047
    // y.numBits = 52, y.bits = 000fffff ffffffff
    // y = 1^[52] * 2^{1996}
    //   = 1.1^[51] * 2^{2047}
    UTAssert(
        y.GetBiasedExponent() == 1996 &&
        y.GetExponent() == 2047 &&
        y.GetUInteger().GetNumBits() == 52 &&
        y.GetUInteger().GetNumBlocks() == 2 &&
        y.GetUInteger().GetBits()[0] == 0xffffffffu &&
        y.GetUInteger().GetBits()[1] == 0x000fffffu,
        "Incorrect representation.");

    xsqr.Convert(52, APRoundingMode::TO_NEAREST, y);
    // y.biasedExponent = 1996, y.exponent = 2047
    // y.numBits = 52, y.bits = 000fffff ffffffff
    // y = 1^[52] * 2^{1996}
    //   = 1.1^[51] * 2^{2047}
    // xsqr = 1.1^[51] 0^[53] 1^[1] * 2^{2047}
    UTAssert(
        y.GetBiasedExponent() == 1996 &&
        y.GetExponent() == 2047 &&
        y.GetUInteger().GetNumBits() == 52 &&
        y.GetUInteger().GetNumBlocks() == 2 &&
        y.GetUInteger().GetBits()[0] == 0xffffffffu &&
        y.GetUInteger().GetBits()[1] == 0x000fffffu,
        "Incorrect representation.");

    xsqr.Convert(52, APRoundingMode::UPWARD, y);
    // y.biasedExponent = 2048, y.exponent = 2048
    // y.numBits = 1, y.bits = 1
    // y = 1 * 2^{2048}
    // xsqr = 1.1^[51] 0^[53] 1^[1] * 2^{2047}
    UTAssert(
        y.GetBiasedExponent() == 2048 &&
        y.GetExponent() == 2048 &&
        y.GetUInteger().GetNumBits() == 1 &&
        y.GetUInteger().GetNumBlocks() == 1 &&
        y.GetUInteger().GetBits()[0] == 0x00000001u,
        "Incorrect representation.");

    xsqr.Convert(52, APRoundingMode::DOWNWARD, y);
    // y.biasedExponent = 1996, y.exponent = 2047
    // y.numBits = 52, y.bits = 000fffff ffffffff
    // y = 1^[52] * 2^{1996}
    //   = 1.1^[51] * 2^{2047}
    // xsqr = 1.1^[51] 0^[53] 1^[1] * 2^{2047}
    UTAssert(
        y.GetBiasedExponent() == 1996 &&
        y.GetExponent() == 2047 &&
        y.GetUInteger().GetNumBits() == 52 &&
        y.GetUInteger().GetNumBlocks() == 2 &&
        y.GetUInteger().GetBits()[0] == 0xffffffffu &&
        y.GetUInteger().GetBits()[1] == 0x000fffffu,
        "Incorrect representation.");

    xsqr.Convert(52, APRoundingMode::TOWARD_ZERO, y);
    // y.biasedExponent = 1996, y.exponent = 2047
    // y.numBits = 52, y.bits = 000fffff ffffffff
    // y = 1^[52] * 2^{1996}
    //   = 1.1^[51] * 2^{2047}
    // xsqr = 1.1^[51] 0^[53] 1^[1] * 2^{2047}
    UTAssert(
        y.GetBiasedExponent() == 1996 &&
        y.GetExponent() == 2047 &&
        y.GetUInteger().GetNumBits() == 52 &&
        y.GetUInteger().GetNumBlocks() == 2 &&
        y.GetUInteger().GetBits()[0] == 0xffffffffu &&
        y.GetUInteger().GetBits()[1] == 0x000fffffu,
        "Incorrect representation.");
}

template <typename Number>
void UnitTestBSNumber::EqualityComparisons()
{
    Number x, y;
    bool result;

    // signs both zero
    x = Number(0.0f);
    y = Number(0.0f);
    result = (x == y);
    UTAssert(result == true, "operator== failed on (0,0).");
    result = (x != y);
    UTAssert(result == false, "operator!= failed on (0,0).");

    // signs differ
    x = Number(0.0f);
    y = Number(1.0f);
    result = (x == y);
    UTAssert(result == false, "operator== failed on (0,1).");
    result = (x != y);
    UTAssert(result == true, "operator!= failed on (0,1).");

    // signs the same, biased exponents differ
    x = Number(1.0f);
    y = Number(2.0f);
    result = (x == y);
    UTAssert(result == false, "operator== failed on (1,2).");
    result = (x != y);
    UTAssert(result == true, "operator!= failed on (1,2).");

    // signs the same, biased exponents the same, number of bits differ
    x = Number(12.0f);
    y = Number(28.0f);
    result = (x == y);
    UTAssert(result == false, "operator== failed on (12,28).");
    result = (x != y);
    UTAssert(result == true, "operator!= failed on (12,28).");

    // signs the same, biased exponents the same, number of bits the same,
    // bits differ
    x = Number(20.0f);
    y = Number(28.0f);
    result = (x == y);
    UTAssert(result == false, "operator== failed on (20,28).");
    result = (x != y);
    UTAssert(result == true, "operator!= failed on (20,28).");

    // numbers the same
    x = Number(1.2345f);
    y = Number(1.2345f);
    result = (x == y);
    UTAssert(result == true, "operator== failed on (1.2345,1.2345).");
    result = (x != y);
    UTAssert(result == false, "operator!= failed on (1.2345,1.2345).");

    x = Number(-1.2345f);
    y = Number(-1.2345f);
    result = (x == y);
    UTAssert(result == true, "operator== failed on (1.2345,1.2345).");
    result = (x != y);
    UTAssert(result == false, "operator!= failed on (1.2345,1.2345).");
}

template <typename Number>
void UnitTestBSNumber::InequalityComparisons()
{
    Number x, y;
    bool result;

    // Return based on sign comparison.
    x = Number(1.0f);
    y = Number(-2.0f);
    result = (x < y);
    UTAssert(result == false, "operator< failed on (1,-2).");
    result = (x >= y);
    UTAssert(result == true, "operator>= failed on (1,-2).");
    x = Number(-2.0f);
    y = Number(1.0f);
    result = (x < y);
    UTAssert(result == true, "operator< failed on (-2,1).");
    result = (x >= y);
    UTAssert(result == false, "operator>= failed on (-2,1).");
    x = Number(0.0f);
    y = Number(1.0f);
    result = (x < y);
    UTAssert(result == true, "operator< failed on (0,1).");
    result = (x >= y);
    UTAssert(result == false, "operator>= failed on (0,1).");

    // Return based on exponent comparison.
    x = Number(1.0f);
    y = Number(2.0f);
    result = (x < y);
    UTAssert(result == true, "operator< failed on (1,2).");
    result = (x >= y);
    UTAssert(result == false, "operator>= failed on (1,2).");
    x = Number(2.0f);
    y = Number(1.0f);
    result = (x < y);
    UTAssert(result == false, "operator< failed on (2,1).");
    result = (x >= y);
    UTAssert(result == true, "operator>= failed on (2,1).");
    x = Number(-1.0f);
    y = Number(-2.0f);
    result = (x < y);
    UTAssert(result == false, "operator< failed on (-1,-2).");
    result = (x >= y);
    UTAssert(result == true, "operator>= failed on (-1,-2).");
    x = Number(-2.0f);
    y = Number(-1.0f);
    result = (x < y);
    UTAssert(result == true, "operator< failed on (-2,-1).");
    result = (x >= y);
    UTAssert(result == false, "operator>= failed on (-2,-1).");

    // Signs the same, exponents the same.  Return based on bits comparison.
    IEEEBinary32 x32, y32;
    IEEEBinary64 x64, y64;

    // All bits in block0.
    x32.SetEncoding(0, 127, 0x00400000);
    y32.SetEncoding(0, 127, 0x00200000);
    x = Number(x32.number);
    y = Number(y32.number);
    result = (x < y);
    UTAssert(result == false,
        "operator< failed on (0x3fc00000,0x3fa00000).");
    result = (x >= y);
    UTAssert(result == true,
        "operator>= failed on (0x3fc00000,0x3fa00000).");
    x32.SetEncoding(1, 127, 0x00400000);
    y32.SetEncoding(1, 127, 0x00200000);
    x = Number(x32.number);
    y = Number(y32.number);
    result = (x < y);
    UTAssert(result == true,
        "operator< failed on (0xbfc00000,0xbfa00000).");
    result = (x >= y);
    UTAssert(result == false,
        "operator>= failed on (0xbfc00000,0xbfa00000).");

    // x has bits in block0 and block1, y has bits in block0
    x64.SetEncoding(0, 0x03FF, 0x0008000000000001ull);
    y64.SetEncoding(0, 0x03FF, 0x0004000000000000ull);
    x = Number(x64.number);
    y = Number(y64.number);
    result = (x < y);
    UTAssert(result == false,
        "operator< failed on (0x3ff8000000000001,0x3ff4000000000000).");
    result = (x >= y);
    UTAssert(result == true,
        "operator>= failed on (0x3ff8000000000001,0x3ff4000000000000).");

    // x has bits in block0, y has bits in block0 and block1
    x64.SetEncoding(0, 0x03FF, 0x0008000000000000ull);
    y64.SetEncoding(0, 0x03FF, 0x0004000000000001ull);
    x = Number(x64.number);
    y = Number(y64.number);
    result = (x < y);
    UTAssert(result == false,
        "operator< failed on (0x3ff8000000000000,0x3ff4000000000001).");
    result = (x >= y);
    UTAssert(result == true,
        "operator>= failed on (0x3ff8000000000000,0x3ff4000000000001).");

    // x has bits in block0 and block1, y has bits in block0 and block1
    x64.SetEncoding(0, 0x03FF, 0x0008000000000001ull);
    y64.SetEncoding(0, 0x03FF, 0x0004000000000001ull);
    x = Number(x64.number);
    y = Number(y64.number);
    result = (x < y);
    UTAssert(result == false,
        "operator< failed on (0x3ff8000000000001,0x3ff4000000000001).");
    result = (x >= y);
    UTAssert(result == true,
        "operator>= failed on (0x3ff8000000000001,0x3ff4000000000001).");

    // x has bits in block0 and block1, y has bits in block0 and block1,
    // matching block1
    x64.SetEncoding(0, 0x03FF, 0x00080000000A0000ull);
    y64.SetEncoding(0, 0x03FF, 0x00080000000B0000ull);
    x = Number(x64.number);
    y = Number(y64.number);
    result = (x < y);
    UTAssert(result == true,
        "operator< failed on (0x3ff80000000A0000,0x3ff80000000B0000).");
    result = (x >= y);
    UTAssert(result == false,
        "operator>= failed on (0x3ff80000000A0000,0x3ff80000000B0000).");

    // x has bits in block0, y has bits in block0 and block1,
    // matching shifted block1
    x64.SetEncoding(0, 0x03FF, 0x0008000000A00000ull);
    y64.SetEncoding(0, 0x03FF, 0x0008000000B00000ull);
    x = Number(x64.number);
    y = Number(y64.number);
    result = (x < y);
    UTAssert(result == true,
        "operator< failed on (0x3ff8000000A00000,0x3ff8000000B00000).");
    result = (x >= y);
    UTAssert(result == false,
        "operator>= failed on (0x3ff8000000A00000,0x3ff8000000B00000).");

    // x and y are the same number
    x64.SetEncoding(0, 0x03FF, 0x0008000000B00000ull);
    y64.SetEncoding(0, 0x03FF, 0x0008000000B00000ull);
    x = Number(x64.number);
    y = Number(y64.number);
    result = (x < y);
    UTAssert(result == false,
        "operator< failed on (0x3ff8000000B00000,0x3ff8000000B00000).");
    result = (x >= y);
    UTAssert(result == true,
        "operator>= failed on (0x3ff8000000B00000,0x3ff8000000B00000).");
}

template <typename Number>
void UnitTestBSNumber::UnaryOperators()
{
    Number x, y;
    bool result;

    x = Number(+1.0f);
    y = Number(1.0);
    result = (x == +y);
    UTAssert(result == true, "operator+() failed");

    x = Number(+1.0f);
    y = Number(-1.0);
    result = (x == -y);
    UTAssert(result == true, "operator-() failed");
}

template <typename Number>
void UnitTestBSNumber::Addition()
{
    Number x, y, z, z0;

    std::uint32_t ux, uy;
    std::uint64_t uz;
    ux = 0xFFFFFFFFu;
    uy = 0x00FFFFFFu;
    uz = static_cast<std::uint64_t>(ux) + static_cast<std::uint64_t>(uy);
    x = Number(ux);
    y = Number(uy);
    z = x + y;
    z0 = Number(uz);
    UTAssert(z == z0, "Failed to add numbers.");

    ux = 0xFFFFFFFFu;
    uy = 0xB0000000u;
    uz = static_cast<std::uint64_t>(ux) + static_cast<std::uint64_t>(uy);
    x = Number(ux);
    y = Number(uy);
    z = x + y;
    z0 = Number(uz);
    UTAssert(z == z0, "Failed to add numbers.");

    double fx, fy, fz, fz0;

    // signs 0, *
    fx = 0.0;
    fy = 1.2345;
    fz = fx + fy;
    x = Number(fx);
    y = Number(fy);
    z = x + y;
    fz0 = static_cast<double>(z);
    UTAssert(fz == fz0, "Failed to add numbers.");

    // signs *, 0
    fx = -1.2345;
    fy = 0.0;
    fz = fx + fy;
    x = Number(fx);
    y = Number(fy);
    z = x + y;
    fz0 = static_cast<double>(z);
    UTAssert(fz == fz0, "Failed to add numbers.");

    // signs +, +
    fx = 1.2345;
    fy = 2.3456;
    fz = fx + fy;
    x = Number(fx);
    y = Number(fy);
    z = x + y;
    fz0 = static_cast<double>(z);
    UTAssert(fz == fz0, "Failed to add numbers.");

    // signs +, -, n0 + n1 = -(|n1| - |n0|) < 0
    fx = 1.2345;
    fy = -2.3456;
    fz = fx + fy;
    x = Number(fx);
    y = Number(fy);
    z = x + y;
    fz0 = static_cast<double>(z);
    UTAssert(fz == fz0, "Failed to add numbers.");

    // signs +, -, n0 + n1 = |n0| - |n1| > 0
    fx = 2.3456;
    fy = -1.2345;
    fz = fx + fy;
    x = Number(fx);
    y = Number(fy);
    z = x + y;
    fz0 = static_cast<double>(z);
    UTAssert(fz == fz0, "Failed to add numbers.");

    // signs +, -, n0 + n1 = 0
    fx = 1.2345;
    fy = -1.2345;
    fz = fx + fy;
    x = Number(fx);
    y = Number(fy);
    z = x + y;
    fz0 = static_cast<double>(z);
    UTAssert(fz == fz0, "Failed to add numbers.");

    // signs -, +, n0 + n1 = |n1| - |n0| > 0
    fx = -1.2345;
    fy = 2.3456;
    fz = fx + fy;
    x = Number(fx);
    y = Number(fy);
    z = x + y;
    fz0 = static_cast<double>(z);
    UTAssert(fz == fz0, "Failed to add numbers.");

    // signs -, +, n0 + n1 = -(|n0| - |n1|) < 0
    fx = -2.3456;
    fy = 1.2345;
    fz = fx + fy;
    x = Number(fx);
    y = Number(fy);
    z = x + y;
    fz0 = static_cast<double>(z);
    UTAssert(fz == fz0, "Failed to add numbers.");

    // signs -, +, n0 + n1 = 0
    fx = -1.2345;
    fy = 1.2345;
    fz = fx + fy;
    x = Number(fx);
    y = Number(fy);
    z = x + y;
    fz0 = static_cast<double>(z);
    UTAssert(fz == fz0, "Failed to add numbers.");

    // signs -, -
    fx = -1.2345;
    fy = -2.3456;
    fz = fx + fy;
    x = Number(fx);
    y = Number(fy);
    z = x + y;
    fz0 = static_cast<double>(z);
    UTAssert(fz == fz0, "Failed to add numbers.");
}

template <typename Number>
void UnitTestBSNumber::Subtraction()
{
    Number x, y, z, z0;

    std::uint32_t ux, uy;
    std::uint64_t uz;
    ux = 0xFFFFFFFFu;
    uy = 0x00FFFFFFu;
    uz = static_cast<std::uint64_t>(ux) - static_cast<std::uint64_t>(uy);
    x = Number(ux);
    y = Number(uy);
    z = x - y;
    z0 = Number(uz);
    UTAssert(z == z0, "Failed to subtract numbers.");

    ux = 0xFFFFFFFFu;
    uy = 0xB0000000u;
    uz = static_cast<std::uint64_t>(ux) - static_cast<std::uint64_t>(uy);
    x = Number(ux);
    y = Number(uy);
    z = x - y;
    z0 = Number(uz);
    UTAssert(z == z0, "Failed to subtract numbers.");

    ux = 0xB0000000u;
    uy = 0xFFFFFFFFu;
    std::int64_t iz = static_cast<std::int64_t>(ux) - static_cast<std::int64_t>(uy);
    x = Number(ux);
    y = Number(uy);
    z = x - y;
    z0 = Number(iz);
    UTAssert(z == z0, "Failed to subtract numbers.");

    double fx, fy, fz, fz0;

    // signs 0, *
    fx = 0.0;
    fy = 1.2345;
    fz = fx - fy;
    x = Number(fx);
    y = Number(fy);
    z = x - y;
    fz0 = static_cast<double>(z);
    UTAssert(fz == fz0, "Failed to subtract numbers.");

    // signs *, 0
    fx = -1.2345;
    fy = 0.0;
    fz = fx - fy;
    x = Number(fx);
    y = Number(fy);
    z = x - y;
    fz0 = static_cast<double>(z);
    UTAssert(fz == fz0, "Failed to subtract numbers.");

    // signs +, +, n0 - n1 = -(|n1| - |n0|) < 0
    fx = 1.2345;
    fy = 2.3456;
    fz = fx - fy;
    x = Number(fx);
    y = Number(fy);
    z = x - y;
    fz0 = static_cast<double>(z);
    UTAssert(fz == fz0, "Failed to subtract numbers.");

    // signs +, +, |n0| - |n1| > 0
    fx = 2.3456;
    fy = 1.2345;
    fz = fx - fy;
    x = Number(fx);
    y = Number(fy);
    z = x - y;
    fz0 = static_cast<double>(z);
    UTAssert(fz == fz0, "Failed to subtract numbers.");

    // signs +, +, n0 - n1 = 0
    fx = 2.3456;
    fy = 2.3456;
    fz = fx - fy;
    x = Number(fx);
    y = Number(fy);
    z = x - y;
    fz0 = static_cast<double>(z);
    UTAssert(fz == fz0, "Failed to subtract numbers.");

    // signs -, -, n0 - n1 = (|n1| - |n0|) > 0
    fx = -1.2345;
    fy = -2.3456;
    fz = fx - fy;
    x = Number(fx);
    y = Number(fy);
    z = x - y;
    fz0 = static_cast<double>(z);
    UTAssert(fz == fz0, "Failed to subtract numbers.");

    // signs -, -, n0 - n1 = (|n1| - |n0|) > 0
    fx = -2.3456;
    fy = -1.2345;
    fz = fx - fy;
    x = Number(fx);
    y = Number(fy);
    z = x - y;
    fz0 = static_cast<double>(z);
    UTAssert(fz == fz0, "Failed to subtract numbers.");

    // signs -, -, n0 - n1 = 0
    fx = -1.2345;
    fy = -1.2345;
    fz = fx - fy;
    x = Number(fx);
    y = Number(fy);
    z = x - y;
    fz0 = static_cast<double>(z);
    UTAssert(fz == fz0, "Failed to subtract numbers.");

    // signs +, -
    fx = 2.3456;
    fy = -1.2345;
    fz = fx - fy;
    x = Number(fx);
    y = Number(fy);
    z = x - y;
    fz0 = static_cast<double>(z);
    UTAssert(fz == fz0, "Failed to subtract numbers.");

    // signs -, +
    fx = -2.3456;
    fy = 1.2345;
    fz = fx - fy;
    x = Number(fx);
    y = Number(fy);
    z = x - y;
    fz0 = static_cast<double>(z);
    UTAssert(fz == fz0, "Failed to subtract numbers.");

    // This is an example where UIntegerAP32::LessThanIgnoreSign has
    // a right-shift of the form (u >> 32). The GTE code had u as type
    // std::uint64_t, and the subtraction x-y was correct. When I ported the
    // code to GTL, I modified u to be type std::uint32_t. MSVS 2019 (16.5.1)
    // generates code so that (u >> 32) is u, not 0. A right-shift of an
    // n-bit unsigned integer by n bits is undefined, the result being
    // compiler dependent.
    std::vector<std::array<float, 2>> v(4);
    v[0][0] = 698.725708f;
    v[0][1] = 492.614685f;
    v[1][0] = 709.944458f;
    v[1][1] = 696.817078f;
    v[2][0] = 831.284912f;
    v[2][1] = 619.338257f;
    v[3][0] = 845.306763f;
    v[3][1] = 651.284119f;

    std::vector<std::array<Number, 2>> bsn(4);
    for (std::size_t i = 0; i < 4; ++i)
    {
        bsn[i][0] = v[i][0];
        bsn[i][1] = v[i][1];
    }

    Number s1x = bsn[2][0] + bsn[0][0];
    Number d1x = bsn[2][0] - bsn[0][0];
    Number s1y = bsn[2][1] + bsn[0][1];
    Number d1y = bsn[2][1] - bsn[0][1];
    Number s2x = bsn[3][0] + bsn[0][0];
    Number d2x = bsn[3][0] - bsn[0][0];
    Number s2y = bsn[3][1] + bsn[0][1];
    Number d2y = bsn[3][1] - bsn[0][1];
    Number z1 = s1x * d1x + s1y * d1y;
    Number z2 = s2x * d2x + s2y * d2y;
    x = d1y * z2;
    y = d2y * z1;
    Number c00 = x - y; // -2857689.0161162107
    double dC00 = c00;
    UTAssert(dC00 == -2857689.0161162107, "Invalid result for subtraction.");
}

template <typename Number>
void UnitTestBSNumber::Multiplication()
{
    Number x{}, y{}, z0{}, z1{};
    std::uint64_t number0, number1;

    x = Number(0x87654321u);
    y = Number(0x0000DCBAu);
    z0 = x * y;
    number0 = 0x87654321u;
    number1 = 0x0000DCBAu;
    z1 = Number(number0 * number1);
    UTAssert(z0 == z1, "Failed to multiply numbers.");

    x = Number(0xFFFFFFFFu);
    y = Number(0xFFFFFFFFu);
    z0 = x * y;
    number0 = 0xFFFFFFFFu;
    number1 = 0xFFFFFFFFu;
    z1 = Number(number0 * number1);
    UTAssert(z0 == z1, "Failed to multiply numbers.");

    number0 = 0xFF00123400FFABC1ull;
    number1 = 0x01234567ABCDEF01ull;
    x = Number(number0);
    y = Number(number1);
    z0 = x * y;

    std::array<std::uint64_t, 2> u0 = { 0, 0 }, u1 = { 0, 0 };
    std::array<std::uint64_t, 4> p = { 0, 0, 0, 0 };
    std::uint64_t term = 0, carry = 0, sum = 0;
    std::array<std::uint32_t, 4> v = { 0, 0, 0, 0 };
    u0[0] = number0 & 0x00000000FFFFFFFFull;
    u0[1] = (number0 & 0xFFFFFFFF00000000ull) >> 32;
    u1[0] = number1 & 0x00000000FFFFFFFFull;
    u1[1] = (number1 & 0xFFFFFFFF00000000ull) >> 32;

    carry = 0;
    term = u0[0] * u1[0] + carry;
    p[0] = term & 0x00000000FFFFFFFFull;
    carry = (term & 0xFFFFFFFF00000000ull) >> 32;
    term = u0[0] * u1[1] + carry;
    p[1] = term & 0x00000000FFFFFFFFull;
    p[2] = (term & 0xFFFFFFFF00000000ull) >> 32;
    carry = 0;
    sum = p[0] + v[0] + carry;
    v[0] = sum & 0x00000000FFFFFFFFull;
    carry = (sum & 0xFFFFFFFF00000000ull) >> 32;
    sum = p[1] + v[1] + carry;
    v[1] = sum & 0x00000000FFFFFFFFull;
    carry = (sum & 0xFFFFFFFF00000000ull) >> 32;
    sum = p[2] + v[2] + carry;
    v[2] = sum & 0x00000000FFFFFFFFull;

    carry = 0;
    term = u0[1] * u1[0];
    p[1] = term & 0x00000000FFFFFFFFull;
    carry = (term & 0xFFFFFFFF00000000ull) >> 32;
    term = u0[1] * u1[1] + carry;
    p[2] = term & 0x00000000FFFFFFFFull;
    p[3] = (term & 0xFFFFFFFF00000000ull) >> 32;
    carry = 0;
    sum = p[1] + v[1] + carry;
    v[1] = sum & 0x00000000FFFFFFFFull;
    carry = (sum & 0xFFFFFFFF00000000ull) >> 32;
    sum = p[2] + v[2] + carry;
    v[2] = sum & 0x00000000FFFFFFFFull;
    carry = (sum & 0xFFFFFFFF00000000ull) >> 32;
    sum = p[3] + v[3] + carry;
    v[3] = sum & 0x00000000FFFFFFFFull;

    UTAssert(
        v[0] == z0.GetUInteger().GetBits()[0] &&
        v[1] == z0.GetUInteger().GetBits()[1] &&
        v[2] == z0.GetUInteger().GetBits()[2] &&
        v[3] == z0.GetUInteger().GetBits()[3],
        "Failed to multiply numbers.");

    // Round-trip for the tests.
    double fx, fy, fz, fz0;
    fx = 1.2345;
    fy = 2.3456;
    fz = fx * fy;
    x = Number(fx);
    y = Number(fy);
    z0 = x * y;
    fz0 = static_cast<double>(z0);
    UTAssert(fz == fz0, "Failed to multiply numbers.");

    x = Number(0x3E9DC812u);
    y = Number(1.0f);
    z0 = x * y;
    UTAssert(z0 == x, "Failed to multiply numbers.");
}

template <typename Number>
void UnitTestBSNumber::Conversions()
{
    IEEEBinary32 z;
    z.number = 100.0f / 7.0f;
    z.encoding &= 0xFFFFFFF0u;
    // z.number = 14.2857056
    // z.encoding = 0x41649240
    // z = 1.11001001001001001000000 * 2^3
    //   = 1110.01001001001001

    Number input(z.number);
    // exponent = 3
    // biased exponent = -14
    // numBits = 18
    // bits  = 0x00039249 = 0000 0000 0000 0011 1001 0010 0100 1001
    //       = 111001001001001001
    // input = 111001001001001001 * 2^{-14}
    //       = 1110.01001001001001

    PositiveRoundNearest(z.number, input);
    PositiveRoundUpward(z.number, input);
    PositiveRoundDownward(z.number, input);
    PositiveRoundTowardZero(z.number, input);

    z.number = -z.number;
    input = -input;
    NegativeRoundNearest(z.number, input);
    NegativeRoundUpward(z.number, input);
    NegativeRoundDownward(z.number, input);
    NegativeRoundTowardZero(z.number, input);
}

template <typename Number>
void UnitTestBSNumber::PositiveRoundNearest(float z, Number const& input)
{
    // ROUND TO NEAREST: The outputs for precision 18 through 24 are all the
    // same and the result is the input.
    Number output{};
    std::array<IEEEBinary32, 19> values;
    values[0].number = z;
    for (std::size_t precision = 1; precision < 19; ++precision)
    {
        input.Convert(precision, APRoundingMode::TO_NEAREST, output);
        values[precision].number = (float)output;
    }

    // pre  binary               round nearest          number
    // ---+--------------------+----------------------+---------------
    //  1   1.11001001001001001  u 1000.0               16.0
    //  2   11.1001001001001001  u 1000.0               16.0
    //  3   111.001001001001001  d 1110.0               14.0
    //  4   1110.01001001001001  d 1110.0               14.0
    //  5   11100.1001001001001  u 1110.1               14.5
    //  6   111001.001001001001  d 1110.01              14.25
    //  7   1110010.01001001001  d 1110.01              14.25
    //  8   11100100.1001001001  u 1110.0101            14.3125
    //  9   111001001.001001001  d 1110.01001           14.28125
    // 10   1110010010.01001001  d 1110.01001           14.28125
    // 11   11100100100.1001001  u 1110.0100101         14.2890625
    // 12   111001001001.001001  d 1110.01001001        14.28515625
    // 13   1110010010010.01001  d 1110.01001001        14.28515625
    // 14   11100100100100.1001  u 1110.0100100101      14.2861328125
    // 15   111001001001001.001  d 1110.01001001001     14.28564453125
    // 16   1110010010010010.01  d 1110.01001001001     14.28564453125
    // 17   11100100100100100.1  d 1110.01001001001     14.28564453125 (tie)
    // 18   111001001001001001.  d 1110.01001001001001  14.28570556640625
    std::array<float, 19> truth =
    {
        14.28570556640625f,
        16.0f,
        16.0f,
        14.0f,
        14.0f,
        14.5f,
        14.25f,
        14.25f,
        14.3125f,
        14.28125f,
        14.28125f,
        14.2890625f,
        14.28515625f,
        14.28515625f,
        14.2861328125f,
        14.28564453125f,
        14.28564453125f,
        14.28564453125f,
        14.28570556640625f
    };

    for (std::size_t precision = 0; precision < 19; ++precision)
    {
        UTAssert(values[precision].number == truth[precision], "Incorrect result.");
    }
}

template <typename Number>
void UnitTestBSNumber::PositiveRoundUpward(float z, Number const& input)
{
    // ROUND TO +INFINITY: The outputs for precision 18 through 24 are all the
    // same and the result is the input.
    Number output{};
    std::array<IEEEBinary32, 19> values;
    values[0].number = z;
    for (std::size_t precision = 1; precision < 19; ++precision)
    {
        input.Convert(precision, APRoundingMode::UPWARD, output);
        values[precision].number = (float)output;
    }

    // pre  binary               round to +infinity     number
    // ---+--------------------+--------------------+-------------
    //  1   1.11001001001001001  1000.0               16.0
    //  2   11.1001001001001001  1000.0               16.0
    //  3   111.001001001001001  1000.0               16.0
    //  4   1110.01001001001001  1111.0               15.0
    //  5   11100.1001001001001  1110.1               14.5
    //  6   111001.001001001001  1110.1               14.5
    //  7   1110010.01001001001  1110.011             14.375
    //  8   11100100.1001001001  1110.0101            14.3125
    //  9   111001001.001001001  1110.0101            14.3125
    // 10   1110010010.01001001  1110.010011          14.296875
    // 11   11100100100.1001001  1110.0100101         14.2890625
    // 12   111001001001.001001  1110.0100101         14.2890625
    // 13   1110010010010.01001  1110.010010011       14.287109375
    // 14   11100100100100.1001  1110.0100100101      14.2861328125
    // 15   111001001001001.001  1110.0100100101      14.2861328125
    // 16   1110010010010010.01  1110.010010010011    14.285888671875
    // 17   11100100100100100.1  1110.0100100100101   14.2857666015625
    // 18   111001001001001001.  1110.01001001001001  14.28570556640625
    std::array<float, 19> truth =
    {
        14.28570556640625f,
        16.0f,
        16.0f,
        16.0f,
        15.0f,
        14.5f,
        14.5f,
        14.375f,
        14.3125f,
        14.3125f,
        14.296875f,
        14.2890625f,
        14.2890625f,
        14.287109375f,
        14.2861328125f,
        14.2861328125f,
        14.285888671875f,
        14.2857666015625f,
        14.28570556640625f
    };

    for (std::size_t precision = 0; precision < 19; ++precision)
    {
        UTAssert(values[precision].number == truth[precision], "Incorrect result.");
    }
}

template <typename Number>
void UnitTestBSNumber::PositiveRoundDownward(float z, Number const& input)
{
    // ROUND TO -INFINITY: The outputs for precision 18 through 24 are all the
    // same and the result is the input.
    Number output{};
    std::array<IEEEBinary32, 19> values;
    values[0].number = z;
    for (std::size_t precision = 1; precision < 19; ++precision)
    {
        input.Convert(precision, APRoundingMode::DOWNWARD, output);
        values[precision].number = (float)output;
    }

    // pre  binary               round to +infinity     number
    // ---+--------------------+--------------------+-------------
    //  1   1.11001001001001001  100.0                 8.0
    //  2   11.1001001001001001  110.0                12.0
    //  3   111.001001001001001  1110.0               14.0
    //  4   1110.01001001001001  1110.0               14.0
    //  5   11100.1001001001001  1110.0               14.0
    //  6   111001.001001001001  1110.01              14.25
    //  7   1110010.01001001001  1110.01              14.25
    //  8   11100100.1001001001  1110.01              14.25
    //  9   111001001.001001001  1110.1001            14.28125
    // 10   1110010010.01001001  1110.1001            14.28125
    // 11   11100100100.1001001  1110.1001            14.28125
    // 12   111001001001.001001  1110.01001001        14.28515625
    // 13   1110010010010.01001  1110.01001001        14.28515625
    // 14   11100100100100.1001  1110.01001001        14.28515625
    // 15   111001001001001.001  1110.01001001001     14.28564453125
    // 16   1110010010010010.01  1110.01001001001     14.28564453125
    // 17   11100100100100100.1  1110.01001001001     14.28564453125
    // 18   111001001001001001.  1110.01001001001001  14.28570556640625
    std::array<float, 19> truth =
    {
        14.28570556640625f,
        8.0f,
        12.0f,
        14.0f,
        14.0f,
        14.0f,
        14.25f,
        14.25f,
        14.25f,
        14.28125f,
        14.28125f,
        14.28125f,
        14.28515625f,
        14.28515625f,
        14.28515625f,
        14.28564453125f,
        14.28564453125f,
        14.28564453125f,
        14.28570556640625f
    };

    for (std::size_t precision = 0; precision < 19; ++precision)
    {
        UTAssert(values[precision].number == truth[precision], "Incorrect result.");
    }
}

template <typename Number>
void UnitTestBSNumber::PositiveRoundTowardZero(float z, Number const& input)
{
    // ROUND TOWARD ZERO: The outputs for precision 18 through 24 are all the
    // same and the result is the input.
    Number output{};
    std::array<IEEEBinary32, 19> values;
    values[0].number = z;
    for (std::size_t precision = 1; precision < 19; ++precision)
    {
        input.Convert(precision, APRoundingMode::TOWARD_ZERO, output);
        values[precision].number = (float)output;
    }

    // pre  binary               round to +infinity     number
    // ---+--------------------+--------------------+-------------
    //  1   1.11001001001001001  100.0                 8.0
    //  2   11.1001001001001001  110.0                12.0
    //  3   111.001001001001001  1110.0               14.0
    //  4   1110.01001001001001  1110.0               14.0
    //  5   11100.1001001001001  1110.0               14.0
    //  6   111001.001001001001  1110.01              14.25
    //  7   1110010.01001001001  1110.01              14.25
    //  8   11100100.1001001001  1110.01              14.25
    //  9   111001001.001001001  1110.1001            14.28125
    // 10   1110010010.01001001  1110.1001            14.28125
    // 11   11100100100.1001001  1110.1001            14.28125
    // 12   111001001001.001001  1110.01001001        14.28515625
    // 13   1110010010010.01001  1110.01001001        14.28515625
    // 14   11100100100100.1001  1110.01001001        14.28515625
    // 15   111001001001001.001  1110.01001001001     14.28564453125
    // 16   1110010010010010.01  1110.01001001001     14.28564453125
    // 17   11100100100100100.1  1110.01001001001     14.28564453125
    // 18   111001001001001001.  1110.01001001001001  14.28570556640625
    std::array<float, 19> truth =
    {
        14.28570556640625f,
        8.0f,
        12.0f,
        14.0f,
        14.0f,
        14.0f,
        14.25f,
        14.25f,
        14.25f,
        14.28125f,
        14.28125f,
        14.28125f,
        14.28515625f,
        14.28515625f,
        14.28515625f,
        14.28564453125f,
        14.28564453125f,
        14.28564453125f,
        14.28570556640625f
    };

    for (std::size_t precision = 0; precision < 19; ++precision)
    {
        UTAssert(values[precision].number == truth[precision], "Incorrect result.");
    }
}

template <typename Number>
void UnitTestBSNumber::NegativeRoundNearest(float z, Number const& input)
{
    // ROUND TO NEAREST: The outputs for precision 18 through 24 are all the
    // same and the result is the input.
    Number output{};
    std::array<IEEEBinary32, 19> values;
    values[0].number = z;
    for (std::size_t precision = 1; precision < 19; ++precision)
    {
        input.Convert(precision, APRoundingMode::TO_NEAREST, output);
        values[precision].number = (float)output;
    }

    // pre  binary               round nearest          number
    // ---+--------------------+----------------------+---------------
    //  1   1.11001001001001001  u 1000.0               -16.0
    //  2   11.1001001001001001  u 1000.0               -16.0
    //  3   111.001001001001001  d 1110.0               -14.0
    //  4   1110.01001001001001  d 1110.0               -14.0
    //  5   11100.1001001001001  u 1110.1               -14.5
    //  6   111001.001001001001  d 1110.01              -14.25
    //  7   1110010.01001001001  d 1110.01              -14.25
    //  8   11100100.1001001001  u 1110.0101            -14.3125
    //  9   111001001.001001001  d 1110.01001           -14.28125
    // 10   1110010010.01001001  d 1110.01001           -14.28125
    // 11   11100100100.1001001  u 1110.0100101         -14.2890625
    // 12   111001001001.001001  d 1110.01001001        -14.28515625
    // 13   1110010010010.01001  d 1110.01001001        -14.28515625
    // 14   11100100100100.1001  u 1110.0100100101      -14.2861328125
    // 15   111001001001001.001  d 1110.01001001001     -14.28564453125
    // 16   1110010010010010.01  d 1110.01001001001     -14.28564453125
    // 17   11100100100100100.1  d 1110.0100100100101   -14.28564453125 (tie)
    // 18   111001001001001001.  d 1110.01001001001001  -14.28570556640625
    std::array<float, 19> truth =
    {
        -14.28570556640625f,
        -16.0f,
        -16.0f,
        -14.0f,
        -14.0f,
        -14.5f,
        -14.25f,
        -14.25f,
        -14.3125f,
        -14.28125f,
        -14.28125f,
        -14.2890625f,
        -14.28515625f,
        -14.28515625f,
        -14.2861328125f,
        -14.28564453125f,
        -14.28564453125f,
        -14.28564453125f,
        -14.28570556640625f
    };

    for (std::size_t precision = 0; precision < 19; ++precision)
    {
        UTAssert(values[precision].number == truth[precision], "Incorrect result.");
    }
}

template <typename Number>
void UnitTestBSNumber::NegativeRoundUpward(float z, Number const& input)
{
    // ROUND TO +INFINITY: The outputs for precision 18 through 24 are all the
    // same and the result is the input.
    Number output{};
    std::array<IEEEBinary32, 19> values;
    values[0].number = z;
    for (std::size_t precision = 1; precision < 19; ++precision)
    {
        input.Convert(precision, APRoundingMode::UPWARD, output);
        values[precision].number = (float)output;
    }

    // pre  binary               round to +infinity     number
    // ---+--------------------+--------------------+-------------
    //  1   1.11001001001001001  100.0                 -8.0
    //  2   11.1001001001001001  110.0                -12.0
    //  3   111.001001001001001  1110.0               -14.0
    //  4   1110.01001001001001  1110.0               -14.0
    //  5   11100.1001001001001  1110.0               -14.0
    //  6   111001.001001001001  1110.01              -14.25
    //  7   1110010.01001001001  1110.01              -14.25
    //  8   11100100.1001001001  1110.01              -14.25
    //  9   111001001.001001001  1110.1001            -14.28125
    // 10   1110010010.01001001  1110.1001            -14.28125
    // 11   11100100100.1001001  1110.1001            -14.28125
    // 12   111001001001.001001  1110.01001001        -14.28515625
    // 13   1110010010010.01001  1110.01001001        -14.28515625
    // 14   11100100100100.1001  1110.01001001        -14.28515625
    // 15   111001001001001.001  1110.01001001001     -14.28564453125
    // 16   1110010010010010.01  1110.01001001001     -14.28564453125
    // 17   11100100100100100.1  1110.01001001001     -14.28564453125
    // 18   111001001001001001.  1110.01001001001001  -14.28570556640625
    std::array<float, 19> truth =
    {
        -14.28570556640625f,
        -8.0f,
        -12.0f,
        -14.0f,
        -14.0f,
        -14.0f,
        -14.25f,
        -14.25f,
        -14.25f,
        -14.28125f,
        -14.28125f,
        -14.28125f,
        -14.28515625f,
        -14.28515625f,
        -14.28515625f,
        -14.28564453125f,
        -14.28564453125f,
        -14.28564453125f,
        -14.28570556640625f
    };

    for (std::size_t precision = 0; precision < 19; ++precision)
    {
        UTAssert(values[precision].number == truth[precision], "Incorrect result.");
    }
}

template <typename Number>
void UnitTestBSNumber::NegativeRoundDownward(float z, Number const& input)
{
    // ROUND TO -INFINITY: The outputs for precision 18 through 24 are all the
    // same and the result is the input.
    Number output{};
    std::array<IEEEBinary32, 19> values;
    values[0].number = z;
    for (std::size_t precision = 1; precision < 19; ++precision)
    {
        input.Convert(precision, APRoundingMode::DOWNWARD, output);
        values[precision].number = (float)output;
    }

    // pre  binary               round to +infinity     number
    // ---+--------------------+--------------------+-------------
    //  1   1.11001001001001001  1000.0               -16.0
    //  2   11.1001001001001001  1000.0               -16.0
    //  3   111.001001001001001  1000.0               -16.0
    //  4   1110.01001001001001  1111.0               -15.0
    //  5   11100.1001001001001  1110.1               -14.5
    //  6   111001.001001001001  1110.1               -14.5
    //  7   1110010.01001001001  1110.011             -14.375
    //  8   11100100.1001001001  1110.0101            -14.3125
    //  9   111001001.001001001  1110.0101            -14.3125
    // 10   1110010010.01001001  1110.010011          -14.296875
    // 11   11100100100.1001001  1110.0100101         -14.2890625
    // 12   111001001001.001001  1110.0100101         -14.2890625
    // 13   1110010010010.01001  1110.010010011       -14.287109375
    // 14   11100100100100.1001  1110.0100100101      -14.2861328125
    // 15   111001001001001.001  1110.0100100101      -14.2861328125
    // 16   1110010010010010.01  1110.010010010011    -14.285888671875
    // 17   11100100100100100.1  1110.0100100100101   -14.2857666015625
    // 18   111001001001001001.  1110.01001001001001  -14.28570556640625
    std::array<float, 19> truth =
    {
        -14.28570556640625f,
        -16.0f,
        -16.0f,
        -16.0f,
        -15.0f,
        -14.5f,
        -14.5f,
        -14.375f,
        -14.3125f,
        -14.3125f,
        -14.296875f,
        -14.2890625f,
        -14.2890625f,
        -14.287109375f,
        -14.2861328125f,
        -14.2861328125f,
        -14.285888671875f,
        -14.2857666015625f,
        -14.28570556640625f
    };

    for (std::size_t precision = 0; precision < 19; ++precision)
    {
        UTAssert(values[precision].number == truth[precision], "Incorrect result.");
    }
}

template <typename Number>
void UnitTestBSNumber::NegativeRoundTowardZero(float z, Number const& input)
{
    // ROUND TO +INFINITY: The outputs for precision 18 through 24 are all the
    // same and the result is the input.
    Number output{};
    std::array<IEEEBinary32, 19> values;
    values[0].number = z;
    for (std::size_t precision = 1; precision < 19; ++precision)
    {
        input.Convert(precision, APRoundingMode::TOWARD_ZERO, output);
        values[precision].number = (float)output;
    }

    // pre  binary               round to +infinity     number
    // ---+--------------------+--------------------+-------------
    //  1   1.11001001001001001  100.0                 -8.0
    //  2   11.1001001001001001  110.0                -12.0
    //  3   111.001001001001001  1110.0               -14.0
    //  4   1110.01001001001001  1110.0               -14.0
    //  5   11100.1001001001001  1110.0               -14.0
    //  6   111001.001001001001  1110.01              -14.25
    //  7   1110010.01001001001  1110.01              -14.25
    //  8   11100100.1001001001  1110.01              -14.25
    //  9   111001001.001001001  1110.1001            -14.28125
    // 10   1110010010.01001001  1110.1001            -14.28125
    // 11   11100100100.1001001  1110.1001            -14.28125
    // 12   111001001001.001001  1110.01001001        -14.28515625
    // 13   1110010010010.01001  1110.01001001        -14.28515625
    // 14   11100100100100.1001  1110.01001001        -14.28515625
    // 15   111001001001001.001  1110.01001001001     -14.28564453125
    // 16   1110010010010010.01  1110.01001001001     -14.28564453125
    // 17   11100100100100100.1  1110.01001001001     -14.28564453125
    // 18   111001001001001001.  1110.01001001001001  -14.28570556640625
    std::array<float, 19> truth =
    {
        -14.28570556640625f,
        -8.0f,
        -12.0f,
        -14.0f,
        -14.0f,
        -14.0f,
        -14.25f,
        -14.25f,
        -14.25f,
        -14.28125f,
        -14.28125f,
        -14.28125f,
        -14.28515625f,
        -14.28515625f,
        -14.28515625f,
        -14.28564453125f,
        -14.28564453125f,
        -14.28564453125f,
        -14.28570556640625f
    };

    for (std::size_t precision = 0; precision < 19; ++precision)
    {
        UTAssert(values[precision].number == truth[precision], "Incorrect result.");
    }
}

#else

#include <GTL/Mathematics/Arithmetic/UIntegerAP32.h>
#include <GTL/Mathematics/Arithmetic/UIntegerFP32.h>
#include <GTL/Mathematics/Arithmetic/BSNumber.h>

namespace gtl
{
    template class BSNumber<UIntegerAP32>;
    template class BSNumber<UIntegerFP32<8>>;
}

namespace std
{
    template gtl::BSNumber<gtl::UIntegerAP32> abs(gtl::BSNumber<gtl::UIntegerAP32> const&);
    template gtl::BSNumber<gtl::UIntegerFP32<8>> abs(gtl::BSNumber<gtl::UIntegerFP32<8>> const&);

    template gtl::BSNumber<gtl::UIntegerAP32> acos(gtl::BSNumber<gtl::UIntegerAP32> const&);
    template gtl::BSNumber<gtl::UIntegerFP32<8>> acos(gtl::BSNumber<gtl::UIntegerFP32<8>> const&);

    template gtl::BSNumber<gtl::UIntegerAP32> acosh(gtl::BSNumber<gtl::UIntegerAP32> const&);
    template gtl::BSNumber<gtl::UIntegerFP32<8>> acosh(gtl::BSNumber<gtl::UIntegerFP32<8>> const&);

    template gtl::BSNumber<gtl::UIntegerAP32> asin(gtl::BSNumber<gtl::UIntegerAP32> const&);
    template gtl::BSNumber<gtl::UIntegerFP32<8>> asin(gtl::BSNumber<gtl::UIntegerFP32<8>> const&);

    template gtl::BSNumber<gtl::UIntegerAP32> asinh(gtl::BSNumber<gtl::UIntegerAP32> const&);
    template gtl::BSNumber<gtl::UIntegerFP32<8>> asinh(gtl::BSNumber<gtl::UIntegerFP32<8>> const&);

    template gtl::BSNumber<gtl::UIntegerAP32> atan(gtl::BSNumber<gtl::UIntegerAP32> const&);
    template gtl::BSNumber<gtl::UIntegerFP32<8>> atan(gtl::BSNumber<gtl::UIntegerFP32<8>> const&);

    template gtl::BSNumber<gtl::UIntegerAP32> atanh(gtl::BSNumber<gtl::UIntegerAP32> const&);
    template gtl::BSNumber<gtl::UIntegerFP32<8>> atanh(gtl::BSNumber<gtl::UIntegerFP32<8>> const&);

    template gtl::BSNumber<gtl::UIntegerAP32> atan2(gtl::BSNumber<gtl::UIntegerAP32> const&, gtl::BSNumber<gtl::UIntegerAP32> const&);
    template gtl::BSNumber<gtl::UIntegerFP32<8>> atan2(gtl::BSNumber<gtl::UIntegerFP32<8>> const&, gtl::BSNumber<gtl::UIntegerFP32<8>> const&);

    template gtl::BSNumber<gtl::UIntegerAP32> ceil(gtl::BSNumber<gtl::UIntegerAP32> const&);
    template gtl::BSNumber<gtl::UIntegerFP32<8>> ceil(gtl::BSNumber<gtl::UIntegerFP32<8>> const&);

    template gtl::BSNumber<gtl::UIntegerAP32> cos(gtl::BSNumber<gtl::UIntegerAP32> const&);
    template gtl::BSNumber<gtl::UIntegerFP32<8>> cos(gtl::BSNumber<gtl::UIntegerFP32<8>> const&);

    template gtl::BSNumber<gtl::UIntegerAP32> cosh(gtl::BSNumber<gtl::UIntegerAP32> const&);
    template gtl::BSNumber<gtl::UIntegerFP32<8>> cosh(gtl::BSNumber<gtl::UIntegerFP32<8>> const&);

    template gtl::BSNumber<gtl::UIntegerAP32> exp(gtl::BSNumber<gtl::UIntegerAP32> const&);
    template gtl::BSNumber<gtl::UIntegerFP32<8>> exp(gtl::BSNumber<gtl::UIntegerFP32<8>> const&);

    template gtl::BSNumber<gtl::UIntegerAP32> exp2(gtl::BSNumber<gtl::UIntegerAP32> const&);
    template gtl::BSNumber<gtl::UIntegerFP32<8>> exp2(gtl::BSNumber<gtl::UIntegerFP32<8>> const&);

    template gtl::BSNumber<gtl::UIntegerAP32> fabs(gtl::BSNumber<gtl::UIntegerAP32> const&);
    template gtl::BSNumber<gtl::UIntegerFP32<8>> fabs(gtl::BSNumber<gtl::UIntegerFP32<8>> const&);

    template gtl::BSNumber<gtl::UIntegerAP32> floor(gtl::BSNumber<gtl::UIntegerAP32> const&);
    template gtl::BSNumber<gtl::UIntegerFP32<8>> floor(gtl::BSNumber<gtl::UIntegerFP32<8>> const&);

    template gtl::BSNumber<gtl::UIntegerAP32> fmod(gtl::BSNumber<gtl::UIntegerAP32> const&, gtl::BSNumber<gtl::UIntegerAP32> const&);
    template gtl::BSNumber<gtl::UIntegerFP32<8>> fmod(gtl::BSNumber<gtl::UIntegerFP32<8>> const&, gtl::BSNumber<gtl::UIntegerFP32<8>> const&);

    template gtl::BSNumber<gtl::UIntegerAP32> frexp(gtl::BSNumber<gtl::UIntegerAP32> const&, std::int32_t*);
    template gtl::BSNumber<gtl::UIntegerFP32<8>> frexp(gtl::BSNumber<gtl::UIntegerFP32<8>> const&, std::int32_t*);

    template gtl::BSNumber<gtl::UIntegerAP32> ldexp(gtl::BSNumber<gtl::UIntegerAP32> const&, std::int32_t);
    template gtl::BSNumber<gtl::UIntegerFP32<8>> ldexp(gtl::BSNumber<gtl::UIntegerFP32<8>> const&, std::int32_t);

    template gtl::BSNumber<gtl::UIntegerAP32> log(gtl::BSNumber<gtl::UIntegerAP32> const&);
    template gtl::BSNumber<gtl::UIntegerFP32<8>> log(gtl::BSNumber<gtl::UIntegerFP32<8>> const&);

    template gtl::BSNumber<gtl::UIntegerAP32> log2(gtl::BSNumber<gtl::UIntegerAP32> const&);
    template gtl::BSNumber<gtl::UIntegerFP32<8>> log2(gtl::BSNumber<gtl::UIntegerFP32<8>> const&);

    template gtl::BSNumber<gtl::UIntegerAP32> log10(gtl::BSNumber<gtl::UIntegerAP32> const&);
    template gtl::BSNumber<gtl::UIntegerFP32<8>> log10(gtl::BSNumber<gtl::UIntegerFP32<8>> const&);

    template gtl::BSNumber<gtl::UIntegerAP32> pow(gtl::BSNumber<gtl::UIntegerAP32> const&, gtl::BSNumber<gtl::UIntegerAP32> const&);
    template gtl::BSNumber<gtl::UIntegerFP32<8>> pow(gtl::BSNumber<gtl::UIntegerFP32<8>> const&, gtl::BSNumber<gtl::UIntegerFP32<8>> const&);

    template gtl::BSNumber<gtl::UIntegerAP32> remainder(gtl::BSNumber<gtl::UIntegerAP32> const&, gtl::BSNumber<gtl::UIntegerAP32> const&);
    template gtl::BSNumber<gtl::UIntegerFP32<8>> remainder(gtl::BSNumber<gtl::UIntegerFP32<8>> const&, gtl::BSNumber<gtl::UIntegerFP32<8>> const&);

    template gtl::BSNumber<gtl::UIntegerAP32> sin(gtl::BSNumber<gtl::UIntegerAP32> const&);
    template gtl::BSNumber<gtl::UIntegerFP32<8>> sin(gtl::BSNumber<gtl::UIntegerFP32<8>> const&);

    template gtl::BSNumber<gtl::UIntegerAP32> sinh(gtl::BSNumber<gtl::UIntegerAP32> const&);
    template gtl::BSNumber<gtl::UIntegerFP32<8>> sinh(gtl::BSNumber<gtl::UIntegerFP32<8>> const&);

    template gtl::BSNumber<gtl::UIntegerAP32> sqrt(gtl::BSNumber<gtl::UIntegerAP32> const&);
    template gtl::BSNumber<gtl::UIntegerFP32<8>> sqrt(gtl::BSNumber<gtl::UIntegerFP32<8>> const&);

    template gtl::BSNumber<gtl::UIntegerAP32> tan(gtl::BSNumber<gtl::UIntegerAP32> const&);
    template gtl::BSNumber<gtl::UIntegerFP32<8>> tan(gtl::BSNumber<gtl::UIntegerFP32<8>> const&);

    template gtl::BSNumber<gtl::UIntegerAP32> tanh(gtl::BSNumber<gtl::UIntegerAP32> const&);
    template gtl::BSNumber<gtl::UIntegerFP32<8>> tanh(gtl::BSNumber<gtl::UIntegerFP32<8>> const&);
}

namespace gtl
{
    template BSNumber<UIntegerAP32> atandivpi(BSNumber<UIntegerAP32> const&);
    template BSNumber<UIntegerFP32<8>> atandivpi(BSNumber<UIntegerFP32<8>> const&);

    template BSNumber<UIntegerAP32> atan2divpi(BSNumber<UIntegerAP32> const&, BSNumber<UIntegerAP32> const&);
    template BSNumber<UIntegerFP32<8>> atan2divpi(BSNumber<UIntegerFP32<8>> const&, BSNumber<UIntegerFP32<8>> const&);

    template BSNumber<UIntegerAP32> clamp(BSNumber<UIntegerAP32> const&, BSNumber<UIntegerAP32> const&, BSNumber<UIntegerAP32> const&);
    template BSNumber<UIntegerFP32<8>> clamp(BSNumber<UIntegerFP32<8>> const&, BSNumber<UIntegerFP32<8>> const&, BSNumber<UIntegerFP32<8>> const&);

    template BSNumber<UIntegerAP32> cospi(BSNumber<UIntegerAP32> const&);
    template BSNumber<UIntegerFP32<8>> cospi(BSNumber<UIntegerFP32<8>> const&);

    template BSNumber<UIntegerAP32> exp10(BSNumber<UIntegerAP32> const&);
    template BSNumber<UIntegerFP32<8>> exp10(BSNumber<UIntegerFP32<8>> const&);

    template BSNumber<UIntegerAP32> invsqrt(BSNumber<UIntegerAP32> const&);
    template BSNumber<UIntegerFP32<8>> invsqrt(BSNumber<UIntegerFP32<8>> const&);

    template std::int32_t isign(BSNumber<UIntegerAP32> const&);
    template std::int32_t isign(BSNumber<UIntegerFP32<8>> const&);

    template BSNumber<UIntegerAP32> saturate(BSNumber<UIntegerAP32> const&);
    template BSNumber<UIntegerFP32<8>> saturate(BSNumber<UIntegerFP32<8>> const&);

    template BSNumber<UIntegerAP32> sign(BSNumber<UIntegerAP32> const&);
    template BSNumber<UIntegerFP32<8>> sign(BSNumber<UIntegerFP32<8>> const&);

    template BSNumber<UIntegerAP32> sinpi(BSNumber<UIntegerAP32> const&);
    template BSNumber<UIntegerFP32<8>> sinpi(BSNumber<UIntegerFP32<8>> const&);

    template BSNumber<UIntegerAP32> sqr(BSNumber<UIntegerAP32> const&);
    template BSNumber<UIntegerFP32<8>> sqr(BSNumber<UIntegerFP32<8>> const&);

    template BSNumber<UIntegerAP32> FMA(
        BSNumber<UIntegerAP32> const&,
        BSNumber<UIntegerAP32> const&,
        BSNumber<UIntegerAP32> const&);

    template BSNumber<UIntegerFP32<8>> FMA(
        BSNumber<UIntegerFP32<8>> const&,
        BSNumber<UIntegerFP32<8>> const&,
        BSNumber<UIntegerFP32<8>> const&);

    template BSNumber<UIntegerAP32> RobustDOP(
        BSNumber<UIntegerAP32> const&,
        BSNumber<UIntegerAP32> const&,
        BSNumber<UIntegerAP32> const&,
        BSNumber<UIntegerAP32> const&);

    template BSNumber<UIntegerFP32<8>> RobustDOP(
        BSNumber<UIntegerFP32<8>> const&,
        BSNumber<UIntegerFP32<8>> const&,
        BSNumber<UIntegerFP32<8>> const&,
        BSNumber<UIntegerFP32<8>> const&);
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(BSNumber)
