// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Functions/TanEstimate.h>
#include <random>
using namespace gtl;

namespace gtl
{
    class UnitTestTanEstimate
    {
    public:
        UnitTestTanEstimate();

    private:
        union Binary32
        {
            float number;
            std::uint32_t encoding;
        };

        template <std::size_t Degree>
        void TestDegree()
        {
            Binary32 zero{ 0.0f }, quarterPi{ C_PI_DIV_4<float> }, x{ 0.0f };
            std::default_random_engine dre;
            std::uniform_int_distribution<std::uint32_t> urd(zero.encoding, quarterPi.encoding);

            double maxError = 0.0, maxX = 0.0;
            double dx, actual, estimate, error;
            for (std::size_t k = 0; k < 65536ull; ++k)
            {
                x.encoding = urd(dre);
                dx = static_cast<double>(x.number);
                actual = std::tan(dx);
                estimate = TanEstimate<double, Degree>(dx);
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
            UTAssert((maxError <= GetTanEstimateMaxError<double, Degree>()), "Maximum error is too large.");
        }
    };
}

UnitTestTanEstimate::UnitTestTanEstimate()
{
    UTInformation("Mathematics/Functions/TanEstimate");

    TestDegree<3>();
    // maxError = 1.1661892256204620e-2
    // maxX = 0.62224906682968140

    TestDegree<5>();
    // maxError = 5.8431854390145199e-4
    // maxX = 0.44228720664978027

    TestDegree<7>();
    // maxError = 3.5418688397792497e-5
    // maxX = 0.34249043464660645

    TestDegree<9>();
    // maxError = 2.2988173248306154e-6
    // maxX = 0.66454464197158813

    TestDegree<11>();
    // maxError = 1.5426258070938559e-7
    // maxX = 0.69993448257446289

    TestDegree<13>();
    // maxError = 1.0550265105990775e-8
    // maxX = 0.52341157197952271

    double x = C_PI_DIV_4<double> + 0.1;
    double actual = std::tan(x);
    double estimate = TanEstimateRR<double, 13>(x);
    double absError = std::fabs(actual - estimate);
    UTAssert(absError <= 8.0180440065192e-9, "Maximum error exceeded.");

    x = -x;
    actual = std::tan(x);
    estimate = TanEstimateRR<double, 13>(x);
    absError = std::fabs(actual - estimate);
    UTAssert(absError <= 8.0180440065192e-9, "Maximum error exceeded.");

    x = C_PI_DIV_2<double> - 0.01;
    actual = std::tan(x);
    estimate = TanEstimateRR<double, 13>(x);
    absError = std::fabs(actual - estimate);
    UTAssert(absError <= 5.1825666545824e-5, "Maximum error exceeded.");

    x = -x;
    actual = std::tan(x);
    estimate = TanEstimateRR<double, 13>(x);
    absError = std::fabs(actual - estimate);
    UTAssert(absError <= 5.1825666545824e-5, "Maximum error exceeded.");

    x = C_PI_DIV_2<double> + 0.01;
    actual = std::tan(x);
    estimate = TanEstimateRR<double, 13>(x);
    absError = std::fabs(actual - estimate);
    UTAssert(absError <= 5.1825666545824e-5, "Maximum error exceeded.");

    x = -x;
    actual = std::tan(x);
    estimate = TanEstimateRR<double, 13>(x);
    absError = std::fabs(actual - estimate);
    UTAssert(absError <= 5.1825666545824e-5, "Maximum error exceeded.");

    x = 100.0;
    actual = std::tan(x);
    estimate = TanEstimateRR<double, 13>(x);
    absError = std::fabs(actual - estimate);
    UTAssert(absError <= 1.0370054148723e-8, "Maximum error exceeded.");

    x = -x;
    actual = std::tan(x);
    estimate = TanEstimateRR<double, 13>(x);
    absError = std::fabs(actual - estimate);
    UTAssert(absError <= 1.0370054148723e-8, "Maximum error exceeded.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Functions/TanEstimate.h>

namespace gtl
{
    template float TanEstimate<float, 3>(float);
    template float TanEstimate<float, 5>(float);
    template float TanEstimate<float, 7>(float);
    template float TanEstimate<float, 9>(float);
    template float TanEstimate<float, 11>(float);
    template float TanEstimate<float, 13>(float);

    template float TanEstimateRR<float, 3>(float);
    template float TanEstimateRR<float, 5>(float);
    template float TanEstimateRR<float, 7>(float);
    template float TanEstimateRR<float, 9>(float);
    template float TanEstimateRR<float, 11>(float);
    template float TanEstimateRR<float, 13>(float);

    template float GetTanEstimateMaxError<float, 3>();
    template float GetTanEstimateMaxError<float, 5>();
    template float GetTanEstimateMaxError<float, 7>();
    template float GetTanEstimateMaxError<float, 9>();
    template float GetTanEstimateMaxError<float, 11>();
    template float GetTanEstimateMaxError<float, 13>();

    template double TanEstimate<double, 3>(double);
    template double TanEstimate<double, 5>(double);
    template double TanEstimate<double, 7>(double);
    template double TanEstimate<double, 9>(double);
    template double TanEstimate<double, 11>(double);
    template double TanEstimate<double, 13>(double);

    template double TanEstimateRR<double, 3>(double);
    template double TanEstimateRR<double, 5>(double);
    template double TanEstimateRR<double, 7>(double);
    template double TanEstimateRR<double, 9>(double);
    template double TanEstimateRR<double, 11>(double);
    template double TanEstimateRR<double, 13>(double);

    template double GetTanEstimateMaxError<double, 3>();
    template double GetTanEstimateMaxError<double, 5>();
    template double GetTanEstimateMaxError<double, 7>();
    template double GetTanEstimateMaxError<double, 9>();
    template double GetTanEstimateMaxError<double, 11>();
    template double GetTanEstimateMaxError<double, 13>();

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;

    template Rational TanEstimate<Rational, 3>(Rational);
    template Rational TanEstimate<Rational, 5>(Rational);
    template Rational TanEstimate<Rational, 7>(Rational);
    template Rational TanEstimate<Rational, 9>(Rational);
    template Rational TanEstimate<Rational, 11>(Rational);
    template Rational TanEstimate<Rational, 13>(Rational);

    template Rational TanEstimateRR<Rational, 3>(Rational);
    template Rational TanEstimateRR<Rational, 5>(Rational);
    template Rational TanEstimateRR<Rational, 7>(Rational);
    template Rational TanEstimateRR<Rational, 9>(Rational);
    template Rational TanEstimateRR<Rational, 11>(Rational);
    template Rational TanEstimateRR<Rational, 13>(Rational);

    template Rational GetTanEstimateMaxError<Rational, 3>();
    template Rational GetTanEstimateMaxError<Rational, 5>();
    template Rational GetTanEstimateMaxError<Rational, 7>();
    template Rational GetTanEstimateMaxError<Rational, 9>();
    template Rational GetTanEstimateMaxError<Rational, 11>();
    template Rational GetTanEstimateMaxError<Rational, 13>();
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(TanEstimate)
