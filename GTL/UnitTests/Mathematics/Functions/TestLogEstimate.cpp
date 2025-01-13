// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Functions/LogEstimate.h>
#include <random>
using namespace gtl;

namespace gtl
{
    class UnitTestLogEstimate
    {
    public:
        UnitTestLogEstimate();
    private:
        union Binary32
        {
            float number;
            std::uint32_t encoding;
        };

        template <std::size_t Degree>
        void TestDegree()
        {
            Binary32 one{ 1.0f }, two{ 2.0f }, x{ 0.0f };
            std::default_random_engine dre;
            std::uniform_int_distribution<std::uint32_t> urd(one.encoding, two.encoding);

            double maxError = 0.0, maxX = 1.0;
            double dx, actual, estimate, error;
            for (std::size_t k = 0; k < 65536ull; ++k)
            {
                x.encoding = urd(dre);
                dx = static_cast<double>(x.number);
                actual = std::log(dx);
                estimate = LogEstimate<double, Degree>(dx);
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
            UTAssert((maxError <= GetLogEstimateMaxError<double, Degree>()), "Maximum error is too large.");
        }
    };
}

UnitTestLogEstimate::UnitTestLogEstimate()
{
    UTInformation("Mathematics/Functions/LogEstimate");

    TestDegree<1>();
    // maxError = 8.6071332055934091e-2
    // maxX = 1.4426950216293335

    TestDegree<2>();
    // maxError = 7.6362868906658110e-3
    // maxX = 1.1824274063110352

    TestDegree<3>();
    // maxError = 8.7902902652947645e-4
    // maxX = 1.8689513206481934

    TestDegree<4>();
    // maxError = 1.1318551356387374e-4
    // maxX = 1.9178400039672852

    TestDegree<5>();
    // maxError = 1.5521274483454306e-5
    // maxX = 1.7416102886199951

    TestDegree<6>();
    // maxError = 2.2162052037977276e-6
    // maxX = 1.8077982664108276

    TestDegree<7>();
    // maxError = 3.2546558681456617e-7
    // maxX = 1.8519464731216431

    TestDegree<8>();
    // maxError = 4.8798286744755615e-8
    // maxX = 1.9756996631622314

    double x = 2.1;
    double actual = std::log(x);
    double estimate = LogEstimateRR<double, 8>(x);
    double absError = std::fabs(actual - estimate);
    UTAssert((absError <= GetLogEstimateMaxError<double, 8>()), "Maximum error exceeded.");

    x = 100.1;
    actual = std::log(x);
    estimate = LogEstimateRR<double, 8>(x);
    absError = std::fabs(actual - estimate);
    UTAssert((absError <= GetLogEstimateMaxError<double, 8>()), "Maximum error exceeded.");

    x = 0.01;
    actual = std::log(x);
    estimate = LogEstimateRR<double, 8>(x);
    absError = std::fabs(actual - estimate);
    UTAssert((absError <= GetLogEstimateMaxError<double, 8>()), "Maximum error exceeded.");

    x = 1e-16;
    actual = std::log(x);
    estimate = LogEstimateRR<double, 8>(x);
    absError = std::fabs(actual - estimate);
    UTAssert((absError <= GetLogEstimateMaxError<double, 8>()), "Maximum error exceeded.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Functions/LogEstimate.h>

namespace gtl
{
    template float LogEstimate<float, 1>(float);
    template float LogEstimate<float, 2>(float);
    template float LogEstimate<float, 3>(float);
    template float LogEstimate<float, 4>(float);
    template float LogEstimate<float, 5>(float);
    template float LogEstimate<float, 6>(float);
    template float LogEstimate<float, 7>(float);
    template float LogEstimate<float, 8>(float);

    template float LogEstimateRR<float, 1>(float);
    template float LogEstimateRR<float, 2>(float);
    template float LogEstimateRR<float, 3>(float);
    template float LogEstimateRR<float, 4>(float);
    template float LogEstimateRR<float, 5>(float);
    template float LogEstimateRR<float, 6>(float);
    template float LogEstimateRR<float, 7>(float);
    template float LogEstimateRR<float, 8>(float);

    template float GetLogEstimateMaxError<float, 1>();
    template float GetLogEstimateMaxError<float, 2>();
    template float GetLogEstimateMaxError<float, 3>();
    template float GetLogEstimateMaxError<float, 4>();
    template float GetLogEstimateMaxError<float, 5>();
    template float GetLogEstimateMaxError<float, 6>();
    template float GetLogEstimateMaxError<float, 7>();
    template float GetLogEstimateMaxError<float, 8>();

    template double LogEstimate<double, 1>(double);
    template double LogEstimate<double, 2>(double);
    template double LogEstimate<double, 3>(double);
    template double LogEstimate<double, 4>(double);
    template double LogEstimate<double, 5>(double);
    template double LogEstimate<double, 6>(double);
    template double LogEstimate<double, 7>(double);
    template double LogEstimate<double, 8>(double);

    template double LogEstimateRR<double, 1>(double);
    template double LogEstimateRR<double, 2>(double);
    template double LogEstimateRR<double, 3>(double);
    template double LogEstimateRR<double, 4>(double);
    template double LogEstimateRR<double, 5>(double);
    template double LogEstimateRR<double, 6>(double);
    template double LogEstimateRR<double, 7>(double);
    template double LogEstimateRR<double, 8>(double);

    template double GetLogEstimateMaxError<double, 1>();
    template double GetLogEstimateMaxError<double, 2>();
    template double GetLogEstimateMaxError<double, 3>();
    template double GetLogEstimateMaxError<double, 4>();
    template double GetLogEstimateMaxError<double, 5>();
    template double GetLogEstimateMaxError<double, 6>();
    template double GetLogEstimateMaxError<double, 7>();
    template double GetLogEstimateMaxError<double, 8>();

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;

    template Rational LogEstimate<Rational, 1>(Rational);
    template Rational LogEstimate<Rational, 2>(Rational);
    template Rational LogEstimate<Rational, 3>(Rational);
    template Rational LogEstimate<Rational, 4>(Rational);
    template Rational LogEstimate<Rational, 5>(Rational);
    template Rational LogEstimate<Rational, 6>(Rational);
    template Rational LogEstimate<Rational, 7>(Rational);
    template Rational LogEstimate<Rational, 8>(Rational);

    template Rational LogEstimateRR<Rational, 1>(Rational);
    template Rational LogEstimateRR<Rational, 2>(Rational);
    template Rational LogEstimateRR<Rational, 3>(Rational);
    template Rational LogEstimateRR<Rational, 4>(Rational);
    template Rational LogEstimateRR<Rational, 5>(Rational);
    template Rational LogEstimateRR<Rational, 6>(Rational);
    template Rational LogEstimateRR<Rational, 7>(Rational);
    template Rational LogEstimateRR<Rational, 8>(Rational);

    template Rational GetLogEstimateMaxError<Rational, 1>();
    template Rational GetLogEstimateMaxError<Rational, 2>();
    template Rational GetLogEstimateMaxError<Rational, 3>();
    template Rational GetLogEstimateMaxError<Rational, 4>();
    template Rational GetLogEstimateMaxError<Rational, 5>();
    template Rational GetLogEstimateMaxError<Rational, 6>();
    template Rational GetLogEstimateMaxError<Rational, 7>();
    template Rational GetLogEstimateMaxError<Rational, 8>();
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(LogEstimate)
