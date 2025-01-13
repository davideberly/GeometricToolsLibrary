// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/MatrixAnalysis/LDLTDecomposition.h>
#include <GTL/Mathematics/Algebra/RigidMotion.h>
#include <random>
using namespace gtl;

namespace gtl
{
    class UnitTestLDLTDecomposition
    {
    public:
        UnitTestLDLTDecomposition();

    private:
        void TestFactorCT();
        void TestSolveCT();
        void TestFactorRT();
        void TestSolveRT();
    };
}

UnitTestLDLTDecomposition::UnitTestLDLTDecomposition()
{
    UTInformation("Mathematics/MatrixAnalysis/LDLTDecomposition");

    TestFactorCT();
    TestSolveCT();
    TestFactorRT();
    TestSolveRT();
}

void UnitTestLDLTDecomposition::TestFactorCT()
{
    Matrix<double, 3, 3> A;
    A(0, 0) = 11.97;
    A(0, 1) = -2.67;
    A(0, 2) = -3.92;
    A(1, 0) = A(0, 1);
    A(1, 1) = 6.53;
    A(1, 2) = -0.23;
    A(2, 0) = A(0, 2);
    A(2, 1) = A(1, 2);
    A(2, 2) = 5.38;

    LDLTDecomposition<double, 3> decomposer;
    Matrix<double, 3, 3> L, D;
    decomposer.Factor(A, L, D);
    Matrix<double, 3, 3> test = L * D * Transpose(L) - A;
    UTAssert(LInfinityNorm(test) < 1e-15, "Factor failed.");
}

void UnitTestLDLTDecomposition::TestSolveCT()
{
    Matrix<double, 3, 3> A;
    A(0, 0) = 11.97;
    A(0, 1) = -2.67;
    A(0, 2) = -3.92;
    A(1, 0) = A(0, 1);
    A(1, 1) = 6.53;
    A(1, 2) = -0.23;
    A(2, 0) = A(0, 2);
    A(2, 1) = A(1, 2);
    A(2, 2) = 5.38;

    LDLTDecomposition<double, 3> decomposer;
    Vector<double, 3> B{ 1.0, 2.0, 3.0 }, X;
    decomposer.Solve(A, B, X);
    Vector<double, 3> vtest = A * X - B;
    UTAssert(Length(vtest) < 1e-15, "Factor failed.");

    Matrix<double, 3, 3> L, D;
    decomposer.Factor(A, L, D);
    decomposer.Solve(L, D, B, X);
    vtest = A * X - B;
    UTAssert(Length(vtest) < 1e-15, "Factor failed.");
}

void UnitTestLDLTDecomposition::TestFactorRT()
{
    Matrix<double> A(3, 3);
    A(0, 0) = 11.97;
    A(0, 1) = -2.67;
    A(0, 2) = -3.92;
    A(1, 0) = A(0, 1);
    A(1, 1) = 6.53;
    A(1, 2) = -0.23;
    A(2, 0) = A(0, 2);
    A(2, 1) = A(1, 2);
    A(2, 2) = 5.38;

    LDLTDecomposition<double> decomposer(3);
    Matrix<double> L, D;
    decomposer.Factor(A, L, D);
    Matrix<double> test = L * D * Transpose(L) - A;
    UTAssert(LInfinityNorm(test) < 1e-15, "Factor failed.");
}

void UnitTestLDLTDecomposition::TestSolveRT()
{
    Matrix<double> A(3, 3);
    A(0, 0) = 11.97;
    A(0, 1) = -2.67;
    A(0, 2) = -3.92;
    A(1, 0) = A(0, 1);
    A(1, 1) = 6.53;
    A(1, 2) = -0.23;
    A(2, 0) = A(0, 2);
    A(2, 1) = A(1, 2);
    A(2, 2) = 5.38;

    LDLTDecomposition<double> decomposer(3);
    Vector<double> B(3), X;
    B[0] = 1.0;
    B[1] = 2.0;
    B[2] = 3.0;
    decomposer.Solve(A, B, X);
    Vector<double> vtest = A * X - B;
    UTAssert(Length(vtest) < 1e-15, "Factor failed.");

    Matrix<double> L, D;
    decomposer.Factor(A, L, D);
    decomposer.Solve(L, D, B, X);
    vtest = A * X - B;
    UTAssert(Length(vtest) < 1e-15, "Factor-solve failed.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/MatrixAnalysis/LDLTDecomposition.h>

namespace gtl
{
    template class LDLTDecomposition<float, 2>;
    template class LDLTDecomposition<float>;

    template class LDLTDecomposition<double, 2>;
    template class LDLTDecomposition<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class LDLTDecomposition<Rational, 2>;
    template class LDLTDecomposition<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(LDLTDecomposition)
