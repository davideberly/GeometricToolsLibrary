// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Functions/Exp2Estimate.h>
#include <random>
using namespace gtl;

namespace gtl
{
    class UnitTestExp2Estimate
    {
    public:
        UnitTestExp2Estimate();
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
                actual = std::exp2(dx);
                estimate = Exp2Estimate<double, Degree>(dx);
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
            UTAssert((maxError <= GetExp2EstimateMaxError<double, Degree>()), "Maximum error is too large.");
        }
    };
}

UnitTestExp2Estimate::UnitTestExp2Estimate()
{
    UTInformation("Mathematics/Functions/Exp2Estimate");

    TestDegree<1>();
    // maxError = 8.6071332055934091e-2
    // maxX = 0.52876639366149902

    TestDegree<2>();
    // maxError = 3.8132476831058693e-3
    // maxX = 0.22102764248847961

    TestDegree<3>();
    // maxError = 1.4694877755228042e-4
    // maxX = 0.50865358114242554

    TestDegree<4>();
    // maxError = 4.7617792662268954e-6
    // maxX = 0.92692458629608154

    TestDegree<5>();
    // maxError = 1.3162098788654930104e-7
    // maxX = 0.7618465423583984375

    TestDegree<6>();
    // maxError = 3.1590552396210114239e-9
    // maxX = 0.82159823179244995117

    TestDegree<7>();
    // maxError = 6.715739075957571913e-11
    // maxX = 0.86165434122085571289

    // The errors are magnified because of the multiplication by 2^p in
    // the Exp2EstimateRR code. Use a relative error bound.
    double x = 2.1;
    double actual = std::exp2(x);
    double estimate = Exp2EstimateRR<double, 7>(x);
    double relError = std::fabs(actual - estimate) / actual;
    UTAssert(relError <= 3.6023371737899e-11, "Maximum error exceeded.");

    x = -x;
    actual = std::exp2(x);
    estimate = Exp2EstimateRR<double, 7>(x);
    relError = std::fabs(actual - estimate) / actual;
    UTAssert(relError <= 2.2155e-11, "Maximum error exceeded.");

    x = 10.1;
    actual = std::exp2(x);
    estimate = Exp2EstimateRR<double, 7>(x);
    relError = std::fabs(actual - estimate) / actual;
    UTAssert(relError <= 3.6024e-11, "Maximum error exceeded.");

    x = -x;
    actual = std::exp2(x);
    estimate = Exp2EstimateRR<double, 7>(x);
    relError = std::fabs(actual - estimate) / actual;
    UTAssert(relError <= 2.2153580245993e-11, "Maximum error exceeded.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Functions/Exp2Estimate.h>

namespace gtl
{
    template float Exp2Estimate<float, 1>(float);
    template float Exp2Estimate<float, 2>(float);
    template float Exp2Estimate<float, 3>(float);
    template float Exp2Estimate<float, 4>(float);
    template float Exp2Estimate<float, 5>(float);
    template float Exp2Estimate<float, 6>(float);
    template float Exp2Estimate<float, 7>(float);

    template float Exp2EstimateRR<float, 1>(float);
    template float Exp2EstimateRR<float, 2>(float);
    template float Exp2EstimateRR<float, 3>(float);
    template float Exp2EstimateRR<float, 4>(float);
    template float Exp2EstimateRR<float, 5>(float);
    template float Exp2EstimateRR<float, 6>(float);
    template float Exp2EstimateRR<float, 7>(float);

    template float GetExp2EstimateMaxError<float, 1>();
    template float GetExp2EstimateMaxError<float, 2>();
    template float GetExp2EstimateMaxError<float, 3>();
    template float GetExp2EstimateMaxError<float, 4>();
    template float GetExp2EstimateMaxError<float, 5>();
    template float GetExp2EstimateMaxError<float, 6>();
    template float GetExp2EstimateMaxError<float, 7>();

    template double Exp2Estimate<double, 1>(double);
    template double Exp2Estimate<double, 2>(double);
    template double Exp2Estimate<double, 3>(double);
    template double Exp2Estimate<double, 4>(double);
    template double Exp2Estimate<double, 5>(double);
    template double Exp2Estimate<double, 6>(double);
    template double Exp2Estimate<double, 7>(double);

    template double Exp2EstimateRR<double, 1>(double);
    template double Exp2EstimateRR<double, 2>(double);
    template double Exp2EstimateRR<double, 3>(double);
    template double Exp2EstimateRR<double, 4>(double);
    template double Exp2EstimateRR<double, 5>(double);
    template double Exp2EstimateRR<double, 6>(double);
    template double Exp2EstimateRR<double, 7>(double);

    template double GetExp2EstimateMaxError<double, 1>();
    template double GetExp2EstimateMaxError<double, 2>();
    template double GetExp2EstimateMaxError<double, 3>();
    template double GetExp2EstimateMaxError<double, 4>();
    template double GetExp2EstimateMaxError<double, 5>();
    template double GetExp2EstimateMaxError<double, 6>();
    template double GetExp2EstimateMaxError<double, 7>();

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;

    template Rational Exp2Estimate<Rational, 1>(Rational);
    template Rational Exp2Estimate<Rational, 2>(Rational);
    template Rational Exp2Estimate<Rational, 3>(Rational);
    template Rational Exp2Estimate<Rational, 4>(Rational);
    template Rational Exp2Estimate<Rational, 5>(Rational);
    template Rational Exp2Estimate<Rational, 6>(Rational);
    template Rational Exp2Estimate<Rational, 7>(Rational);

    template Rational Exp2EstimateRR<Rational, 1>(Rational);
    template Rational Exp2EstimateRR<Rational, 2>(Rational);
    template Rational Exp2EstimateRR<Rational, 3>(Rational);
    template Rational Exp2EstimateRR<Rational, 4>(Rational);
    template Rational Exp2EstimateRR<Rational, 5>(Rational);
    template Rational Exp2EstimateRR<Rational, 6>(Rational);
    template Rational Exp2EstimateRR<Rational, 7>(Rational);

    template Rational GetExp2EstimateMaxError<Rational, 1>();
    template Rational GetExp2EstimateMaxError<Rational, 2>();
    template Rational GetExp2EstimateMaxError<Rational, 3>();
    template Rational GetExp2EstimateMaxError<Rational, 4>();
    template Rational GetExp2EstimateMaxError<Rational, 5>();
    template Rational GetExp2EstimateMaxError<Rational, 6>();
    template Rational GetExp2EstimateMaxError<Rational, 7>();
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(Exp2Estimate)
