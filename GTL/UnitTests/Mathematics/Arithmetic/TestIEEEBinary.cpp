// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Arithmetic/IEEEBinary.h>
#include <cmath>
#include <random>
using namespace gtl;

namespace gtl
{
    template class IEEEBinary<float, std::uint32_t, 32, 24>;

    class UnitTestIEEEBinary
    {
    public:
        UnitTestIEEEBinary();

    private:
        void TestSignBiasedTrailing();
        void TestClassification();
        void TestNextUpNextDown();
    };
}

UnitTestIEEEBinary::UnitTestIEEEBinary()
{
    UTInformation("Mathematics/Arithmetic/IEEEBinary");

    TestSignBiasedTrailing();
    TestClassification();
    TestNextUpNextDown();
}

void UnitTestIEEEBinary::TestSignBiasedTrailing()
{
    IEEEBinary32 x(1.2345f), y(0x3f9e0419u);
    UTAssert(x.encoding == y.encoding, "Assignment failed.");

    std::uint32_t sign = x.GetSign();
    std::uint32_t biased = x.GetBiased();
    std::uint32_t trailing = x.GetTrailing();
    UTAssert(sign == 0, "GetSign failed.");
    UTAssert(biased == 0x0000007F, "GetBiased failed.");
    UTAssert(trailing == 0x001e0419, "GetTrailing failed.");
    y.SetEncoding(sign, biased, trailing);
    UTAssert(x.encoding == y.encoding, "SetEncoding failed.");
    y = IEEEBinary32(sign, biased, trailing);
    UTAssert(x.encoding == y.encoding, "Constructor failed.");
}

void UnitTestIEEEBinary::TestClassification()
{
    IEEEBinary32 x;

    x.number = 0.0f;
    UTAssert(
        x.GetClassification() == IEEEBinary32::Classification::POS_ZERO,
        "GetClassification failed.");
    UTAssert(x.IsZero(), "IsZero failed.");
    UTAssert(!x.IsSignMinus(), "IsSignMinus failed.");
    UTAssert(!x.IsSubnormal(), "IsSubnormal failed.");
    UTAssert(!x.IsNormal(), "IsNormal failed.");
    UTAssert(x.IsFinite(), "IsFinite failed.");
    UTAssert(!x.IsInfinite(), "IsInfinite failed.");
    UTAssert(!x.IsNaN(), "IsInfinite failed.");
    UTAssert(!x.IsQuietNaN(), "IsInfinite failed.");
    UTAssert(!x.IsSignalingNaN(), "IsInfinite failed.");

    x.number = -0.0f;
    UTAssert(
        x.GetClassification() == IEEEBinary32::Classification::NEG_ZERO,
        "GetClassification failed.");
    UTAssert(x.IsZero(), "IsZero failed.");
    UTAssert(x.IsSignMinus(), "IsSignMinus failed.");
    UTAssert(!x.IsSubnormal(), "IsSubnormal failed.");
    UTAssert(!x.IsNormal(), "IsNormal failed.");
    UTAssert(x.IsFinite(), "IsFinite failed.");
    UTAssert(!x.IsInfinite(), "IsInfinite failed.");
    UTAssert(!x.IsNaN(), "IsInfinite failed.");
    UTAssert(!x.IsQuietNaN(), "IsInfinite failed.");
    UTAssert(!x.IsSignalingNaN(), "IsInfinite failed.");

    x.encoding = 0x00001234u;
    UTAssert(
        x.GetClassification() == IEEEBinary32::Classification::POS_SUBNORMAL,
        "GetClassification failed.");
    UTAssert(!x.IsZero(), "IsZero failed.");
    UTAssert(!x.IsSignMinus(), "IsSignMinus failed.");
    UTAssert(x.IsSubnormal(), "IsSubnormal failed.");
    UTAssert(!x.IsNormal(), "IsNormal failed.");
    UTAssert(x.IsFinite(), "IsFinite failed.");
    UTAssert(!x.IsInfinite(), "IsInfinite failed.");
    UTAssert(!x.IsNaN(), "IsInfinite failed.");
    UTAssert(!x.IsQuietNaN(), "IsInfinite failed.");
    UTAssert(!x.IsSignalingNaN(), "IsInfinite failed.");

    x.encoding = 0x80001234u;
    UTAssert(
        x.GetClassification() == IEEEBinary32::Classification::NEG_SUBNORMAL,
        "GetClassification failed.");
    UTAssert(!x.IsZero(), "IsZero failed.");
    UTAssert(x.IsSignMinus(), "IsSignMinus failed.");
    UTAssert(x.IsSubnormal(), "IsSubnormal failed.");
    UTAssert(!x.IsNormal(), "IsNormal failed.");
    UTAssert(x.IsFinite(), "IsFinite failed.");
    UTAssert(!x.IsInfinite(), "IsInfinite failed.");
    UTAssert(!x.IsNaN(), "IsInfinite failed.");
    UTAssert(!x.IsQuietNaN(), "IsInfinite failed.");
    UTAssert(!x.IsSignalingNaN(), "IsInfinite failed.");

    x.number = 1.2345f;
    UTAssert(
        x.GetClassification() == IEEEBinary32::Classification::POS_NORMAL,
        "GetClassification failed.");
    UTAssert(!x.IsZero(), "IsZero failed.");
    UTAssert(!x.IsSignMinus(), "IsSignMinus failed.");
    UTAssert(!x.IsSubnormal(), "IsSubnormal failed.");
    UTAssert(x.IsNormal(), "IsNormal failed.");
    UTAssert(x.IsFinite(), "IsFinite failed.");
    UTAssert(!x.IsInfinite(), "IsInfinite failed.");
    UTAssert(!x.IsNaN(), "IsInfinite failed.");
    UTAssert(!x.IsQuietNaN(), "IsInfinite failed.");
    UTAssert(!x.IsSignalingNaN(), "IsInfinite failed.");

    x.number = -1.2345f;
    UTAssert(
        x.GetClassification() == IEEEBinary32::Classification::NEG_NORMAL,
        "GetClassification failed.");
    UTAssert(!x.IsZero(), "IsZero failed.");
    UTAssert(x.IsSignMinus(), "IsSignMinus failed.");
    UTAssert(!x.IsSubnormal(), "IsSubnormal failed.");
    UTAssert(x.IsNormal(), "IsNormal failed.");
    UTAssert(x.IsFinite(), "IsFinite failed.");
    UTAssert(!x.IsInfinite(), "IsInfinite failed.");
    UTAssert(!x.IsNaN(), "IsInfinite failed.");
    UTAssert(!x.IsQuietNaN(), "IsInfinite failed.");
    UTAssert(!x.IsSignalingNaN(), "IsInfinite failed.");

    x.number = std::numeric_limits<float>::infinity();
    UTAssert(
        x.GetClassification() == IEEEBinary32::Classification::POS_INFINITY,
        "GetClassification failed.");
    UTAssert(!x.IsZero(), "IsZero failed.");
    UTAssert(!x.IsSignMinus(), "IsSignMinus failed.");
    UTAssert(!x.IsSubnormal(), "IsSubnormal failed.");
    UTAssert(!x.IsNormal(), "IsNormal failed.");
    UTAssert(!x.IsFinite(), "IsFinite failed.");
    UTAssert(x.IsInfinite(), "IsInfinite failed.");
    UTAssert(!x.IsNaN(), "IsInfinite failed.");
    UTAssert(!x.IsQuietNaN(), "IsInfinite failed.");
    UTAssert(!x.IsSignalingNaN(), "IsInfinite failed.");

    x.number = -std::numeric_limits<float>::infinity();
    UTAssert(
        x.GetClassification() == IEEEBinary32::Classification::NEG_INFINITY,
        "GetClassification failed.");
    UTAssert(!x.IsZero(), "IsZero failed.");
    UTAssert(x.IsSignMinus(), "IsSignMinus failed.");
    UTAssert(!x.IsSubnormal(), "IsSubnormal failed.");
    UTAssert(!x.IsNormal(), "IsNormal failed.");
    UTAssert(!x.IsFinite(), "IsFinite failed.");
    UTAssert(x.IsInfinite(), "IsInfinite failed.");
    UTAssert(!x.IsNaN(), "IsInfinite failed.");
    UTAssert(!x.IsQuietNaN(), "IsInfinite failed.");
    UTAssert(!x.IsSignalingNaN(), "IsInfinite failed.");

    x.SetEncoding(0, 255, 0x1234);
    UTAssert(
        x.GetClassification() == IEEEBinary32::Classification::SIGNALING_NAN,
        "GetClassification failed.");
    UTAssert(!x.IsZero(), "IsZero failed.");
    UTAssert(!x.IsSignMinus(), "IsSignMinus failed.");
    UTAssert(!x.IsSubnormal(), "IsSubnormal failed.");
    UTAssert(!x.IsNormal(), "IsNormal failed.");
    UTAssert(!x.IsFinite(), "IsFinite failed.");
    UTAssert(!x.IsInfinite(), "IsInfinite failed.");
    UTAssert(x.IsNaN(), "IsInfinite failed.");
    UTAssert(!x.IsQuietNaN(), "IsInfinite failed.");
    UTAssert(x.IsSignalingNaN(), "IsInfinite failed.");

    x.encoding |= IEEEBinary32::NAN_QUIET_MASK;
    UTAssert(
        x.GetClassification() == IEEEBinary32::Classification::QUIET_NAN,
        "GetClassification failed.");
    UTAssert(!x.IsZero(), "IsZero failed.");
    UTAssert(!x.IsSignMinus(), "IsSignMinus failed.");
    UTAssert(!x.IsSubnormal(), "IsSubnormal failed.");
    UTAssert(!x.IsNormal(), "IsNormal failed.");
    UTAssert(!x.IsFinite(), "IsFinite failed.");
    UTAssert(!x.IsInfinite(), "IsInfinite failed.");
    UTAssert(x.IsNaN(), "IsInfinite failed.");
    UTAssert(x.IsQuietNaN(), "IsInfinite failed.");
    UTAssert(!x.IsSignalingNaN(), "IsInfinite failed.");
}

void UnitTestIEEEBinary::TestNextUpNextDown()
{
    std::default_random_engine dre;
    std::uniform_int_distribution<std::uint32_t> posEncoding(0x00000000u, 0x7F7FFFFEu);
    std::uniform_int_distribution<std::uint32_t> negEncoding(0x80000000u, 0xFF7FFFFEu);

    IEEEBinary32 x, y;
    std::uint32_t encoding = 0;

    for (std::uint32_t k = 0; k < 1024; ++k)
    {
        encoding = posEncoding(dre);
        x.encoding = encoding;
        y.encoding = encoding;
        x.encoding = x.GetNextUp();
        y.number = std::nextafter(y.number, std::numeric_limits<float>::infinity());
        UTAssert(x.encoding == y.encoding,
            "GetNextUp failed: encoding = " + std::to_string(encoding));
    }

    for (std::uint32_t k = 0; k < 1024; ++k)
    {
        encoding = posEncoding(dre);
        x.encoding = encoding;
        y.encoding = encoding;
        x.encoding = x.GetNextDown();
        y.number = std::nextafter(y.number, -std::numeric_limits<float>::infinity());
        UTAssert(x.encoding == y.encoding,
            "GetNextDown failed: encoding = " + std::to_string(encoding));
    }

    for (std::uint32_t k = 0; k < 1024; ++k)
    {
        encoding = negEncoding(dre);
        x.encoding = encoding;
        y.encoding = encoding;
        x.encoding = x.GetNextUp();
        y.number = std::nextafter(y.number, std::numeric_limits<float>::infinity());
        UTAssert(x.encoding == y.encoding,
            "GetNextUp failed: encoding = " + std::to_string(encoding));
    }

    for (std::uint32_t k = 0; k < 1024; ++k)
    {
        encoding = negEncoding(dre);
        x.encoding = encoding;
        y.encoding = encoding;
        x.encoding = x.GetNextDown();
        y.number = std::nextafter(y.number, -std::numeric_limits<float>::infinity());
        UTAssert(x.encoding == y.encoding,
            "GetNextDown failed: encoding = " + std::to_string(encoding));
    }
}

#else

#include <GTL/Mathematics/Arithmetic/IEEEBinary.h>

namespace gtl
{
    template class IEEEBinary<float, std::uint32_t, 32, 24>;
    template class IEEEBinary<double, std::uint64_t, 64, 53>;
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IEEEBinary)
