// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Arithmetic/Constants.h>
#include <cmath>
using namespace gtl;

namespace gtl
{
    class UnitTestConstants
    {
    public:
        UnitTestConstants();

    private:
        void Test();
    };
}

UnitTestConstants::UnitTestConstants()
{
    UTInformation("Mathematics/Arithmetic/Constants");

    Test();
}

void UnitTestConstants::Test()
{
    double const maxError = 1e-15;
    double const pi = 4.0 * std::atan(1.0);
    double error = 0.0;

    error = std::fabs(C_<double>(17) - 17.0);
    UTAssert(error <= maxError, "Error too large.");

    error = std::fabs(C_<double>(3, 17) - 3.0 / 17.0);
    UTAssert(error <= maxError, "Error too large.");

    error = std::fabs(C_PI<double> - pi);
    UTAssert(error <= maxError, "Error too large.");

    error = std::fabs(C_PI_DIV_2<double> - 0.5 * pi);
    UTAssert(error <= maxError, "Error too large.");

    error = std::fabs(C_PI_DIV_4<double> - 0.25 * pi);
    UTAssert(error <= maxError, "Error too large.");

    error = std::fabs(C_TWO_PI<double> - 2.0 * pi);
    UTAssert(error <= maxError, "Error too large.");

    error = std::fabs(C_INV_PI<double> - 1.0 / pi);
    UTAssert(error <= maxError, "Error too large.");

    error = std::fabs(C_INV_TWO_PI<double> - 1.0 / (2.0 * pi));
    UTAssert(error <= maxError, "Error too large.");

    error = std::fabs(C_INV_HALF_PI<double> - 1.0 / (0.5 * pi));
    UTAssert(error <= maxError, "Error too large.");

    error = std::fabs(C_DEG_TO_RAD<double> - pi / 180.0);
    UTAssert(error <= maxError, "Error too large.");

    error = std::fabs(C_RAD_TO_DEG<double> - 180.0 / pi);
    UTAssert(error <= maxError, "Error too large.");

    double const sqrt2 = std::sqrt(2.0);
    error = std::fabs(C_SQRT_2<double> - sqrt2);
    UTAssert(error <= maxError, "Error too large.");

    error = std::fabs(C_INV_SQRT_2<double> - 1.0 / sqrt2);
    UTAssert(error <= maxError, "Error too large.");

    double const sqrt3 = std::sqrt(3.0);
    error = std::fabs(C_SQRT_3<double> -sqrt3);
    UTAssert(error <= maxError, "Error too large.");

    error = std::fabs(C_INV_SQRT_3<double> -1.0 / sqrt3);
    UTAssert(error <= maxError, "Error too large.");

    double const ln2 = std::log(2.0);
    error = std::fabs(C_LN_2<double> - ln2);
    UTAssert(error <= maxError, "Error too large.");

    error = std::fabs(C_INV_LN_2<double> - 1.0 / ln2);
    UTAssert(error <= maxError, "Error too large.");

    double const ln10 = std::log(10.0);
    error = std::fabs(C_LN_10<double> - ln10);
    UTAssert(error <= maxError, "Error too large.");

    error = std::fabs(C_INV_LN_10<double> - 1.0 / ln10);
    UTAssert(error <= maxError, "Error too large.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Arithmetic/Constants.h>

namespace gtl
{
    template float const C_<float>(std::int32_t);
    template float const C_<float>(std::int32_t, std::int32_t);
    template double const C_<double>(std::int32_t);
    template double const C_<double>(std::int32_t, std::int32_t);

#if defined(GTL_INSTANTIATE_RATIONAL)
    using BSN = BSNumber<UIntegerAP32>;
    template BSN const C_<BSN>(std::int32_t);
    template BSN const C_<BSN>(std::int32_t, std::int32_t);

    using BSR = BSRational<UIntegerAP32>;
    template BSR const C_<BSR>(std::int32_t);
    template BSR const C_<BSR>(std::int32_t, std::int32_t);

    using BSRFP = BSRational<UIntegerFP32<8>>;
    template BSRFP const C_<BSRFP>(std::int32_t);
    template BSRFP const C_<BSRFP>(std::int32_t, std::int32_t);
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(Constants)
