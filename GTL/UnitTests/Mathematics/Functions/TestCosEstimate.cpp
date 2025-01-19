// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Functions/CosEstimate.h>
#include <random>
using namespace gtl;

namespace gtl
{
    class UnitTestCosEstimate
    {
    public:
        UnitTestCosEstimate();
    private:
        union Binary32
        {
            float number;
            std::uint32_t encoding;
        };

        template <std::size_t Degree>
        void TestDegree()
        {
            Binary32 zero{ 0.0f }, halfPi{ C_PI_DIV_2<float> }, x{ 0.0f };
            std::default_random_engine dre;
            std::uniform_int_distribution<std::uint32_t> urd(zero.encoding, halfPi.encoding);

            double maxError = 0.0, maxX = 0.0;
            double dx, actual, estimate, error;
            for (std::size_t k = 0; k < 65536ull; ++k)
            {
                x.encoding = urd(dre);
                dx = static_cast<double>(x.number);
                actual = std::cos(dx);
                estimate = CosEstimate<double, Degree>(dx);
                error = std::fabs(actual - estimate);
                if (error > maxError)
                {
                    maxError = error;
                    maxX = x.number;
                }
            }

            std::ostringstream ostream;
            ostream << "Degree = " << Degree << ": " << std::setprecision(20)
                << "maxError = " << maxError << ", maxX = " << maxX;
            UTInformation(ostream.str());
            UTAssert((maxError <= GetCosEstimateMaxError<double, Degree>()), "Maximum error is too large.");
        }
    };
}

UnitTestCosEstimate::UnitTestCosEstimate()
{
    UTInformation("Mathematics/Functions/CosEstimate");

    TestDegree<2>();
    // maxError = 5.6009595954127800e-2
    // maxX = 1.0988243818283081

    TestDegree<4>();
    // maxError = 9.1879932449726032e-4
    // maxX = 1.3927623033523560

    TestDegree<6>();
    // maxError = 9.2028470144445151e-6
    // maxX = 1.4751803874969482

    TestDegree<8>();
    // maxError = 5.9804535233742850e-8
    // maxX = 1.5107959508895874

    TestDegree<10>();
    // maxError = 2.7008567604625000e-10
    // maxX = 1.3679423332214355

    double x = C_PI<double> - 1.0;
    double actual = std::cos(x);
    double estimate = CosEstimateRR<double, 10>(x);
    double error = std::fabs(actual - estimate);
    UTAssert(((error <= GetCosEstimateMaxError<double, 10>())), "Maximum error exceeded.");

    x = -x;
    actual = std::cos(x);
    estimate = CosEstimateRR<double, 10>(x);
    error = std::fabs(actual - estimate);
    UTAssert(((error <= GetCosEstimateMaxError<double, 10>())), "Maximum error exceeded.");

    x = C_PI<double>;
    actual = std::cos(x);
    estimate = CosEstimateRR<double, 10>(x);
    error = std::fabs(actual - estimate);
    UTAssert(((error <= GetCosEstimateMaxError<double, 10>())), "Maximum error exceeded.");

    x = -x;
    actual = std::cos(x);
    estimate = CosEstimateRR<double, 10>(x);
    error = std::fabs(actual - estimate);
    UTAssert(((error <= GetCosEstimateMaxError<double, 10>())), "Maximum error exceeded.");

    x = C_PI<double> + 1.0;
    actual = std::cos(x);
    estimate = CosEstimateRR<double, 10>(x);
    error = std::fabs(actual - estimate);
    UTAssert(((error <= GetCosEstimateMaxError<double, 10>())), "Maximum error exceeded.");

    x = -x;
    actual = std::cos(x);
    estimate = CosEstimateRR<double, 10>(x);
    error = std::fabs(actual - estimate);
    UTAssert(((error <= GetCosEstimateMaxError<double, 10>())), "Maximum error exceeded.");

    x = C_TWO_PI<double> - 1.0;
    actual = std::cos(x);
    estimate = CosEstimateRR<double, 10>(x);
    error = std::fabs(actual - estimate);
    UTAssert(((error <= GetCosEstimateMaxError<double, 10>())), "Maximum error exceeded.");

    x = -x;
    actual = std::cos(x);
    estimate = CosEstimateRR<double, 10>(x);
    error = std::fabs(actual - estimate);
    UTAssert(((error <= GetCosEstimateMaxError<double, 10>())), "Maximum error exceeded.");

    x = C_TWO_PI<double>;
    actual = std::cos(x);
    estimate = CosEstimateRR<double, 10>(x);
    error = std::fabs(actual - estimate);
    UTAssert(((error <= GetCosEstimateMaxError<double, 10>())), "Maximum error exceeded.");

    x = -x;
    actual = std::cos(x);
    estimate = CosEstimateRR<double, 10>(x);
    error = std::fabs(actual - estimate);
    UTAssert(((error <= GetCosEstimateMaxError<double, 10>())), "Maximum error exceeded.");

    x = C_TWO_PI<double> + 1.0;
    actual = std::cos(x);
    estimate = CosEstimateRR<double, 10>(x);
    error = std::fabs(actual - estimate);
    UTAssert(((error <= GetCosEstimateMaxError<double, 10>())), "Maximum error exceeded.");

    x = -x;
    actual = std::cos(x);
    estimate = CosEstimateRR<double, 10>(x);
    error = std::fabs(actual - estimate);
    UTAssert(((error <= GetCosEstimateMaxError<double, 10>())), "Maximum error exceeded.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Functions/CosEstimate.h>

namespace gtl
{
    template float CosEstimate<float, 2>(float);
    template float CosEstimate<float, 4>(float);
    template float CosEstimate<float, 6>(float);
    template float CosEstimate<float, 8>(float);
    template float CosEstimate<float, 10>(float);

    template float CosEstimateRR<float, 2>(float);
    template float CosEstimateRR<float, 4>(float);
    template float CosEstimateRR<float, 6>(float);
    template float CosEstimateRR<float, 8>(float);
    template float CosEstimateRR<float, 10>(float);

    template float GetCosEstimateMaxError<float, 2>();
    template float GetCosEstimateMaxError<float, 4>();
    template float GetCosEstimateMaxError<float, 6>();
    template float GetCosEstimateMaxError<float, 8>();
    template float GetCosEstimateMaxError<float, 10>();

    template double CosEstimate<double, 2>(double);
    template double CosEstimate<double, 4>(double);
    template double CosEstimate<double, 6>(double);
    template double CosEstimate<double, 8>(double);
    template double CosEstimate<double, 10>(double);

    template double CosEstimateRR<double, 2>(double);
    template double CosEstimateRR<double, 4>(double);
    template double CosEstimateRR<double, 6>(double);
    template double CosEstimateRR<double, 8>(double);
    template double CosEstimateRR<double, 10>(double);

    template double GetCosEstimateMaxError<double, 2>();
    template double GetCosEstimateMaxError<double, 4>();
    template double GetCosEstimateMaxError<double, 6>();
    template double GetCosEstimateMaxError<double, 8>();
    template double GetCosEstimateMaxError<double, 10>();

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;

    template Rational CosEstimate<Rational, 2>(Rational);
    template Rational CosEstimate<Rational, 4>(Rational);
    template Rational CosEstimate<Rational, 6>(Rational);
    template Rational CosEstimate<Rational, 8>(Rational);
    template Rational CosEstimate<Rational, 10>(Rational);

    template Rational CosEstimateRR<Rational, 2>(Rational);
    template Rational CosEstimateRR<Rational, 4>(Rational);
    template Rational CosEstimateRR<Rational, 6>(Rational);
    template Rational CosEstimateRR<Rational, 8>(Rational);
    template Rational CosEstimateRR<Rational, 10>(Rational);

    template Rational GetCosEstimateMaxError<Rational, 2>();
    template Rational GetCosEstimateMaxError<Rational, 4>();
    template Rational GetCosEstimateMaxError<Rational, 6>();
    template Rational GetCosEstimateMaxError<Rational, 8>();
    template Rational GetCosEstimateMaxError<Rational, 10>();
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(CosEstimate)
