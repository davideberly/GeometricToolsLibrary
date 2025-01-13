// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Functions/SinEstimate.h>
#include <random>
using namespace gtl;

namespace gtl
{
    class UnitTestSinEstimate
    {
    public:
        UnitTestSinEstimate();
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
                actual = std::sin(dx);
                estimate = SinEstimate<double, Degree>(dx);
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
            UTAssert((maxError <= GetSinEstimateMaxError<double, Degree>()), "Maximum error is too large.");
        }
    };
}

UnitTestSinEstimate::UnitTestSinEstimate()
{
    UTInformation("Mathematics/Functions/SinEstimate");

    TestDegree<3>();
    // maxError = 1.3481903639145698e-2
    // maxX = 1.2108652591705322

    TestDegree<5>();
    // maxError = 1.4001209384650881e-4
    // maxX = 0.85721266269683838

    TestDegree<7>();
    // maxError = 1.0205878939739677e-06
    // maxX = 1.1807202100753784

    TestDegree<9>();
    // maxError = 5.2010783457845378e-09
    // maxX = 1.5194612741470337

    TestDegree<11>();
    // maxError = 1.9323431743600850e-11
    // maxX = 1.5347530841827393

    double x = C_PI<double> - 1.0;
    double actual = std::sin(x);
    double estimate = SinEstimateRR<double, 11>(x);
    double error = std::fabs(actual - estimate);
    UTAssert(((error <= GetSinEstimateMaxError<double, 11>())), "Maximum error exceeded.");

    x = -x;
    actual = std::sin(x);
    estimate = SinEstimateRR<double, 11>(x);
    error = std::fabs(actual - estimate);
    UTAssert(((error <= GetSinEstimateMaxError<double, 11>())), "Maximum error exceeded.");

    x = C_PI<double>;
    actual = std::sin(x);
    estimate = SinEstimateRR<double, 11>(x);
    error = std::fabs(actual - estimate);
    UTAssert(((error <= GetSinEstimateMaxError<double, 11>())), "Maximum error exceeded.");

    x = -x;
    actual = std::sin(x);
    estimate = SinEstimateRR<double, 11>(x);
    error = std::fabs(actual - estimate);
    UTAssert(((error <= GetSinEstimateMaxError<double, 11>())), "Maximum error exceeded.");

    x = C_PI<double> + 1.0;
    actual = std::sin(x);
    estimate = SinEstimateRR<double, 11>(x);
    error = std::fabs(actual - estimate);
    UTAssert(((error <= GetSinEstimateMaxError<double, 11>())), "Maximum error exceeded.");

    x = -x;
    actual = std::sin(x);
    estimate = SinEstimateRR<double, 11>(x);
    error = std::fabs(actual - estimate);
    UTAssert(((error <= GetSinEstimateMaxError<double, 11>())), "Maximum error exceeded.");

    x = C_TWO_PI<double> - 1.0;
    actual = std::sin(x);
    estimate = SinEstimateRR<double, 11>(x);
    error = std::fabs(actual - estimate);
    UTAssert(((error <= GetSinEstimateMaxError<double, 11>())), "Maximum error exceeded.");

    x = -x;
    actual = std::sin(x);
    estimate = SinEstimateRR<double, 11>(x);
    error = std::fabs(actual - estimate);
    UTAssert(((error <= GetSinEstimateMaxError<double, 11>())), "Maximum error exceeded.");

    x = C_TWO_PI<double>;
    actual = std::sin(x);
    estimate = SinEstimateRR<double, 11>(x);
    error = std::fabs(actual - estimate);
    UTAssert(((error <= GetSinEstimateMaxError<double, 11>())), "Maximum error exceeded.");

    x = -x;
    actual = std::sin(x);
    estimate = SinEstimateRR<double, 11>(x);
    error = std::fabs(actual - estimate);
    UTAssert(((error <= GetSinEstimateMaxError<double, 11>())), "Maximum error exceeded.");

    x = C_TWO_PI<double> + 1.0;
    actual = std::sin(x);
    estimate = SinEstimateRR<double, 11>(x);
    error = std::fabs(actual - estimate);
    UTAssert(((error <= GetSinEstimateMaxError<double, 11>())), "Maximum error exceeded.");

    x = -x;
    actual = std::sin(x);
    estimate = SinEstimateRR<double, 11>(x);
    error = std::fabs(actual - estimate);
    UTAssert(((error <= GetSinEstimateMaxError<double, 11>())), "Maximum error exceeded.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Functions/SinEstimate.h>

namespace gtl
{
    template float SinEstimate<float, 3>(float);
    template float SinEstimate<float, 5>(float);
    template float SinEstimate<float, 7>(float);
    template float SinEstimate<float, 9>(float);
    template float SinEstimate<float, 11>(float);

    template float SinEstimateRR<float, 3>(float);
    template float SinEstimateRR<float, 5>(float);
    template float SinEstimateRR<float, 7>(float);
    template float SinEstimateRR<float, 9>(float);
    template float SinEstimateRR<float, 11>(float);

    template float GetSinEstimateMaxError<float, 3>();
    template float GetSinEstimateMaxError<float, 5>();
    template float GetSinEstimateMaxError<float, 7>();
    template float GetSinEstimateMaxError<float, 9>();
    template float GetSinEstimateMaxError<float, 11>();

    template double SinEstimate<double, 3>(double);
    template double SinEstimate<double, 5>(double);
    template double SinEstimate<double, 7>(double);
    template double SinEstimate<double, 9>(double);
    template double SinEstimate<double, 11>(double);

    template double SinEstimateRR<double, 3>(double);
    template double SinEstimateRR<double, 5>(double);
    template double SinEstimateRR<double, 7>(double);
    template double SinEstimateRR<double, 9>(double);
    template double SinEstimateRR<double, 11>(double);

    template double GetSinEstimateMaxError<double, 3>();
    template double GetSinEstimateMaxError<double, 5>();
    template double GetSinEstimateMaxError<double, 7>();
    template double GetSinEstimateMaxError<double, 9>();
    template double GetSinEstimateMaxError<double, 11>();

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;

    template Rational SinEstimate<Rational, 3>(Rational);
    template Rational SinEstimate<Rational, 5>(Rational);
    template Rational SinEstimate<Rational, 7>(Rational);
    template Rational SinEstimate<Rational, 9>(Rational);
    template Rational SinEstimate<Rational, 11>(Rational);

    template Rational SinEstimateRR<Rational, 3>(Rational);
    template Rational SinEstimateRR<Rational, 5>(Rational);
    template Rational SinEstimateRR<Rational, 7>(Rational);
    template Rational SinEstimateRR<Rational, 9>(Rational);
    template Rational SinEstimateRR<Rational, 11>(Rational);

    template Rational GetSinEstimateMaxError<Rational, 3>();
    template Rational GetSinEstimateMaxError<Rational, 5>();
    template Rational GetSinEstimateMaxError<Rational, 7>();
    template Rational GetSinEstimateMaxError<Rational, 9>();
    template Rational GetSinEstimateMaxError<Rational, 11>();
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(SinEstimate)
