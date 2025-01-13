// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Functions/ExpEstimate.h>
#include <random>
using namespace gtl;

namespace gtl
{
    class UnitTestExpEstimate
    {
    public:
        UnitTestExpEstimate();
    private:
        union Binary32
        {
            float number;
            std::uint32_t encoding;
        };

        template <std::size_t Degree>
        void TestDegree()
        {
            Binary32 zero{ 0.0f }, ln2{ C_LN_2<float> }, x{ 0.0f };
            std::default_random_engine dre;
            std::uniform_int_distribution<std::uint32_t> urd(zero.encoding, ln2.encoding);

            double maxError = 0.0, maxX = 0.0;
            double dx, actual, estimate, error;
            for (std::size_t k = 0; k < 65536ull; ++k)
            {
                x.encoding = urd(dre);
                dx = static_cast<double>(x.number);
                actual = std::exp(dx);
                estimate = ExpEstimate<double, Degree>(dx);
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
            UTAssert((maxError <= GetExpEstimateMaxError<double, Degree>()), "Maximum error is too large.");
        }
    };
}

UnitTestExpEstimate::UnitTestExpEstimate()
{
    UTInformation("Mathematics/Functions/ExpEstimate");

    TestDegree<1>();
    // maxError = 8.6071332055934091e-2
    // maxX = 0.36651292443275452

    TestDegree<2>();
    // maxError = 3.8132476831058693e-3
    // maxX = 0.15320469439029694

    TestDegree<3>();
    // maxError = 1.4694877755228042e-4
    // maxX = 0.35257184505462646

    TestDegree<4>();
    // maxError = 4.7617792662268954e-6
    // maxX = 0.64249539375305176

    TestDegree<5>();
    // maxError = 1.3162098766450470e-7
    // maxX = 0.52807027101516724

    TestDegree<6>();
    // maxError = 3.1590550175764065e-9
    // maxX = 0.56948006153106689

    TestDegree<7>();
    // maxError = 6.7157168714970794e-11
    // maxX = 0.59723049402236938

    // The errors are magnified because of the multiplication by 2^p in
    // the ExpEstimateRR code. Use a relative error bound.
    double x = 2.1;
    double actual = std::exp(x);
    double estimate = ExpEstimateRR<double, 7>(x);
    double relError = std::fabs(actual - estimate) / actual;
    UTAssert(relError <= 6.5496085470386e-11, "Maximum error exceeded.");

    x = -x;
    actual = std::exp(x);
    estimate = ExpEstimateRR<double, 7>(x);
    relError = std::fabs(actual - estimate) / actual;
    UTAssert(relError <= 3.3807440337454e-11, "Maximum error exceeded.");

    x = 10.1;
    actual = std::exp(x);
    estimate = ExpEstimateRR<double, 7>(x);
    relError = std::fabs(actual - estimate) / actual;
    UTAssert(relError <= 2.1075849345665e-11, "Maximum error exceeded.");

    x = -x;
    actual = std::exp(x);
    estimate = ExpEstimateRR<double, 7>(x);
    relError = std::fabs(actual - estimate) / actual;
    UTAssert(relError <= 1.9904417572747e-11, "Maximum error exceeded.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Functions/ExpEstimate.h>

namespace gtl
{
    template float ExpEstimate<float, 1>(float);
    template float ExpEstimate<float, 2>(float);
    template float ExpEstimate<float, 3>(float);
    template float ExpEstimate<float, 4>(float);
    template float ExpEstimate<float, 5>(float);
    template float ExpEstimate<float, 6>(float);
    template float ExpEstimate<float, 7>(float);

    template float ExpEstimateRR<float, 1>(float);
    template float ExpEstimateRR<float, 2>(float);
    template float ExpEstimateRR<float, 3>(float);
    template float ExpEstimateRR<float, 4>(float);
    template float ExpEstimateRR<float, 5>(float);
    template float ExpEstimateRR<float, 6>(float);
    template float ExpEstimateRR<float, 7>(float);

    template float GetExpEstimateMaxError<float, 1>();
    template float GetExpEstimateMaxError<float, 2>();
    template float GetExpEstimateMaxError<float, 3>();
    template float GetExpEstimateMaxError<float, 4>();
    template float GetExpEstimateMaxError<float, 5>();
    template float GetExpEstimateMaxError<float, 6>();
    template float GetExpEstimateMaxError<float, 7>();

    template double ExpEstimate<double, 1>(double);
    template double ExpEstimate<double, 2>(double);
    template double ExpEstimate<double, 3>(double);
    template double ExpEstimate<double, 4>(double);
    template double ExpEstimate<double, 5>(double);
    template double ExpEstimate<double, 6>(double);
    template double ExpEstimate<double, 7>(double);

    template double ExpEstimateRR<double, 1>(double);
    template double ExpEstimateRR<double, 2>(double);
    template double ExpEstimateRR<double, 3>(double);
    template double ExpEstimateRR<double, 4>(double);
    template double ExpEstimateRR<double, 5>(double);
    template double ExpEstimateRR<double, 6>(double);
    template double ExpEstimateRR<double, 7>(double);

    template double GetExpEstimateMaxError<double, 1>();
    template double GetExpEstimateMaxError<double, 2>();
    template double GetExpEstimateMaxError<double, 3>();
    template double GetExpEstimateMaxError<double, 4>();
    template double GetExpEstimateMaxError<double, 5>();
    template double GetExpEstimateMaxError<double, 6>();
    template double GetExpEstimateMaxError<double, 7>();

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;

    template Rational ExpEstimate<Rational, 1>(Rational);
    template Rational ExpEstimate<Rational, 2>(Rational);
    template Rational ExpEstimate<Rational, 3>(Rational);
    template Rational ExpEstimate<Rational, 4>(Rational);
    template Rational ExpEstimate<Rational, 5>(Rational);
    template Rational ExpEstimate<Rational, 6>(Rational);
    template Rational ExpEstimate<Rational, 7>(Rational);

    template Rational ExpEstimateRR<Rational, 1>(Rational);
    template Rational ExpEstimateRR<Rational, 2>(Rational);
    template Rational ExpEstimateRR<Rational, 3>(Rational);
    template Rational ExpEstimateRR<Rational, 4>(Rational);
    template Rational ExpEstimateRR<Rational, 5>(Rational);
    template Rational ExpEstimateRR<Rational, 6>(Rational);
    template Rational ExpEstimateRR<Rational, 7>(Rational);

    template Rational GetExpEstimateMaxError<Rational, 1>();
    template Rational GetExpEstimateMaxError<Rational, 2>();
    template Rational GetExpEstimateMaxError<Rational, 3>();
    template Rational GetExpEstimateMaxError<Rational, 4>();
    template Rational GetExpEstimateMaxError<Rational, 5>();
    template Rational GetExpEstimateMaxError<Rational, 6>();
    template Rational GetExpEstimateMaxError<Rational, 7>();
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ExpEstimate)
