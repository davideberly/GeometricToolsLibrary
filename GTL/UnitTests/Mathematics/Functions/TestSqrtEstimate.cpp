// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Functions/SqrtEstimate.h>
#include <random>
using namespace gtl;

namespace gtl
{
    class UnitTestSqrtEstimate
    {
    public:
        UnitTestSqrtEstimate();
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
                actual = std::sqrt(dx);
                estimate = SqrtEstimate<double, Degree>(dx);
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
            UTAssert((maxError <= GetSqrtEstimateMaxError<double, Degree>()), "Maximum error is too large.");
        }
    };
}

UnitTestSqrtEstimate::UnitTestSqrtEstimate()
{
    UTInformation("Mathematics/Functions/SqrtEstimate");

    TestDegree<1>();
    // maxError = 1.7766952966368654e-2
    // maxX = 1.4571068286895752

    TestDegree<2>();
    // maxError = 1.1795695163110409e-3
    // maxX = 1.7651908397674561

    TestDegree<3>();
    // maxError = 1.1309620116484176e-4
    // maxX = 1.1044137477874756

    TestDegree<4>();
    // maxError = 1.2741170151819858e-5
    // maxX = 1.0671542882919312

    TestDegree<5>();
    // maxError = 1.5725569051383559e-6
    // maxX = 1.7430185079574585

    TestDegree<6>();
    // maxError = 2.0584162152559315e-7
    // maxX = 1.8086235523223877

    TestDegree<7>();
    // maxError = 2.8072338675855235e-8
    // maxX = 1.8524538278579712

    TestDegree<8>();
    // maxError = 3.9468401880071724e-9
    // maxX = 1.8830072879791260

    // The errors are magnified because of the range reduction used in
    // SqrtEstimateRR. Use a relative error bound.
    double x = 2.1;
    double actual = std::sqrt(x);
    double estimate = SqrtEstimateRR<double, 8>(x);
    double absError = std::fabs(actual - estimate);
    double relError = absError / actual;
    UTAssert(relError <= 9.2642013843426e-10, "Maximum error exceeded.");

    x = 100.1;
    actual = std::sqrt(x);
    estimate = SqrtEstimateRR<double, 8>(x);
    absError = std::fabs(actual - estimate);
    relError = absError / actual;
    UTAssert(relError <= 3.1175801468766e-9, "Maximum error exceeded.");

    x = 0.01;
    actual = std::sqrt(x);
    estimate = SqrtEstimateRR<double, 8>(x);
    absError = std::fabs(actual - estimate);
    relError = absError / actual;
    UTAssert(relError <= 2.0224191765728e-9, "Maximum error exceeded.");

    x = 1e-16;
    actual = std::sqrt(x);
    estimate = SqrtEstimateRR<double, 8>(x);
    absError = std::fabs(actual - estimate);
    relError = absError / actual;
    UTAssert(relError <= 1.0207905067272e-9, "Maximum error exceeded.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Functions/SqrtEstimate.h>

namespace gtl
{
    template float SqrtEstimate<float, 1>(float);
    template float SqrtEstimate<float, 2>(float);
    template float SqrtEstimate<float, 3>(float);
    template float SqrtEstimate<float, 4>(float);
    template float SqrtEstimate<float, 5>(float);
    template float SqrtEstimate<float, 6>(float);
    template float SqrtEstimate<float, 7>(float);
    template float SqrtEstimate<float, 8>(float);

    template float SqrtEstimateRR<float, 1>(float);
    template float SqrtEstimateRR<float, 2>(float);
    template float SqrtEstimateRR<float, 3>(float);
    template float SqrtEstimateRR<float, 4>(float);
    template float SqrtEstimateRR<float, 5>(float);
    template float SqrtEstimateRR<float, 6>(float);
    template float SqrtEstimateRR<float, 7>(float);
    template float SqrtEstimateRR<float, 8>(float);

    template float GetSqrtEstimateMaxError<float, 1>();
    template float GetSqrtEstimateMaxError<float, 2>();
    template float GetSqrtEstimateMaxError<float, 3>();
    template float GetSqrtEstimateMaxError<float, 4>();
    template float GetSqrtEstimateMaxError<float, 5>();
    template float GetSqrtEstimateMaxError<float, 6>();
    template float GetSqrtEstimateMaxError<float, 7>();
    template float GetSqrtEstimateMaxError<float, 8>();

    template double SqrtEstimate<double, 1>(double);
    template double SqrtEstimate<double, 2>(double);
    template double SqrtEstimate<double, 3>(double);
    template double SqrtEstimate<double, 4>(double);
    template double SqrtEstimate<double, 5>(double);
    template double SqrtEstimate<double, 6>(double);
    template double SqrtEstimate<double, 7>(double);
    template double SqrtEstimate<double, 8>(double);

    template double SqrtEstimateRR<double, 1>(double);
    template double SqrtEstimateRR<double, 2>(double);
    template double SqrtEstimateRR<double, 3>(double);
    template double SqrtEstimateRR<double, 4>(double);
    template double SqrtEstimateRR<double, 5>(double);
    template double SqrtEstimateRR<double, 6>(double);
    template double SqrtEstimateRR<double, 7>(double);
    template double SqrtEstimateRR<double, 8>(double);

    template double GetSqrtEstimateMaxError<double, 1>();
    template double GetSqrtEstimateMaxError<double, 2>();
    template double GetSqrtEstimateMaxError<double, 3>();
    template double GetSqrtEstimateMaxError<double, 4>();
    template double GetSqrtEstimateMaxError<double, 5>();
    template double GetSqrtEstimateMaxError<double, 6>();
    template double GetSqrtEstimateMaxError<double, 7>();
    template double GetSqrtEstimateMaxError<double, 8>();

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;

    template Rational SqrtEstimate<Rational, 1>(Rational);
    template Rational SqrtEstimate<Rational, 2>(Rational);
    template Rational SqrtEstimate<Rational, 3>(Rational);
    template Rational SqrtEstimate<Rational, 4>(Rational);
    template Rational SqrtEstimate<Rational, 5>(Rational);
    template Rational SqrtEstimate<Rational, 6>(Rational);
    template Rational SqrtEstimate<Rational, 7>(Rational);
    template Rational SqrtEstimate<Rational, 8>(Rational);

    template Rational SqrtEstimateRR<Rational, 1>(Rational);
    template Rational SqrtEstimateRR<Rational, 2>(Rational);
    template Rational SqrtEstimateRR<Rational, 3>(Rational);
    template Rational SqrtEstimateRR<Rational, 4>(Rational);
    template Rational SqrtEstimateRR<Rational, 5>(Rational);
    template Rational SqrtEstimateRR<Rational, 6>(Rational);
    template Rational SqrtEstimateRR<Rational, 7>(Rational);
    template Rational SqrtEstimateRR<Rational, 8>(Rational);

    template Rational GetSqrtEstimateMaxError<Rational, 1>();
    template Rational GetSqrtEstimateMaxError<Rational, 2>();
    template Rational GetSqrtEstimateMaxError<Rational, 3>();
    template Rational GetSqrtEstimateMaxError<Rational, 4>();
    template Rational GetSqrtEstimateMaxError<Rational, 5>();
    template Rational GetSqrtEstimateMaxError<Rational, 6>();
    template Rational GetSqrtEstimateMaxError<Rational, 7>();
    template Rational GetSqrtEstimateMaxError<Rational, 8>();
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(SqrtEstimate)
