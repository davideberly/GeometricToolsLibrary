// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Functions/ACosEstimate.h>
#include <random>
using namespace gtl;

namespace gtl
{
    class UnitTestACosEstimate
    {
    public:
        UnitTestACosEstimate();
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
                actual = std::acos(dx);
                estimate = ACosEstimate<double, Degree>(dx);
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
            UTAssert((maxError <= GetACosEstimateMaxError<double, Degree>()), "Maximum error is too large.");
        }
    };
}

UnitTestACosEstimate::UnitTestACosEstimate()
{
    UTInformation("Mathematics/Functions/ACosEstimate");

    TestDegree<1>();
    // maxError = 9.0128265558584441663e-3
    // maxX = 0.36083114147186279297

    TestDegree<2>();
    // maxError = 8.185127586319840276e-4
    // maxX = 0.17198413610458374023

    TestDegree<3>();
    // maxError = 8.8200141836525247641e-5
    // maxX = 0.099832557141780853271

    TestDegree<4>();
    // maxError = 1.0563052499801273143e-5
    // maxX = 0.065296493470668792725

    TestDegree<5>();
    // maxError = 1.3535063234648703201e-6
    // maxX = 0.046084616333246231079

    TestDegree<6>();
    // maxError = 1.8169471727169650421e-7
    // maxX = 0.034284457564353942871

    TestDegree<7>();
    // maxError = 2.5231622347021698261e-8
    // maxX = 0.026513285934925079346

    TestDegree<8>();
    // maxError = 3.5952707477804324299e-9
    // maxX = 0.021116610616445541382
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Functions/ACosEstimate.h>

namespace gtl
{
    template float ACosEstimate<float, 1>(float);
    template float ACosEstimate<float, 2>(float);
    template float ACosEstimate<float, 3>(float);
    template float ACosEstimate<float, 4>(float);
    template float ACosEstimate<float, 5>(float);
    template float ACosEstimate<float, 6>(float);
    template float ACosEstimate<float, 7>(float);
    template float ACosEstimate<float, 8>(float);

    template float GetACosEstimateMaxError<float, 1>();
    template float GetACosEstimateMaxError<float, 2>();
    template float GetACosEstimateMaxError<float, 3>();
    template float GetACosEstimateMaxError<float, 4>();
    template float GetACosEstimateMaxError<float, 5>();
    template float GetACosEstimateMaxError<float, 6>();
    template float GetACosEstimateMaxError<float, 7>();
    template float GetACosEstimateMaxError<float, 8>();

    template double ACosEstimate<double, 1>(double);
    template double ACosEstimate<double, 2>(double);
    template double ACosEstimate<double, 3>(double);
    template double ACosEstimate<double, 4>(double);
    template double ACosEstimate<double, 5>(double);
    template double ACosEstimate<double, 6>(double);
    template double ACosEstimate<double, 7>(double);
    template double ACosEstimate<double, 8>(double);

    template double GetACosEstimateMaxError<double, 1>();
    template double GetACosEstimateMaxError<double, 2>();
    template double GetACosEstimateMaxError<double, 3>();
    template double GetACosEstimateMaxError<double, 4>();
    template double GetACosEstimateMaxError<double, 5>();
    template double GetACosEstimateMaxError<double, 6>();
    template double GetACosEstimateMaxError<double, 7>();
    template double GetACosEstimateMaxError<double, 8>();

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;

    template Rational ACosEstimate<Rational, 1>(Rational);
    template Rational ACosEstimate<Rational, 2>(Rational);
    template Rational ACosEstimate<Rational, 3>(Rational);
    template Rational ACosEstimate<Rational, 4>(Rational);
    template Rational ACosEstimate<Rational, 5>(Rational);
    template Rational ACosEstimate<Rational, 6>(Rational);
    template Rational ACosEstimate<Rational, 7>(Rational);
    template Rational ACosEstimate<Rational, 8>(Rational);

    template Rational GetACosEstimateMaxError<Rational, 1>();
    template Rational GetACosEstimateMaxError<Rational, 2>();
    template Rational GetACosEstimateMaxError<Rational, 3>();
    template Rational GetACosEstimateMaxError<Rational, 4>();
    template Rational GetACosEstimateMaxError<Rational, 5>();
    template Rational GetACosEstimateMaxError<Rational, 6>();
    template Rational GetACosEstimateMaxError<Rational, 7>();
    template Rational GetACosEstimateMaxError<Rational, 8>();
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ACosEstimate)
