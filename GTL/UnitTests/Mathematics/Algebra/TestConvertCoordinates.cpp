// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Algebra/ConvertCoordinates.h>
using namespace gtl;

namespace gtl
{
    class UnitTestConvertCoordinates
    {
    public:
        UnitTestConvertCoordinates();

    private:
        void Test0();
        void Test1();
    };
}

UnitTestConvertCoordinates::UnitTestConvertCoordinates()
{
    UTInformation("Mathematics/Algebra/ConvertCoordinates");

    Test0();
    Test1();
}

void UnitTestConvertCoordinates::Test0()
{
    ConvertCoordinates<double, 3> convert;
    Vector<double, 3> X, Y, P0, P1, diff;
    Matrix<double, 3, 3> U, V, A, B;
    bool isRHU, isRHV;
    U.SetCol(0, Vector<double, 3>{1.0, 0.0, 0.0});
    U.SetCol(1, Vector<double, 3>{0.0, 1.0, 0.0});
    U.SetCol(2, Vector<double, 3>{0.0, 0.0, 1.0});
    V.SetCol(0, Vector<double, 3>{1.0, 0.0, 0.0});
    V.SetCol(1, Vector<double, 3>{0.0, 0.0, 1.0});
    V.SetCol(2, Vector<double, 3>{0.0, 1.0, 0.0});
    convert(U, true, V, true);

    isRHU = convert.IsRightHandedU();
    UTAssert(
        isRHU == true,
        "U-conversion should be right-handed.");

    isRHV = convert.IsRightHandedV();
    UTAssert(
        isRHV == false,
        "V-conversion should be left-handed.");

    X = { 1.0, 2.0, 3.0 };
    Y = convert.UToV(X);
    UTAssert(
        Y[0] == 1.0 && Y[1] == 3.0 && Y[2] == 2.0,
        "Incorrect Y-value.");

    P0 = U*X;
    P1 = V*Y;
    diff = P0 - P1;
    UTAssert(
        diff[0] == 0.0 && diff[1] == 0.0 && diff[2] == 0.0,
        "Incorrect diff-value.");

    Y = { 0.0, 1.0, 2.0 };
    X = convert.VToU(Y);
    UTAssert(
        X[0] == 0.0 && X[1] == 2.0 && X[2] == 1.0,
        "Incorrect X-value.");

    P0 = U*X;
    P1 = V*Y;
    diff = P0 - P1;
    UTAssert(
        diff[0] == 0.0 && diff[1] == 0.0 && diff[2] == 0.0,
        "Incorrect diff-value.");

    double cs = 0.6, sn = 0.8;  // cs*cs + sn*sn = 1
    A.SetCol(0, Vector<double, 3>{  cs,  sn, 0.0});
    A.SetCol(1, Vector<double, 3>{ -sn,  cs, 0.0});
    A.SetCol(2, Vector<double, 3>{ 0.0, 0.0, 1.0});
    B = convert.UToV(A);
    UTAssert(
        (B.GetCol(0) == Vector<double, 3>{ cs, 0.0, sn }),
        "Incorrect B-column 0.");
    UTAssert(
        (B.GetCol(1) == Vector<double, 3>{ 0.0, 1.0, 0.0 }),
        "Incorrect B-column 1.");
    UTAssert(
        (B.GetCol(2) == Vector<double, 3>{ -sn, 0.0, cs }),
        "Incorrect B-column 2.");

    X = A*X;  // U is VOR
    Y = B*Y;  // U is VOR
    P0 = U*X;
    P1 = V*Y;
    diff = P0 - P1;
    UTAssert(
        diff[0] == 0.0 && diff[1] == 0.0 && diff[2] == 0.0,
        "Incorrect diff-value.");
}

void UnitTestConvertCoordinates::Test1()
{
    ConvertCoordinates<double, 4> convert;
    Vector<double, 4> X, Y, P0, P1, diff;
    Matrix<double, 4, 4> U, V, A, B;
    bool isRHU, isRHV;
    U.SetCol(0, Vector<double, 4>{-1.0, 0.0, 0.0, 0.0});
    U.SetCol(1, Vector<double, 4>{0.0, 0.0, 1.0, 0.0});
    U.SetCol(2, Vector<double, 4>{0.0, -1.0, 0.0, 0.0});
    U.SetCol(3, Vector<double, 4>{1.0, 2.0, 3.0, 1.0});
    V.SetCol(0, Vector<double, 4>{0.0, 1.0, 0.0, 0.0});
    V.SetCol(1, Vector<double, 4>{-1.0, 0.0, 0.0, 0.0});
    V.SetCol(2, Vector<double, 4>{0.0, 0.0, 1.0, 0.0});
    V.SetCol(3, Vector<double, 4>{4.0, 5.0, 6.0, 1.0});
    convert(U, true, V, false);

    isRHU = convert.IsRightHandedU();
    UTAssert(
        isRHU == false,
        "U-conversion should be left-handed.");

    isRHV = convert.IsRightHandedV();
    UTAssert(
        isRHV == true,
        "V-conversion should be right-handed.");

    X = { -1.0, 4.0, -3.0, 1.0 };
    Y = convert.UToV(X);
    UTAssert(
        Y[0] == 0.0 && Y[1] == 2.0 && Y[2] == 1.0 && Y[3] == 1.0,
        "Incorrect Y-value.");

    P0 = U*X;
    P1 = V*Y;
    diff = P0 - P1;
    UTAssert(
        diff[0] == 0.0 && diff[1] == 0.0 && diff[2] == 0.0 && diff[3] == 0.0,
        "Incorrect diff-value.");

    Y = { 1.0, 2.0, 3.0, 1.0 };
    X = convert.VToU(Y);
    UTAssert(
        X[0] == -1.0 && X[1] == 6.0 && X[2] == -4.0 && X[3] == 1.0,
        "Incorrect X-value.");

    P0 = U*X;
    P1 = V*Y;
    diff = P0 - P1;
    UTAssert(
        diff[0] == 0.0 && diff[1] == 0.0 && diff[2] == 0.0 && diff[3] == 0.0,
        "Incorrect diff-value.");

    double cs = 0.6, sn = 0.8;  // cs*cs + sn*sn = 1
    A.SetCol(0, Vector<double, 4>{  cs,  sn,   0.0, 0.0});
    A.SetCol(1, Vector<double, 4>{ -sn,  cs,   0.0, 0.0});
    A.SetCol(2, Vector<double, 4>{ 0.0, 0.0,  1.0, 0.0});
    A.SetCol(3, Vector<double, 4>{ 0.3, 1.0, -2.0, 1.0});
    B = convert.UToV(A);
    UTAssert(
        (B.GetCol(0) == Vector<double, 4>{ 1.0, 0.0, 0.0, 2.0 }),
        "Incorrect B-column 0.");
    UTAssert(
        (B.GetCol(1) == Vector<double, 4>{ 0.0, cs, -sn, -0.90000000000000036 }),
        "Incorrect B-column 1.");
    UTAssert(
        (B.GetCol(2) == Vector<double, 4>{ 0.0, sn, cs, -2.6000000000000005 }),
        "Incorrect B-column 2.");
    UTAssert(
        (B.GetCol(3) == Vector<double, 4>{ 0.0, 0.0, 0.0, 1.0 }),
        "Incorrect B-column 2.");
    X = A*X;  // U is VOR
    Y = Y*B;  // V is VOL (not VOR)
    P0 = U*X;
    P1 = V*Y;
    diff = P0 - P1;  // { 0, 0, 0, 0 }
    UTAssert(
        diff[0] == 0.0 && diff[1] == 0.0 && diff[2] == 8.8817841970012523e-16 && diff[3] == 0.0,
        "Incorrect diff-value.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Algebra/ConvertCoordinates.h>

namespace gtl
{
    template class ConvertCoordinates<float, 2>;
    template class ConvertCoordinates<double, 3>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class ConvertCoordinates<Rational, 4>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ConvertCoordinates)
