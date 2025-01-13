// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Functions/Slerp.h>
using namespace gtl;

namespace gtl
{
    class UnitTestSlerp
    {
    public:
        UnitTestSlerp();
    private:
        std::array<double, 2> SquareRoot(std::array<double, 2> const& q0,
            std::array<double, 2> const& q1);

        std::array<double, 2> FourthRoot(std::array<double, 2> const& q0,
            std::array<double, 2> const& q1);

        void Test();
    };
}

UnitTestSlerp::UnitTestSlerp()
{
    UTInformation("Mathematics/Functions/Slerp");

    Test();
}

std::array<double, 2> UnitTestSlerp::SquareRoot(std::array<double, 2> const& q0,
    std::array<double, 2> const& q1)
{
    std::array<double, 2> half = { q0[0] + q1[0], q0[1] + q1[1] };
    double length = std::sqrt(half[0] * half[0] + half[1] * half[1]);
    half[0] /= length;
    half[1] /= length;
    return half;
}

std::array<double, 2> UnitTestSlerp::FourthRoot(std::array<double, 2> const& q0,
    std::array<double, 2> const& q1)
{
    std::array<double, 2> half = SquareRoot(q0, q1);
    std::array<double, 2> quarter = SquareRoot(q0, half);
    return quarter;
}

void UnitTestSlerp::Test()
{
    double t, angle, cosA, cosAH, length, difflen;
    std::array<double, 2> q0, q1, qh, s, fourth, diff;

    // experiment 1
    t = 0.25;
    angle = 0.123;
    q0[0] = std::cos(angle);
    q0[1] = std::sin(angle);
    angle = 0.456;
    q1[0] = std::cos(angle);
    q1[1] = std::sin(angle);
    cosA = q0[0] * q1[0] + q0[1] * q1[1];
    fourth = FourthRoot(q0, q1);

    s = Slerp(t, q0, q1);
    UTAssert(s[0] == 0.97880576074459469 && s[1] == 0.20479082677989963, "Invalid slerp.");
    length = std::sqrt(s[0] * s[0] + s[1] * s[1]);
    difflen = std::fabs(1.0 - length);
    UTAssert(difflen == 2.2204460492503131e-16, "Invalid slerp length.");
    diff[0] = fourth[0] - s[0];
    diff[1] = fourth[1] - s[1];
    length = std::sqrt(diff[0] * diff[0] + diff[1] * diff[1]);
    UTAssert(length == 2.2887833992611187e-16, "Invalid slerp angle.");

    s = Slerp(t, q0, q1, cosA);
    UTAssert(s[0] == 0.97880576074459469 && s[1] == 0.20479082677989963, "Invalid slerp.");
    length = std::sqrt(s[0] * s[0] + s[1] * s[1]);
    difflen = std::fabs(1.0 - length);
    UTAssert(difflen == 2.2204460492503131e-16, "Invalid slerp length.");
    diff[0] = fourth[0] - s[0];
    diff[1] = fourth[1] - s[1];
    length = std::sqrt(diff[0] * diff[0] + diff[1] * diff[1]);
    UTAssert(length == 2.2887833992611187e-16, "Invalid slerp angle.");

    cosAH = std::sqrt((1.0 + cosA) / 2.0);
    qh[0] = (q0[0] + q1[0]) / (2.0 * cosAH);
    qh[1] = (q0[1] + q1[1]) / (2.0 * cosAH);
    s = Slerp(t, q0, q1, qh, cosAH);
    UTAssert(s[0] == 0.97880576074459469 && s[1] == 0.20479082677989963, "Invalid slerp.");
    length = std::sqrt(s[0] * s[0] + s[1] * s[1]);
    difflen = std::fabs(1.0 - length);
    UTAssert(difflen == 2.2204460492503131e-16, "Invalid slerp length.");
    diff[0] = fourth[0] - s[0];
    diff[1] = fourth[1] - s[1];
    length = std::sqrt(diff[0] * diff[0] + diff[1] * diff[1]);
    UTAssert(length == 2.2887833992611187e-16, "Invalid slerp angle.");

    // experiment 2
    angle = 0.123;
    q0[0] = std::cos(angle);
    q0[1] = std::sin(angle);
    angle = 2.123;
    q1[0] = std::cos(angle);
    q1[1] = std::sin(angle);
    cosA = q0[0] * q1[0] + q0[1] * q1[1];
    fourth = FourthRoot(q0, q1);

    s = Slerp(t, q0, q1);
    UTAssert(s[0] == 0.81213169134527075 && s[1] == 0.58347417758857978, "Invalid slerp.");
    length = std::sqrt(s[0] * s[0] + s[1] * s[1]);
    difflen = std::fabs(1.0 - length);
    UTAssert(difflen == 2.2204460492503131e-16, "Invalid slerp length.");
    diff[0] = fourth[0] - s[0];
    diff[1] = fourth[1] - s[1];
    length = std::sqrt(diff[0] * diff[0] + diff[1] * diff[1]);
    UTAssert(length == 1.5700924586837752e-16, "Invalid slerp angle.");

    s = Slerp(t, q0, q1, cosA);
    UTAssert(s[0] == 0.81213169134527075 && s[1] == 0.58347417758857978, "Invalid slerp.");
    length = std::sqrt(s[0] * s[0] + s[1] * s[1]);
    difflen = std::fabs(1.0 - length);
    UTAssert(difflen == 2.2204460492503131e-16, "Invalid slerp length.");
    diff[0] = fourth[0] - s[0];
    diff[1] = fourth[1] - s[1];
    length = std::sqrt(diff[0] * diff[0] + diff[1] * diff[1]);
    UTAssert(length == 1.5700924586837752e-16, "Invalid slerp angle.");

    cosAH = std::sqrt((1.0 + cosA) / 2.0);
    qh[0] = (q0[0] + q1[0]) / (2.0 * cosAH);
    qh[1] = (q0[1] + q1[1]) / (2.0 * cosAH);
    s = Slerp(t, q0, q1, qh, cosAH);
    UTAssert(s[0] == 0.81213169134527097 && s[1] == 0.58347417758857989, "Invalid slerp.");
    length = std::sqrt(s[0] * s[0] + s[1] * s[1]);
    difflen = std::fabs(1.0 - length);
    UTAssert(difflen == 0.0, "Invalid slerp length.");
    diff[0] = fourth[0] - s[0];
    diff[1] = fourth[1] - s[1];
    length = std::sqrt(diff[0] * diff[0] + diff[1] * diff[1]);
    UTAssert(length == 1.1102230246251565e-16, "Invalid slerp angle.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Functions/Slerp.h>

namespace gtl
{
    using FTuple4 = std::array<float, 4>;
    template FTuple4 Slerp(float, FTuple4 const&, FTuple4 const&);
    template FTuple4 Slerp(float, FTuple4 const&, FTuple4 const&, float);
    template FTuple4 Slerp(float, FTuple4 const&, FTuple4 const&, FTuple4 const&, float);

    using DTuple4 = std::array<double, 4>;
    template DTuple4 Slerp(double, DTuple4 const&, DTuple4 const&);
    template DTuple4 Slerp(double, DTuple4 const&, DTuple4 const&, double);
    template DTuple4 Slerp(double, DTuple4 const&, DTuple4 const&, DTuple4 const&, double);

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;

    using RTuple4 = std::array<Rational, 4>;
    template RTuple4 Slerp(Rational, RTuple4 const&, RTuple4 const&);
    template RTuple4 Slerp(Rational, RTuple4 const&, RTuple4 const&, Rational);
    template RTuple4 Slerp(Rational, RTuple4 const&, RTuple4 const&, RTuple4 const&, Rational);
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(Slerp)
