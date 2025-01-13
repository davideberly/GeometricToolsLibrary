// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Arithmetic/Constants.h>
#include <GTL/Mathematics/Functions/ChebyshevRatioEstimate.h>
#include <cmath>
using namespace gtl;

namespace gtl
{
    class UnitTestChebyshevRatioEstimate
    {
    public:
        UnitTestChebyshevRatioEstimate();
    private:
        template <std::size_t Degree>
        void TestDegree()
        {
            std::size_t const imax = 1024;
            std::size_t const jmax = 1024;
            double const iFactor = 1.0 / static_cast<double>(imax);
            double const jFactor = C_PI_DIV_2<double> / static_cast<double>(jmax);

            double maxError = 0.0, maxT = 0.0, maxAngle = 0.0;
            for (std::size_t i = 1; i < imax; ++i)
            {
                double t = static_cast<double>(i) * iFactor;
                for (std::size_t j = 1; j < 1024; ++j)
                {
                    double angle = static_cast<double>(j) * jFactor;
                    double x = std::cos(angle);
                    double actual = std::sin(t * angle) / std::sin(angle);
                    double approximate = ChebyshevRatioEstimate<double, Degree>(t, x)[1];
                    double error = std::fabs(actual - approximate);
                    if (error > maxError)
                    {
                        maxError = error;
                        maxT = t;
                        maxAngle = angle;
                    }
                }
            }

            std::ostringstream ostream;
            ostream << "Degree = " << Degree << ": " << std::setprecision(20)
                << "maxError = " << maxError << ", maxT = " << maxT
                << ", maxAngle = " << maxAngle;
            UTInformation(ostream.str());
            UTAssert((maxError <= GetChebyshevRatioEstimateMaxError<double, Degree>()),
                "Maximum error is too large.");
        }
        template <std::size_t Degree>
        void TestDegreeReduced()
        {
            std::size_t const imax = 1024;
            std::size_t const jmax = 1024;
            double const iFactor = 1.0 / static_cast<double>(imax);
            double const jFactor = C_PI_DIV_4<double> / static_cast<double>(jmax);

            double maxError = 0.0, maxT = 0.0, maxAngle = 0.0;
            for (std::size_t i = 1; i < imax; ++i)
            {
                double t = static_cast<double>(i) * iFactor;
                for (std::size_t j = 1; j < 1024; ++j)
                {
                    double angle = static_cast<double>(j) * jFactor;
                    double x = std::cos(angle);
                    double actual = std::sin(t * angle) / std::sin(angle);
                    double approximate = ChebyshevRatioEstimateR<double, Degree>(t, x)[1];
                    double error = std::fabs(actual - approximate);
                    if (error > maxError)
                    {
                        maxError = error;
                        maxT = t;
                        maxAngle = angle;
                    }
                }
            }

            std::ostringstream ostream;
            ostream << "Degree = " << Degree << ": " << std::setprecision(20)
                << "maxError = " << maxError << ", maxT = " << maxT
                << ", maxAngle = " << maxAngle;
            UTInformation(ostream.str());
            UTAssert((maxError <= GetChebyshevRatioEstimateRMaxError<double, Degree>()),
                "Maximum error is too large.");
        }
    };
}

UnitTestChebyshevRatioEstimate::UnitTestChebyshevRatioEstimate()
{
    UTInformation("Mathematics/Functions/ChebyshevRatioEstimate");

    TestDegree<1>();
    TestDegree<2>();
    TestDegree<3>();
    TestDegree<4>();
    TestDegree<5>();
    TestDegree<6>();
    TestDegree<7>();
    TestDegree<9>();
    TestDegree<10>();
    TestDegree<11>();
    TestDegree<12>();
    TestDegree<13>();
    TestDegree<14>();
    TestDegree<15>();
    TestDegree<16>();

    TestDegreeReduced<1>();
    TestDegreeReduced<2>();
    TestDegreeReduced<3>();
    TestDegreeReduced<4>();
    TestDegreeReduced<5>();
    TestDegreeReduced<6>();
    TestDegreeReduced<7>();
    TestDegreeReduced<9>();
    TestDegreeReduced<10>();
    TestDegreeReduced<11>();
    TestDegreeReduced<12>();
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Functions/ChebyshevRatioEstimate.h>

namespace gtl
{
    template std::array<float, 2> ChebyshevRatioEstimate<float, 1>(float, float);
    template std::array<float, 2> ChebyshevRatioEstimate<float, 2>(float, float);
    template std::array<float, 2> ChebyshevRatioEstimate<float, 3>(float, float);
    template std::array<float, 2> ChebyshevRatioEstimate<float, 4>(float, float);
    template std::array<float, 2> ChebyshevRatioEstimate<float, 5>(float, float);
    template std::array<float, 2> ChebyshevRatioEstimate<float, 6>(float, float);
    template std::array<float, 2> ChebyshevRatioEstimate<float, 7>(float, float);
    template std::array<float, 2> ChebyshevRatioEstimate<float, 8>(float, float);
    template std::array<float, 2> ChebyshevRatioEstimate<float, 9>(float, float);
    template std::array<float, 2> ChebyshevRatioEstimate<float, 10>(float, float);
    template std::array<float, 2> ChebyshevRatioEstimate<float, 11>(float, float);
    template std::array<float, 2> ChebyshevRatioEstimate<float, 12>(float, float);

    template std::array<float, 2> ChebyshevRatioEstimateR<float, 1>(float, float);
    template std::array<float, 2> ChebyshevRatioEstimateR<float, 2>(float, float);
    template std::array<float, 2> ChebyshevRatioEstimateR<float, 3>(float, float);
    template std::array<float, 2> ChebyshevRatioEstimateR<float, 4>(float, float);
    template std::array<float, 2> ChebyshevRatioEstimateR<float, 5>(float, float);
    template std::array<float, 2> ChebyshevRatioEstimateR<float, 6>(float, float);
    template std::array<float, 2> ChebyshevRatioEstimateR<float, 7>(float, float);
    template std::array<float, 2> ChebyshevRatioEstimateR<float, 8>(float, float);
    template std::array<float, 2> ChebyshevRatioEstimateR<float, 9>(float, float);
    template std::array<float, 2> ChebyshevRatioEstimateR<float, 10>(float, float);
    template std::array<float, 2> ChebyshevRatioEstimateR<float, 11>(float, float);
    template std::array<float, 2> ChebyshevRatioEstimateR<float, 12>(float, float);

    template float GetChebyshevRatioEstimateRMaxError<float, 1>();
    template float GetChebyshevRatioEstimateRMaxError<float, 2>();
    template float GetChebyshevRatioEstimateRMaxError<float, 3>();
    template float GetChebyshevRatioEstimateRMaxError<float, 4>();
    template float GetChebyshevRatioEstimateRMaxError<float, 5>();
    template float GetChebyshevRatioEstimateRMaxError<float, 6>();
    template float GetChebyshevRatioEstimateRMaxError<float, 7>();
    template float GetChebyshevRatioEstimateRMaxError<float, 8>();
    template float GetChebyshevRatioEstimateRMaxError<float, 9>();
    template float GetChebyshevRatioEstimateRMaxError<float, 10>();
    template float GetChebyshevRatioEstimateRMaxError<float, 11>();
    template float GetChebyshevRatioEstimateRMaxError<float, 12>();

    template std::array<double, 2> ChebyshevRatioEstimate<double, 1>(double, double);
    template std::array<double, 2> ChebyshevRatioEstimate<double, 2>(double, double);
    template std::array<double, 2> ChebyshevRatioEstimate<double, 3>(double, double);
    template std::array<double, 2> ChebyshevRatioEstimate<double, 4>(double, double);
    template std::array<double, 2> ChebyshevRatioEstimate<double, 5>(double, double);
    template std::array<double, 2> ChebyshevRatioEstimate<double, 6>(double, double);
    template std::array<double, 2> ChebyshevRatioEstimate<double, 7>(double, double);
    template std::array<double, 2> ChebyshevRatioEstimate<double, 8>(double, double);
    template std::array<double, 2> ChebyshevRatioEstimate<double, 9>(double, double);
    template std::array<double, 2> ChebyshevRatioEstimate<double, 10>(double, double);
    template std::array<double, 2> ChebyshevRatioEstimate<double, 11>(double, double);
    template std::array<double, 2> ChebyshevRatioEstimate<double, 12>(double, double);

    template std::array<double, 2> ChebyshevRatioEstimateR<double, 1>(double, double);
    template std::array<double, 2> ChebyshevRatioEstimateR<double, 2>(double, double);
    template std::array<double, 2> ChebyshevRatioEstimateR<double, 3>(double, double);
    template std::array<double, 2> ChebyshevRatioEstimateR<double, 4>(double, double);
    template std::array<double, 2> ChebyshevRatioEstimateR<double, 5>(double, double);
    template std::array<double, 2> ChebyshevRatioEstimateR<double, 6>(double, double);
    template std::array<double, 2> ChebyshevRatioEstimateR<double, 7>(double, double);
    template std::array<double, 2> ChebyshevRatioEstimateR<double, 8>(double, double);
    template std::array<double, 2> ChebyshevRatioEstimateR<double, 9>(double, double);
    template std::array<double, 2> ChebyshevRatioEstimateR<double, 10>(double, double);
    template std::array<double, 2> ChebyshevRatioEstimateR<double, 11>(double, double);
    template std::array<double, 2> ChebyshevRatioEstimateR<double, 12>(double, double);

    template double GetChebyshevRatioEstimateRMaxError<double, 1>();
    template double GetChebyshevRatioEstimateRMaxError<double, 2>();
    template double GetChebyshevRatioEstimateRMaxError<double, 3>();
    template double GetChebyshevRatioEstimateRMaxError<double, 4>();
    template double GetChebyshevRatioEstimateRMaxError<double, 5>();
    template double GetChebyshevRatioEstimateRMaxError<double, 6>();
    template double GetChebyshevRatioEstimateRMaxError<double, 7>();
    template double GetChebyshevRatioEstimateRMaxError<double, 8>();
    template double GetChebyshevRatioEstimateRMaxError<double, 9>();
    template double GetChebyshevRatioEstimateRMaxError<double, 10>();
    template double GetChebyshevRatioEstimateRMaxError<double, 11>();
    template double GetChebyshevRatioEstimateRMaxError<double, 12>();

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;

    template std::array<Rational, 2> ChebyshevRatioEstimate<Rational, 1>(Rational, Rational);
    template std::array<Rational, 2> ChebyshevRatioEstimate<Rational, 2>(Rational, Rational);
    template std::array<Rational, 2> ChebyshevRatioEstimate<Rational, 3>(Rational, Rational);
    template std::array<Rational, 2> ChebyshevRatioEstimate<Rational, 4>(Rational, Rational);
    template std::array<Rational, 2> ChebyshevRatioEstimate<Rational, 5>(Rational, Rational);
    template std::array<Rational, 2> ChebyshevRatioEstimate<Rational, 6>(Rational, Rational);
    template std::array<Rational, 2> ChebyshevRatioEstimate<Rational, 7>(Rational, Rational);
    template std::array<Rational, 2> ChebyshevRatioEstimate<Rational, 8>(Rational, Rational);
    template std::array<Rational, 2> ChebyshevRatioEstimate<Rational, 9>(Rational, Rational);
    template std::array<Rational, 2> ChebyshevRatioEstimate<Rational, 10>(Rational, Rational);
    template std::array<Rational, 2> ChebyshevRatioEstimate<Rational, 11>(Rational, Rational);
    template std::array<Rational, 2> ChebyshevRatioEstimate<Rational, 12>(Rational, Rational);

    template std::array<Rational, 2> ChebyshevRatioEstimateR<Rational, 1>(Rational, Rational);
    template std::array<Rational, 2> ChebyshevRatioEstimateR<Rational, 2>(Rational, Rational);
    template std::array<Rational, 2> ChebyshevRatioEstimateR<Rational, 3>(Rational, Rational);
    template std::array<Rational, 2> ChebyshevRatioEstimateR<Rational, 4>(Rational, Rational);
    template std::array<Rational, 2> ChebyshevRatioEstimateR<Rational, 5>(Rational, Rational);
    template std::array<Rational, 2> ChebyshevRatioEstimateR<Rational, 6>(Rational, Rational);
    template std::array<Rational, 2> ChebyshevRatioEstimateR<Rational, 7>(Rational, Rational);
    template std::array<Rational, 2> ChebyshevRatioEstimateR<Rational, 8>(Rational, Rational);
    template std::array<Rational, 2> ChebyshevRatioEstimateR<Rational, 9>(Rational, Rational);
    template std::array<Rational, 2> ChebyshevRatioEstimateR<Rational, 10>(Rational, Rational);
    template std::array<Rational, 2> ChebyshevRatioEstimateR<Rational, 11>(Rational, Rational);
    template std::array<Rational, 2> ChebyshevRatioEstimateR<Rational, 12>(Rational, Rational);

    template Rational GetChebyshevRatioEstimateRMaxError<Rational, 1>();
    template Rational GetChebyshevRatioEstimateRMaxError<Rational, 2>();
    template Rational GetChebyshevRatioEstimateRMaxError<Rational, 3>();
    template Rational GetChebyshevRatioEstimateRMaxError<Rational, 4>();
    template Rational GetChebyshevRatioEstimateRMaxError<Rational, 5>();
    template Rational GetChebyshevRatioEstimateRMaxError<Rational, 6>();
    template Rational GetChebyshevRatioEstimateRMaxError<Rational, 7>();
    template Rational GetChebyshevRatioEstimateRMaxError<Rational, 8>();
    template Rational GetChebyshevRatioEstimateRMaxError<Rational, 9>();
    template Rational GetChebyshevRatioEstimateRMaxError<Rational, 10>();
    template Rational GetChebyshevRatioEstimateRMaxError<Rational, 11>();
    template Rational GetChebyshevRatioEstimateRMaxError<Rational, 12>();
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ChebyshevRatioEstimate)
