// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Functions/ChebyshevRatio.h>
using namespace gtl;

namespace gtl
{
    class UnitTestChebyshevRatio
    {
    public:
        UnitTestChebyshevRatio();
    private:
        void Test();
    };
}

UnitTestChebyshevRatio::UnitTestChebyshevRatio()
{
    UTInformation("Mathematics/Functions/ChebyshevRatio");

    Test();
}

void UnitTestChebyshevRatio::Test()
{
    double t, angle, cosAngle;
    std::array<double, 2> ratio;

    t = 0.25;
    angle = 0.0;
    cosAngle = 1.0;
    ratio[0] = ChebyshevRatio(t, angle);
    UTAssert(ratio[0] == t, "Invalid ratio.");
    ratio[0] = ChebyshevRatioUsingCosAngle(t, cosAngle);
    UTAssert(ratio[0] == t, "Invalid ratio.");
    ratio = ChebyshevRatios(t, angle);
    UTAssert(ratio[0] == 1.0 - t && ratio[1] == t, "Invalid ratio.");
    ratio = ChebyshevRatiosUsingCosAngle(t, cosAngle);
    UTAssert(ratio[0] == 1.0 - t && ratio[1] == t, "Invalid ratio.");

    angle = 0.123;
    cosAngle = std::cos(angle);
    ratio[0] = ChebyshevRatio(t, angle);
    UTAssert(ratio[0] == 0.25059199333213694, "Invalid ratio.");
    ratio[0] = ChebyshevRatioUsingCosAngle(t, cosAngle);
    UTAssert(ratio[0] == 0.25059199333213694, "Invalid ratio.");
    ratio = ChebyshevRatios(t, angle);
    UTAssert(ratio[0] == 0.75082847712717604 && ratio[1] == 0.25059199333213694, "Invalid ratio.");
    ratio = ChebyshevRatiosUsingCosAngle(t, cosAngle);
    UTAssert(ratio[0] == 0.75082847712717604 && ratio[1] == 0.25059199333213694, "Invalid ratio.");

    angle = 2.0;
    cosAngle = std::cos(angle);
    ratio[0] = ChebyshevRatio(t, angle);
    UTAssert(ratio[0] == 0.52724831772356051, "Invalid ratio.");
    ratio[0] = ChebyshevRatioUsingCosAngle(t, cosAngle);
    UTAssert(ratio[0] == 0.52724831772356051, "Invalid ratio.");
    ratio = ChebyshevRatios(t, angle);
    UTAssert(ratio[0] == 1.0969952813858350 && ratio[1] == 0.52724831772356051, "Invalid ratio.");
    ratio = ChebyshevRatiosUsingCosAngle(t, cosAngle);
    UTAssert(ratio[0] == 1.0969952813858350 && ratio[1] == 0.52724831772356051, "Invalid ratio.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Functions/ChebyshevRatio.h>

namespace gtl
{
    template float ChebyshevRatio(float, float);
    template float ChebyshevRatioUsingCosAngle(float, float);
    template std::array<float, 2> ChebyshevRatios(float, float);
    template std::array<float, 2> ChebyshevRatiosUsingCosAngle(float, float);

    template double ChebyshevRatio(double, double);
    template double ChebyshevRatioUsingCosAngle(double, double);
    template std::array<double, 2> ChebyshevRatios(double, double);
    template std::array<double, 2> ChebyshevRatiosUsingCosAngle(double, double);

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;

    template Rational ChebyshevRatio(Rational, Rational);
    template Rational ChebyshevRatioUsingCosAngle(Rational, Rational);
    template std::array<Rational, 2> ChebyshevRatios(Rational, Rational);
    template std::array<Rational, 2> ChebyshevRatiosUsingCosAngle(Rational, Rational);
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ChebyshevRatio)
