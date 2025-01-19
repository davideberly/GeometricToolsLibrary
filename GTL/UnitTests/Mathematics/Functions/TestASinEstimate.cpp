// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Functions/ASinEstimate.h>
#include <random>
using namespace gtl;

namespace gtl
{
    class UnitTestASinEstimate
    {
    public:
        UnitTestASinEstimate();
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
                actual = std::asin(dx);
                estimate = ASinEstimate<double, Degree>(dx);
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
            UTAssert((maxError <= GetASinEstimateMaxError<double, Degree>()), "Maximum error is too large.");
        }
    };
}

UnitTestASinEstimate::UnitTestASinEstimate()
{
    UTInformation("Mathematics/Functions/ASinEstimate");

    TestDegree<1>();
    // maxError = 9.0128265558585551886e-3
    // maxX = 0.36083120107650756836

    TestDegree<2>();
    // maxError = 8.1851275863201178318e-4
    // maxX = 0.17198419570922851563

    TestDegree<3>();
    // maxError = 8.8200141836566881004e-5
    // maxX = 0.099832564592361450195

    TestDegree<4>();
    // maxError = 1.0563052499870662082e-5
    // maxX = 0.065296493470668792725

    TestDegree<5>();
    // maxError = 1.3535063235065036835e-6
    // maxX = 0.046085115522146224976

    TestDegree<6>();
    // maxError = 1.8169471743822995791e-7
    // maxX = 0.034285377711057662964

    TestDegree<7>();
    // maxError = 2.5231622315796675693e-8
    // maxX = 0.026513285934925079346

    TestDegree<8>();
    // maxError = 3.5952707963526897572e-9
    // maxX = 0.021119816228747367859
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Functions/ASinEstimate.h>

namespace gtl
{
    template float ASinEstimate<float, 1>(float);
    template float ASinEstimate<float, 2>(float);
    template float ASinEstimate<float, 3>(float);
    template float ASinEstimate<float, 4>(float);
    template float ASinEstimate<float, 5>(float);
    template float ASinEstimate<float, 6>(float);
    template float ASinEstimate<float, 7>(float);
    template float ASinEstimate<float, 8>(float);

    template float GetASinEstimateMaxError<float, 1>();
    template float GetASinEstimateMaxError<float, 2>();
    template float GetASinEstimateMaxError<float, 3>();
    template float GetASinEstimateMaxError<float, 4>();
    template float GetASinEstimateMaxError<float, 5>();
    template float GetASinEstimateMaxError<float, 6>();
    template float GetASinEstimateMaxError<float, 7>();
    template float GetASinEstimateMaxError<float, 8>();

    template double ASinEstimate<double, 1>(double);
    template double ASinEstimate<double, 2>(double);
    template double ASinEstimate<double, 3>(double);
    template double ASinEstimate<double, 4>(double);
    template double ASinEstimate<double, 5>(double);
    template double ASinEstimate<double, 6>(double);
    template double ASinEstimate<double, 7>(double);
    template double ASinEstimate<double, 8>(double);

    template double GetASinEstimateMaxError<double, 1>();
    template double GetASinEstimateMaxError<double, 2>();
    template double GetASinEstimateMaxError<double, 3>();
    template double GetASinEstimateMaxError<double, 4>();
    template double GetASinEstimateMaxError<double, 5>();
    template double GetASinEstimateMaxError<double, 6>();
    template double GetASinEstimateMaxError<double, 7>();
    template double GetASinEstimateMaxError<double, 8>();

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;

    template Rational ASinEstimate<Rational, 1>(Rational);
    template Rational ASinEstimate<Rational, 2>(Rational);
    template Rational ASinEstimate<Rational, 3>(Rational);
    template Rational ASinEstimate<Rational, 4>(Rational);
    template Rational ASinEstimate<Rational, 5>(Rational);
    template Rational ASinEstimate<Rational, 6>(Rational);
    template Rational ASinEstimate<Rational, 7>(Rational);
    template Rational ASinEstimate<Rational, 8>(Rational);

    template Rational GetASinEstimateMaxError<Rational, 1>();
    template Rational GetASinEstimateMaxError<Rational, 2>();
    template Rational GetASinEstimateMaxError<Rational, 3>();
    template Rational GetASinEstimateMaxError<Rational, 4>();
    template Rational GetASinEstimateMaxError<Rational, 5>();
    template Rational GetASinEstimateMaxError<Rational, 6>();
    template Rational GetASinEstimateMaxError<Rational, 7>();
    template Rational GetASinEstimateMaxError<Rational, 8>();
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ASinEstimate)
