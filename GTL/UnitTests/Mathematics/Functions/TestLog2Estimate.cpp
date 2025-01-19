// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Functions/Log2Estimate.h>
#include <random>
using namespace gtl;

namespace gtl
{
    class UnitTestLog2Estimate
    {
    public:
        UnitTestLog2Estimate();
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
                actual = std::log2(dx);
                estimate = Log2Estimate<double, Degree>(dx);
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
            UTAssert((maxError <= GetLog2EstimateMaxError<double, Degree>()), "Maximum error is too large.");
        }
    };
}

UnitTestLog2Estimate::UnitTestLog2Estimate()
{
    UTInformation("Mathematics/Functions/Log2Estimate");

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
    double actual = std::log2(x);
    double estimate = Log2EstimateRR<double, 8>(x);
    double absError = std::fabs(actual - estimate);
    UTAssert((absError <= GetLog2EstimateMaxError<double, 8>()), "Maximum error exceeded.");

    x = 100.1;
    actual = std::log2(x);
    estimate = Log2EstimateRR<double, 8>(x);
    absError = std::fabs(actual - estimate);
    UTAssert((absError <= GetLog2EstimateMaxError<double, 8>()), "Maximum error exceeded.");

    x = 0.01;
    actual = std::log2(x);
    estimate = Log2EstimateRR<double, 8>(x);
    absError = std::fabs(actual - estimate);
    UTAssert((absError <= GetLog2EstimateMaxError<double, 8>()), "Maximum error exceeded.");

    x = 1e-16;
    actual = std::log2(x);
    estimate = Log2EstimateRR<double, 8>(x);
    absError = std::fabs(actual - estimate);
    UTAssert((absError <= GetLog2EstimateMaxError<double, 8>()), "Maximum error exceeded.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Functions/Log2Estimate.h>

namespace gtl
{
    template float Log2Estimate<float, 1>(float);
    template float Log2Estimate<float, 2>(float);
    template float Log2Estimate<float, 3>(float);
    template float Log2Estimate<float, 4>(float);
    template float Log2Estimate<float, 5>(float);
    template float Log2Estimate<float, 6>(float);
    template float Log2Estimate<float, 7>(float);
    template float Log2Estimate<float, 8>(float);

    template float Log2EstimateRR<float, 1>(float);
    template float Log2EstimateRR<float, 2>(float);
    template float Log2EstimateRR<float, 3>(float);
    template float Log2EstimateRR<float, 4>(float);
    template float Log2EstimateRR<float, 5>(float);
    template float Log2EstimateRR<float, 6>(float);
    template float Log2EstimateRR<float, 7>(float);
    template float Log2EstimateRR<float, 8>(float);

    template float GetLog2EstimateMaxError<float, 1>();
    template float GetLog2EstimateMaxError<float, 2>();
    template float GetLog2EstimateMaxError<float, 3>();
    template float GetLog2EstimateMaxError<float, 4>();
    template float GetLog2EstimateMaxError<float, 5>();
    template float GetLog2EstimateMaxError<float, 6>();
    template float GetLog2EstimateMaxError<float, 7>();
    template float GetLog2EstimateMaxError<float, 8>();

    template double Log2Estimate<double, 1>(double);
    template double Log2Estimate<double, 2>(double);
    template double Log2Estimate<double, 3>(double);
    template double Log2Estimate<double, 4>(double);
    template double Log2Estimate<double, 5>(double);
    template double Log2Estimate<double, 6>(double);
    template double Log2Estimate<double, 7>(double);
    template double Log2Estimate<double, 8>(double);

    template double Log2EstimateRR<double, 1>(double);
    template double Log2EstimateRR<double, 2>(double);
    template double Log2EstimateRR<double, 3>(double);
    template double Log2EstimateRR<double, 4>(double);
    template double Log2EstimateRR<double, 5>(double);
    template double Log2EstimateRR<double, 6>(double);
    template double Log2EstimateRR<double, 7>(double);
    template double Log2EstimateRR<double, 8>(double);

    template double GetLog2EstimateMaxError<double, 1>();
    template double GetLog2EstimateMaxError<double, 2>();
    template double GetLog2EstimateMaxError<double, 3>();
    template double GetLog2EstimateMaxError<double, 4>();
    template double GetLog2EstimateMaxError<double, 5>();
    template double GetLog2EstimateMaxError<double, 6>();
    template double GetLog2EstimateMaxError<double, 7>();
    template double GetLog2EstimateMaxError<double, 8>();

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;

    template Rational Log2Estimate<Rational, 1>(Rational);
    template Rational Log2Estimate<Rational, 2>(Rational);
    template Rational Log2Estimate<Rational, 3>(Rational);
    template Rational Log2Estimate<Rational, 4>(Rational);
    template Rational Log2Estimate<Rational, 5>(Rational);
    template Rational Log2Estimate<Rational, 6>(Rational);
    template Rational Log2Estimate<Rational, 7>(Rational);
    template Rational Log2Estimate<Rational, 8>(Rational);

    template Rational Log2EstimateRR<Rational, 1>(Rational);
    template Rational Log2EstimateRR<Rational, 2>(Rational);
    template Rational Log2EstimateRR<Rational, 3>(Rational);
    template Rational Log2EstimateRR<Rational, 4>(Rational);
    template Rational Log2EstimateRR<Rational, 5>(Rational);
    template Rational Log2EstimateRR<Rational, 6>(Rational);
    template Rational Log2EstimateRR<Rational, 7>(Rational);
    template Rational Log2EstimateRR<Rational, 8>(Rational);

    template Rational GetLog2EstimateMaxError<Rational, 1>();
    template Rational GetLog2EstimateMaxError<Rational, 2>();
    template Rational GetLog2EstimateMaxError<Rational, 3>();
    template Rational GetLog2EstimateMaxError<Rational, 4>();
    template Rational GetLog2EstimateMaxError<Rational, 5>();
    template Rational GetLog2EstimateMaxError<Rational, 6>();
    template Rational GetLog2EstimateMaxError<Rational, 7>();
    template Rational GetLog2EstimateMaxError<Rational, 8>();
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(Log2Estimate)
