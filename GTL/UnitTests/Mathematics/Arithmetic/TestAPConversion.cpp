// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Arithmetic/APConversion.h>
using namespace gtl;

namespace gtl
{
    class UnitTestAPConversion
    {
    public:
        UnitTestAPConversion();

    private:
        using BSR = BSRational<UIntegerAP32>;

        // This is a test for the Estimate function of Listing 5.7 in Section
        // 5.5.3 of RAEFGC. The implementation is
        //   APConversion<BSR>::Estimate(QFNumber<BSR, 1> const&, BSR&, BSR&);
        // and produces a rational bounding interval for the quadratic field
        // number. This function calls
        //   APConversion<BSR>::EstimateSqrt(BSR const&, BSR&, BSR&);
        // which produces a rational bounding interval for the sqrt(a^2)
        // where a^2 is known but a itself is not known.
        static BSR F(QFNumber<BSR, 1> const& z, BSR const& zEstimate)
        {
            return zEstimate * (zEstimate - BSR(2) * z.x[0]) +
                (z.x[0] * z.x[0] - z.x[1] * z.x[1] * z.d);
        }

        void EstimateSqrt();

        // This is a test for the Estimate function of Listing 5.8 in Section
        // 5.5.3 of RAEFGC. The implementation is
        //   APConversion<BSR>::EstimateApB(BSR const&, BSR const&, BSR&, BSR&);
        // where the precision input of the listing is an APConversion class
        // member.
        void EstimateApB();

        // This is a test for the Estimate function of Listing 5.9 in Section
        // 5.5.3 of RAEFGC. The implementation is
        //   APConversion<BSR>::EstimateAmB(BSR const&, BSR const&, BSR&, BSR&);
        // where the precision input of the listing is an APConversion class
        // member.
        void EstimateAmB();
    };
}

UnitTestAPConversion::UnitTestAPConversion()
{
    UTInformation("Mathematics/Arithmetic/APConversion");

    EstimateSqrt();
    EstimateApB();
    EstimateAmB();
}

void UnitTestAPConversion::EstimateSqrt()
{
    std::size_t constexpr precision = std::numeric_limits<double>::digits;
    std::size_t const maxIterations = 8;
    APConversion<BSR> apc(precision, maxIterations);

    QFNumber<BSR, 1> z;
    std::size_t numIterations;
    BSR zMin, zMax, fMin, fMax;
    double dZDbl, dZMin, dZMax, dFMin, dFMax;

    z.x[0] = 0.13547700429678050;
    z.x[1] = 0.83500858999457950;
    z.d = 0.96886777112423139;
    dZDbl = z.x[0] + z.x[1] * std::sqrt(z.d);
    UTAssert(dZDbl == 0.95738498605851619, "Unexpected result (floating-point issue)");
    numIterations = apc.Estimate(z, zMin, zMax);
    UTAssert(numIterations == 2, "Unexpected number of iterations.");
    UTAssert(
        zMin.GetNumerator().GetSign() == 1 &&
        zMin.GetNumerator().GetBiasedExponent() == -159 &&
        zMin.GetNumerator().GetUInteger().GetNumBits() == 160 &&
        zMin.GetNumerator().GetUInteger().GetBits()[0] == 0xef8a4ea5u &&
        zMin.GetNumerator().GetUInteger().GetBits()[1] == 0xaaf9a932u &&
        zMin.GetNumerator().GetUInteger().GetBits()[2] == 0x283147f2u &&
        zMin.GetNumerator().GetUInteger().GetBits()[3] == 0x2162ed0au &&
        zMin.GetNumerator().GetUInteger().GetBits()[4] == 0xc9711f59u,
        "Unexpected zMin numerator");
    UTAssert(
        zMin.GetDenominator().GetSign() == 1 &&
        zMin.GetDenominator().GetBiasedExponent() == -105 &&
        zMin.GetDenominator().GetUInteger().GetNumBits() == 106 &&
        zMin.GetDenominator().GetUInteger().GetBits()[0] == 0x3c796dd9u &&
        zMin.GetDenominator().GetUInteger().GetBits()[1] == 0xbb61cbcdu &&
        zMin.GetDenominator().GetUInteger().GetBits()[2] == 0xa23ef7f3u &&
        zMin.GetDenominator().GetUInteger().GetBits()[3] == 0x00000349u,
        "Unexpected zMin denominator");
    UTAssert(
        zMax.GetNumerator().GetSign() == 1 &&
        zMax.GetNumerator().GetBiasedExponent() == -106 &&
        zMax.GetNumerator().GetUInteger().GetNumBits() == 106 &&
        zMax.GetNumerator().GetUInteger().GetBits()[0] == 0x3c796dd9u &&
        zMax.GetNumerator().GetUInteger().GetBits()[1] == 0x0251cbcdu &&
        zMax.GetNumerator().GetUInteger().GetBits()[2] == 0x5cbad336u &&
        zMax.GetNumerator().GetUInteger().GetBits()[3] == 0x000003d4u,
        "Unexpected zMax numerator");
    UTAssert(
        zMax.GetDenominator().GetSign() == 1 &&
        zMax.GetDenominator().GetBiasedExponent() == 0 &&
        zMax.GetDenominator().GetUInteger().GetNumBits() == 1 &&
        zMax.GetDenominator().GetUInteger().GetBits()[0] == 0x00000001u,
        "Unexpected zMax denominator");
    dZMin = zMin;
    UTAssert(dZMin == 0.95738498605851630, "Unexpected dZMin");
    dZMax = zMax;
    UTAssert(dZMax == 0.95738498605851630, "Unexpected dZMax");
    fMin = F(z, zMin);
    UTAssert(
        fMin.GetNumerator().GetSign() == -1 &&
        fMin.GetNumerator().GetBiasedExponent() == -370 &&
        fMin.GetNumerator().GetUInteger().GetNumBits() == 265 &&
        fMin.GetNumerator().GetUInteger().GetBits()[0] == 0x686e5657u &&
        fMin.GetNumerator().GetUInteger().GetBits()[1] == 0xcb1bc450u &&
        fMin.GetNumerator().GetUInteger().GetBits()[2] == 0x5e1797f2u &&
        fMin.GetNumerator().GetUInteger().GetBits()[3] == 0x7752953fu &&
        fMin.GetNumerator().GetUInteger().GetBits()[4] == 0xdf3c6fb9u &&
        fMin.GetNumerator().GetUInteger().GetBits()[5] == 0xdb6b35fdu &&
        fMin.GetNumerator().GetUInteger().GetBits()[6] == 0x712b5155u &&
        fMin.GetNumerator().GetUInteger().GetBits()[7] == 0x3c157532u &&
        fMin.GetNumerator().GetUInteger().GetBits()[8] == 0x00000138u,
        "Unexpected fMin numerator");
    UTAssert(
        fMin.GetDenominator().GetSign() == 1 &&
        fMin.GetDenominator().GetBiasedExponent() == -211 &&
        fMin.GetDenominator().GetUInteger().GetNumBits() == 212 &&
        fMin.GetDenominator().GetUInteger().GetBits()[0] == 0xbe4481f1u &&
        fMin.GetDenominator().GetUInteger().GetBits()[1] == 0xc16b38a0u &&
        fMin.GetDenominator().GetUInteger().GetBits()[2] == 0xe3bba4e8u &&
        fMin.GetDenominator().GetUInteger().GetBits()[3] == 0xc9ddad06u &&
        fMin.GetDenominator().GetUInteger().GetBits()[4] == 0xabebcf69u &&
        fMin.GetDenominator().GetUInteger().GetBits()[5] == 0x688ce0b2u &&
        fMin.GetDenominator().GetUInteger().GetBits()[6] == 0x000acefbu,
        "Unexpected fMin denominator");
    fMax = F(z, zMax);
    UTAssert(
        fMax.GetNumerator().GetSign() == 1 &&
        fMax.GetNumerator().GetBiasedExponent() == -212 &&
        fMax.GetNumerator().GetUInteger().GetNumBits() == 106 &&
        fMax.GetNumerator().GetUInteger().GetBits()[0] == 0xbe4481f1u &&
        fMax.GetNumerator().GetUInteger().GetBits()[1] == 0x288b38a0u &&
        fMax.GetNumerator().GetUInteger().GetBits()[2] == 0x69088716u &&
        fMax.GetNumerator().GetUInteger().GetBits()[3] == 0x0000039cu,
        "Unexpected fMax numerator");
    UTAssert(
        fMax.GetDenominator().GetSign() == 1 &&
        fMax.GetDenominator().GetBiasedExponent() == 0 &&
        fMax.GetDenominator().GetUInteger().GetNumBits() == 1 &&
        fMax.GetDenominator().GetUInteger().GetBits()[0] == 0x00000001u,
        "Unexpected fMax denominator");
    dFMin = fMin;
    UTAssert(dFMin == -1.1127184071397530e-32, "Unexpected dFMin");
    dFMax = fMax;
    UTAssert(dFMax == +1.1127184071397530e-32, "Unexpected dFMin");

    z.x[0] = 0.13547700429678050;
    z.x[1] = -0.83500858999457950;
    z.d = 0.96886777112423139;
    dZDbl = z.x[0] + z.x[1] * std::sqrt(z.d);
    UTAssert(dZDbl == -0.68643097746495518, "Unexpected result (floating-point issue)");
    numIterations = apc.Estimate(z, zMin, zMax);
    UTAssert(numIterations == 2, "Unexpected number of iterations.");
    UTAssert(
        zMin.GetNumerator().GetSign() == -1 &&
        zMin.GetNumerator().GetBiasedExponent() == -106 &&
        zMin.GetNumerator().GetUInteger().GetNumBits() == 106 &&
        zMin.GetNumerator().GetUInteger().GetBits()[0] == 0x3c796dd9u &&
        zMin.GetNumerator().GetUInteger().GetBits()[1] == 0x7471cbcdu &&
        zMin.GetNumerator().GetUInteger().GetBits()[2] == 0xe7c31cb1u &&
        zMin.GetNumerator().GetUInteger().GetBits()[3] == 0x000002beu,
        "Unexpected zMin numerator");
    UTAssert(
        zMin.GetDenominator().GetSign() == 1 &&
        zMin.GetDenominator().GetBiasedExponent() == 0 &&
        zMin.GetDenominator().GetUInteger().GetNumBits() == 1 &&
        zMin.GetDenominator().GetUInteger().GetBits()[0] == 0x00000001u,
        "Unexpected zMin denominator");
    UTAssert(
        zMax.GetNumerator().GetSign() == -1 &&
        zMax.GetNumerator().GetBiasedExponent() == -159 &&
        zMax.GetNumerator().GetUInteger().GetNumBits() == 160 &&
        zMax.GetNumerator().GetUInteger().GetBits()[0] == 0x74300477u &&
        zMax.GetNumerator().GetUInteger().GetBits()[1] == 0x0a33a623u &&
        zMax.GetNumerator().GetUInteger().GetBits()[2] == 0x51bbf149u &&
        zMax.GetNumerator().GetUInteger().GetBits()[3] == 0x7ab36873u &&
        zMax.GetNumerator().GetUInteger().GetBits()[4] == 0x906e4db8u,
        "Unexpected zMax numerator");
    UTAssert(
        zMax.GetDenominator().GetSign() == 1 &&
        zMax.GetDenominator().GetBiasedExponent() == -105 &&
        zMax.GetDenominator().GetUInteger().GetNumBits() == 106 &&
        zMax.GetDenominator().GetUInteger().GetBits()[0] == 0x3c796dd9u &&
        zMax.GetDenominator().GetUInteger().GetBits()[1] == 0xbb61cbcdu &&
        zMax.GetDenominator().GetUInteger().GetBits()[2] == 0xa23ef7f3u &&
        zMax.GetDenominator().GetUInteger().GetBits()[3] == 0x00000349u,
        "Unexpected zMax denominator");
    dZMin = zMin;
    UTAssert(dZMin == -0.68643097746495529, "Unexpected dZMin");
    dZMax = zMax;
    UTAssert(dZMax == -0.68643097746495529, "Unexpected dZMax");
    fMin = F(z, zMin);
    UTAssert(
        fMin.GetNumerator().GetSign() == 1 &&
        fMin.GetNumerator().GetBiasedExponent() == -212 &&
        fMin.GetNumerator().GetUInteger().GetNumBits() == 106 &&
        fMin.GetNumerator().GetUInteger().GetBits()[0] == 0xbe4481f1u &&
        fMin.GetNumerator().GetUInteger().GetBits()[1] == 0x288b38a0u &&
        fMin.GetNumerator().GetUInteger().GetBits()[2] == 0x69088716u &&
        fMin.GetNumerator().GetUInteger().GetBits()[3] == 0x0000039cu,
        "Unexpected fMin numerator");
    UTAssert(
        fMin.GetDenominator().GetSign() == 1 &&
        fMin.GetDenominator().GetBiasedExponent() == 0 &&
        fMin.GetDenominator().GetUInteger().GetNumBits() == 1 &&
        fMin.GetDenominator().GetUInteger().GetBits()[0] == 0x00000001u,
        "Unexpected fMin denominator");
    fMax = F(z, zMax);
    UTAssert(
        fMax.GetNumerator().GetSign() == -1 &&
        fMax.GetNumerator().GetBiasedExponent() == -370 &&
        fMax.GetNumerator().GetUInteger().GetNumBits() == 265 &&
        fMax.GetNumerator().GetUInteger().GetBits()[0] == 0x686e5657u &&
        fMax.GetNumerator().GetUInteger().GetBits()[1] == 0xcb1bc450u &&
        fMax.GetNumerator().GetUInteger().GetBits()[2] == 0x5e1797f2u &&
        fMax.GetNumerator().GetUInteger().GetBits()[3] == 0x7752953fu &&
        fMax.GetNumerator().GetUInteger().GetBits()[4] == 0xdf3c6fb9u &&
        fMax.GetNumerator().GetUInteger().GetBits()[5] == 0xdb6b35fdu &&
        fMax.GetNumerator().GetUInteger().GetBits()[6] == 0x712b5155u &&
        fMax.GetNumerator().GetUInteger().GetBits()[7] == 0x3c157532u &&
        fMax.GetNumerator().GetUInteger().GetBits()[8] == 0x00000138u,
        "Unexpected fMax numerator");
    UTAssert(
        fMax.GetDenominator().GetSign() == 1 &&
        fMax.GetDenominator().GetBiasedExponent() == -211 &&
        fMax.GetDenominator().GetUInteger().GetNumBits() == 212 &&
        fMax.GetDenominator().GetUInteger().GetBits()[0] == 0xbe4481f1u &&
        fMax.GetDenominator().GetUInteger().GetBits()[1] == 0xc16b38a0u &&
        fMax.GetDenominator().GetUInteger().GetBits()[2] == 0xe3bba4e8u &&
        fMax.GetDenominator().GetUInteger().GetBits()[3] == 0xc9ddad06u &&
        fMax.GetDenominator().GetUInteger().GetBits()[4] == 0xabebcf69u &&
        fMax.GetDenominator().GetUInteger().GetBits()[5] == 0x688ce0b2u &&
        fMax.GetDenominator().GetUInteger().GetBits()[6] == 0x000acefbu,
        "Unexpected fMax denominator");
    dFMin = fMin;
    UTAssert(dFMin == +1.1127184071397530e-32, "Unexpected dFMin");
    dFMax = fMax;
    UTAssert(dFMax == -1.1127184071397530e-32, "Unexpected dFMax");

    z.x[0] = 0.13547700429678050;
    z.x[1] = 0.83500858999457950;
    z.d = 96886.777112423139;
    dZDbl = z.x[0] + z.x[1] * std::sqrt(z.d);
    UTAssert(dZDbl == 260.04560194821846, "Unexpected result (floating-point issue)");
    numIterations = apc.Estimate(z, zMin, zMax);
    UTAssert(numIterations == 2, "Unexpected number of iterations.");
    UTAssert(
        zMin.GetNumerator().GetSign() == 1 &&
        zMin.GetNumerator().GetBiasedExponent() == -159 &&
        zMin.GetNumerator().GetUInteger().GetNumBits() == 168 &&
        zMin.GetNumerator().GetUInteger().GetBits()[0] == 0xa291b0a7 &&
        zMin.GetNumerator().GetUInteger().GetBits()[1] == 0xbb56f5e2 &&
        zMin.GetNumerator().GetUInteger().GetBits()[2] == 0xb5f66168 &&
        zMin.GetNumerator().GetUInteger().GetBits()[3] == 0x50b1dc39 &&
        zMin.GetNumerator().GetUInteger().GetBits()[4] == 0x023e10fd &&
        zMin.GetNumerator().GetUInteger().GetBits()[5] == 0x00000084,
        "Unexpected zMin numerator");
    UTAssert(
        zMin.GetDenominator().GetSign() == 1 &&
        zMin.GetDenominator().GetBiasedExponent() == -105 &&
        zMin.GetDenominator().GetUInteger().GetNumBits() == 106 &&
        zMin.GetDenominator().GetUInteger().GetBits()[0] == 0xfdf1f949 &&
        zMin.GetDenominator().GetUInteger().GetBits()[1] == 0xd5b4fd77 &&
        zMin.GetDenominator().GetUInteger().GetBits()[2] == 0xd1fbe58a &&
        zMin.GetDenominator().GetUInteger().GetBits()[3] == 0x00000207,
        "Unexpected zMin denominator");
    UTAssert(
        zMax.GetNumerator().GetSign() == 1 &&
        zMax.GetNumerator().GetBiasedExponent() == -97 &&
        zMax.GetNumerator().GetUInteger().GetNumBits() == 106 &&
        zMax.GetNumerator().GetUInteger().GetBits()[0] == 0xfdf1f949 &&
        zMax.GetNumerator().GetUInteger().GetBits()[1] == 0x76d87577 &&
        zMax.GetNumerator().GetUInteger().GetBits()[2] == 0x17592378 &&
        zMax.GetNumerator().GetUInteger().GetBits()[3] == 0x00000208,
        "Unexpected zMax numerator");
    UTAssert(
        zMax.GetDenominator().GetSign() == 1 &&
        zMax.GetDenominator().GetBiasedExponent() == 0 &&
        zMax.GetDenominator().GetUInteger().GetNumBits() == 1 &&
        zMax.GetDenominator().GetUInteger().GetBits()[0] == 0x00000001u,
        "Unexpected zMax denominator");
    dZMin = zMin;
    UTAssert(dZMin == 260.04560194821846, "Unexpected dZMin");
    dZMax = zMax;
    UTAssert(dZMax == 260.04560194821846, "Unexpected dZMax");
    fMin = F(z, zMin);
    UTAssert(
        fMin.GetNumerator().GetSign() == -1 &&
        fMin.GetNumerator().GetBiasedExponent() == -352 &&
        fMin.GetNumerator().GetUInteger().GetNumBits() == 266 &&
        fMin.GetNumerator().GetUInteger().GetBits()[0] == 0x3428171b &&
        fMin.GetNumerator().GetUInteger().GetBits()[1] == 0x9d857153 &&
        fMin.GetNumerator().GetUInteger().GetBits()[2] == 0x6769d60b &&
        fMin.GetNumerator().GetUInteger().GetBits()[3] == 0x3e433e79 &&
        fMin.GetNumerator().GetUInteger().GetBits()[4] == 0x799e0e57 &&
        fMin.GetNumerator().GetUInteger().GetBits()[5] == 0x09f64b3a &&
        fMin.GetNumerator().GetUInteger().GetBits()[6] == 0x82921fbe &&
        fMin.GetNumerator().GetUInteger().GetBits()[7] == 0xfcdc4012 &&
        fMin.GetNumerator().GetUInteger().GetBits()[8] == 0x00000259,
        "Unexpected fMin numerator");
    UTAssert(
        fMin.GetDenominator().GetSign() == 1 &&
        fMin.GetDenominator().GetBiasedExponent() == -210 &&
        fMin.GetDenominator().GetUInteger().GetNumBits() == 211 &&
        fMin.GetDenominator().GetUInteger().GetBits()[0] == 0x983116d1 &&
        fMin.GetDenominator().GetUInteger().GetBits()[1] == 0x6240374f &&
        fMin.GetDenominator().GetUInteger().GetBits()[2] == 0xd04b85fc &&
        fMin.GetDenominator().GetUInteger().GetBits()[3] == 0x881276f5 &&
        fMin.GetDenominator().GetUInteger().GetBits()[4] == 0x20de00ef &&
        fMin.GetDenominator().GetUInteger().GetBits()[5] == 0x1799fd97 &&
        fMin.GetDenominator().GetUInteger().GetBits()[6] == 0x00041f85,
        "Unexpected fMin denominator");
    fMax = F(z, zMax);
    UTAssert(
        fMax.GetNumerator().GetSign() == 1 &&
        fMax.GetNumerator().GetBiasedExponent() == -194 &&
        fMax.GetNumerator().GetUInteger().GetNumBits() == 108 &&
        fMax.GetNumerator().GetUInteger().GetBits()[0] == 0x983116d1 &&
        fMax.GetNumerator().GetUInteger().GetBits()[1] == 0x3f90374f &&
        fMax.GetNumerator().GetUInteger().GetBits()[2] == 0x0b78c362 &&
        fMax.GetNumerator().GetUInteger().GetBits()[3] == 0x00000920,
        "Unexpected fMax numerator");
    UTAssert(
        fMax.GetDenominator().GetSign() == 1 &&
        fMax.GetDenominator().GetBiasedExponent() == 0 &&
        fMax.GetDenominator().GetUInteger().GetNumBits() == 1 &&
        fMax.GetDenominator().GetUInteger().GetBits()[0] == 0x00000001u,
        "Unexpected fMax denominator");
    dFMin = fMin;
    UTAssert(dFMin == -7.3712576983906573e-27, "Unexpected dFMin");
    dFMax = fMax;
    UTAssert(dFMax == 7.3712576983906573e-27, "Unexpected dFMax");

    z.x[0] = 0.13547700429678050;
    z.x[1] = -0.83500858999457950;
    z.d = 0.0000096886777112423139;
    dZDbl = z.x[0] + z.x[1] * std::sqrt(z.d);
    UTAssert(dZDbl == 0.13287790304734129, "Unexpected result (floating-point issue)");
    numIterations = apc.Estimate(z, zMin, zMax);
    UTAssert(numIterations == 1, "Unexpected number of iterations.");
    UTAssert(
        zMin.GetNumerator().GetSign() == 1 &&
        zMin.GetNumerator().GetBiasedExponent() == -59 &&
        zMin.GetNumerator().GetUInteger().GetNumBits() == 57 &&
        zMin.GetNumerator().GetUInteger().GetBits()[0] == 0x3f98598du &&
        zMin.GetNumerator().GetUInteger().GetBits()[1] == 0x0110224au,
        "Unexpected zMin numerator");
    UTAssert(
        zMin.GetDenominator().GetSign() == 1 &&
        zMin.GetDenominator().GetBiasedExponent() == 0 &&
        zMin.GetDenominator().GetUInteger().GetNumBits() == 1 &&
        zMin.GetDenominator().GetUInteger().GetBits()[0] == 0x00000001u,
        "Unexpected zMin denominator");
    UTAssert(
        zMax.GetNumerator().GetSign() == 1 &&
        zMax.GetNumerator().GetBiasedExponent() == -166 &&
        zMax.GetNumerator().GetUInteger().GetNumBits() == 164 &&
        zMax.GetNumerator().GetUInteger().GetBits()[0] == 0x00f146e1u &&
        zMax.GetNumerator().GetUInteger().GetBits()[1] == 0x2730c109u &&
        zMax.GetNumerator().GetUInteger().GetBits()[2] == 0xd24b0e1au &&
        zMax.GetNumerator().GetUInteger().GetBits()[3] == 0xd5f6b839u &&
        zMax.GetNumerator().GetUInteger().GetBits()[4] == 0x511da93au &&
        zMax.GetNumerator().GetUInteger().GetBits()[5] == 0x0000000bu,
        "Unexpected zMax numerator");
    UTAssert(
        zMax.GetDenominator().GetSign() == 1 &&
        zMax.GetDenominator().GetBiasedExponent() == -50 &&
        zMax.GetDenominator().GetUInteger().GetNumBits() == 51 &&
        zMax.GetDenominator().GetUInteger().GetBits()[0] == 0x76ec3453u &&
        zMax.GetDenominator().GetUInteger().GetBits()[1] == 0x000552adu,
        "Unexpected zMax denominator");
    dZMin = zMin;
    UTAssert(dZMin == 0.13287790304734129, "Unexpected dZMin");
    dZMax = zMax;
    UTAssert(dZMax == 0.13287790304734129, "Unexpected dZMax");
    fMin = F(z, zMin);
    UTAssert(
        fMin.GetNumerator().GetSign() == 1 &&
        fMin.GetNumerator().GetBiasedExponent() == -175 &&
        fMin.GetNumerator().GetUInteger().GetNumBits() == 107 &&
        fMin.GetNumerator().GetUInteger().GetBits()[0] == 0x00f146e1u &&
        fMin.GetNumerator().GetUInteger().GetBits()[1] == 0xb930c109u &&
        fMin.GetNumerator().GetUInteger().GetBits()[2] == 0xd211eac0u &&
        fMin.GetNumerator().GetUInteger().GetBits()[3] == 0x000004c3u,
        "Unexpected fMin numerator");
    UTAssert(
        fMin.GetDenominator().GetSign() == 1 &&
        fMin.GetDenominator().GetBiasedExponent() == 0 &&
        fMin.GetDenominator().GetUInteger().GetNumBits() == 1 &&
        fMin.GetDenominator().GetUInteger().GetBits()[0] == 0x00000001u,
        "Unexpected fMin denominator");
    fMax = F(z, zMax);
    UTAssert(
        fMax.GetNumerator().GetSign() == -1 &&
        fMax.GetNumerator().GetBiasedExponent() == -332 &&
        fMax.GetNumerator().GetUInteger().GetNumBits() == 265 &&
        fMax.GetNumerator().GetUInteger().GetBits()[0] == 0x78be2e3fu &&
        fMax.GetNumerator().GetUInteger().GetBits()[1] == 0xedc8dec7u &&
        fMax.GetNumerator().GetUInteger().GetBits()[2] == 0x78a36200u &&
        fMax.GetNumerator().GetUInteger().GetBits()[3] == 0xb48bd1f9u &&
        fMax.GetNumerator().GetUInteger().GetBits()[4] == 0x946efb45u &&
        fMax.GetNumerator().GetUInteger().GetBits()[5] == 0x49c1cd37u &&
        fMax.GetNumerator().GetUInteger().GetBits()[6] == 0x045547feu &&
        fMax.GetNumerator().GetUInteger().GetBits()[7] == 0x048a4143u &&
        fMax.GetNumerator().GetUInteger().GetBits()[8] == 0x0000010eu,
        "Unexpected fMax numerator");
    UTAssert(
        fMax.GetDenominator().GetSign() == 1 &&
        fMax.GetDenominator().GetBiasedExponent() == -100 &&
        fMax.GetDenominator().GetUInteger().GetNumBits() == 101 &&
        fMax.GetDenominator().GetUInteger().GetBits()[0] == 0x07b9d2e9u &&
        fMax.GetDenominator().GetUInteger().GetBits()[1] == 0x4f407e5bu &&
        fMax.GetDenominator().GetUInteger().GetBits()[2] == 0x557a3af3u &&
        fMax.GetDenominator().GetUInteger().GetBits()[3] == 0x0000001cu,
        "Unexpected fMax denominator");
    dFMin = fMin;
    UTAssert(dFMin == +2.0180238788854996e-21, "Unexpected dFMin");
    dFMax = fMax;
    UTAssert(dFMax == -2.0180238788854989e-21, "Unexpected dFMax");
}

void UnitTestAPConversion::EstimateApB()
{
    std::int32_t constexpr precision = std::numeric_limits<double>::digits;
    std::uint32_t const maxIterations = 8;
    APConversion<BSR> apc(precision, maxIterations);

    double dD1 = 5.0;
    double dD2 = 2.0;
    double dY = 2.0;
    double dZ = 3.0;
    double dA = dY * std::sqrt(dD1);
    UTAssert(dA == 4.4721359549995796, "Unexpected dA (floating-point issue)");
    double dB = dZ * std::sqrt(dD2);
    UTAssert(dB == 4.2426406871192857, "Unexpected dB (floating-point issue)");
    double inflection = std::sqrt((dA * dA + dB * dB) / 3.0);
    UTAssert(inflection == 3.5590260840104375, "Unexpected inflection (floating-point issue)");
    double secder = 8.0 * (dA * dA + 3.0 * dA * dB + dB * dB);
    UTAssert(secder == 759.36798306424669, "Unexpected secder (floating-point issue)");
    BSR d1 = dD1;
    BSR d2 = dD2;
    BSR y = dY;
    BSR z = dZ;
    BSR aSqr = y * y * d1;
    BSR bSqr = z * z * d2;
    BSR a2pb2 = aSqr + bSqr;
    BSR a2mb2 = aSqr - bSqr;
    double dActual = dA + dB;
    UTAssert(dActual == 8.7147766421188653, "Unexpected dActual (floating-point issue)");
    BSR t = dActual;
    BSR tSqr = t * t;
    BSR const two(2);
    BSR fActual = tSqr * (tSqr - two * a2pb2) + a2mb2 * a2mb2;
    double dFActual = fActual;
    UTAssert(dFActual == 9.6478890817133654e-13, "Unexpected dFActual (floating-point issue)");
    BSR tMin, tMax;
    std::size_t numIterations = apc.EstimateApB(aSqr, bSqr, tMin, tMax);
    UTAssert(numIterations == 2, "Unexpected number of iterations");
    double dTMin = tMin;
    UTAssert(dTMin == 8.7147766421188653, "Unexpected dTMin (floating-point issue)");
    double dTMax = tMax;
    UTAssert(dTMax == 8.7147766421188653, "Unexpected dTMax (floating-point issue)");
    t = dTMin;
    tSqr = t * t;
    fActual = tSqr * (tSqr - two * a2pb2) + a2mb2 * a2mb2;
    dFActual = fActual;
    UTAssert(dFActual == 9.6478890817133654e-13, "Unexpected dFActual (floating-point issue)");
    t = tMin;
    tSqr = t * t;
    BSR fMin = tSqr * (tSqr - two * a2pb2) + a2mb2 * a2mb2;
    double dFMin = fMin;
    UTAssert(dFMin == -3.2347920561969508e-27, "Unexpected dFMin (floating-point issue)");
    t = tMax;
    tSqr = t * t;
    BSR fMax = tSqr * (tSqr - two * a2pb2) + a2mb2 * a2mb2;
    double dFMax = fMax;
    UTAssert(dFMax == 1.0782640187323168e-27, "Unexpected dFMax (floating-point issue)");
}

void UnitTestAPConversion::EstimateAmB()
{
    std::size_t constexpr precision = std::numeric_limits<double>::digits;
    std::size_t const maxIterations = 8;
    APConversion<BSR> apc(precision, maxIterations);

    double dD1, dD2, dY, dZ, dA, dB, inflection, secder;
    BSR d1, d2, y, z, aSqr, bSqr, a2pb2, a2mb2;
    BSR const two(2);
    BSR tMin, tMax, fMin, fMax, fActual, t, tSqr;
    double dActual, dTMin, dTMax, dFMin, dFMax, dFActual;

    // second derivative is negative
    dD1 = 20.0;
    dD2 = 5.0;
    dY = 1.0;
    dZ = 1.0;
    dA = dY * std::sqrt(dD1);
    UTAssert(dA == 4.4721359549995796, "Unexpected dA (floating-point issue)");
    dB = dZ * std::sqrt(dD2);
    UTAssert(dB == 2.2360679774997898, "Unexpected dB (floating-point issue)");
    inflection = std::sqrt((dA * dA + dB * dB) / 3.0);
    UTAssert(inflection == 2.8867513459481291, "Unexpected inflection (floating-point issue)");
    secder = 8.0 * (dA * dA - 3.0 * dA * dB + dB * dB);
    UTAssert(secder == -39.999999999999993, "Unexpected secder (floating-point issue)");
    d1 = dD1;
    d2 = dD2;
    y = dY;
    z = dZ;
    aSqr = y * y * d1;
    bSqr = z * z * d2;
    a2pb2 = aSqr + bSqr;
    a2mb2 = aSqr - bSqr;
    dActual = dA - dB;
    UTAssert(dActual == 2.2360679774997898, "Unexpected dActual (floating-point issue)");
    t = dActual;
    tSqr = t * t;
    fActual = tSqr * (tSqr - two * a2pb2) + a2mb2 * a2mb2;
    dFActual = fActual;
    UTAssert(dFActual == -1.9434526171270719e-14, "Unexpected dFActual (floating-point issue)");
    apc.EstimateAmB(aSqr, bSqr, tMin, tMax); // 2
    dTMin = tMin;
    UTAssert(dTMin == 2.2360679774997898, "Unexpected dTMin (floating-point issue)");
    dTMax = tMax;
    UTAssert(dTMax == 2.2360679774997898, "Unexpected dTMax (floating-point issue)");
    t = dTMin;
    tSqr = t * t;
    fActual = tSqr * (tSqr - two * a2pb2) + a2mb2 * a2mb2;
    dFActual = fActual;
    UTAssert(dFActual == -1.9434526171270719e-14, "Unexpected dFActual (floating-point issue)");
    t = tMin;
    tSqr = t * t;
    fMin = tSqr * (tSqr - two * a2pb2) + a2mb2 * a2mb2;
    dFMin = fMin;
    UTAssert(dFMin == 1.3606187468005375e-28, "Unexpected dFMin (floating-point issue)");
    t = tMax;
    tSqr = t * t;
    fMax = tSqr * (tSqr - two * a2pb2) + a2mb2 * a2mb2;
    dFMax = fMax;
    UTAssert(dFMax == -4.5353958226684587e-29, "Unexpected dFMax (floating-point issue)");

    // second derivative is positive
    dD1 = 24.0;
    dD2 = 3.0;
    dY = 1.0;
    dZ = 1.0;
    dA = dY * std::sqrt(dD1);
    UTAssert(dA == 4.8989794855663558, "Unexpected dA (floating-point issue)");
    dB = dZ * std::sqrt(dD2);
    UTAssert(dB == 1.7320508075688772, "Unexpected dB (floating-point issue)");
    inflection = std::sqrt((dA * dA + dB * dB) / 3.0);
    UTAssert(inflection == 2.9999999999999996, "Unexpected inflection (floating-point issue)");
    secder = 8.0 * (dA * dA - 3.0 * dA * dB + dB * dB);
    UTAssert(secder == 12.353247018274313, "Unexpected secder (floating-point issue)");
    d1 = dD1;
    d2 = dD2;
    y = dY;
    z = dZ;
    aSqr = y * y * d1;
    bSqr = z * z * d2;
    a2pb2 = aSqr + bSqr;
    a2mb2 = aSqr - bSqr;
    dActual = dA - dB;
    UTAssert(dActual == 3.1669286779974786, "Unexpected dActual (floating-point issue)");
    t = dActual;
    tSqr = t * t;
    fActual = tSqr * (tSqr - two * a2pb2) + a2mb2 * a2mb2;
    dFActual = fActual;
    UTAssert(dFActual == 7.1667827483628122e-14, "Unexpected dFActual (floating-point issue)");
    apc.EstimateAmB(aSqr, bSqr, tMin, tMax);  // 2
    dTMin = tMin;
    UTAssert(dTMin == 3.1669286779974790, "Unexpected dTMin (floating-point issue)");
    dTMax = tMax;
    UTAssert(dTMax == 3.1669286779974790, "Unexpected dTMax (floating-point issue)");
    t = dTMin;
    tSqr = t * t;
    fActual = tSqr * (tSqr - two * a2pb2) + a2mb2 * a2mb2;
    dFActual = fActual;
    UTAssert(dFActual == -2.3801692539025683e-14, "Unexpected dFActual (floating-point issue)");
    t = tMin;
    tSqr = t * t;
    fMin = tSqr * (tSqr - two * a2pb2) + a2mb2 * a2mb2;
    dFMin = fMin;
    UTAssert(dFMin == 1.7113518038918942e-29, "Unexpected dFMin (floating-point issue)");
    t = tMax;
    tSqr = t * t;
    fMax = tSqr * (tSqr - two * a2pb2) + a2mb2 * a2mb2;
    dFMax = fMax;
    UTAssert(dFMax == -5.1340554116756819e-29, "Unexpected dFMax (floating-point issue)");
}

#else

#include <GTL/Mathematics/Arithmetic/APConversion.h>

namespace gtl
{
    template class APConversion<BSRational<UIntegerAP32>>;
    template class APConversion<BSRational<UIntegerFP32<8>>>;
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(APConversion)
