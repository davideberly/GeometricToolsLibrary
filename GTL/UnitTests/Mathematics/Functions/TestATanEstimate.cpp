// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Functions/ATanEstimate.h>
#include <random>
using namespace gtl;

namespace gtl
{
    class UnitTestATanEstimate
    {
    public:
        UnitTestATanEstimate();
    private:
        union Binary32
        {
            float number;
            std::uint32_t encoding;
        };

        template <std::size_t Degree>
        void TestDegree()
        {
            Binary32 zero{ 0.0f }, one{ 1.0f }, x{ 0.0f };
            std::default_random_engine dre;
            std::uniform_int_distribution<std::uint32_t> urd(zero.encoding, one.encoding);

            double maxError = 0.0, maxX = 0.0;
            double dx, actual, estimate, error;
            for (std::size_t k = 0; k < 65536ull; ++k)
            {
                x.encoding = urd(dre);
                dx = static_cast<double>(x.number);
                actual = std::atan(dx);
                estimate = ATanEstimate<double, Degree>(dx);
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
            UTAssert((maxError <= GetATanEstimateMaxError<double, Degree>()), "Maximum error is too large.");
        }
    };
}

UnitTestATanEstimate::UnitTestATanEstimate()
{
    UTInformation("Mathematics/Functions/ATanEstimate");

    TestDegree<3>();
    // maxError = 1.5970326392614309e-2
    // maxX = 0.74381726980209351

    TestDegree<5>();
    // maxError = 1.3509832247372566e-3
    // maxX = 0.51733189821243286

    TestDegree<7>();
    // maxError = 1.5051227215523433e-4
    // maxX = 0.73211741447448730

    TestDegree<9>();
    // maxError = 1.8921598624710434e-5
    // maxX = 0.61348402500152588

    TestDegree<11>();
    // maxError = 2.5477725020817132e-6
    // maxX = 0.87865966558456421

    TestDegree<13>();
    // maxError = 3.5859106295443866e-7
    // maxX = 0.91021180152893066

    double x = 10.0;
    double actual = std::atan(x);
    double estimate = ATanEstimateRR<double, 13>(x);
    double error = std::fabs(actual - estimate);
    // error = 6.9886065379165530e-8
    UTAssert((error <= GetATanEstimateMaxError<double, 13>()), "Maximum error exceeded.");
    x = -x;
    actual = std::atan(x);
    estimate = ATanEstimateRR<double, 13>(x);
    error = std::fabs(actual - estimate);
    // error = 6.9886065379165530e-8
    UTAssert((error <= GetATanEstimateMaxError<double, 13>()), "Maximum error exceeded.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Functions/ATanEstimate.h>

namespace gtl
{
    template float ATanEstimate<float, 3>(float);
    template float ATanEstimate<float, 5>(float);
    template float ATanEstimate<float, 7>(float);
    template float ATanEstimate<float, 9>(float);
    template float ATanEstimate<float, 11>(float);
    template float ATanEstimate<float, 13>(float);

    template float ATanEstimateRR<float, 3>(float);
    template float ATanEstimateRR<float, 5>(float);
    template float ATanEstimateRR<float, 7>(float);
    template float ATanEstimateRR<float, 9>(float);
    template float ATanEstimateRR<float, 11>(float);
    template float ATanEstimateRR<float, 13>(float);

    template float GetATanEstimateMaxError<float, 3>();
    template float GetATanEstimateMaxError<float, 5>();
    template float GetATanEstimateMaxError<float, 7>();
    template float GetATanEstimateMaxError<float, 9>();
    template float GetATanEstimateMaxError<float, 11>();
    template float GetATanEstimateMaxError<float, 13>();

    template double ATanEstimate<double, 3>(double);
    template double ATanEstimate<double, 5>(double);
    template double ATanEstimate<double, 7>(double);
    template double ATanEstimate<double, 9>(double);
    template double ATanEstimate<double, 11>(double);
    template double ATanEstimate<double, 13>(double);

    template double ATanEstimateRR<double, 3>(double);
    template double ATanEstimateRR<double, 5>(double);
    template double ATanEstimateRR<double, 7>(double);
    template double ATanEstimateRR<double, 9>(double);
    template double ATanEstimateRR<double, 11>(double);
    template double ATanEstimateRR<double, 13>(double);

    template double GetATanEstimateMaxError<double, 3>();
    template double GetATanEstimateMaxError<double, 5>();
    template double GetATanEstimateMaxError<double, 7>();
    template double GetATanEstimateMaxError<double, 9>();
    template double GetATanEstimateMaxError<double, 11>();
    template double GetATanEstimateMaxError<double, 13>();

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;

    template Rational ATanEstimate<Rational, 3>(Rational);
    template Rational ATanEstimate<Rational, 5>(Rational);
    template Rational ATanEstimate<Rational, 7>(Rational);
    template Rational ATanEstimate<Rational, 9>(Rational);
    template Rational ATanEstimate<Rational, 11>(Rational);
    template Rational ATanEstimate<Rational, 13>(Rational);

    template Rational ATanEstimateRR<Rational, 3>(Rational);
    template Rational ATanEstimateRR<Rational, 5>(Rational);
    template Rational ATanEstimateRR<Rational, 7>(Rational);
    template Rational ATanEstimateRR<Rational, 9>(Rational);
    template Rational ATanEstimateRR<Rational, 11>(Rational);
    template Rational ATanEstimateRR<Rational, 13>(Rational);

    template Rational GetATanEstimateMaxError<Rational, 3>();
    template Rational GetATanEstimateMaxError<Rational, 5>();
    template Rational GetATanEstimateMaxError<Rational, 7>();
    template Rational GetATanEstimateMaxError<Rational, 9>();
    template Rational GetATanEstimateMaxError<Rational, 11>();
    template Rational GetATanEstimateMaxError<Rational, 13>();
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ATanEstimate)
