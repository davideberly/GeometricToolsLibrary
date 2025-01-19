// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Arithmetic/UIntegerAP32.h>
#include <GTL/Mathematics/Arithmetic/UIntegerFP32.h>
#include <GTL/Mathematics/Arithmetic/BSRational.h>
#include <random>
using namespace gtl;

namespace gtl
{
    template class BSNumber<UIntegerAP32>;
    template class BSNumber<UIntegerFP32<44>>;
    template class BSRational<UIntegerAP32>;
    template class BSRational<UIntegerFP32<44>>;
    using APNumber = BSNumber<UIntegerAP32>;
    using FPNumber = BSNumber<UIntegerFP32<44>>;
    using APRational = BSRational<UIntegerAP32>;
    using FPRational = BSRational<UIntegerFP32<44>>;
}

namespace gtl
{
    class UnitTestBSRational
    {
    public:
        UnitTestBSRational();

    private:
        template <typename Rational> void Constructors();
        template <typename Number, typename Rational> void ConvertBSRationalToFloat();
        template <typename Number, typename Rational> void ConvertBSRationalToDouble();
        template <typename Number, typename Rational> void ConvertBSRationalSpecifiedPrecision();
        template <typename Number, typename Rational> void EqualityComparisons();
        template <typename Number, typename Rational> void InequalityComparisons();
        template <typename Number, typename Rational> void UnaryOperators();
        template <typename Number, typename Rational> void Addition();
        template <typename Number, typename Rational> void Subtraction();
        template <typename Number, typename Rational> void Multiplication();

        template <typename Number, typename Rational> void Conversions();
        template <typename Number, typename Rational> void PositiveRoundNearest(float z, Rational const& input);
        template <typename Number, typename Rational> void PositiveRoundUpward(float z, Rational const& input);
        template <typename Number, typename Rational> void PositiveRoundDownward(float z, Rational const& input);
        template <typename Number, typename Rational> void PositiveRoundTowardZero(float z, Rational const& input);
        template <typename Number, typename Rational> void NegativeRoundNearest(float z, Rational const& input);
        template <typename Number, typename Rational> void NegativeRoundUpward(float z, Rational const& input);
        template <typename Number, typename Rational> void NegativeRoundDownward(float z, Rational const& input);
        template <typename Number, typename Rational> void NegativeRoundTowardZero(float z, Rational const& input);

        std::default_random_engine mDRE;
        std::uniform_int_distribution<std::uint32_t> mURD;
    };
}

UnitTestBSRational::UnitTestBSRational()
    :
    mURD(0u, (1u << 23))
{
    UTInformation("Mathematics/Arithmetic/BSRational");

    Constructors<APRational>();
    Constructors<FPRational>();

    ConvertBSRationalToFloat<APNumber, APRational>();
    ConvertBSRationalToFloat<FPNumber, FPRational>();

    ConvertBSRationalToDouble<APNumber, APRational>();
    ConvertBSRationalToDouble<FPNumber, FPRational>();

    ConvertBSRationalSpecifiedPrecision<APNumber, APRational>();
    ConvertBSRationalSpecifiedPrecision<FPNumber, FPRational>();

    EqualityComparisons<APNumber, APRational>();
    EqualityComparisons<FPNumber, FPRational>();

    InequalityComparisons<APNumber, APRational>();
    InequalityComparisons<FPNumber, FPRational>();

    UnaryOperators<APNumber, APRational>();
    UnaryOperators<FPNumber, FPRational>();

    Addition<APNumber, APRational>();
    Addition<FPNumber, FPRational>();

    Subtraction<APNumber, APRational>();
    Subtraction<FPNumber, FPRational>();

    Multiplication<APNumber, APRational>();
    Multiplication<FPNumber, FPRational>();

    Conversions<APNumber, APRational>();
    Conversions<FPNumber, FPRational>();
}

template <typename Rational>
void UnitTestBSRational::Constructors()
{
    Rational x;

    x = Rational(0.0f);
    UTAssert(x.GetSign() == 0, "Failed to construct 0.0f.");
    x = Rational(0.0);
    UTAssert(x.GetSign() == 0, "Failed to construct 0.0.");
    x = Rational(0);
    UTAssert(x.GetSign() == 0, "Failed to construct 0.");
    x = Rational(0u);
    UTAssert(x.GetSign() == 0, "Failed to construct 0u.");
    x = Rational(static_cast<std::int64_t>(0));
    UTAssert(x.GetSign() == 0, "Failed to construct 0ll.");
    x = Rational(static_cast<std::uint64_t>(0));
    UTAssert(x.GetSign() == 0, "Failed to construct 0ull.");

    x = Rational(0.1f);
    // x.mNumerator.mValue = 0.10000000149011612
    // x.mDenominator.mValue = 1.0000000000000000
    // x.mValue = 0.10000000149011612
    UTAssert(
        x.GetNumerator().GetSign() == 1 &&
        x.GetNumerator().GetBiasedExponent() == -27 &&
        x.GetNumerator().GetUInteger().GetNumBits() == 24 &&
        x.GetNumerator().GetUInteger().GetNumBlocks() == 1 &&
        x.GetNumerator().GetUInteger().GetBits()[0] == 0x00cccccdu &&
        x.GetDenominator().GetSign() == 1 &&
        x.GetDenominator().GetBiasedExponent() == 0 &&
        x.GetDenominator().GetUInteger().GetNumBits() == 1 &&
        x.GetDenominator().GetUInteger().GetNumBlocks() == 1 &&
        x.GetDenominator().GetUInteger().GetBits()[0] == 0x00000001u,
        "Failed to construct 0.1f.");

    x = Rational(0.1);
    // x.mNumerator.mValue = 0.10000000000000001
    // x.mDenominator.mValue = 1.0000000000000000
    // x.mValue = 0.10000000000000001
    UTAssert(
        x.GetNumerator().GetSign() == 1 &&
        x.GetNumerator().GetBiasedExponent() == -55 &&
        x.GetNumerator().GetUInteger().GetNumBits() == 52 &&
        x.GetNumerator().GetUInteger().GetNumBlocks() == 2 &&
        x.GetNumerator().GetUInteger().GetBits()[0] == 0xcccccccdu &&
        x.GetNumerator().GetUInteger().GetBits()[1] == 0x000cccccu &&
        x.GetDenominator().GetSign() == 1 &&
        x.GetDenominator().GetBiasedExponent() == 0 &&
        x.GetDenominator().GetUInteger().GetNumBits() == 1 &&
        x.GetDenominator().GetUInteger().GetNumBlocks() == 1 &&
        x.GetDenominator().GetUInteger().GetBits()[0] == 0x00000001u,
        "Failed to construct 0.1.");

    x = Rational(10);
    UTAssert(
        x.GetNumerator().GetSign() == 1 &&
        x.GetNumerator().GetBiasedExponent() == 1 &&
        x.GetNumerator().GetUInteger().GetNumBits() == 3 &&
        x.GetNumerator().GetUInteger().GetNumBlocks() == 1 &&
        x.GetNumerator().GetUInteger().GetBits()[0] == 0x00000005u &&
        x.GetDenominator().GetSign() == 1 &&
        x.GetDenominator().GetBiasedExponent() == 0 &&
        x.GetDenominator().GetUInteger().GetNumBits() == 1 &&
        x.GetDenominator().GetUInteger().GetNumBlocks() == 1 &&
        x.GetDenominator().GetUInteger().GetBits()[0] == 0x00000001u,
        "Failed to construct 10.");

    x = Rational(10u);
    UTAssert(
        x.GetNumerator().GetSign() == 1 &&
        x.GetNumerator().GetBiasedExponent() == 1 &&
        x.GetNumerator().GetUInteger().GetNumBits() == 3 &&
        x.GetNumerator().GetUInteger().GetNumBlocks() == 1 &&
        x.GetNumerator().GetUInteger().GetBits()[0] == 0x00000005u &&
        x.GetDenominator().GetSign() == 1 &&
        x.GetDenominator().GetBiasedExponent() == 0 &&
        x.GetDenominator().GetUInteger().GetNumBits() == 1 &&
        x.GetDenominator().GetUInteger().GetNumBlocks() == 1 &&
        x.GetDenominator().GetUInteger().GetBits()[0] == 0x00000001u,
        "Failed to construct 10.");

    x = Rational(static_cast<std::int64_t>(0x1234567812345678ll));
    UTAssert(
        x.GetNumerator().GetSign() == 1 &&
        x.GetNumerator().GetBiasedExponent() == 3 &&
        x.GetNumerator().GetUInteger().GetNumBits() == 58 &&
        x.GetNumerator().GetUInteger().GetNumBlocks() == 2 &&
        x.GetNumerator().GetUInteger().GetBits()[0] == 0x02468acfu &&
        x.GetNumerator().GetUInteger().GetBits()[1] == 0x02468acfu &&
        x.GetDenominator().GetSign() == 1 &&
        x.GetDenominator().GetBiasedExponent() == 0 &&
        x.GetDenominator().GetUInteger().GetNumBits() == 1 &&
        x.GetDenominator().GetUInteger().GetNumBlocks() == 1 &&
        x.GetDenominator().GetUInteger().GetBits()[0] == 0x00000001u,
        "Failed to construct 0x1234567812345678ll.");

    x = Rational(static_cast<std::uint64_t>(0x1234567812345678ull));
    UTAssert(
        x.GetNumerator().GetSign() == 1 &&
        x.GetNumerator().GetBiasedExponent() == 3 &&
        x.GetNumerator().GetUInteger().GetNumBits() == 58 &&
        x.GetNumerator().GetUInteger().GetNumBlocks() == 2 &&
        x.GetNumerator().GetUInteger().GetBits()[0] == 0x02468acfu &&
        x.GetNumerator().GetUInteger().GetBits()[1] == 0x02468acfu &&
        x.GetDenominator().GetSign() == 1 &&
        x.GetDenominator().GetBiasedExponent() == 0 &&
        x.GetDenominator().GetUInteger().GetNumBits() == 1 &&
        x.GetDenominator().GetUInteger().GetNumBlocks() == 1 &&
        x.GetDenominator().GetUInteger().GetBits()[0] == 0x00000001u,
        "Failed to construct 0x1234567812345678ull.");

    x = Rational(x.GetNumerator());
    UTAssert(
        x.GetNumerator().GetSign() == 1 &&
        x.GetNumerator().GetBiasedExponent() == 3 &&
        x.GetNumerator().GetUInteger().GetNumBits() == 58 &&
        x.GetNumerator().GetUInteger().GetNumBlocks() == 2 &&
        x.GetNumerator().GetUInteger().GetBits()[0] == 0x02468acfu &&
        x.GetNumerator().GetUInteger().GetBits()[1] == 0x02468acfu &&
        x.GetDenominator().GetSign() == 1 &&
        x.GetDenominator().GetBiasedExponent() == 0 &&
        x.GetDenominator().GetUInteger().GetNumBits() == 1 &&
        x.GetDenominator().GetUInteger().GetNumBlocks() == 1 &&
        x.GetDenominator().GetUInteger().GetBits()[0] == 0x00000001u,
        "Failed to construct from a Rational numerator.");

    x = Rational(17.0f, 2.0f);
    UTAssert(
        x.GetNumerator().GetSign() == 1 &&
        x.GetNumerator().GetBiasedExponent() == 0 &&
        x.GetNumerator().GetUInteger().GetNumBits() == 5 &&
        x.GetNumerator().GetUInteger().GetNumBlocks() == 1 &&
        x.GetNumerator().GetUInteger().GetBits()[0] == 17 &&
        x.GetDenominator().GetSign() == 1 &&
        x.GetDenominator().GetBiasedExponent() == 1 &&
        x.GetDenominator().GetUInteger().GetNumBits() == 1 &&
        x.GetDenominator().GetUInteger().GetNumBlocks() == 1 &&
        x.GetDenominator().GetUInteger().GetBits()[0] == 1,
        "Failed to construct 17/2.");

    x = Rational(17.0, -2.0);
    UTAssert(
        x.GetNumerator().GetSign() == -1 &&
        x.GetNumerator().GetBiasedExponent() == 0 &&
        x.GetNumerator().GetUInteger().GetNumBits() == 5 &&
        x.GetNumerator().GetUInteger().GetNumBlocks() == 1 &&
        x.GetNumerator().GetUInteger().GetBits()[0] == 17 &&
        x.GetDenominator().GetSign() == 1 &&
        x.GetDenominator().GetBiasedExponent() == 1 &&
        x.GetDenominator().GetUInteger().GetNumBits() == 1 &&
        x.GetDenominator().GetUInteger().GetNumBlocks() == 1 &&
        x.GetDenominator().GetUInteger().GetBits()[0] == 1,
        "Failed to construct -17/2.");

    x = Rational(-17, 2);
    UTAssert(
        x.GetNumerator().GetSign() == -1 &&
        x.GetNumerator().GetBiasedExponent() == 0 &&
        x.GetNumerator().GetUInteger().GetNumBits() == 5 &&
        x.GetNumerator().GetUInteger().GetNumBlocks() == 1 &&
        x.GetNumerator().GetUInteger().GetBits()[0] == 17 &&
        x.GetDenominator().GetSign() == 1 &&
        x.GetDenominator().GetBiasedExponent() == 1 &&
        x.GetDenominator().GetUInteger().GetNumBits() == 1 &&
        x.GetDenominator().GetUInteger().GetNumBlocks() == 1 &&
        x.GetDenominator().GetUInteger().GetBits()[0] == 1,
        "Failed to construct 17/2.");

    x = Rational(17u, 2u);
    UTAssert(
        x.GetNumerator().GetSign() == 1 &&
        x.GetNumerator().GetBiasedExponent() == 0 &&
        x.GetNumerator().GetUInteger().GetNumBits() == 5 &&
        x.GetNumerator().GetUInteger().GetNumBlocks() == 1 &&
        x.GetNumerator().GetUInteger().GetBits()[0] == 17 &&
        x.GetDenominator().GetSign() == 1 &&
        x.GetDenominator().GetBiasedExponent() == 1 &&
        x.GetDenominator().GetUInteger().GetNumBits() == 1 &&
        x.GetDenominator().GetUInteger().GetNumBlocks() == 1 &&
        x.GetDenominator().GetUInteger().GetBits()[0] == 1,
        "Failed to construct 17/2.");

    x = Rational(static_cast<std::int64_t>(-17), static_cast<std::int64_t>(2));
    UTAssert(
        x.GetNumerator().GetSign() == -1 &&
        x.GetNumerator().GetBiasedExponent() == 0 &&
        x.GetNumerator().GetUInteger().GetNumBits() == 5 &&
        x.GetNumerator().GetUInteger().GetNumBlocks() == 1 &&
        x.GetNumerator().GetUInteger().GetBits()[0] == 17 &&
        x.GetDenominator().GetSign() == 1 &&
        x.GetDenominator().GetBiasedExponent() == 1 &&
        x.GetDenominator().GetUInteger().GetNumBits() == 1 &&
        x.GetDenominator().GetUInteger().GetNumBlocks() == 1 &&
        x.GetDenominator().GetUInteger().GetBits()[0] == 1,
        "Failed to construct 17/2.");

    x = Rational(static_cast<std::uint64_t>(17), static_cast<std::uint64_t>(2));
    UTAssert(
        x.GetNumerator().GetSign() == 1 &&
        x.GetNumerator().GetBiasedExponent() == 0 &&
        x.GetNumerator().GetUInteger().GetNumBits() == 5 &&
        x.GetNumerator().GetUInteger().GetNumBlocks() == 1 &&
        x.GetNumerator().GetUInteger().GetBits()[0] == 17 &&
        x.GetDenominator().GetSign() == 1 &&
        x.GetDenominator().GetBiasedExponent() == 1 &&
        x.GetDenominator().GetUInteger().GetNumBits() == 1 &&
        x.GetDenominator().GetUInteger().GetNumBlocks() == 1 &&
        x.GetDenominator().GetUInteger().GetBits()[0] == 1,
        "Failed to construct 17/2.");

    x = Rational(x.GetNumerator(), x.GetDenominator());
    UTAssert(
        x.GetNumerator().GetSign() == 1 &&
        x.GetNumerator().GetBiasedExponent() == -1 &&
        x.GetNumerator().GetUInteger().GetNumBits() == 5 &&
        x.GetNumerator().GetUInteger().GetNumBlocks() == 1 &&
        x.GetNumerator().GetUInteger().GetBits()[0] == 17 &&
        x.GetDenominator().GetSign() == 1 &&
        x.GetDenominator().GetBiasedExponent() == 0 &&
        x.GetDenominator().GetUInteger().GetNumBits() == 1 &&
        x.GetDenominator().GetUInteger().GetNumBlocks() == 1 &&
        x.GetDenominator().GetUInteger().GetBits()[0] == 1,
        "Failed to construct from Rational numerator and denominator.");

    x = Rational("123456.314159");
    // x.mNumerator.mValue = 209143.13127850115
    // x.mDenominator.mValue = 3.3881317890172014
    // x.mValue = 123456.31415900000
    UTAssert(
        x.GetNumerator().GetSign() == 1 &&
        x.GetNumerator().GetBiasedExponent() == -54 &&
        x.GetNumerator().GetUInteger().GetNumBits() == 72 &&
        x.GetNumerator().GetUInteger().GetNumBlocks() == 3 &&
        x.GetNumerator().GetUInteger().GetBits()[0] == 0xf1491fe3u &&
        x.GetNumerator().GetUInteger().GetBits()[1] == 0x3dc866ddu &&
        x.GetNumerator().GetUInteger().GetBits()[2] == 0x000000ccu &&
        x.GetDenominator().GetSign() == 1 &&
        x.GetDenominator().GetBiasedExponent() == -48 &&
        x.GetDenominator().GetUInteger().GetNumBits() == 49 &&
        x.GetDenominator().GetUInteger().GetNumBlocks() == 2 &&
        x.GetDenominator().GetUInteger().GetBits()[0] == 0x4d6e2ef5 &&
        x.GetDenominator().GetUInteger().GetBits()[1] == 0x0001b1aeu,
        "Failed to construct from string.");

    Rational y = x;
    UTAssert(
        y.GetNumerator().GetSign() == 1 &&
        y.GetNumerator().GetBiasedExponent() == -54 &&
        y.GetNumerator().GetUInteger().GetNumBits() == 72 &&
        y.GetNumerator().GetUInteger().GetNumBlocks() == 3 &&
        y.GetNumerator().GetUInteger().GetBits()[0] == 0xf1491fe3u &&
        y.GetNumerator().GetUInteger().GetBits()[1] == 0x3dc866ddu &&
        y.GetNumerator().GetUInteger().GetBits()[2] == 0x000000ccu &&
        y.GetDenominator().GetSign() == 1 &&
        y.GetDenominator().GetBiasedExponent() == -48 &&
        y.GetDenominator().GetUInteger().GetNumBits() == 49 &&
        y.GetDenominator().GetUInteger().GetNumBlocks() == 2 &&
        y.GetDenominator().GetUInteger().GetBits()[0] == 0x4d6e2ef5 &&
        y.GetDenominator().GetUInteger().GetBits()[1] == 0x0001b1aeu,
        "Failed to construct from copy constructor.");

    Rational z = std::move(x);
    UTAssert(
        z.GetNumerator().GetSign() == 1 &&
        z.GetNumerator().GetBiasedExponent() == -54 &&
        z.GetNumerator().GetUInteger().GetNumBits() == 72 &&
        z.GetNumerator().GetUInteger().GetNumBlocks() == 3 &&
        z.GetNumerator().GetUInteger().GetBits()[0] == 0xf1491fe3u &&
        z.GetNumerator().GetUInteger().GetBits()[1] == 0x3dc866ddu &&
        z.GetNumerator().GetUInteger().GetBits()[2] == 0x000000ccu &&
        z.GetDenominator().GetSign() == 1 &&
        z.GetDenominator().GetBiasedExponent() == -48 &&
        z.GetDenominator().GetUInteger().GetNumBits() == 49 &&
        z.GetDenominator().GetUInteger().GetNumBlocks() == 2 &&
        z.GetDenominator().GetUInteger().GetBits()[0] == 0x4d6e2ef5 &&
        z.GetDenominator().GetUInteger().GetBits()[1] == 0x0001b1aeu,
        "Failed to construct from move constructor.");
}

template <typename Number, typename Rational>
void UnitTestBSRational::ConvertBSRationalToFloat()
{
    Number const multiplier(3.45678);
    Rational x;
    IEEEBinary32 x32;
    IEEEBinary64 x64, x64prev, x64next;

    // Convert from numbers smaller than minsubnormal.
    x64.number = std::ldexp(1.0, -150);
    x64prev.encoding = x64.GetNextDown();
    x64next.encoding = x64.GetNextUp();

    x = Rational(Number(x64.number) * multiplier, multiplier);
    x32.number = static_cast<float>(x);
    UTAssert(x32.encoding == 0x00000000u,
        "Failed to convert 2^{-150}.");

    x = Rational(Number(x64prev.number) * multiplier, multiplier);
    x32.number = static_cast<float>(x);
    UTAssert(x32.encoding == 0x00000000u,
        "Failed to convert 2^{-150}-epsilon.");

    x = Rational(Number(x64next.number) * multiplier, multiplier);
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
        x = Rational(Number(value) * multiplier, multiplier);
        x32.number = static_cast<float>(x);
        UTAssert(x32.number == fy, "Failed to convert " + std::to_string(value));

        value = (di + 0.12345) * twoPowN149;
        fy = static_cast<float>(value);
        x = Rational(Number(value) * multiplier, multiplier);
        x32.number = static_cast<float>(x);
        UTAssert(x32.number == fy, "Failed to convert " + std::to_string(value));

        value = (di + 0.5) * twoPowN149;
        fy = static_cast<float>(value);
        x = Rational(Number(value) * multiplier, multiplier);
        x32.number = static_cast<float>(x);
        UTAssert(x32.number == fy, "Failed to convert " + std::to_string(value));

        value = (di + 0.56789) * twoPowN149;
        fy = static_cast<float>(value);
        x = Rational(Number(value) * multiplier, multiplier);
        x32.number = static_cast<float>(x);
        UTAssert(x32.number == fy, "Failed to convert " + std::to_string(value));
    }

    // Convert from numbers smaller than maxnormal. The "_" indicates the
    // implied binary point after ULPS.

    // 1.0 * 2^{-126} is an exact conversion
    x64.number = std::ldexp(1.0, -126);
    x = Rational(Number(x64.number) * multiplier, multiplier);
    x32.number = static_cast<float>(x);
    UTAssert(x32.encoding == 0x00800000u,
        "Failed to convert 2^{-126}.");

    // 1.0^{22}1_01 * 2^{1}, round down
    // 02000005 * 2^{-24}, 26 bits
    x64.number = 2.0 * (1.0 + std::ldexp(1.0, -23) + std::ldexp(1.0, -25));
    x = Rational(Number(x64.number) * multiplier, multiplier);
    x32.number = static_cast<float>(x);
    UTAssert(x32.encoding == 0x40000001u,
        "Failed to convert 2*(1+2^{-23}+2^{-25}).");

    // 1.0^{22}1_10 * 2^{1}, round up
    // 01000003 * 2^{-23}, 25 bits
    x64.number = 2.0 * (1.0 + std::ldexp(1.0, -23) + std::ldexp(1.0, -24));
    x = Rational(Number(x64.number) * multiplier, multiplier);
    x32.number = static_cast<float>(x);
    UTAssert(x32.encoding == 0x40000002u,
        "Failed to convert 2*(1+2^{-23}+2^{-25}).");

    // 1.0^{22}1_11 * 2^{1}, round up
    // 02000007 * 2^{-24}, 26 bits
    x64.number = 2.0 * (1.0 + std::ldexp(1.0, -23) + std::ldexp(1.0, -24) + std::ldexp(1.0, -25));
    x = Rational(Number(x64.number) * multiplier, multiplier);
    x32.number = static_cast<float>(x);
    UTAssert(x32.encoding == 0x40000002u,
        "Failed to convert 2*(1+2^{-23}+2^{-24}+2^{-25}).");

    // 1.0^{21}10_01 * 2^{1}, round down
    // 02000009 * 2^{-24}, 26 bits
    x64.number = 2.0 * (1.0 + std::ldexp(1.0, -22) + std::ldexp(1.0, -25));
    x = Rational(Number(x64.number) * multiplier, multiplier);
    x32.number = static_cast<float>(x);
    UTAssert(x32.encoding == 0x40000002u,
        "Failed to convert 2*(1+2^{-22}+2^{-25}).");

    // 1.0^{21}10_10 * 2^{1}, round down
    // 01000005 * 2^{-23}, 25 bits
    x64.number = 2.0 * (1.0 + std::ldexp(1.0, -22) + std::ldexp(1.0, -24));
    x = Rational(Number(x64.number) * multiplier, multiplier);
    x32.number = static_cast<float>(x);
    UTAssert(x32.encoding == 0x40000002u,
        "Failed to convert 2*(1+2^{-22}+2^{-24}).");

    // 1.0^{21}10_11 * 2^{1}, round up
    // 0200000b * 2^{-24}, 26 bits
    x64.number = 2.0 * (1.0 + std::ldexp(1.0, -22) + std::ldexp(1.0, -24) + std::ldexp(1.0, -25));
    x = Rational(Number(x64.number) * multiplier, multiplier);
    x32.number = static_cast<float>(x);
    UTAssert(x32.encoding == 0x40000003u,
        "Failed to convert 2*(1+2^{-22}+2^{-24}+2^{-25}).");

    // 1.1^{23}_1 * 2^{1}, round up, carry out
    // 01ffffff * 2^{-24}, 25 bits
    x64.number = 2.0 * (2.0 - std::ldexp(1.0, -24));
    x = Rational(Number(x64.number) * multiplier, multiplier);
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
    x = Rational(Number(maxnor) * multiplier, multiplier);
    x32.number = static_cast<float>(x);
    UTAssert(x32.encoding == 0x7f7fffffu,
        "Failed to convert maxnor.");

    // Rounds to inf.
    x = Number(inf);
    x32.number = static_cast<float>(x);
    UTAssert(x32.encoding == 0x7f800000u,
        "Failed to convert number inf.");

    // Rounds to inf.
    x = Rational(Number(avrmaxnorinf) * multiplier, multiplier);
    x32.number = static_cast<float>(x);
    UTAssert(x32.encoding == 0x7f800000u,
        "Failed to convert number avrmaxnorinf.");

    // Rounds to maxnor.
    x = Rational(Number(mid0) * multiplier, multiplier);
    x32.number = static_cast<float>(x);
    UTAssert(x32.encoding == 0x7f7fffffu,
        "Failed to convert number mid0.");

    // Rounds to inf.
    x = Rational(Number(mid1) * multiplier, multiplier);
    x32.number = static_cast<float>(x);
    UTAssert(x32.encoding == 0x7f800000u,
        "Failed to convert number mid1.");
}

template <typename Number, typename Rational>
void UnitTestBSRational::ConvertBSRationalToDouble()
{
    Number const one(1), two(2), half(0.5);
    Number const multiplier(3.45678);
    Number temp;
    Rational x, y;
    IEEEBinary64 x64;

    // Convert from numbers smaller than minsubnormal.
    temp = std::ldexp(one, -1075) - std::ldexp(one, -1076);
    x = Rational(temp * multiplier, multiplier);
    x64.number = static_cast<double>(x);
    UTAssert(x64.encoding == 0x0000000000000000ull,
        "Failed to convert 2^{-1075}.");

    temp = std::ldexp(one, -1075) - std::ldexp(one, -1076);
    x = Rational(temp * multiplier, multiplier);
    x64.number = static_cast<double>(x);
    UTAssert(x64.encoding == 0x0000000000000000ull,
        "Failed to convert 2^{-1075} - 2^{-1076}.");

    temp = std::ldexp(one, -1075) + std::ldexp(one, -1076);
    x = Rational(temp * multiplier, multiplier);
    x64.number = static_cast<double>(x);
    UTAssert(x64.encoding == 0x0000000000000001ull,
        "Failed to convert 2^{-1075} + 2^{-1076}.");

    // Convert from numbers smaller than minnormal. The "_" indicates the
    // implied binary point after ULPS.
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

        x = Rational(min * multiplier, multiplier);
        x64.number = static_cast<double>(x);
        y = Rational(x64.number);
        UTAssert(y == min, "Failed to convert number to double: " + std::to_string(i));

        x = Rational((min + delta0 * twoPowN1074) * multiplier, multiplier);
        x64.number = static_cast<double>(x);
        y = Rational(x64.number);
        UTAssert(y == min, "Failed to convert number to double: " + std::to_string(i));

        x = Rational((min + half * twoPowN1074) * multiplier, multiplier);
        x64.number = static_cast<double>(x);
        y = Rational(x64.number);
        if (i & 1)
        {
            UTAssert(y == max, "Failed to convert number to double: " + std::to_string(i));
        }
        else
        {
            UTAssert(y == min, "Failed to convert number to double: " + std::to_string(i));
        }

        x = Rational((min + delta1 * twoPowN1074) * multiplier, multiplier);
        x64.number = static_cast<double>(x);
        y = Rational(x64.number);
        UTAssert(y == max, "Failed to convert number to double: " + std::to_string(i));
    }

    di = Number(static_cast<std::uint64_t>((1ull << 52) - 1ull));
    dip1 = Number(static_cast<std::uint64_t>(1ull << 52));
    min = di * twoPowN1074;
    max = dip1 * twoPowN1074;
    x = Rational(min * multiplier, multiplier);
    x64.number = static_cast<double>(x);
    y = Rational(x64.number);
    UTAssert(y == min, "Failed to convert number to double: (1ull << 52)");

    x = Rational((min + delta0 * twoPowN1074) * multiplier, multiplier);
    x64.number = static_cast<double>(x);
    y = Rational(x64.number);
    UTAssert(y == min, "Failed to convert number to double: (1ull << 52)");

    x = Rational((min + half * twoPowN1074) * multiplier, multiplier);
    x64.number = static_cast<double>(x);
    y = Rational(x64.number);
    UTAssert(y == max, "Failed to convert number to double: (1ull << 52)");

    x = Rational((min + delta1 * twoPowN1074) * multiplier, multiplier);
    x64.number = static_cast<double>(x);
    y = Rational(x64.number);
    UTAssert(y == max, "Failed to convert number to double: (1ull << 52)");

    // Convert from numbers smaller than maxnormal.  The "_" indicates the
    // implied binary point after ULPS.

    // 1.0 * 2^{-126} is an exact conversion
    temp = std::ldexp(one, -1022);
    x = Rational(temp * multiplier, multiplier);
    x64.number = static_cast<double>(x);
    UTAssert(x64.encoding == 0x0010000000000000ull,
        "Failed to convert 2^{-1022}.");

    // 1.0^{51}1_01 * 2^{1}, round down
    // 00400000 00000005 * 2^{-53}, 55 bits
    temp = two * (one + std::ldexp(one, -52) + std::ldexp(one, -54));
    x = Rational(temp * multiplier, multiplier);
    x64.number = static_cast<double>(x);
    UTAssert(x64.encoding == 0x4000000000000001ull,
        "Failed to convert 2*(1+2^{-52}+2^{-54}).");

    // 1.0^{51}1_10 * 2^{1}, round up
    // 00200000 0000003 * 2^{-52}, 54 bits
    temp = two * (one + std::ldexp(one, -52) + std::ldexp(one, -53));
    x = Rational(temp * multiplier, multiplier);
    x64.number = static_cast<double>(x);
    UTAssert(x64.encoding == 0x4000000000000002ull,
        "Failed to convert 2*(1+2^{-52}+2^{-53}).");

    // 1.0^{51}1_11 * 2^{1}, round up
    // 00400000 00000007  * 2^{-53}, 55 bits
    temp = two * (one + std::ldexp(one, -52) + std::ldexp(one, -53) + std::ldexp(one, -54));
    x = Rational(temp * multiplier, multiplier);
    x64.number = static_cast<double>(x);
    UTAssert(x64.encoding == 0x4000000000000002ull,
        "Failed to convert 2*(1+2^{-52}+2^{-53}+2^{-54}).");

    // 1.0^{50}10_01 * 2^{1}, round down
    // 00400000 00000009 * 2^{-53}, 55 bits
    temp = two * (one + std::ldexp(one, -51) + std::ldexp(one, -54));
    x = Rational(temp * multiplier, multiplier);
    x64.number = static_cast<double>(x);
    UTAssert(x64.encoding == 0x4000000000000002ull,
        "Failed to convert 2*(1+2^{-51}+2^{-53}).");

    // 1.0^{50}10_10 * 2^{1}, round down
    // 00200000 00000005  * 2^{-52}, 54 bits
    temp = two * (one + std::ldexp(one, -51) + std::ldexp(one, -53));
    x = Rational(temp * multiplier, multiplier);
    x64.number = static_cast<double>(x);
    UTAssert(x64.encoding == 0x4000000000000002ull,
        "Failed to convert 2*(1+2^{-51}+2^{-53}).");

    // 1.0^{51}10_11 * 2^{1}, round up
    // 00400000 0000000b * 2^{-53}, 55 bits
    temp = two * (one + std::ldexp(one, -51) + std::ldexp(one, -53) + std::ldexp(one, -54));
    x = Rational(temp * multiplier, multiplier);
    x64.number = static_cast<double>(x);
    UTAssert(x64.encoding == 0x4000000000000003ull,
        "Failed to convert 2*(1+2^{-51}+2^{-53}+2^{-54}).");

    // 1.1^{52}_1 * 2^{1}, round up, carry out
    // 003fffff ffffffff * 2^{-52}, 54 bits
    temp = two * (two - std::ldexp(one, -53));
    x = Rational(temp * multiplier, multiplier);
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
    x = Rational(maxnor * multiplier, multiplier);
    x64.number = static_cast<double>(x);
    UTAssert(x64.number == std::numeric_limits<double>::max(),
        "Failed to convert maxnor.");

    // Rounds to inf.
    x = Rational(inf * multiplier, multiplier);
    x64.number = static_cast<double>(x);
    UTAssert(x64.number == std::numeric_limits<double>::infinity(),
        "Failed to convert number inf.");

    // Rounds to inf.
    x = Rational(avrmaxnorinf * multiplier, multiplier);
    x64.number = static_cast<double>(x);
    UTAssert(x64.number == std::numeric_limits<double>::infinity(),
        "Failed to convert number avrmaxnorinf.");

    // Rounds to maxnor.
    x = Rational(mid0 * multiplier, multiplier);
    x64.number = static_cast<double>(x);
    UTAssert(x64.number == std::numeric_limits<double>::max(),
        "Failed to convert number mid0.");

    // Rounds to inf.
    x = Rational(mid1 * multiplier, multiplier);
    x64.number = static_cast<double>(x);
    UTAssert(x64.number == std::numeric_limits<double>::infinity(),
        "Failed to convert number mid1.");
}

template <typename Number, typename Rational>
void UnitTestBSRational::ConvertBSRationalSpecifiedPrecision()
{
    Number const multiplier(2.34567);
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

    Number temp = x * x;
    // temp.biasedExponent = 1942, temp.exponent = 2047
    // temp.numBits = 106, temp.bits = 000003ff ffffffff ffc00000 00000001
    // temp = 1^[52] 0^[53] 1^[1] * 2^{1942}
    //      = 1.1^[51] 0^[53] 1^[1] * 2^{2047}
    UTAssert(
        temp.GetBiasedExponent() == 1942 &&
        temp.GetExponent() == 2047 &&
        temp.GetUInteger().GetNumBits() == 106 &&
        temp.GetUInteger().GetNumBlocks() == 4 &&
        temp.GetUInteger().GetBits()[0] == 0x00000001u &&
        temp.GetUInteger().GetBits()[1] == 0xffc00000u &&
        temp.GetUInteger().GetBits()[2] == 0xffffffffu &&
        temp.GetUInteger().GetBits()[3] == 0x000003ffu,
        "Incorrect representation.");

    Rational xsqr(temp * multiplier, multiplier);

    // The first example here leads first to a bit pattern that has
    // 1^[52] 0^[20] internally. This has to be shifted right to satisfy
    // the invariant that the UInteger part of y is a positive odd number.
    Number y(0);
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

template <typename Number, typename Rational>
void UnitTestBSRational::EqualityComparisons()
{
    Number const multiplier(2.34567);
    Rational x, y;
    bool result;

    // signs both zero
    x = Rational(0.0f);
    y = Number(0.0f);
    result = (x == y);
    UTAssert(result == true, "operator== failed on (0,0).");
    result = (x != y);
    UTAssert(result == false, "operator!= failed on (0,0).");

    // signs differ
    x = Rational(Number(0), multiplier);
    y = Rational(Number(1) * multiplier, multiplier);
    result = (x == y);
    UTAssert(result == false, "operator== failed on (0,1).");
    result = (x != y);
    UTAssert(result == true, "operator!= failed on (0,1).");

    // signs the same, biased exponents differ
    x = Rational(Number(1) * multiplier, multiplier);
    y = Rational(Number(2) * multiplier, multiplier);
    result = (x == y);
    UTAssert(result == false, "operator== failed on (1,2).");
    result = (x != y);
    UTAssert(result == true, "operator!= failed on (1,2).");

    // signs the same, biased exponents the same, number of bits differ
    x = Rational(Number(12) * multiplier, multiplier);
    y = Rational(Number(28) * multiplier, multiplier);
    result = (x == y);
    UTAssert(result == false, "operator== failed on (12,28).");
    result = (x != y);
    UTAssert(result == true, "operator!= failed on (12,28).");

    // signs the same, biased exponents the same, number of bits the same,
    // bits differ
    x = Rational(Number(20) * multiplier, multiplier);
    y = Rational(Number(28) * multiplier, multiplier);
    result = (x == y);
    UTAssert(result == false, "operator== failed on (20,28).");
    result = (x != y);
    UTAssert(result == true, "operator!= failed on (20,28).");

    // numbers the same
    x = Rational(Number(1.2345f) * multiplier, multiplier);
    y = x;
    result = (x == y);
    UTAssert(result == true, "operator== failed on (1.2345,1.2345).");
    result = (x != y);
    UTAssert(result == false, "operator!= failed on (1.2345,1.2345).");

    x = Rational(Number(-1.2345f) * multiplier, multiplier);
    y = x;
    result = (x == y);
    UTAssert(result == true, "operator== failed on (1.2345,1.2345).");
    result = (x != y);
    UTAssert(result == false, "operator!= failed on (1.2345,1.2345).");
}

template <typename Number, typename Rational>
void UnitTestBSRational::InequalityComparisons()
{
    Number const multiplier0(2.34567), multiplier1(-9.87654);
    Rational x, y;
    bool result;

    // Return based on sign comparison.
    x = Rational(Number(1.0f) * multiplier0, multiplier0);
    y = Rational(Number(-2.0f) * multiplier1, multiplier1);
    result = (x < y);
    UTAssert(result == false, "operator< failed on (1,-2).");
    result = (x >= y);
    UTAssert(result == true, "operator>= failed on (1,-2).");

    x = Rational(Number(-2.0f) * multiplier0, multiplier0);
    y = Rational(Number(1.0f) * multiplier1, multiplier1);
    result = (x < y);
    UTAssert(result == true, "operator< failed on (-2,1).");
    result = (x >= y);
    UTAssert(result == false, "operator>= failed on (-2,1).");

    x = Rational(Number(0.0f) * multiplier0, multiplier0);
    y = Rational(Number(1.0f) * multiplier1, multiplier1);
    result = (x < y);
    UTAssert(result == true, "operator< failed on (0,1).");
    result = (x >= y);
    UTAssert(result == false, "operator>= failed on (0,1).");

    // Return based on exponent comparison.
    x = Rational(Number(1.0f) * multiplier0, multiplier0);
    y = Rational(Number(2.0f) * multiplier1, multiplier1);
    result = (x < y);
    UTAssert(result == true, "operator< failed on (1,2).");
    result = (x >= y);
    UTAssert(result == false, "operator>= failed on (1,2).");

    x = Rational(Number(2.0f) * multiplier0, multiplier0);
    y = Rational(Number(1.0f) * multiplier1, multiplier1);
    result = (x < y);
    UTAssert(result == false, "operator< failed on (2,1).");
    result = (x >= y);
    UTAssert(result == true, "operator>= failed on (2,1).");

    x = Rational(Number(-1.0f) * multiplier0, multiplier0);
    y = Rational(Number(-2.0f) * multiplier1, multiplier1);
    result = (x < y);
    UTAssert(result == false, "operator< failed on (-1,-2).");
    result = (x >= y);
    UTAssert(result == true, "operator>= failed on (-1,-2).");

    x = Rational(Number(-2.0f) * multiplier0, multiplier0);
    y = Rational(Number(-1.0f) * multiplier1, multiplier1);
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
    x = Rational(Number(x32.number) * multiplier0, multiplier0);
    y = Rational(Number(y32.number) * multiplier1, multiplier1);
    result = (x < y);
    UTAssert(result == false,
        "operator< failed on (0x3fc00000,0x3fa00000).");
    result = (x >= y);
    UTAssert(result == true,
        "operator>= failed on (0x3fc00000,0x3fa00000).");
    x32.SetEncoding(1, 127, 0x00400000);
    y32.SetEncoding(1, 127, 0x00200000);
    x = Rational(Number(x32.number) * multiplier0, multiplier0);
    y = Rational(Number(y32.number) * multiplier1, multiplier1);
    result = (x < y);
    UTAssert(result == true,
        "operator< failed on (0xbfc00000,0xbfa00000).");
    result = (x >= y);
    UTAssert(result == false,
        "operator>= failed on (0xbfc00000,0xbfa00000).");

    // x has bits in block0 and block1, y has bits in block0
    x64.SetEncoding(0, 0x03FF, 0x0008000000000001ull);
    y64.SetEncoding(0, 0x03FF, 0x0004000000000000ull);
    x = Rational(Number(x64.number) * multiplier0, multiplier0);
    y = Rational(Number(y64.number) * multiplier1, multiplier1);
    result = (x < y);
    UTAssert(result == false,
        "operator< failed on (0x3ff8000000000001,0x3ff4000000000000).");
    result = (x >= y);
    UTAssert(result == true,
        "operator>= failed on (0x3ff8000000000001,0x3ff4000000000000).");

    // x has bits in block0, y has bits in block0 and block1
    x64.SetEncoding(0, 0x03FF, 0x0008000000000000ull);
    y64.SetEncoding(0, 0x03FF, 0x0004000000000001ull);
    x = Rational(Number(x64.number) * multiplier0, multiplier0);
    y = Rational(Number(y64.number) * multiplier1, multiplier1);
    result = (x < y);
    UTAssert(result == false,
        "operator< failed on (0x3ff8000000000000,0x3ff4000000000001).");
    result = (x >= y);
    UTAssert(result == true,
        "operator>= failed on (0x3ff8000000000000,0x3ff4000000000001).");

    // x has bits in block0 and block1, y has bits in block0 and block1
    x64.SetEncoding(0, 0x03FF, 0x0008000000000001ull);
    y64.SetEncoding(0, 0x03FF, 0x0004000000000001ull);
    x = Rational(Number(x64.number) * multiplier0, multiplier0);
    y = Rational(Number(y64.number) * multiplier1, multiplier1);
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
    x = Rational(Number(x64.number) * multiplier0, multiplier0);
    y = Rational(Number(y64.number) * multiplier1, multiplier1);
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
    x = Rational(Number(x64.number) * multiplier0, multiplier0);
    y = Rational(Number(y64.number) * multiplier1, multiplier1);
    result = (x < y);
    UTAssert(result == true,
        "operator< failed on (0x3ff8000000A00000,0x3ff8000000B00000).");
    result = (x >= y);
    UTAssert(result == false,
        "operator>= failed on (0x3ff8000000A00000,0x3ff8000000B00000).");

    // x and y are the same number
    x64.SetEncoding(0, 0x03FF, 0x0008000000B00000ull);
    y64.SetEncoding(0, 0x03FF, 0x0008000000B00000ull);
    x = Rational(Number(x64.number) * multiplier0, multiplier0);
    y = Rational(Number(y64.number) * multiplier1, multiplier1);
    result = (x < y);
    UTAssert(result == false,
        "operator< failed on (0x3ff8000000B00000,0x3ff8000000B00000).");
    result = (x >= y);
    UTAssert(result == true,
        "operator>= failed on (0x3ff8000000B00000,0x3ff8000000B00000).");
}

template <typename Number, typename Rational>
void UnitTestBSRational::UnaryOperators()
{
    Number const multiplier0(2.34567), multiplier1(-9.87654);
    Rational x, y;
    bool result;

    x = Rational(Number(+1.0f) * multiplier0, multiplier0);
    y = Rational(Number(1.0) * multiplier1, multiplier1);
    result = (x == +y);
    UTAssert(result == true, "operator+() failed");

    x = Rational(Number(+1.0f) * multiplier0, multiplier0);
    y = Rational(Number(-1.0) * multiplier1, multiplier1);
    result = (x == -y);
    UTAssert(result == true, "operator-() failed");
}

template <typename Number, typename Rational>
void UnitTestBSRational::Addition()
{
    Number const multiplier0(2.34567), multiplier1(-9.87654);
    Rational x, y, z, z0;

    std::uint32_t ux, uy;
    std::uint64_t uz;
    ux = 0xFFFFFFFFu;
    uy = 0x00FFFFFFu;
    uz = static_cast<std::uint64_t>(ux) + static_cast<std::uint64_t>(uy);
    x = Rational(Number(ux) * multiplier0, multiplier0);
    y = Rational(Number(uy) * multiplier1, multiplier1);
    z = x + y;
    z0 = Rational(uz);
    UTAssert(z == z0, "Failed to add numbers.");

    ux = 0xFFFFFFFFu;
    uy = 0xB0000000u;
    uz = static_cast<std::uint64_t>(ux) + static_cast<std::uint64_t>(uy);
    x = Rational(Number(ux) * multiplier0, multiplier0);
    y = Rational(Number(uy) * multiplier1, multiplier1);
    z = x + y;
    z0 = Rational(uz);
    UTAssert(z == z0, "Failed to add numbers.");

    double fx, fy, fz, fz0;

    // signs 0, *
    fx = 0.0;
    fy = 1.2345;
    fz = fx + fy;
    x = Rational(Number(fx) * multiplier0, multiplier0);
    y = Rational(Number(fy) * multiplier1, multiplier1);
    z = x + y;
    fz0 = static_cast<double>(z);
    UTAssert(fz == fz0, "Failed to add numbers.");

    // signs *, 0
    fx = -1.2345;
    fy = 0.0;
    fz = fx + fy;
    x = Rational(Number(fx) * multiplier0, multiplier0);
    y = Rational(Number(fy) * multiplier1, multiplier1);
    z = x + y;
    fz0 = static_cast<double>(z);
    UTAssert(fz == fz0, "Failed to add numbers.");

    // signs +, +
    fx = 1.2345;
    fy = 2.3456;
    fz = fx + fy;
    x = Rational(Number(fx) * multiplier0, multiplier0);
    y = Rational(Number(fy) * multiplier1, multiplier1);
    z = x + y;
    fz0 = static_cast<double>(z);
    UTAssert(fz == fz0, "Failed to add numbers.");

    // signs +, -, n0 + n1 = -(|n1| - |n0|) < 0
    fx = 1.2345;
    fy = -2.3456;
    fz = fx + fy;
    x = Rational(Number(fx) * multiplier0, multiplier0);
    y = Rational(Number(fy) * multiplier1, multiplier1);
    z = x + y;
    fz0 = static_cast<double>(z);
    UTAssert(fz == fz0, "Failed to add numbers.");

    // signs +, -, n0 + n1 = |n0| - |n1| > 0
    fx = 2.3456;
    fy = -1.2345;
    fz = fx + fy;
    x = Rational(Number(fx) * multiplier0, multiplier0);
    y = Rational(Number(fy) * multiplier1, multiplier1);
    z = x + y;
    fz0 = static_cast<double>(z);
    UTAssert(fz == fz0, "Failed to add numbers.");

    // signs +, -, n0 + n1 = 0
    fx = 1.2345;
    fy = -1.2345;
    fz = fx + fy;
    x = Rational(Number(fx) * multiplier0, multiplier0);
    y = Rational(Number(fy) * multiplier1, multiplier1);
    z = x + y;
    fz0 = static_cast<double>(z);
    UTAssert(fz == fz0, "Failed to add numbers.");

    // signs -, +, n0 + n1 = |n1| - |n0| > 0
    fx = -1.2345;
    fy = 2.3456;
    fz = fx + fy;
    x = Rational(Number(fx) * multiplier0, multiplier0);
    y = Rational(Number(fy) * multiplier1, multiplier1);
    z = x + y;
    fz0 = static_cast<double>(z);
    UTAssert(fz == fz0, "Failed to add numbers.");

    // signs -, +, n0 + n1 = -(|n0| - |n1|) < 0
    fx = -2.3456;
    fy = 1.2345;
    fz = fx + fy;
    x = Rational(Number(fx) * multiplier0, multiplier0);
    y = Rational(Number(fy) * multiplier1, multiplier1);
    z = x + y;
    fz0 = static_cast<double>(z);
    UTAssert(fz == fz0, "Failed to add numbers.");

    // signs -, +, n0 + n1 = 0
    fx = -1.2345;
    fy = 1.2345;
    fz = fx + fy;
    x = Rational(Number(fx) * multiplier0, multiplier0);
    y = Rational(Number(fy) * multiplier1, multiplier1);
    z = x + y;
    fz0 = static_cast<double>(z);
    UTAssert(fz == fz0, "Failed to add numbers.");

    // signs -, -
    fx = -1.2345;
    fy = -2.3456;
    fz = fx + fy;
    x = Rational(Number(fx) * multiplier0, multiplier0);
    y = Rational(Number(fy) * multiplier1, multiplier1);
    z = x + y;
    fz0 = static_cast<double>(z);
    UTAssert(fz == fz0, "Failed to add numbers.");
}

template <typename Number, typename Rational>
void UnitTestBSRational::Subtraction()
{
    Number const multiplier0(2.34567), multiplier1(-9.87654);
    Rational x, y, z, z0;

    std::uint32_t ux, uy;
    std::uint64_t uz;
    ux = 0xFFFFFFFFu;
    uy = 0x00FFFFFFu;
    uz = static_cast<std::uint64_t>(ux) - static_cast<std::uint64_t>(uy);
    x = Rational(Number(ux) * multiplier0, multiplier0);
    y = Rational(Number(uy) * multiplier1, multiplier1);
    z = x - y;
    z0 = Rational(uz);
    UTAssert(z == z0, "Failed to subtract numbers.");

    ux = 0xFFFFFFFFu;
    uy = 0xB0000000u;
    uz = static_cast<std::uint64_t>(ux) - static_cast<std::uint64_t>(uy);
    x = Rational(Number(ux) * multiplier0, multiplier0);
    y = Rational(Number(uy) * multiplier1, multiplier1);
    z = x - y;
    z0 = Rational(uz);
    UTAssert(z == z0, "Failed to subtract numbers.");

    ux = 0xB0000000u;
    uy = 0xFFFFFFFFu;
    std::int64_t iz = static_cast<std::int64_t>(ux) - static_cast<std::int64_t>(uy);
    x = Rational(Number(ux) * multiplier0, multiplier0);
    y = Rational(Number(uy) * multiplier1, multiplier1);
    z = x - y;
    z0 = Rational(iz);
    UTAssert(z == z0, "Failed to subtract numbers.");

    double fx, fy, fz, fz0;

    // signs 0, *
    fx = 0.0;
    fy = 1.2345;
    fz = fx - fy;
    x = Rational(Number(fx) * multiplier0, multiplier0);
    y = Rational(Number(fy) * multiplier1, multiplier1);
    z = x - y;
    fz0 = static_cast<double>(z);
    UTAssert(fz == fz0, "Failed to subtract numbers.");

    // signs *, 0
    fx = -1.2345;
    fy = 0.0;
    fz = fx - fy;
    x = Rational(Number(fx) * multiplier0, multiplier0);
    y = Rational(Number(fy) * multiplier1, multiplier1);
    z = x - y;
    fz0 = static_cast<double>(z);
    UTAssert(fz == fz0, "Failed to subtract numbers.");

    // signs +, +, n0 - n1 = -(|n1| - |n0|) < 0
    fx = 1.2345;
    fy = 2.3456;
    fz = fx - fy;
    x = Rational(Number(fx) * multiplier0, multiplier0);
    y = Rational(Number(fy) * multiplier1, multiplier1);
    z = x - y;
    fz0 = static_cast<double>(z);
    UTAssert(fz == fz0, "Failed to subtract numbers.");

    // signs +, +, |n0| - |n1| > 0
    fx = 2.3456;
    fy = 1.2345;
    fz = fx - fy;
    x = Rational(Number(fx) * multiplier0, multiplier0);
    y = Rational(Number(fy) * multiplier1, multiplier1);
    z = x - y;
    fz0 = static_cast<double>(z);
    UTAssert(fz == fz0, "Failed to subtract numbers.");

    // signs +, +, n0 - n1 = 0
    fx = 2.3456;
    fy = 2.3456;
    fz = fx - fy;
    x = Rational(Number(fx) * multiplier0, multiplier0);
    y = Rational(Number(fy) * multiplier1, multiplier1);
    z = x - y;
    fz0 = static_cast<double>(z);
    UTAssert(fz == fz0, "Failed to subtract numbers.");

    // signs -, -, n0 - n1 = (|n1| - |n0|) > 0
    fx = -1.2345;
    fy = -2.3456;
    fz = fx - fy;
    x = Rational(Number(fx) * multiplier0, multiplier0);
    y = Rational(Number(fy) * multiplier1, multiplier1);
    z = x - y;
    fz0 = static_cast<double>(z);
    UTAssert(fz == fz0, "Failed to subtract numbers.");

    // signs -, -, n0 - n1 = (|n1| - |n0|) > 0
    fx = -2.3456;
    fy = -1.2345;
    fz = fx - fy;
    x = Rational(Number(fx) * multiplier0, multiplier0);
    y = Rational(Number(fy) * multiplier1, multiplier1);
    z = x - y;
    fz0 = static_cast<double>(z);
    UTAssert(fz == fz0, "Failed to subtract numbers.");

    // signs -, -, n0 - n1 = 0
    fx = -1.2345;
    fy = -1.2345;
    fz = fx - fy;
    x = Rational(Number(fx) * multiplier0, multiplier0);
    y = Rational(Number(fy) * multiplier1, multiplier1);
    z = x - y;
    fz0 = static_cast<double>(z);
    UTAssert(fz == fz0, "Failed to subtract numbers.");

    // signs +, -
    fx = 2.3456;
    fy = -1.2345;
    fz = fx - fy;
    x = Rational(Number(fx) * multiplier0, multiplier0);
    y = Rational(Number(fy) * multiplier1, multiplier1);
    z = x - y;
    fz0 = static_cast<double>(z);
    UTAssert(fz == fz0, "Failed to subtract numbers.");

    // signs -, +
    fx = -2.3456;
    fy = 1.2345;
    fz = fx - fy;
    x = Rational(Number(fx) * multiplier0, multiplier0);
    y = Rational(Number(fy) * multiplier1, multiplier1);
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

    std::vector<std::array<Rational, 2>> bsn(4);
    for (std::size_t i = 0; i < 4; ++i)
    {
        bsn[i][0] = Rational(Number(v[i][0]) * multiplier0, multiplier0);
        bsn[i][1] = Rational(Number(v[i][1]) * multiplier1, multiplier1);
    }

    Rational s1x = bsn[2][0] + bsn[0][0];
    Rational d1x = bsn[2][0] - bsn[0][0];
    Rational s1y = bsn[2][1] + bsn[0][1];
    Rational d1y = bsn[2][1] - bsn[0][1];
    Rational s2x = bsn[3][0] + bsn[0][0];
    Rational d2x = bsn[3][0] - bsn[0][0];
    Rational s2y = bsn[3][1] + bsn[0][1];
    Rational d2y = bsn[3][1] - bsn[0][1];
    Rational z1 = s1x * d1x + s1y * d1y;
    Rational z2 = s2x * d2x + s2y * d2y;
    x = d1y * z2;
    y = d2y * z1;
    Rational c00 = x - y; // -2857689.0161162107
    double dC00 = c00;
    UTAssert(dC00 == -2857689.0161162107, "Invalid result for subtraction.");
}

template <typename Number, typename Rational>
void UnitTestBSRational::Multiplication()
{
    Number const multiplier0(2.34567), multiplier1(-9.87654);
    Rational x, y, z0, z1;
    Number z2{};
    std::uint64_t number0, number1;

    x = Rational(Number(0x87654321u) * multiplier0, multiplier0);
    y = Rational(Number(0x0000DCBAu) * multiplier1, multiplier1);
    z0 = x * y;
    number0 = 0x87654321u;
    number1 = 0x0000DCBAu;
    z1 = Rational(number0 * number1);
    UTAssert(z0 == z1, "Failed to multiply numbers.");

    x = Rational(Number(0xFFFFFFFFu) * multiplier0, multiplier0);
    y = Rational(Number(0xFFFFFFFFu) * multiplier1, multiplier1);
    z0 = x * y;
    number0 = 0xFFFFFFFFu;
    number1 = 0xFFFFFFFFu;
    z1 = Rational(number0 * number1);
    UTAssert(z0 == z1, "Failed to multiply numbers.");

    number0 = 0xFF00123400FFABC1ull;
    number1 = 0x01234567ABCDEF01ull;
    x = Rational(Number(number0) * multiplier0, multiplier0);
    y = Rational(Number(number1) * multiplier1, multiplier1);
    z0 = x * y;
    z0.Convert(128, APRoundingMode::TO_NEAREST, z2);

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
        v[0] == z2.GetUInteger().GetBits()[0] &&
        v[1] == z2.GetUInteger().GetBits()[1] &&
        v[2] == z2.GetUInteger().GetBits()[2] &&
        v[3] == z2.GetUInteger().GetBits()[3],
        "Failed to multiply numbers.");

    // Round-trip for the tests.
    double fx, fy, fz, fz0;
    fx = 1.2345;
    fy = 2.3456;
    fz = fx * fy;
    x = Rational(Number(fx) * multiplier0, multiplier0);
    y = Rational(Number(fy) * multiplier1, multiplier1);
    z0 = x * y;
    fz0 = static_cast<double>(z0);
    UTAssert(fz == fz0, "Failed to multiply numbers.");

    x = Rational(Number(0x3E9DC812u) * multiplier0, multiplier0);
    y = Rational(Number(1.0f) * multiplier1, multiplier1);
    z0 = x * y;
    UTAssert(z0 == x, "Failed to multiply numbers.");
}

template <typename Number, typename Rational>
void UnitTestBSRational::Conversions()
{
    IEEEBinary32 z;
    z.number = 100.0f / 7.0f;
    z.encoding &= 0xFFFFFFF0u;
    // z.number = 14.2857056
    // z.encoding = 0x41649240
    // z = 1.11001001001001001000000 * 2^3
    //   = 1110.01001001001001

    Rational input(100, 7);

    PositiveRoundNearest<Number, Rational>(z.number, input);
    PositiveRoundUpward<Number, Rational>(z.number, input);
    PositiveRoundDownward<Number, Rational>(z.number, input);
    PositiveRoundTowardZero<Number, Rational>(z.number, input);

    z.number = -z.number;
    input = -input;
    NegativeRoundNearest<Number, Rational>(z.number, input);
    NegativeRoundUpward<Number, Rational>(z.number, input);
    NegativeRoundDownward<Number, Rational>(z.number, input);
    NegativeRoundTowardZero<Number, Rational>(z.number, input);

    // These have a finite representation within the specified number
    // of bits, so no rounding occurs.
    IEEEBinary64 u, v;
    u.number = 1.3286488144750983054;  // u.encoding has final bits 0000
    Rational ru(u.number), rv{};
    ru.Convert(128, APRoundingMode::TO_NEAREST, rv);
    v.number = rv;
    UTAssert(v.encoding == u.encoding, "Incorrect conversion.");

    u.encoding |= 1;  // u.encoding has final bits 0001
    ru = Rational(u.number);
    ru.Convert(128, APRoundingMode::TO_NEAREST, rv);
    v.number = rv;
    UTAssert(v.encoding == u.encoding, "Incorrect conversion.");

    u.number = 1.3286488144750983054;
    u.encoding |= 2;  // u.encoding has final bits 0010
    ru = Rational(u.number);
    ru.Convert(128, APRoundingMode::TO_NEAREST, rv);
    v.number = rv;
    UTAssert(v.encoding == u.encoding, "Incorrect conversion.");
}

template <typename Number, typename Rational>
void UnitTestBSRational::PositiveRoundNearest(float z, Rational const& input)
{
    Number output{};
    std::array<IEEEBinary32, 19> values;
    values[0].number = z;
    for (std::size_t precision = 1; precision < 19; ++precision)
    {
        input.Convert(precision, APRoundingMode::TO_NEAREST, output);
        values[precision].number = (float)output;
    }

    // The r-term (001)^{infinity}, a pattern of 3 bits that repeats
    // ad infinitum.
    //
    // pre  binary                round nearest          number
    // ---+---------------------+----------------------+---------------
    //  1   1.11001001001001001r  u 1000.0               16.0
    //  2   11.1001001001001001r  u 1000.0               16.0
    //  3   111.001001001001001r  d 1110.0               14.0
    //  4   1110.01001001001001r  d 1110.0               14.0
    //  5   11100.1001001001001r  u 1110.1               14.5
    //  6   111001.001001001001r  d 1110.01              14.25
    //  7   1110010.01001001001r  d 1110.01              14.25
    //  8   11100100.1001001001r  u 1110.0101            14.3125
    //  9   111001001.001001001r  d 1110.01001           14.28125
    // 10   1110010010.01001001r  d 1110.01001           14.28125
    // 11   11100100100.1001001r  u 1110.0100101         14.2890625
    // 12   111001001001.001001r  d 1110.01001001        14.28515625
    // 13   1110010010010.01001r  d 1110.01001001        14.28515625
    // 14   11100100100100.1001r  u 1110.0100100101      14.2861328125
    // 15   111001001001001.001r  d 1110.01001001001     14.28564453125
    // 16   1110010010010010.01r  d 1110.01001001001     14.28564453125
    // 17   11100100100100100.1r  d 1110.0100100100101   14.2857666015625
    // 18   111001001001001001.r  d 1110.01001001001001  14.28570556640625
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
        14.2857666015625f,
        14.28570556640625f
    };

    for (std::size_t precision = 0; precision < 19; ++precision)
    {
        UTAssert(values[precision].number == truth[precision], "Incorrect result.");
    }
}

template <typename Number, typename Rational>
void UnitTestBSRational::PositiveRoundUpward(float z, Rational const& input)
{
    Number output{};
    std::array<IEEEBinary32, 19> values;
    values[0].number = z;
    for (std::size_t precision = 1; precision < 19; ++precision)
    {
        input.Convert(precision, APRoundingMode::UPWARD, output);
        values[precision].number = (float)output;
    }

    // The r-term (001)^{infinity}, a pattern of 3 bits that repeats
    // ad infinitum.
    //
    // pre  binary                round nearest          number
    // ---+----------------------+--------------------+-------------
    //  1   1.11001001001001001r  1000.0               16.0
    //  2   11.1001001001001001r  1000.0               16.0
    //  3   111.001001001001001r  1000.0               16.0
    //  4   1110.01001001001001r  1111.0               15.0
    //  5   11100.1001001001001r  1110.1               14.5
    //  6   111001.001001001001r  1110.1               14.5
    //  7   1110010.01001001001r  1110.011             14.375
    //  8   11100100.1001001001r  1110.0101            14.3125
    //  9   111001001.001001001r  1110.0101            14.3125
    // 10   1110010010.01001001r  1110.010011          14.296875
    // 11   11100100100.1001001r  1110.0100101         14.2890625
    // 12   111001001001.001001r  1110.0100101         14.2890625
    // 13   1110010010010.01001r  1110.010010011       14.287109375
    // 14   11100100100100.1001r  1110.0100100101      14.2861328125
    // 15   111001001001001.001r  1110.0100100101      14.2861328125
    // 16   1110010010010010.01r  1110.010010010011    14.285888671875
    // 17   11100100100100100.1r  1110.0100100100101   14.2857666015625
    // 18   111001001001001001.r  1110.0100100100101   14.2857666015625
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
        14.2857666015625f
    };

    for (std::size_t precision = 0; precision < 19; ++precision)
    {
        UTAssert(values[precision].number == truth[precision], "Incorrect result.");
    }
}

template <typename Number, typename Rational>
void UnitTestBSRational::PositiveRoundDownward(float z, Rational const& input)
{
    Number output{};
    std::array<IEEEBinary32, 19> values;
    values[0].number = z;
    for (std::size_t precision = 1; precision < 19; ++precision)
    {
        input.Convert(precision, APRoundingMode::DOWNWARD, output);
        values[precision].number = (float)output;
    }

    // The r-term (001)^{infinity}, a pattern of 3 bits that repeats
    // ad infinitum.
    //
    // pre  binary                round to +infinity     number
    // ---+---------------------+--------------------+-------------
    //  1   1.11001001001001001r  100.0                 8.0
    //  2   11.1001001001001001r  110.0                12.0
    //  3   111.001001001001001r  1110.0               14.0
    //  4   1110.01001001001001r  1110.0               14.0
    //  5   11100.1001001001001r  1110.0               14.0
    //  6   111001.001001001001r  1110.01              14.25
    //  7   1110010.01001001001r  1110.01              14.25
    //  8   11100100.1001001001r  1110.01              14.25
    //  9   111001001.001001001r  1110.1001            14.28125
    // 10   1110010010.01001001r  1110.1001            14.28125
    // 11   11100100100.1001001r  1110.1001            14.28125
    // 12   111001001001.001001r  1110.01001001        14.28515625
    // 13   1110010010010.01001r  1110.01001001        14.28515625
    // 14   11100100100100.1001r  1110.01001001        14.28515625
    // 15   111001001001001.001r  1110.01001001001     14.28564453125
    // 16   1110010010010010.01r  1110.01001001001     14.28564453125
    // 17   11100100100100100.1r  1110.01001001001     14.28564453125
    // 18   111001001001001001.r  1110.01001001001001  14.28570556640625
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

template <typename Number, typename Rational>
void UnitTestBSRational::PositiveRoundTowardZero(float z, Rational const& input)
{
    Number output{};
    std::array<IEEEBinary32, 19> values;
    values[0].number = z;
    for (std::size_t precision = 1; precision < 19; ++precision)
    {
        input.Convert(precision, APRoundingMode::TOWARD_ZERO, output);
        values[precision].number = (float)output;
    }

    // The r-term (001)^{infinity}, a pattern of 3 bits that repeats
    // ad infinitum.
    //
    // pre  binary                round to +infinity     number
    // ---+---------------------+--------------------+-------------
    //  1   1.11001001001001001r  100.0                 8.0
    //  2   11.1001001001001001r  110.0                12.0
    //  3   111.001001001001001r  1110.0               14.0
    //  4   1110.01001001001001r  1110.0               14.0
    //  5   11100.1001001001001r  1110.0               14.0
    //  6   111001.001001001001r  1110.01              14.25
    //  7   1110010.01001001001r  1110.01              14.25
    //  8   11100100.1001001001r  1110.01              14.25
    //  9   111001001.001001001r  1110.1001            14.28125
    // 10   1110010010.01001001r  1110.1001            14.28125
    // 11   11100100100.1001001r  1110.1001            14.28125
    // 12   111001001001.001001r  1110.01001001        14.28515625
    // 13   1110010010010.01001r  1110.01001001        14.28515625
    // 14   11100100100100.1001r  1110.01001001        14.28515625
    // 15   111001001001001.001r  1110.01001001001     14.28564453125
    // 16   1110010010010010.01r  1110.01001001001     14.28564453125
    // 17   11100100100100100.1r  1110.01001001001     14.28564453125
    // 18   111001001001001001.r  1110.01001001001001  14.28570556640625
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

template <typename Number, typename Rational>
void UnitTestBSRational::NegativeRoundNearest(float z, Rational const& input)
{
    Number output{};
    std::array<IEEEBinary32, 19> values;
    values[0].number = z;
    for (std::size_t precision = 1; precision < 19; ++precision)
    {
        input.Convert(precision, APRoundingMode::TO_NEAREST, output);
        values[precision].number = (float)output;
    }

    // The r-term (001)^{infinity}, a pattern of 3 bits that repeats
    // ad infinitum.
    //
    // pre  binary                round nearest          number
    // ---+---------------------+----------------------+---------------
    //  1   1.11001001001001001r  u 1000.0               -16.0
    //  2   11.1001001001001001r  u 1000.0               -16.0
    //  3   111.001001001001001r  d 1110.0               -14.0
    //  4   1110.01001001001001r  d 1110.0               -14.0
    //  5   11100.1001001001001r  u 1110.1               -14.5
    //  6   111001.001001001001r  d 1110.01              -14.25
    //  7   1110010.01001001001r  d 1110.01              -14.25
    //  8   11100100.1001001001r  u 1110.0101            -14.3125
    //  9   111001001.001001001r  d 1110.01001           -14.28125
    // 10   1110010010.01001001r  d 1110.01001           -14.28125
    // 11   11100100100.1001001r  u 1110.0100101         -14.2890625
    // 12   111001001001.001001r  d 1110.01001001        -14.28515625
    // 13   1110010010010.01001r  d 1110.01001001        -14.28515625
    // 14   11100100100100.1001r  u 1110.0100100101      -14.2861328125
    // 15   111001001001001.001r  d 1110.01001001001     -14.28564453125
    // 16   1110010010010010.01r  d 1110.01001001001     -14.28564453125
    // 17   11100100100100100.1r  d 1110.0100100100101   -14.2857666015625
    // 18   111001001001001001.r  d 1110.01001001001001  -14.28570556640625
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
        -14.2857666015625f,
        -14.28570556640625f
    };

    for (std::size_t precision = 0; precision < 19; ++precision)
    {
        UTAssert(values[precision].number == truth[precision], "Incorrect result.");
    }
}

template <typename Number, typename Rational>
void UnitTestBSRational::NegativeRoundUpward(float z, Rational const& input)
{
    Number output{};
    std::array<IEEEBinary32, 19> values;
    values[0].number = z;
    for (std::size_t precision = 1; precision < 19; ++precision)
    {
        input.Convert(precision, APRoundingMode::UPWARD, output);
        values[precision].number = (float)output;
    }

    // The r-term (001)^{infinity}, a pattern of 3 bits that repeats
    // ad infinitum.
    //
    // pre  binary                round to +infinity     number
    // ---+---------------------+--------------------+-------------
    //  1   1.11001001001001001r  100.0                 -8.0
    //  2   11.1001001001001001r  110.0                -12.0
    //  3   111.001001001001001r  1110.0               -14.0
    //  4   1110.01001001001001r  1110.0               -14.0
    //  5   11100.1001001001001r  1110.0               -14.0
    //  6   111001.001001001001r  1110.01              -14.25
    //  7   1110010.01001001001r  1110.01              -14.25
    //  8   11100100.1001001001r  1110.01              -14.25
    //  9   111001001.001001001r  1110.1001            -14.28125
    // 10   1110010010.01001001r  1110.1001            -14.28125
    // 11   11100100100.1001001r  1110.1001            -14.28125
    // 12   111001001001.001001r  1110.01001001        -14.28515625
    // 13   1110010010010.01001r  1110.01001001        -14.28515625
    // 14   11100100100100.1001r  1110.01001001        -14.28515625
    // 15   111001001001001.001r  1110.01001001001     -14.28564453125
    // 16   1110010010010010.01r  1110.01001001001     -14.28564453125
    // 17   11100100100100100.1r  1110.01001001001     -14.28564453125
    // 18   111001001001001001.r  1110.01001001001001  -14.28570556640625
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

template <typename Number, typename Rational>
void UnitTestBSRational::NegativeRoundDownward(float z, Rational const& input)
{
    Number output{};
    std::array<IEEEBinary32, 19> values;
    values[0].number = z;
    for (std::size_t precision = 1; precision < 19; ++precision)
    {
        input.Convert(precision, APRoundingMode::DOWNWARD, output);
        values[precision].number = (float)output;
    }

    // The r-term (001)^{infinity}, a pattern of 3 bits that repeats
    // ad infinitum.
    //
    // pre  binary                round nearest          number
    // ---+----------------------+--------------------+-------------
    //  1   1.11001001001001001r  1000.0               -16.0
    //  2   11.1001001001001001r  1000.0               -16.0
    //  3   111.001001001001001r  1000.0               -16.0
    //  4   1110.01001001001001r  1111.0               -15.0
    //  5   11100.1001001001001r  1110.1               -14.5
    //  6   111001.001001001001r  1110.1               -14.5
    //  7   1110010.01001001001r  1110.011             -14.375
    //  8   11100100.1001001001r  1110.0101            -14.3125
    //  9   111001001.001001001r  1110.0101            -14.3125
    // 10   1110010010.01001001r  1110.010011          -14.296875
    // 11   11100100100.1001001r  1110.0100101         -14.2890625
    // 12   111001001001.001001r  1110.0100101         -14.2890625
    // 13   1110010010010.01001r  1110.010010011       -14.287109375
    // 14   11100100100100.1001r  1110.0100100101      -14.2861328125
    // 15   111001001001001.001r  1110.0100100101      -14.2861328125
    // 16   1110010010010010.01r  1110.010010010011    -14.285888671875
    // 17   11100100100100100.1r  1110.0100100100101   -14.2857666015625
    // 18   111001001001001001.r  1110.0100100100101   -14.2857666015625
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
        -14.2857666015625f
    };

    for (std::size_t precision = 0; precision < 19; ++precision)
    {
        UTAssert(values[precision].number == truth[precision], "Incorrect result.");
    }
}

template <typename Number, typename Rational>
void UnitTestBSRational::NegativeRoundTowardZero(float z, Rational const& input)
{
    Number output{};
    std::array<IEEEBinary32, 19> values;
    values[0].number = z;
    for (std::size_t precision = 1; precision < 19; ++precision)
    {
        input.Convert(precision, APRoundingMode::TOWARD_ZERO, output);
        values[precision].number = (float)output;
    }

    // The r-term (001)^{infinity}, a pattern of 3 bits that repeats
    // ad infinitum.
    //
    // pre  binary                round to +infinity     number
    // ---+---------------------+--------------------+-------------
    //  1   1.11001001001001001r  100.0                 -8.0
    //  2   11.1001001001001001r  110.0                -12.0
    //  3   111.001001001001001r  1110.0               -14.0
    //  4   1110.01001001001001r  1110.0               -14.0
    //  5   11100.1001001001001r  1110.0               -14.0
    //  6   111001.001001001001r  1110.01              -14.25
    //  7   1110010.01001001001r  1110.01              -14.25
    //  8   11100100.1001001001r  1110.01              -14.25
    //  9   111001001.001001001r  1110.1001            -14.28125
    // 10   1110010010.01001001r  1110.1001            -14.28125
    // 11   11100100100.1001001r  1110.1001            -14.28125
    // 12   111001001001.001001r  1110.01001001        -14.28515625
    // 13   1110010010010.01001r  1110.01001001        -14.28515625
    // 14   11100100100100.1001r  1110.01001001        -14.28515625
    // 15   111001001001001.001r  1110.01001001001     -14.28564453125
    // 16   1110010010010010.01r  1110.01001001001     -14.28564453125
    // 17   11100100100100100.1r  1110.01001001001     -14.28564453125
    // 18   111001001001001001.r  1110.01001001001001  -14.28570556640625
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
#include <GTL/Mathematics/Arithmetic/BSRational.h>

namespace gtl
{
    template class BSRational<UIntegerAP32>;
    template class BSRational<UIntegerFP32<8>>;
}

namespace std
{
    template gtl::BSRational<gtl::UIntegerAP32> abs(gtl::BSRational<gtl::UIntegerAP32> const&);
    template gtl::BSRational<gtl::UIntegerFP32<8>> abs(gtl::BSRational<gtl::UIntegerFP32<8>> const&);

    template gtl::BSRational<gtl::UIntegerAP32> acos(gtl::BSRational<gtl::UIntegerAP32> const&);
    template gtl::BSRational<gtl::UIntegerFP32<8>> acos(gtl::BSRational<gtl::UIntegerFP32<8>> const&);

    template gtl::BSRational<gtl::UIntegerAP32> acosh(gtl::BSRational<gtl::UIntegerAP32> const&);
    template gtl::BSRational<gtl::UIntegerFP32<8>> acosh(gtl::BSRational<gtl::UIntegerFP32<8>> const&);

    template gtl::BSRational<gtl::UIntegerAP32> asin(gtl::BSRational<gtl::UIntegerAP32> const&);
    template gtl::BSRational<gtl::UIntegerFP32<8>> asin(gtl::BSRational<gtl::UIntegerFP32<8>> const&);

    template gtl::BSRational<gtl::UIntegerAP32> asinh(gtl::BSRational<gtl::UIntegerAP32> const&);
    template gtl::BSRational<gtl::UIntegerFP32<8>> asinh(gtl::BSRational<gtl::UIntegerFP32<8>> const&);

    template gtl::BSRational<gtl::UIntegerAP32> atan(gtl::BSRational<gtl::UIntegerAP32> const&);
    template gtl::BSRational<gtl::UIntegerFP32<8>> atan(gtl::BSRational<gtl::UIntegerFP32<8>> const&);

    template gtl::BSRational<gtl::UIntegerAP32> atanh(gtl::BSRational<gtl::UIntegerAP32> const&);
    template gtl::BSRational<gtl::UIntegerFP32<8>> atanh(gtl::BSRational<gtl::UIntegerFP32<8>> const&);

    template gtl::BSRational<gtl::UIntegerAP32> atan2(gtl::BSRational<gtl::UIntegerAP32> const&, gtl::BSRational<gtl::UIntegerAP32> const&);
    template gtl::BSRational<gtl::UIntegerFP32<8>> atan2(gtl::BSRational<gtl::UIntegerFP32<8>> const&, gtl::BSRational<gtl::UIntegerFP32<8>> const&);

    template gtl::BSRational<gtl::UIntegerAP32> ceil(gtl::BSRational<gtl::UIntegerAP32> const&);
    template gtl::BSRational<gtl::UIntegerFP32<8>> ceil(gtl::BSRational<gtl::UIntegerFP32<8>> const&);

    template gtl::BSRational<gtl::UIntegerAP32> cos(gtl::BSRational<gtl::UIntegerAP32> const&);
    template gtl::BSRational<gtl::UIntegerFP32<8>> cos(gtl::BSRational<gtl::UIntegerFP32<8>> const&);

    template gtl::BSRational<gtl::UIntegerAP32> cosh(gtl::BSRational<gtl::UIntegerAP32> const&);
    template gtl::BSRational<gtl::UIntegerFP32<8>> cosh(gtl::BSRational<gtl::UIntegerFP32<8>> const&);

    template gtl::BSRational<gtl::UIntegerAP32> exp(gtl::BSRational<gtl::UIntegerAP32> const&);
    template gtl::BSRational<gtl::UIntegerFP32<8>> exp(gtl::BSRational<gtl::UIntegerFP32<8>> const&);

    template gtl::BSRational<gtl::UIntegerAP32> exp2(gtl::BSRational<gtl::UIntegerAP32> const&);
    template gtl::BSRational<gtl::UIntegerFP32<8>> exp2(gtl::BSRational<gtl::UIntegerFP32<8>> const&);

    template gtl::BSRational<gtl::UIntegerAP32> fabs(gtl::BSRational<gtl::UIntegerAP32> const&);
    template gtl::BSRational<gtl::UIntegerFP32<8>> fabs(gtl::BSRational<gtl::UIntegerFP32<8>> const&);

    template gtl::BSRational<gtl::UIntegerAP32> floor(gtl::BSRational<gtl::UIntegerAP32> const&);
    template gtl::BSRational<gtl::UIntegerFP32<8>> floor(gtl::BSRational<gtl::UIntegerFP32<8>> const&);

    template gtl::BSRational<gtl::UIntegerAP32> fmod(gtl::BSRational<gtl::UIntegerAP32> const&, gtl::BSRational<gtl::UIntegerAP32> const&);
    template gtl::BSRational<gtl::UIntegerFP32<8>> fmod(gtl::BSRational<gtl::UIntegerFP32<8>> const&, gtl::BSRational<gtl::UIntegerFP32<8>> const&);

    template gtl::BSRational<gtl::UIntegerAP32> frexp(gtl::BSRational<gtl::UIntegerAP32> const&, std::int32_t*);
    template gtl::BSRational<gtl::UIntegerFP32<8>> frexp(gtl::BSRational<gtl::UIntegerFP32<8>> const&, std::int32_t*);

    template gtl::BSRational<gtl::UIntegerAP32> ldexp(gtl::BSRational<gtl::UIntegerAP32> const&, std::int32_t);
    template gtl::BSRational<gtl::UIntegerFP32<8>> ldexp(gtl::BSRational<gtl::UIntegerFP32<8>> const&, std::int32_t);

    template gtl::BSRational<gtl::UIntegerAP32> log(gtl::BSRational<gtl::UIntegerAP32> const&);
    template gtl::BSRational<gtl::UIntegerFP32<8>> log(gtl::BSRational<gtl::UIntegerFP32<8>> const&);

    template gtl::BSRational<gtl::UIntegerAP32> log2(gtl::BSRational<gtl::UIntegerAP32> const&);
    template gtl::BSRational<gtl::UIntegerFP32<8>> log2(gtl::BSRational<gtl::UIntegerFP32<8>> const&);

    template gtl::BSRational<gtl::UIntegerAP32> log10(gtl::BSRational<gtl::UIntegerAP32> const&);
    template gtl::BSRational<gtl::UIntegerFP32<8>> log10(gtl::BSRational<gtl::UIntegerFP32<8>> const&);

    template gtl::BSRational<gtl::UIntegerAP32> pow(gtl::BSRational<gtl::UIntegerAP32> const&, gtl::BSRational<gtl::UIntegerAP32> const&);
    template gtl::BSRational<gtl::UIntegerFP32<8>> pow(gtl::BSRational<gtl::UIntegerFP32<8>> const&, gtl::BSRational<gtl::UIntegerFP32<8>> const&);

    template gtl::BSRational<gtl::UIntegerAP32> remainder(gtl::BSRational<gtl::UIntegerAP32> const&, gtl::BSRational<gtl::UIntegerAP32> const&);
    template gtl::BSRational<gtl::UIntegerFP32<8>> remainder(gtl::BSRational<gtl::UIntegerFP32<8>> const&, gtl::BSRational<gtl::UIntegerFP32<8>> const&);

    template gtl::BSRational<gtl::UIntegerAP32> sin(gtl::BSRational<gtl::UIntegerAP32> const&);
    template gtl::BSRational<gtl::UIntegerFP32<8>> sin(gtl::BSRational<gtl::UIntegerFP32<8>> const&);

    template gtl::BSRational<gtl::UIntegerAP32> sinh(gtl::BSRational<gtl::UIntegerAP32> const&);
    template gtl::BSRational<gtl::UIntegerFP32<8>> sinh(gtl::BSRational<gtl::UIntegerFP32<8>> const&);

    template gtl::BSRational<gtl::UIntegerAP32> sqrt(gtl::BSRational<gtl::UIntegerAP32> const&);
    template gtl::BSRational<gtl::UIntegerFP32<8>> sqrt(gtl::BSRational<gtl::UIntegerFP32<8>> const&);

    template gtl::BSRational<gtl::UIntegerAP32> tan(gtl::BSRational<gtl::UIntegerAP32> const&);
    template gtl::BSRational<gtl::UIntegerFP32<8>> tan(gtl::BSRational<gtl::UIntegerFP32<8>> const&);

    template gtl::BSRational<gtl::UIntegerAP32> tanh(gtl::BSRational<gtl::UIntegerAP32> const&);
    template gtl::BSRational<gtl::UIntegerFP32<8>> tanh(gtl::BSRational<gtl::UIntegerFP32<8>> const&);
}

namespace gtl
{
    template BSRational<UIntegerAP32> atandivpi(BSRational<UIntegerAP32> const&);
    template BSRational<UIntegerFP32<8>> atandivpi(BSRational<UIntegerFP32<8>> const&);

    template BSRational<UIntegerAP32> atan2divpi(BSRational<UIntegerAP32> const&, BSRational<UIntegerAP32> const&);
    template BSRational<UIntegerFP32<8>> atan2divpi(BSRational<UIntegerFP32<8>> const&, BSRational<UIntegerFP32<8>> const&);

    template BSRational<UIntegerAP32> clamp(BSRational<UIntegerAP32> const&, BSRational<UIntegerAP32> const&, BSRational<UIntegerAP32> const&);
    template BSRational<UIntegerFP32<8>> clamp(BSRational<UIntegerFP32<8>> const&, BSRational<UIntegerFP32<8>> const&, BSRational<UIntegerFP32<8>> const&);

    template BSRational<UIntegerAP32> cospi(BSRational<UIntegerAP32> const&);
    template BSRational<UIntegerFP32<8>> cospi(BSRational<UIntegerFP32<8>> const&);

    template BSRational<UIntegerAP32> exp10(BSRational<UIntegerAP32> const&);
    template BSRational<UIntegerFP32<8>> exp10(BSRational<UIntegerFP32<8>> const&);

    template BSRational<UIntegerAP32> invsqrt(BSRational<UIntegerAP32> const&);
    template BSRational<UIntegerFP32<8>> invsqrt(BSRational<UIntegerFP32<8>> const&);

    template std::int32_t isign(BSRational<UIntegerAP32> const&);
    template std::int32_t isign(BSRational<UIntegerFP32<8>> const&);

    template BSRational<UIntegerAP32> saturate(BSRational<UIntegerAP32> const&);
    template BSRational<UIntegerFP32<8>> saturate(BSRational<UIntegerFP32<8>> const&);

    template BSRational<UIntegerAP32> sign(BSRational<UIntegerAP32> const&);
    template BSRational<UIntegerFP32<8>> sign(BSRational<UIntegerFP32<8>> const&);

    template BSRational<UIntegerAP32> sinpi(BSRational<UIntegerAP32> const&);
    template BSRational<UIntegerFP32<8>> sinpi(BSRational<UIntegerFP32<8>> const&);

    template BSRational<UIntegerAP32> sqr(BSRational<UIntegerAP32> const&);
    template BSRational<UIntegerFP32<8>> sqr(BSRational<UIntegerFP32<8>> const&);

    template BSRational<UIntegerAP32> FMA(
        BSRational<UIntegerAP32> const&,
        BSRational<UIntegerAP32> const&,
        BSRational<UIntegerAP32> const&);

    template BSRational<UIntegerFP32<8>> FMA(
        BSRational<UIntegerFP32<8>> const&,
        BSRational<UIntegerFP32<8>> const&,
        BSRational<UIntegerFP32<8>> const&);

    template BSRational<UIntegerAP32> RobustDOP(
        BSRational<UIntegerAP32> const&,
        BSRational<UIntegerAP32> const&,
        BSRational<UIntegerAP32> const&,
        BSRational<UIntegerAP32> const&);

    template BSRational<UIntegerFP32<8>> RobustDOP(
        BSRational<UIntegerFP32<8>> const&,
        BSRational<UIntegerFP32<8>> const&,
        BSRational<UIntegerFP32<8>> const&,
        BSRational<UIntegerFP32<8>> const&);
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(BSRational)
