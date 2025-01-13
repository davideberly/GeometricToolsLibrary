// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/MatrixAnalysis/SingularValueDecomposition.h>
#include <GTL/Mathematics/Algebra/Matrix.h>
#include <random>
using namespace gtl;

namespace gtl
{
    class UnitTestSingularValueDecomposition
    {
    public:
        UnitTestSingularValueDecomposition();

    private:
        using SVD = SingularValueDecomposition<double>;

        void Test0();
        void Test1();
        void Test2();
        void Test3();
        void Test4();
    };
}

UnitTestSingularValueDecomposition::UnitTestSingularValueDecomposition()
{
    UTInformation("Mathematics/MatrixAnalysis/SingularValueDecomposition");

    Test0();
    Test1();
    Test2();
    Test3();
    Test4();
}

void UnitTestSingularValueDecomposition::Test0()
{
    double const maxError = 1e-14;
    Matrix<double, 5, 4> A{};
    A(0, 0) = -0.72904599140643900;
    A(0, 1) = 0.67001717998915900;
    A(0, 2) = 0.93773554224846278;
    A(0, 3) = -0.55793191403459019;
    A(1, 0) = -0.38366589898599346;
    A(1, 1) = 0.094441192735703838;
    A(1, 2) = -0.62323604790563780;
    A(1, 3) = 0.98576260383561332;
    A(2, 0) = 0.99292265109601741;
    A(2, 1) = 0.93538987402100515;
    A(2, 2) = 0.45167792642377935;
    A(2, 3) = 0.96221938355387793;
    A(3, 0) = -0.78027649831158719;
    A(3, 1) = 0.59621171349909896;
    A(3, 2) = -0.40594110088409829;
    A(3, 3) = -0.99043303116136872;
    A(4, 0) = -0.77507096788763941;
    A(4, 1) = 0.27952671419630559;
    A(4, 2) = 0.75686129079768771;
    A(4, 3) = 0.0073253554103394070;

    SVD svd(5, 4, 100);
    svd.Solve(&A[0]);
    Matrix<double, 5, 5> U{};
    Matrix<double, 4, 4> V{};
    Matrix<double, 5, 4> S{};
    svd.GetU(&U[0]);
    svd.GetV(&V[0]);
    svd.GetS(&S[0]);
    Matrix<double, 5, 4> E = MultiplyATB(U, A * V) - S;
    double normE = L2Norm(E);
    UTAssert(normE <= maxError, "Error is too large.");
}

void UnitTestSingularValueDecomposition::Test1()
{
    double const maxError = 1e-14;
    double t = 30.0 * C_DEG_TO_RAD<double>;
    Matrix<double, 3, 3> A{};
    A(0, 0) = 1.0;
    A(0, 1) = 0.0;
    A(0, 2) = 0.0;
    A(1, 0) = 0.0;
    A(1, 1) = std::cos(t);
    A(1, 2) = -std::sin(t);
    A(2, 0) = 0.0;
    A(2, 1) = std::sin(t);
    A(2, 2) = std::cos(t);

    SVD svd(3, 3, 100);
    svd.Solve(&A[0]);
    Matrix<double, 3, 3> U{}, V{}, S{};
    svd.GetU(&U[0]);
    svd.GetV(&V[0]);
    svd.GetS(&S[0]);
    Matrix3x3<double> E = MultiplyATB(U, A * V) - S;
    double normE = L2Norm(E);
    UTAssert(normE <= maxError, "Error is too large.");
}

void UnitTestSingularValueDecomposition::Test2()
{
    double const maxError = 1e-14;
    Matrix<double, 5, 4> A{};
    A(0, 0) = 1.6953375444936460;
    A(0, 1) = 1.0575057494864246;
    A(1, 1) = -1.4615937540752135;
    A(1, 2) = -0.21009638074068984;
    A(2, 2) = 1.1751139893655338;
    A(2, 3) = -0.66192245329076516;
    A(3, 3) = -1.4310678276027826;

    SVD svd(5, 4, 100);
    svd.Solve(&A[0]);
    Matrix<double, 5, 5> U{};
    Matrix<double, 4, 4> V{};
    Matrix<double, 5, 4> S{};
    svd.GetU(&U[0]);
    svd.GetV(&V[0]);
    svd.GetS(&S[0]);
    Matrix<double, 5, 4> E = Transpose(U) * A * V - S;
    double normE = L2Norm(E);
    UTAssert(normE <= maxError, "Error is too large.");
}

// One of the superdiagonal entries is 0.
void UnitTestSingularValueDecomposition::Test3()
{
    double const maxError = 1e-14;
    Matrix<double, 6, 6> A{};
    A(0, 0) = -0.72904599140643900;
    A(0, 1) = 0.67001717998915900;
    A(1, 1) = 0.93773554224846278;
    A(1, 2) = -0.55793191403459019;
    A(2, 2) = -0.38366589898599346;
    A(2, 3) = 0.0;
    A(3, 3) = -0.62323604790563780;
    A(3, 4) = 0.98576260383561332;
    A(4, 4) = 0.99292265109601741;
    A(4, 5) = 0.93538987402100515;
    A(5, 5) = 0.45167792642377935;

    SVD svd(6, 6, 100);
    svd.Solve(&A[0]);
    Matrix<double, 6, 6> U{};
    Matrix<double, 6, 6> V{};
    Matrix<double, 6, 6> S{};
    svd.GetU(&U[0]);
    svd.GetV(&V[0]);
    svd.GetS(&S[0]);
    Matrix<double, 6, 6> E = Transpose(U) * A * V - S;
    double normE = L2Norm(E);
    UTAssert(normE <= maxError, "Error is too large.");
}

// One of the diagonal entries is 0.
void UnitTestSingularValueDecomposition::Test4()
{
    double const maxError = 1e-14;
    Matrix<double, 6, 6> A{};
    A(0, 0) = -0.72904599140643900;
    A(0, 1) = 0.67001717998915900;
    A(1, 1) = 0.93773554224846278;
    A(1, 2) = -0.55793191403459019;
    A(2, 2) = -0.38366589898599346;
    A(2, 3) = 0.094441192735703838;
    A(3, 3) = 0.0;
    A(3, 4) = 0.98576260383561332;
    A(4, 4) = 0.99292265109601741;
    A(4, 5) = 0.93538987402100515;
    A(5, 5) = 0.45167792642377935;

    SVD svd(6, 6, 100);
    svd.Solve(&A[0]);
    Matrix<double, 6, 6> U{};
    Matrix<double, 6, 6> V{};
    Matrix<double, 6, 6> S{};
    svd.GetU(&U[0]);
    svd.GetV(&V[0]);
    svd.GetS(&S[0]);
    Matrix<double, 6, 6> E = Transpose(U) * A * V - S;
    double normE = L2Norm(E);
    UTAssert(normE <= maxError, "Error is too large.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/MatrixAnalysis/SingularValueDecomposition.h>

namespace gtl
{
    template class SingularValueDecomposition<float>;
    template class SingularValueDecomposition<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class SingularValueDecomposition<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(SingularValueDecomposition)
