// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Functions/InvSqrtEstimate.h>
#include <random>
using namespace gtl;

namespace gtl
{
    class UnitTestInvSqrtEstimate
    {
    public:
        UnitTestInvSqrtEstimate();
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
                actual = 1.0 / std::sqrt(dx);
                estimate = InvSqrtEstimate<double, Degree>(dx);
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
            UTAssert((maxError <= GetInvSqrtEstimateMaxError<double, Degree>()), "Maximum error is too large.");
        }
    };
}

UnitTestInvSqrtEstimate::UnitTestInvSqrtEstimate()
{
    UTInformation("Mathematics/Functions/InvSqrtEstimate");

    TestDegree<1>();
    // maxError = 3.7814314552701900e-2
    // maxX = 1.4283691644668579

    TestDegree<2>();
    // maxError = 4.1953446330580402e-3
    // maxX = 1.7554812431335449

    TestDegree<3>();
    // maxError = 5.6307702007274418e-4
    // maxX = 1.4614144563674927

    TestDegree<4>();
    // maxError = 8.1513919990228167e-5
    // maxX = 1.9171077013015747

    TestDegree<5>();
    // maxError = 1.2289367490980752e-5
    // maxX = 1.9434430599212646

    TestDegree<6>();
    // maxError = 1.9001451476707842e-6
    // maxX = 1.9589867591857910

    TestDegree<7>();
    // maxError = 2.9887737629241684e-7
    // maxX = 1.8514417409896851

    TestDegree<8>();
    // maxError = 4.7597402907939568e-8
    // maxX = 1.8823525905609131

    // The errors are magnified because of the range reduction used in
    // InvSqrtEstimateRR. Use a relative error bound.
    double x = 2.1;
    double actual = 1.0 / std::sqrt(x);
    double estimate = InvSqrtEstimateRR<double, 8>(x);
    double absError = std::fabs(actual - estimate);
    double relError = absError / actual;
    UTAssert(relError <= 1.086824418480e-8, "Maximum error exceeded.");

    x = 100.1;
    actual = 1.0 / std::sqrt(x);
    estimate = InvSqrtEstimateRR<double, 8>(x);
    absError = std::fabs(actual - estimate);
    relError = absError / actual;
    UTAssert(relError <= 5.9075356954362e-8, "Maximum error exceeded.");

    x = 0.01;
    actual = 1.0 / std::sqrt(x);
    estimate = InvSqrtEstimateRR<double, 8>(x);
    absError = std::fabs(actual - estimate);
    relError = absError / actual;
    UTAssert(relError <= 2.9726450989643e-8, "Maximum error exceeded.");

    x = 1e-16;
    actual = 1.0 / std::sqrt(x);
    estimate = InvSqrtEstimateRR<double, 8>(x);
    absError = std::fabs(actual - estimate);
    relError = absError / actual;
    UTAssert(relError <= 2.0839582085610e-8, "Maximum error exceeded.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Functions/InvSqrtEstimate.h>

namespace gtl
{
    template float InvSqrtEstimate<float, 1>(float);
    template float InvSqrtEstimate<float, 2>(float);
    template float InvSqrtEstimate<float, 3>(float);
    template float InvSqrtEstimate<float, 4>(float);
    template float InvSqrtEstimate<float, 5>(float);
    template float InvSqrtEstimate<float, 6>(float);
    template float InvSqrtEstimate<float, 7>(float);
    template float InvSqrtEstimate<float, 8>(float);

    template float InvSqrtEstimateRR<float, 1>(float);
    template float InvSqrtEstimateRR<float, 2>(float);
    template float InvSqrtEstimateRR<float, 3>(float);
    template float InvSqrtEstimateRR<float, 4>(float);
    template float InvSqrtEstimateRR<float, 5>(float);
    template float InvSqrtEstimateRR<float, 6>(float);
    template float InvSqrtEstimateRR<float, 7>(float);
    template float InvSqrtEstimateRR<float, 8>(float);

    template float GetInvSqrtEstimateMaxError<float, 1>();
    template float GetInvSqrtEstimateMaxError<float, 2>();
    template float GetInvSqrtEstimateMaxError<float, 3>();
    template float GetInvSqrtEstimateMaxError<float, 4>();
    template float GetInvSqrtEstimateMaxError<float, 5>();
    template float GetInvSqrtEstimateMaxError<float, 6>();
    template float GetInvSqrtEstimateMaxError<float, 7>();
    template float GetInvSqrtEstimateMaxError<float, 8>();

    template double InvSqrtEstimate<double, 1>(double);
    template double InvSqrtEstimate<double, 2>(double);
    template double InvSqrtEstimate<double, 3>(double);
    template double InvSqrtEstimate<double, 4>(double);
    template double InvSqrtEstimate<double, 5>(double);
    template double InvSqrtEstimate<double, 6>(double);
    template double InvSqrtEstimate<double, 7>(double);
    template double InvSqrtEstimate<double, 8>(double);

    template double InvSqrtEstimateRR<double, 1>(double);
    template double InvSqrtEstimateRR<double, 2>(double);
    template double InvSqrtEstimateRR<double, 3>(double);
    template double InvSqrtEstimateRR<double, 4>(double);
    template double InvSqrtEstimateRR<double, 5>(double);
    template double InvSqrtEstimateRR<double, 6>(double);
    template double InvSqrtEstimateRR<double, 7>(double);
    template double InvSqrtEstimateRR<double, 8>(double);

    template double GetInvSqrtEstimateMaxError<double, 1>();
    template double GetInvSqrtEstimateMaxError<double, 2>();
    template double GetInvSqrtEstimateMaxError<double, 3>();
    template double GetInvSqrtEstimateMaxError<double, 4>();
    template double GetInvSqrtEstimateMaxError<double, 5>();
    template double GetInvSqrtEstimateMaxError<double, 6>();
    template double GetInvSqrtEstimateMaxError<double, 7>();
    template double GetInvSqrtEstimateMaxError<double, 8>();

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;

    template Rational InvSqrtEstimate<Rational, 1>(Rational);
    template Rational InvSqrtEstimate<Rational, 2>(Rational);
    template Rational InvSqrtEstimate<Rational, 3>(Rational);
    template Rational InvSqrtEstimate<Rational, 4>(Rational);
    template Rational InvSqrtEstimate<Rational, 5>(Rational);
    template Rational InvSqrtEstimate<Rational, 6>(Rational);
    template Rational InvSqrtEstimate<Rational, 7>(Rational);
    template Rational InvSqrtEstimate<Rational, 8>(Rational);

    template Rational InvSqrtEstimateRR<Rational, 1>(Rational);
    template Rational InvSqrtEstimateRR<Rational, 2>(Rational);
    template Rational InvSqrtEstimateRR<Rational, 3>(Rational);
    template Rational InvSqrtEstimateRR<Rational, 4>(Rational);
    template Rational InvSqrtEstimateRR<Rational, 5>(Rational);
    template Rational InvSqrtEstimateRR<Rational, 6>(Rational);
    template Rational InvSqrtEstimateRR<Rational, 7>(Rational);
    template Rational InvSqrtEstimateRR<Rational, 8>(Rational);

    template Rational GetInvSqrtEstimateMaxError<Rational, 1>();
    template Rational GetInvSqrtEstimateMaxError<Rational, 2>();
    template Rational GetInvSqrtEstimateMaxError<Rational, 3>();
    template Rational GetInvSqrtEstimateMaxError<Rational, 4>();
    template Rational GetInvSqrtEstimateMaxError<Rational, 5>();
    template Rational GetInvSqrtEstimateMaxError<Rational, 6>();
    template Rational GetInvSqrtEstimateMaxError<Rational, 7>();
    template Rational GetInvSqrtEstimateMaxError<Rational, 8>();
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(InvSqrtEstimate)
