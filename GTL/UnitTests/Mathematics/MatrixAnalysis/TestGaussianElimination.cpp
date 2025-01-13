// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/MatrixAnalysis/GaussianElimination.h>
using namespace gtl;

namespace gtl
{
    class UnitTestGaussianElimination
    {
    public:
        UnitTestGaussianElimination();

    private:
        void TestSolveSystem();
        void TestGetInverse();
        void TestGetDeterminant();
    };
}

UnitTestGaussianElimination::UnitTestGaussianElimination()
{
    UTInformation("Mathematics/MatrixAnalysis/GaussianElimination");

    TestSolveSystem();
    TestGetInverse();
    TestGetDeterminant();
    // GetInverseAndDeterminant(...) called indirectly in TestGetInverse().
}

void UnitTestGaussianElimination::TestSolveSystem()
{
    Matrix<double, 4, 4> A;
    Vector<double, 4> B, X, test;
    double error;

    A(0, 0) = -0.72904599140643900;
    A(0, 1) = 0.67001717998915900;
    A(0, 2) = 0.93773554224846278;
    A(0, 3) = -0.55793191403459019;
    A(1, 0) = 0.094441192735703838;
    A(1, 1) = -0.62323604790563780;
    A(1, 2) = 0.98576260383561332;
    A(1, 3) = 0.99292265109601741;
    A(2, 0) = 0.45167792642377935;
    A(2, 1) = 0.96221938355387793;
    A(2, 2) = -0.78027649831158719;
    A(2, 3) = 0.59621171349909896;
    A(3, 0) = -0.99043303116136872;
    A(3, 1) = -0.77507096788763941;
    A(3, 2) = 0.27952671419630559;
    A(3, 3) = 0.75686129079768771;
    B[0] = -0.38366589898599346;
    B[1] = 0.93538987402100515;
    B[2] = -0.40594110088409829;
    B[3] = 0.0073253554103394070;

    bool success = SolveSystem(A, B, X);
    UTAssert(
        success,
        "Expecting the system to have a solution.");

    test = A * X - B;
    error = Length(test);  // 9.6148134319178191e-17
    UTAssert(
        error <= 1e-16,
        "Expecting small numerical round-off errors.");

    Matrix<double, 4, 2> C, Y, test2;
    C(0, 0) = B[0];
    C(1, 0) = B[1];
    C(2, 0) = B[2];
    C(3, 0) = B[3];
    C(0, 1) = -0.72904599140643900;
    C(1, 1) = 0.67001717998915900;
    C(2, 1) = 0.93773554224846278;
    C(3, 1) = -0.55793191403459019;

    success = SolveSystem(A, C, Y);
    UTAssert(
        success,
        "Expecting the system to have a solution.");

    test2 = A * Y - C;
    error = L2Norm(test2);  // 2.6622212897024595e-16
    UTAssert(
        error <= 1e-15,
        "Expecting small numerical round-off errors.");

    MakeDiagonal({ 1.0, 2.0, 3.0, 0.0 }, A);
    success = SolveSystem(A, B, X);
    UTAssert(
        !success,
        "The matrix is supposed to be singular.");
}

void UnitTestGaussianElimination::TestGetInverse()
{
    Matrix<double, 4, 4> A, invA;
    double determinant = 0.0, error = 0.0;

    A(0, 0) = -0.72904599140643900;
    A(0, 1) = 0.67001717998915900;
    A(0, 2) = 0.93773554224846278;
    A(0, 3) = -0.55793191403459019;
    A(1, 0) = 0.094441192735703838;
    A(1, 1) = -0.62323604790563780;
    A(1, 2) = 0.98576260383561332;
    A(1, 3) = 0.99292265109601741;
    A(2, 0) = 0.45167792642377935;
    A(2, 1) = 0.96221938355387793;
    A(2, 2) = -0.78027649831158719;
    A(2, 3) = 0.59621171349909896;
    A(3, 0) = -0.99043303116136872;
    A(3, 1) = -0.77507096788763941;
    A(3, 2) = 0.27952671419630559;
    A(3, 3) = 0.75686129079768771;

    invA = Inverse(A, &determinant);
    UTAssert(
        determinant != 0.0,
        "Expecting the matrix to have an inverse.");

    Matrix<double, 4, 4> identity;
    MakeIdentity(identity);
    Matrix<double, 4, 4> test = A * invA - identity;
    error = L2Norm(test);  // 3.6802312880990560e-16
    UTAssert(
        error <= 1e-15,
        "Expecting small numerical round-off errors.");
}

void UnitTestGaussianElimination::TestGetDeterminant()
{
    Matrix<double, 4, 4> A;
    double determinant = 0.0, error = 0.0;

    A(0, 0) = -0.72904599140643900;
    A(0, 1) = 0.67001717998915900;
    A(0, 2) = 0.93773554224846278;
    A(0, 3) = -0.55793191403459019;
    A(1, 0) = 0.094441192735703838;
    A(1, 1) = -0.62323604790563780;
    A(1, 2) = 0.98576260383561332;
    A(1, 3) = 0.99292265109601741;
    A(2, 0) = 0.45167792642377935;
    A(2, 1) = 0.96221938355387793;
    A(2, 2) = -0.78027649831158719;
    A(2, 3) = 0.59621171349909896;
    A(3, 0) = -0.99043303116136872;
    A(3, 1) = -0.77507096788763941;
    A(3, 2) = 0.27952671419630559;
    A(3, 3) = 0.75686129079768771;

    determinant = Determinant(A);
    UTAssert(
        determinant != 0.0,
        "Expecting the matrix to have nonzero determinant.");
    error = std::fabs(determinant - 3.1491686039589450);  // 0.0
    UTAssert(
        error <= 1e-15,
        "Expecting small numerical round-off errors.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/MatrixAnalysis/GaussianElimination.h>

namespace gtl
{
    using MF32 = Matrix<float, 3, 2>;
    using MF33 = Matrix<float, 3, 3>;
    using MFD = Matrix<float>;
    using VF3 = Vector<float, 3>;
    using VFD = Vector<float>;
    using GEF = GaussianElimination<float>;
    template bool SolveSystem(MF33 const&, VF3 const&, VF3&);
    template bool SolveSystem(MF33 const&, MF32 const&, MF32&);
    template bool SolveSystem(MFD const&, VFD const&, VFD&);
    template bool SolveSystem(MFD const&, MFD const&, MFD&);
    template MF33 Inverse(MF33 const&, float*);
    template MFD Inverse(MFD const&, float*);
    template float Determinant(MF33 const&);
    template float Determinant(MFD const&);

    using MD32 = Matrix<double, 3, 2>;
    using MD33 = Matrix<double, 3, 3>;
    using MDD = Matrix<double>;
    using VD3 = Vector<double, 3>;
    using VDD = Vector<double>;
    using GED = GaussianElimination<double>;
    template bool SolveSystem(MD33 const&, VD3 const&, VD3&);
    template bool SolveSystem(MD33 const&, MD32 const&, MD32&);
    template bool SolveSystem(MDD const&, VDD const&, VDD&);
    template bool SolveSystem(MDD const&, MDD const&, MDD&);
    template MD33 Inverse(MD33 const&, double*);
    template MDD Inverse(MDD const&, double*);
    template double Determinant(MD33 const&);
    template double Determinant(MDD const&);

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    using MR32 = Matrix<Rational, 3, 2>;
    using MR33 = Matrix<Rational, 3, 3>;
    using MRD = Matrix<Rational>;
    using VR3 = Vector<Rational, 3>;
    using VRD = Vector<Rational>;
    using GER = GaussianElimination<Rational>;
    template bool SolveSystem(MR33 const&, VR3 const&, VR3&);
    template bool SolveSystem(MR33 const&, MR32 const&, MR32&);
    template bool SolveSystem(MRD const&, VRD const&, VRD&);
    template bool SolveSystem(MRD const&, MRD const&, MRD&);
    template MR33 Inverse(MR33 const&, Rational*);
    template MRD Inverse(MRD const&, Rational*);
    template Rational Determinant(MR33 const&);
    template Rational Determinant(MRD const&);
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(GaussianElimination)

