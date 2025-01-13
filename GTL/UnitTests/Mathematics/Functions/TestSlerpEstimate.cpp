// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Functions/SlerpEstimate.h>
#include <GTL/Mathematics/Functions/Slerp.h>
using namespace gtl;

namespace gtl
{
    class UnitTestSlerpEstimate
    {
    public:
        UnitTestSlerpEstimate();

    private:
        void Test();

        template <std::size_t Degree>
        void TestDegree()
        {
            double constexpr errorRegular[16] =
            {
                5.0247366511616152e-3,
                1.2570294289863501e-4,
                3.1717707374114940e-6,
                8.0920146034944251e-8,
                2.0834015656649654e-9,
                5.4032461916199689e-11,
                1.4098922055107572e-12,
                3.7126970556850651e-14,
                1.1514383065176238e-15,
                2.2887833992611187e-16,
                1.1102230246251565e-16,
                1.1102230246251565e-16,
                1.1102230246251565e-16,
                1.1102230246251565e-16,
                1.1102230246251565e-16,
                1.1102230246251565e-16
            };

            double constexpr errorHalf[16] =
            {
                1.7562454503021918e-3,
                1.1349563753616332e-5,
                7.2725854923038059e-8,
                4.6909324085785706e-10,
                3.0483331013949621e-12,
                2.0190058944171350e-14,
                2.2887833992611187e-16,
                2.2887833992611187e-16,
                2.2887833992611187e-16,
                2.2887833992611187e-16,
                2.2887833992611187e-16,
                2.2887833992611187e-16,
                2.2887833992611187e-16,
                2.2887833992611187e-16,
                2.2887833992611187e-16,
                2.2887833992611187e-16
            };

            double t, angle, cosA, cosAH, length;
            std::array<double, 2> q0, q1, qh, diff;
            std::array<double, 2> sActual, sEstimate;

            t = 0.25;
            angle = 0.123;
            q0[0] = std::cos(angle);
            q0[1] = std::sin(angle);
            angle = 0.456;
            q1[0] = std::cos(angle);
            q1[1] = std::sin(angle);
            cosA = q0[0] * q1[0] + q0[1] * q1[1];

            sActual = Slerp(t, q0, q1);
            sEstimate = SlerpEstimate<double, 2, Degree>(t, q0, q1);
            diff[0] = sActual[0] - sEstimate[0];
            diff[1] = sActual[1] - sEstimate[1];
            length = std::sqrt(diff[0] * diff[0] + diff[1] * diff[1]);
            UTAssert(length == errorRegular[Degree - 1], "Incorrect error.");

            sActual = Slerp(t, q0, q1, cosA);
            sEstimate = SlerpEstimate<double, 2, Degree>(t, q0, q1, cosA);
            diff[0] = sActual[0] - sEstimate[0];
            diff[1] = sActual[1] - sEstimate[1];
            length = std::sqrt(diff[0] * diff[0] + diff[1] * diff[1]);
            UTAssert(length == errorRegular[Degree - 1], "Incorrect error.");

            cosAH = std::sqrt((1.0 + cosA) / 2.0);
            qh[0] = (q0[0] + q1[0]) / (2.0 * cosAH);
            qh[1] = (q0[1] + q1[1]) / (2.0 * cosAH);
            sActual = Slerp(t, q0, q1, qh, cosAH);
            sEstimate = SlerpEstimate<double, 2, Degree>(t, q0, q1, qh, cosAH);
            diff[0] = sActual[0] - sEstimate[0];
            diff[1] = sActual[1] - sEstimate[1];
            length = std::sqrt(diff[0] * diff[0] + diff[1] * diff[1]);
            UTAssert(length == errorHalf[Degree - 1], "Incorrect error.");
        }
    };
}

UnitTestSlerpEstimate::UnitTestSlerpEstimate()
{
    UTInformation("Mathematics/Functions/SlerpEstimate");

    Test();
}

void UnitTestSlerpEstimate::Test()
{
    TestDegree<1>();
    TestDegree<2>();
    TestDegree<3>();
    TestDegree<4>();
    TestDegree<5>();
    TestDegree<6>();
    TestDegree<7>();
    TestDegree<8>();
    TestDegree<9>();
    TestDegree<10>();
    TestDegree<11>();
    TestDegree<12>();
    TestDegree<13>();
    TestDegree<14>();
    TestDegree<15>();
    TestDegree<16>();
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Functions/SlerpEstimate.h>

namespace gtl
{
    using FTuple4 = std::array<float, 4>;

    template FTuple4 SlerpEstimate<float, 4, 1>(float, FTuple4 const&, FTuple4 const&);
    template FTuple4 SlerpEstimate<float, 4, 2>(float, FTuple4 const&, FTuple4 const&);
    template FTuple4 SlerpEstimate<float, 4, 3>(float, FTuple4 const&, FTuple4 const&);
    template FTuple4 SlerpEstimate<float, 4, 4>(float, FTuple4 const&, FTuple4 const&);
    template FTuple4 SlerpEstimate<float, 4, 5>(float, FTuple4 const&, FTuple4 const&);
    template FTuple4 SlerpEstimate<float, 4, 6>(float, FTuple4 const&, FTuple4 const&);
    template FTuple4 SlerpEstimate<float, 4, 7>(float, FTuple4 const&, FTuple4 const&);
    template FTuple4 SlerpEstimate<float, 4, 8>(float, FTuple4 const&, FTuple4 const&);
    template FTuple4 SlerpEstimate<float, 4, 9>(float, FTuple4 const&, FTuple4 const&);
    template FTuple4 SlerpEstimate<float, 4, 10>(float, FTuple4 const&, FTuple4 const&);
    template FTuple4 SlerpEstimate<float, 4, 11>(float, FTuple4 const&, FTuple4 const&);
    template FTuple4 SlerpEstimate<float, 4, 12>(float, FTuple4 const&, FTuple4 const&);
    template FTuple4 SlerpEstimate<float, 4, 13>(float, FTuple4 const&, FTuple4 const&);
    template FTuple4 SlerpEstimate<float, 4, 14>(float, FTuple4 const&, FTuple4 const&);
    template FTuple4 SlerpEstimate<float, 4, 15>(float, FTuple4 const&, FTuple4 const&);
    template FTuple4 SlerpEstimate<float, 4, 16>(float, FTuple4 const&, FTuple4 const&);

    template FTuple4 SlerpEstimate<float, 4, 1>(float, FTuple4 const&, FTuple4 const&, float);
    template FTuple4 SlerpEstimate<float, 4, 2>(float, FTuple4 const&, FTuple4 const&, float);
    template FTuple4 SlerpEstimate<float, 4, 3>(float, FTuple4 const&, FTuple4 const&, float);
    template FTuple4 SlerpEstimate<float, 4, 4>(float, FTuple4 const&, FTuple4 const&, float);
    template FTuple4 SlerpEstimate<float, 4, 5>(float, FTuple4 const&, FTuple4 const&, float);
    template FTuple4 SlerpEstimate<float, 4, 6>(float, FTuple4 const&, FTuple4 const&, float);
    template FTuple4 SlerpEstimate<float, 4, 7>(float, FTuple4 const&, FTuple4 const&, float);
    template FTuple4 SlerpEstimate<float, 4, 8>(float, FTuple4 const&, FTuple4 const&, float);
    template FTuple4 SlerpEstimate<float, 4, 9>(float, FTuple4 const&, FTuple4 const&, float);
    template FTuple4 SlerpEstimate<float, 4, 10>(float, FTuple4 const&, FTuple4 const&, float);
    template FTuple4 SlerpEstimate<float, 4, 11>(float, FTuple4 const&, FTuple4 const&, float);
    template FTuple4 SlerpEstimate<float, 4, 12>(float, FTuple4 const&, FTuple4 const&, float);
    template FTuple4 SlerpEstimate<float, 4, 13>(float, FTuple4 const&, FTuple4 const&, float);
    template FTuple4 SlerpEstimate<float, 4, 14>(float, FTuple4 const&, FTuple4 const&, float);
    template FTuple4 SlerpEstimate<float, 4, 15>(float, FTuple4 const&, FTuple4 const&, float);
    template FTuple4 SlerpEstimate<float, 4, 16>(float, FTuple4 const&, FTuple4 const&, float);

    template FTuple4 SlerpEstimate<float, 4, 1>(float, FTuple4 const&, FTuple4 const&, FTuple4 const&, float);
    template FTuple4 SlerpEstimate<float, 4, 2>(float, FTuple4 const&, FTuple4 const&, FTuple4 const&, float);
    template FTuple4 SlerpEstimate<float, 4, 3>(float, FTuple4 const&, FTuple4 const&, FTuple4 const&, float);
    template FTuple4 SlerpEstimate<float, 4, 4>(float, FTuple4 const&, FTuple4 const&, FTuple4 const&, float);
    template FTuple4 SlerpEstimate<float, 4, 5>(float, FTuple4 const&, FTuple4 const&, FTuple4 const&, float);
    template FTuple4 SlerpEstimate<float, 4, 6>(float, FTuple4 const&, FTuple4 const&, FTuple4 const&, float);
    template FTuple4 SlerpEstimate<float, 4, 7>(float, FTuple4 const&, FTuple4 const&, FTuple4 const&, float);
    template FTuple4 SlerpEstimate<float, 4, 8>(float, FTuple4 const&, FTuple4 const&, FTuple4 const&, float);
    template FTuple4 SlerpEstimate<float, 4, 9>(float, FTuple4 const&, FTuple4 const&, FTuple4 const&, float);
    template FTuple4 SlerpEstimate<float, 4, 10>(float, FTuple4 const&, FTuple4 const&, FTuple4 const&, float);
    template FTuple4 SlerpEstimate<float, 4, 11>(float, FTuple4 const&, FTuple4 const&, FTuple4 const&, float);
    template FTuple4 SlerpEstimate<float, 4, 12>(float, FTuple4 const&, FTuple4 const&, FTuple4 const&, float);
    template FTuple4 SlerpEstimate<float, 4, 13>(float, FTuple4 const&, FTuple4 const&, FTuple4 const&, float);
    template FTuple4 SlerpEstimate<float, 4, 14>(float, FTuple4 const&, FTuple4 const&, FTuple4 const&, float);
    template FTuple4 SlerpEstimate<float, 4, 15>(float, FTuple4 const&, FTuple4 const&, FTuple4 const&, float);
    template FTuple4 SlerpEstimate<float, 4, 16>(float, FTuple4 const&, FTuple4 const&, FTuple4 const&, float);

    using DTuple4 = std::array<double, 4>;

    template DTuple4 SlerpEstimate<double, 4, 1>(double, DTuple4 const&, DTuple4 const&);
    template DTuple4 SlerpEstimate<double, 4, 2>(double, DTuple4 const&, DTuple4 const&);
    template DTuple4 SlerpEstimate<double, 4, 3>(double, DTuple4 const&, DTuple4 const&);
    template DTuple4 SlerpEstimate<double, 4, 4>(double, DTuple4 const&, DTuple4 const&);
    template DTuple4 SlerpEstimate<double, 4, 5>(double, DTuple4 const&, DTuple4 const&);
    template DTuple4 SlerpEstimate<double, 4, 6>(double, DTuple4 const&, DTuple4 const&);
    template DTuple4 SlerpEstimate<double, 4, 7>(double, DTuple4 const&, DTuple4 const&);
    template DTuple4 SlerpEstimate<double, 4, 8>(double, DTuple4 const&, DTuple4 const&);
    template DTuple4 SlerpEstimate<double, 4, 9>(double, DTuple4 const&, DTuple4 const&);
    template DTuple4 SlerpEstimate<double, 4, 10>(double, DTuple4 const&, DTuple4 const&);
    template DTuple4 SlerpEstimate<double, 4, 11>(double, DTuple4 const&, DTuple4 const&);
    template DTuple4 SlerpEstimate<double, 4, 12>(double, DTuple4 const&, DTuple4 const&);
    template DTuple4 SlerpEstimate<double, 4, 13>(double, DTuple4 const&, DTuple4 const&);
    template DTuple4 SlerpEstimate<double, 4, 14>(double, DTuple4 const&, DTuple4 const&);
    template DTuple4 SlerpEstimate<double, 4, 15>(double, DTuple4 const&, DTuple4 const&);
    template DTuple4 SlerpEstimate<double, 4, 16>(double, DTuple4 const&, DTuple4 const&);

    template DTuple4 SlerpEstimate<double, 4, 1>(double, DTuple4 const&, DTuple4 const&, double);
    template DTuple4 SlerpEstimate<double, 4, 2>(double, DTuple4 const&, DTuple4 const&, double);
    template DTuple4 SlerpEstimate<double, 4, 3>(double, DTuple4 const&, DTuple4 const&, double);
    template DTuple4 SlerpEstimate<double, 4, 4>(double, DTuple4 const&, DTuple4 const&, double);
    template DTuple4 SlerpEstimate<double, 4, 5>(double, DTuple4 const&, DTuple4 const&, double);
    template DTuple4 SlerpEstimate<double, 4, 6>(double, DTuple4 const&, DTuple4 const&, double);
    template DTuple4 SlerpEstimate<double, 4, 7>(double, DTuple4 const&, DTuple4 const&, double);
    template DTuple4 SlerpEstimate<double, 4, 8>(double, DTuple4 const&, DTuple4 const&, double);
    template DTuple4 SlerpEstimate<double, 4, 9>(double, DTuple4 const&, DTuple4 const&, double);
    template DTuple4 SlerpEstimate<double, 4, 10>(double, DTuple4 const&, DTuple4 const&, double);
    template DTuple4 SlerpEstimate<double, 4, 11>(double, DTuple4 const&, DTuple4 const&, double);
    template DTuple4 SlerpEstimate<double, 4, 12>(double, DTuple4 const&, DTuple4 const&, double);
    template DTuple4 SlerpEstimate<double, 4, 13>(double, DTuple4 const&, DTuple4 const&, double);
    template DTuple4 SlerpEstimate<double, 4, 14>(double, DTuple4 const&, DTuple4 const&, double);
    template DTuple4 SlerpEstimate<double, 4, 15>(double, DTuple4 const&, DTuple4 const&, double);
    template DTuple4 SlerpEstimate<double, 4, 16>(double, DTuple4 const&, DTuple4 const&, double);

    template DTuple4 SlerpEstimate<double, 4, 1>(double, DTuple4 const&, DTuple4 const&, DTuple4 const&, double);
    template DTuple4 SlerpEstimate<double, 4, 2>(double, DTuple4 const&, DTuple4 const&, DTuple4 const&, double);
    template DTuple4 SlerpEstimate<double, 4, 3>(double, DTuple4 const&, DTuple4 const&, DTuple4 const&, double);
    template DTuple4 SlerpEstimate<double, 4, 4>(double, DTuple4 const&, DTuple4 const&, DTuple4 const&, double);
    template DTuple4 SlerpEstimate<double, 4, 5>(double, DTuple4 const&, DTuple4 const&, DTuple4 const&, double);
    template DTuple4 SlerpEstimate<double, 4, 6>(double, DTuple4 const&, DTuple4 const&, DTuple4 const&, double);
    template DTuple4 SlerpEstimate<double, 4, 7>(double, DTuple4 const&, DTuple4 const&, DTuple4 const&, double);
    template DTuple4 SlerpEstimate<double, 4, 8>(double, DTuple4 const&, DTuple4 const&, DTuple4 const&, double);
    template DTuple4 SlerpEstimate<double, 4, 9>(double, DTuple4 const&, DTuple4 const&, DTuple4 const&, double);
    template DTuple4 SlerpEstimate<double, 4, 10>(double, DTuple4 const&, DTuple4 const&, DTuple4 const&, double);
    template DTuple4 SlerpEstimate<double, 4, 11>(double, DTuple4 const&, DTuple4 const&, DTuple4 const&, double);
    template DTuple4 SlerpEstimate<double, 4, 12>(double, DTuple4 const&, DTuple4 const&, DTuple4 const&, double);
    template DTuple4 SlerpEstimate<double, 4, 13>(double, DTuple4 const&, DTuple4 const&, DTuple4 const&, double);
    template DTuple4 SlerpEstimate<double, 4, 14>(double, DTuple4 const&, DTuple4 const&, DTuple4 const&, double);
    template DTuple4 SlerpEstimate<double, 4, 15>(double, DTuple4 const&, DTuple4 const&, DTuple4 const&, double);
    template DTuple4 SlerpEstimate<double, 4, 16>(double, DTuple4 const&, DTuple4 const&, DTuple4 const&, double);

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;

    using RTuple4 = std::array<Rational, 4>;

    template RTuple4 SlerpEstimate<Rational, 4, 1>(Rational, RTuple4 const&, RTuple4 const&);
    template RTuple4 SlerpEstimate<Rational, 4, 2>(Rational, RTuple4 const&, RTuple4 const&);
    template RTuple4 SlerpEstimate<Rational, 4, 3>(Rational, RTuple4 const&, RTuple4 const&);
    template RTuple4 SlerpEstimate<Rational, 4, 4>(Rational, RTuple4 const&, RTuple4 const&);
    template RTuple4 SlerpEstimate<Rational, 4, 5>(Rational, RTuple4 const&, RTuple4 const&);
    template RTuple4 SlerpEstimate<Rational, 4, 6>(Rational, RTuple4 const&, RTuple4 const&);
    template RTuple4 SlerpEstimate<Rational, 4, 7>(Rational, RTuple4 const&, RTuple4 const&);
    template RTuple4 SlerpEstimate<Rational, 4, 8>(Rational, RTuple4 const&, RTuple4 const&);
    template RTuple4 SlerpEstimate<Rational, 4, 9>(Rational, RTuple4 const&, RTuple4 const&);
    template RTuple4 SlerpEstimate<Rational, 4, 10>(Rational, RTuple4 const&, RTuple4 const&);
    template RTuple4 SlerpEstimate<Rational, 4, 11>(Rational, RTuple4 const&, RTuple4 const&);
    template RTuple4 SlerpEstimate<Rational, 4, 12>(Rational, RTuple4 const&, RTuple4 const&);
    template RTuple4 SlerpEstimate<Rational, 4, 13>(Rational, RTuple4 const&, RTuple4 const&);
    template RTuple4 SlerpEstimate<Rational, 4, 14>(Rational, RTuple4 const&, RTuple4 const&);
    template RTuple4 SlerpEstimate<Rational, 4, 15>(Rational, RTuple4 const&, RTuple4 const&);
    template RTuple4 SlerpEstimate<Rational, 4, 16>(Rational, RTuple4 const&, RTuple4 const&);

    template RTuple4 SlerpEstimate<Rational, 4, 1>(Rational, RTuple4 const&, RTuple4 const&, Rational);
    template RTuple4 SlerpEstimate<Rational, 4, 2>(Rational, RTuple4 const&, RTuple4 const&, Rational);
    template RTuple4 SlerpEstimate<Rational, 4, 3>(Rational, RTuple4 const&, RTuple4 const&, Rational);
    template RTuple4 SlerpEstimate<Rational, 4, 4>(Rational, RTuple4 const&, RTuple4 const&, Rational);
    template RTuple4 SlerpEstimate<Rational, 4, 5>(Rational, RTuple4 const&, RTuple4 const&, Rational);
    template RTuple4 SlerpEstimate<Rational, 4, 6>(Rational, RTuple4 const&, RTuple4 const&, Rational);
    template RTuple4 SlerpEstimate<Rational, 4, 7>(Rational, RTuple4 const&, RTuple4 const&, Rational);
    template RTuple4 SlerpEstimate<Rational, 4, 8>(Rational, RTuple4 const&, RTuple4 const&, Rational);
    template RTuple4 SlerpEstimate<Rational, 4, 9>(Rational, RTuple4 const&, RTuple4 const&, Rational);
    template RTuple4 SlerpEstimate<Rational, 4, 10>(Rational, RTuple4 const&, RTuple4 const&, Rational);
    template RTuple4 SlerpEstimate<Rational, 4, 11>(Rational, RTuple4 const&, RTuple4 const&, Rational);
    template RTuple4 SlerpEstimate<Rational, 4, 12>(Rational, RTuple4 const&, RTuple4 const&, Rational);
    template RTuple4 SlerpEstimate<Rational, 4, 13>(Rational, RTuple4 const&, RTuple4 const&, Rational);
    template RTuple4 SlerpEstimate<Rational, 4, 14>(Rational, RTuple4 const&, RTuple4 const&, Rational);
    template RTuple4 SlerpEstimate<Rational, 4, 15>(Rational, RTuple4 const&, RTuple4 const&, Rational);
    template RTuple4 SlerpEstimate<Rational, 4, 16>(Rational, RTuple4 const&, RTuple4 const&, Rational);

    template RTuple4 SlerpEstimate<Rational, 4, 1>(Rational, RTuple4 const&, RTuple4 const&, RTuple4 const&, Rational);
    template RTuple4 SlerpEstimate<Rational, 4, 2>(Rational, RTuple4 const&, RTuple4 const&, RTuple4 const&, Rational);
    template RTuple4 SlerpEstimate<Rational, 4, 3>(Rational, RTuple4 const&, RTuple4 const&, RTuple4 const&, Rational);
    template RTuple4 SlerpEstimate<Rational, 4, 4>(Rational, RTuple4 const&, RTuple4 const&, RTuple4 const&, Rational);
    template RTuple4 SlerpEstimate<Rational, 4, 5>(Rational, RTuple4 const&, RTuple4 const&, RTuple4 const&, Rational);
    template RTuple4 SlerpEstimate<Rational, 4, 6>(Rational, RTuple4 const&, RTuple4 const&, RTuple4 const&, Rational);
    template RTuple4 SlerpEstimate<Rational, 4, 7>(Rational, RTuple4 const&, RTuple4 const&, RTuple4 const&, Rational);
    template RTuple4 SlerpEstimate<Rational, 4, 8>(Rational, RTuple4 const&, RTuple4 const&, RTuple4 const&, Rational);
    template RTuple4 SlerpEstimate<Rational, 4, 9>(Rational, RTuple4 const&, RTuple4 const&, RTuple4 const&, Rational);
    template RTuple4 SlerpEstimate<Rational, 4, 10>(Rational, RTuple4 const&, RTuple4 const&, RTuple4 const&, Rational);
    template RTuple4 SlerpEstimate<Rational, 4, 11>(Rational, RTuple4 const&, RTuple4 const&, RTuple4 const&, Rational);
    template RTuple4 SlerpEstimate<Rational, 4, 12>(Rational, RTuple4 const&, RTuple4 const&, RTuple4 const&, Rational);
    template RTuple4 SlerpEstimate<Rational, 4, 13>(Rational, RTuple4 const&, RTuple4 const&, RTuple4 const&, Rational);
    template RTuple4 SlerpEstimate<Rational, 4, 14>(Rational, RTuple4 const&, RTuple4 const&, RTuple4 const&, Rational);
    template RTuple4 SlerpEstimate<Rational, 4, 15>(Rational, RTuple4 const&, RTuple4 const&, RTuple4 const&, Rational);
    template RTuple4 SlerpEstimate<Rational, 4, 16>(Rational, RTuple4 const&, RTuple4 const&, RTuple4 const&, Rational);
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(SlerpEstimate)
