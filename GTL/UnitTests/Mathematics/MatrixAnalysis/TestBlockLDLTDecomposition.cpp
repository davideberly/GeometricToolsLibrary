// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/MatrixAnalysis/BlockLDLTDecomposition.h>
#include <GTL/Mathematics/Algebra/RigidMotion.h>
#include <random>
using namespace gtl;

namespace gtl
{
    class UnitTestBlockLDLTDecomposition
    {
    public:
        UnitTestBlockLDLTDecomposition();

    private:
        void TestFactorCT();
        void TestSolveCT();
        void TestFactorRT();
        void TestSolveRT();
    };
}

UnitTestBlockLDLTDecomposition::UnitTestBlockLDLTDecomposition()
{
    UTInformation("Mathematics/MatrixAnalysis/BlockLDLTDecomposition");

    TestFactorCT();
    TestSolveCT();
    TestFactorRT();
    TestSolveRT();
}

void UnitTestBlockLDLTDecomposition::TestFactorCT()
{
    Matrix<double, 4, 4> A;
    A(0, 0) = 11.97;
    A(0, 1) = -2.67;
    A(0, 2) = -3.92;
    A(0, 3) = 0.0001;
    A(1, 0) = A(0, 1);
    A(1, 1) = 6.53;
    A(1, 2) = -0.23;
    A(1, 3) = 0.0002;
    A(2, 0) = A(0, 2);
    A(2, 1) = A(1, 2);
    A(2, 2) = 5.38;
    A(2, 3) = 0.0003;
    A(3, 0) = A(0, 3);
    A(3, 1) = A(1, 3);
    A(3, 2) = A(2, 3);
    A(3, 3) = 1.11;

    using Decomposer = BlockLDLTDecomposition<double, 2, 2>;
    Decomposer decomposer;
    Decomposer::BlockMatrix ABlock;
    decomposer.Convert(A, ABlock);

    Decomposer::BlockMatrix LBlock, DBlock;
    decomposer.Factor(ABlock, LBlock, DBlock);

    Matrix<double, 4, 4> L, D;
    decomposer.Convert(LBlock, L);
    decomposer.Convert(DBlock, D);

    Matrix<double, 4, 4> test = L * D * Transpose(L) - A;
    UTAssert(LInfinityNorm(test) < 1e-15, "Factor failed.");
}

void UnitTestBlockLDLTDecomposition::TestSolveCT()
{
    Matrix<double, 4, 4> A;
    A(0, 0) = 11.97;
    A(0, 1) = -2.67;
    A(0, 2) = -3.92;
    A(0, 3) = 0.0001;
    A(1, 0) = A(0, 1);
    A(1, 1) = 6.53;
    A(1, 2) = -0.23;
    A(1, 3) = 0.0002;
    A(2, 0) = A(0, 2);
    A(2, 1) = A(1, 2);
    A(2, 2) = 5.38;
    A(2, 3) = 0.0003;
    A(3, 0) = A(0, 3);
    A(3, 1) = A(1, 3);
    A(3, 2) = A(2, 3);
    A(3, 3) = 1.11;

    using Decomposer = BlockLDLTDecomposition<double, 2, 2>;
    Decomposer decomposer;
    Decomposer::BlockMatrix ABlock;
    decomposer.Convert(A, ABlock);

    Vector<double, 4> B{ 1.0, 2.0, 3.0, 4.0 }, X;
    Decomposer::BlockVector BBlock, XBlock;
    decomposer.Convert(B, BBlock);
    decomposer.Convert(X, XBlock);

    decomposer.Solve(ABlock, BBlock, XBlock);
    decomposer.Convert(XBlock, X);

    Vector<double, 4> vtest = A * X - B;
    UTAssert(Length(vtest) < 1e-14, "Factor failed.");

    Decomposer::BlockMatrix LBlock, DBlock;
    decomposer.Factor(ABlock, LBlock, DBlock);
    decomposer.Solve(LBlock, DBlock, BBlock, XBlock);
    Matrix<double, 4, 4> L, D;
    decomposer.Convert(LBlock, L);
    decomposer.Convert(DBlock, D);
    decomposer.Convert(XBlock, X);
    vtest = A * X - B;
    UTAssert(Length(vtest) < 1e-14, "Factor-Solve failed.");
}

void UnitTestBlockLDLTDecomposition::TestFactorRT()
{
    Matrix<double> A(4, 4);
    A(0, 0) = 11.97;
    A(0, 1) = -2.67;
    A(0, 2) = -3.92;
    A(0, 3) = 0.0001;
    A(1, 0) = A(0, 1);
    A(1, 1) = 6.53;
    A(1, 2) = -0.23;
    A(1, 3) = 0.0002;
    A(2, 0) = A(0, 2);
    A(2, 1) = A(1, 2);
    A(2, 2) = 5.38;
    A(2, 3) = 0.0003;
    A(3, 0) = A(0, 3);
    A(3, 1) = A(1, 3);
    A(3, 2) = A(2, 3);
    A(3, 3) = 1.11;

    using Decomposer = BlockLDLTDecomposition<double>;
    Decomposer decomposer(2, 2);
    Decomposer::BlockMatrix ABlock;
    decomposer.Convert(A, ABlock);

    Decomposer::BlockMatrix LBlock, DBlock;
    decomposer.Factor(ABlock, LBlock, DBlock);

    Matrix<double> L, D;
    decomposer.Convert(LBlock, L);
    decomposer.Convert(DBlock, D);

    Matrix<double> test = L * D * Transpose(L) - A;
    UTAssert(LInfinityNorm(test) < 1e-15, "Factor failed.");
}

void UnitTestBlockLDLTDecomposition::TestSolveRT()
{
    Matrix<double> A(4, 4);
    A(0, 0) = 11.97;
    A(0, 1) = -2.67;
    A(0, 2) = -3.92;
    A(0, 3) = 0.0001;
    A(1, 0) = A(0, 1);
    A(1, 1) = 6.53;
    A(1, 2) = -0.23;
    A(1, 3) = 0.0002;
    A(2, 0) = A(0, 2);
    A(2, 1) = A(1, 2);
    A(2, 2) = 5.38;
    A(2, 3) = 0.0003;
    A(3, 0) = A(0, 3);
    A(3, 1) = A(1, 3);
    A(3, 2) = A(2, 3);
    A(3, 3) = 1.11;

    using Decomposer = BlockLDLTDecomposition<double>;
    Decomposer decomposer(2, 2);
    Decomposer::BlockMatrix ABlock;
    decomposer.Convert(A, ABlock);

    Vector<double> B(4);
    B[0] = 1.0;
    B[1] = 2.0;
    B[2] = 3.0;
    B[3] = 4.0;
    Decomposer::BlockVector BBlock, XBlock;
    decomposer.Convert(B, BBlock);

    decomposer.Solve(ABlock, BBlock, XBlock);
    Vector<double> X;
    decomposer.Convert(XBlock, X);

    Vector<double> vtest = A * X - B;
    UTAssert(Length(vtest) < 1e-14, "Factor failed.");

    Decomposer::BlockMatrix LBlock, DBlock;
    decomposer.Factor(ABlock, LBlock, DBlock);
    decomposer.Solve(LBlock, DBlock, BBlock, XBlock);
    Matrix<double> L, D;
    decomposer.Convert(LBlock, L);
    decomposer.Convert(DBlock, D);
    decomposer.Convert(XBlock, X);
    vtest = A * X - B;
    UTAssert(Length(vtest) < 1e-14, "Factor-Solve failed.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/MatrixAnalysis/BlockLDLTDecomposition.h>

namespace gtl
{
    template class BlockLDLTDecomposition<float, 2, 4>;
    template class BlockLDLTDecomposition<float>;

    template class BlockLDLTDecomposition<double, 2, 4>;
    template class BlockLDLTDecomposition<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class BlockLDLTDecomposition<Rational, 2, 4>;
    template class BlockLDLTDecomposition<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(BlockLDLTDecomposition)
