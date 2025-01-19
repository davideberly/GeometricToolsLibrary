// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/MatrixAnalysis/BlockCholeskyDecomposition.h>
#include <GTL/Mathematics/Algebra/RigidMotion.h>
#include <random>
using namespace gtl;

namespace gtl
{
    class UnitTestBlockCholeskyDecomposition
    {
    public:
        UnitTestBlockCholeskyDecomposition();

    private:
        void TestNInPlace();
        void TestNPreserve();
        void TestDynamicInPlace();
        void TestDynamicPreserve();
    };
}

UnitTestBlockCholeskyDecomposition::UnitTestBlockCholeskyDecomposition()
{
    UTInformation("Mathematics/MatrixAnalysis/BlockCholeskyDecomposition");

    TestNInPlace();
    TestNPreserve();
    TestDynamicInPlace();
    TestDynamicPreserve();
}

void UnitTestBlockCholeskyDecomposition::TestNInPlace()
{
    std::default_random_engine dre;
    std::uniform_real_distribution<double> urd(-1.0, 1.0);
    double error0, error1, error2, error3, error;

    Matrix<double, 4, 4> D, R;
    D(0, 0) = 1.0;
    D(1, 1) = 2.0;
    D(2, 2) = 3.0;
    D(3, 3) = 4.0;
    for (std::size_t i = 0; i < R.size(); ++i)
    {
        R[i] = urd(dre);
    }
    std::vector<Vector<double, 4>> basis(4);
    for (std::size_t i = 0; i < 4; ++i)
    {
        basis[i] = R.GetCol(i);
    }
    Orthonormalize(basis);
    for (std::size_t i = 0; i < 4; ++i)
    {
        R.SetCol(i, basis[i]);
    }

    Matrix<double, 4, 4> A = R * D * Transpose(R);
    for (std::size_t row = 1; row < 4; ++row)
    {
        for (std::size_t col = 0; col < row; ++col)
        {
            A(row, col) = A(col, row);
        }
    }

    using BCD = BlockCholeskyDecomposition<double, 2, 2>;
    BCD decomposer;
    BCD::BlockMatrix Ablock;
    Ablock[0][0] = Matrix<double, 2, 2>{
        { A(0, 0), A(0, 1) },
        { A(1, 0), A(1, 1) } };
    Ablock[0][1] = Matrix<double, 2, 2>{
        { A(0, 2), A(0, 3) },
        { A(1, 2), A(1, 3) } };
    Ablock[1][0] = Matrix<double, 2, 2>{
        { A(2, 0), A(2, 1) },
        { A(3, 0), A(3, 1) } };
    Ablock[1][1] = Matrix<double, 2, 2>{
        { A(2, 2), A(2, 3) },
        { A(3, 2), A(3, 3) } };
    BCD::BlockMatrix saveAblock = Ablock;

    BCD::BlockMatrix convertA;
    decomposer.Convert(A, convertA);
    error0 = L1Norm(Ablock[0][0] - convertA[0][0]);
    error1 = L1Norm(Ablock[0][1] - convertA[0][1]);
    error2 = L1Norm(Ablock[1][0] - convertA[1][0]);
    error3 = L1Norm(Ablock[1][1] - convertA[1][1]);
    error = std::max(std::max(error0, error1), std::max(error2, error3));
    UTAssert(
        error <= 1e-14,
        "Incorrect conversion.");

    Matrix<double, 4, 4> copyA;
    decomposer.Convert(convertA, copyA);
    error = L1Norm(A - copyA);
    UTAssert(
        error <= 1e-14,
        "Incorrect conversion.");

    bool success = decomposer.Factor(Ablock);
    UTAssert(
        success,
        "Factor decomposition failed.");

    BCD::BlockMatrix Lblock;
    Lblock[0][0] = Ablock[0][0];
    Lblock[1][0] = Ablock[1][0];
    Lblock[1][1] = Ablock[1][1];
    Lblock[0][0](0, 1) = 0.0;
    Lblock[1][1](0, 1) = 0.0;

    BCD::BlockMatrix Pblock;
    Pblock[0][0] = Lblock[0][0] * Transpose(Lblock[0][0]) + Lblock[0][1] * Transpose(Lblock[0][1]);
    Pblock[0][1] = Lblock[0][0] * Transpose(Lblock[1][0]) + Lblock[0][1] * Transpose(Lblock[1][1]);
    Pblock[1][0] = Lblock[1][0] * Transpose(Lblock[0][0]) + Lblock[1][1] * Transpose(Lblock[0][1]);
    Pblock[1][1] = Lblock[1][0] * Transpose(Lblock[1][0]) + Lblock[1][1] * Transpose(Lblock[1][1]);

    error0 = L1Norm(Pblock[0][0] - saveAblock[0][0]);
    error1 = L1Norm(Pblock[0][1] - saveAblock[0][1]);
    error2 = L1Norm(Pblock[1][0] - saveAblock[1][0]);
    error3 = L1Norm(Pblock[1][1] - saveAblock[1][1]);
    error = std::max(std::max(error0, error1), std::max(error2, error3));
    UTAssert(
        error <= 1e-14,
        "Incorrect factorization.");

    BCD::BlockVector Bblock;
    Bblock[0] = { 2.0, 5.0 };
    Bblock[1] = { 7.0, 13.0 };
    BCD::BlockVector Xblock = Bblock;
    decomposer.SolveLower(Ablock, Xblock);
    decomposer.SolveUpper(Ablock, Xblock);
    BCD::BlockVector test;
    test[0] = saveAblock[0][0] * Xblock[0] + saveAblock[0][1] * Xblock[1] - Bblock[0];
    test[1] = saveAblock[1][0] * Xblock[0] + saveAblock[1][1] * Xblock[1] - Bblock[1];
    error0 = Length(test[0]);
    error1 = Length(test[1]);
    error = std::max(error0, error1);
    UTAssert(
        error <= 1e-14,
        "Incorrect solution to AX = B.");

    Vector<double, 4> V = { 2.0, 5.0, 7.0, 13.0 };
    BCD::BlockVector convertV;
    decomposer.Convert(V, convertV);
    error0 = Length(Bblock[0] - convertV[0]);
    error1 = Length(Bblock[1] - convertV[1]);
    error = std::max(error0, error1);
    UTAssert(
        error <= 1e-14,
        "Incorrect conversion.");

    Vector<double, 4> copyV;
    decomposer.Convert(convertV, copyV);
    error = Length(V - copyV);
    UTAssert(
        error <= 1e-14,
        "Incorrect conversion.");
}

void UnitTestBlockCholeskyDecomposition::TestNPreserve()
{
    std::default_random_engine dre;
    std::uniform_real_distribution<double> urd(-1.0, 1.0);

    Matrix<double, 4, 4> D, R;
    D(0, 0) = 1.0;
    D(1, 1) = 2.0;
    D(2, 2) = 3.0;
    D(3, 3) = 4.0;
    for (std::size_t i = 0; i < R.size(); ++i)
    {
        R[i] = urd(dre);
    }
    std::vector<Vector<double, 4>> basis(4);
    for (std::size_t i = 0; i < 4; ++i)
    {
        basis[i] = R.GetCol(i);
    }
    Orthonormalize(basis);
    for (std::size_t i = 0; i < 4; ++i)
    {
        R.SetCol(i, basis[i]);
    }

    Matrix<double, 4, 4> A = R * D * Transpose(R);
    for (std::size_t row = 1; row < 4; ++row)
    {
        for (std::size_t col = 0; col < row; ++col)
        {
            A(row, col) = A(col, row);
        }
    }

    using BCD = BlockCholeskyDecomposition<double, 2, 2>;
    BCD::BlockMatrix Ablock, Lblock;
    Ablock[0][0] = Matrix<double, 2, 2>{
        { A(0, 0), A(0, 1) },
        { A(1, 0), A(1, 1) } };
    Ablock[0][1] = Matrix<double, 2, 2>{
        { A(0, 2), A(0, 3) },
        { A(1, 2), A(1, 3) } };
    Ablock[1][0] = Matrix<double, 2, 2>{
        { A(2, 0), A(2, 1) },
        { A(3, 0), A(3, 1) } };
    Ablock[1][1] = Matrix<double, 2, 2>{
        { A(2, 2), A(2, 3) },
        { A(3, 2), A(3, 3) } };

    BlockCholeskyDecomposition<double, 2, 2> decomposer;
    bool success = decomposer.Factor(Ablock, Lblock);
    UTAssert(
        success,
        "Factor decomposition failed.");

    BCD::BlockMatrix Pblock;
    Pblock[0][0] = Lblock[0][0] * Transpose(Lblock[0][0]) + Lblock[0][1] * Transpose(Lblock[0][1]);
    Pblock[0][1] = Lblock[0][0] * Transpose(Lblock[1][0]) + Lblock[0][1] * Transpose(Lblock[1][1]);
    Pblock[1][0] = Lblock[1][0] * Transpose(Lblock[0][0]) + Lblock[1][1] * Transpose(Lblock[0][1]);
    Pblock[1][1] = Lblock[1][0] * Transpose(Lblock[1][0]) + Lblock[1][1] * Transpose(Lblock[1][1]);

    double error0 = L1Norm(Pblock[0][0] - Ablock[0][0]);
    double error1 = L1Norm(Pblock[0][1] - Ablock[0][1]);
    double error2 = L1Norm(Pblock[1][0] - Ablock[1][0]);
    double error3 = L1Norm(Pblock[1][1] - Ablock[1][1]);
    double error = error0 + error1 + error2 + error3;
    UTAssert(
        error <= 1e-14,
        "Incorrect factorization.");

    BCD::BlockVector Bblock;
    Bblock[0] = { 2.0, 5.0 };
    Bblock[1] = { 7.0, 13.0 };
    BCD::BlockVector Xblock = Bblock;
    decomposer.SolveLower(Lblock, Xblock);
    decomposer.SolveUpper(Lblock, Xblock);
    BCD::BlockVector test;
    test[0] = Ablock[0][0] * Xblock[0] + Ablock[0][1] * Xblock[1] - Bblock[0];
    test[1] = Ablock[1][0] * Xblock[0] + Ablock[1][1] * Xblock[1] - Bblock[1];
    error0 = Length(test[0]);
    error1 = Length(test[1]);
    error = std::max(error0, error1);
    UTAssert(
        error <= 1e-14,
        "Incorrect solution to AX = B.");
}

void UnitTestBlockCholeskyDecomposition::TestDynamicInPlace()
{
    std::default_random_engine dre;
    std::uniform_real_distribution<double> urd(-1.0, 1.0);

    Matrix<double> D(4, 4), R(4, 4);
    D(0, 0) = 1.0;
    D(1, 1) = 2.0;
    D(2, 2) = 3.0;
    D(3, 3) = 4.0;
    for (std::size_t i = 0; i < R.size(); ++i)
    {
        R[i] = urd(dre);
    }
    std::vector<Vector<double>> basis(4);
    for (std::size_t i = 0; i < 4; ++i)
    {
        basis[i] = R.GetCol(i);
    }
    Orthonormalize(basis);
    for (std::size_t i = 0; i < 4; ++i)
    {
        R.SetCol(i, basis[i]);
    }

    Matrix<double> A = R * D * Transpose(R);
    for (std::size_t row = 1; row < 4; ++row)
    {
        for (std::size_t col = 0; col < row; ++col)
        {
            A(row, col) = A(col, row);
        }
    }

    using BCD = BlockCholeskyDecomposition<double>;
    BCD decomposer(2, 2);
    BCD::BlockMatrix Ablock;
    decomposer.Convert(A, Ablock);
    BCD::BlockMatrix saveAblock = Ablock;

    bool success = decomposer.Factor(Ablock);
    UTAssert(
        success,
        "Factor decomposition failed.");

    BCD::BlockMatrix Lblock(4);
    Lblock[0] = Ablock[0];
    Lblock[1] = Matrix<double>(2, 2);  // zero
    Lblock[2] = Ablock[2];
    Lblock[3] = Ablock[3];
    Lblock[0](0, 1) = 0.0;
    Lblock[3](0, 1) = 0.0;
    Matrix<double> L;
    decomposer.Convert(Lblock, L);
    double error = L1Norm(A - L * Transpose(L));
    UTAssert(
        error <= 1e-14,
        "Incorrect factorization.");

    Vector<double> B = { 2.0, 5.0, 7.0, 13.0 };
    BCD::BlockVector Bblock;
    decomposer.Convert(B, Bblock);
    BCD::BlockVector Xblock = Bblock;
    decomposer.SolveLower(Ablock, Xblock);
    decomposer.SolveUpper(Ablock, Xblock);
    Vector<double> X;
    decomposer.Convert(Xblock, X);
    error = Length(A * X - B);
    UTAssert(
        error <= 1e-14,
        "Incorrect solution to AX = B.");
}

void UnitTestBlockCholeskyDecomposition::TestDynamicPreserve()
{
    std::default_random_engine dre;
    std::uniform_real_distribution<double> urd(-1.0, 1.0);

    Matrix<double> D(4, 4), R(4, 4);
    D(0, 0) = 1.0;
    D(1, 1) = 2.0;
    D(2, 2) = 3.0;
    D(3, 3) = 4.0;
    for (std::size_t i = 0; i < R.size(); ++i)
    {
        R[i] = urd(dre);
    }
    std::vector<Vector<double>> basis(4);
    for (std::size_t i = 0; i < 4; ++i)
    {
        basis[i] = R.GetCol(i);
    }
    Orthonormalize(basis);
    for (std::size_t i = 0; i < 4; ++i)
    {
        R.SetCol(i, basis[i]);
    }

    Matrix<double> A = R * D * Transpose(R);
    for (std::size_t row = 1; row < 4; ++row)
    {
        for (std::size_t col = 0; col < row; ++col)
        {
            A(row, col) = A(col, row);
        }
    }

    using BCD = BlockCholeskyDecomposition<double>;
    BCD decomposer(2, 2);
    BCD::BlockMatrix Ablock, Lblock;
    decomposer.Convert(A, Ablock);
    bool success = decomposer.Factor(Ablock, Lblock);
    UTAssert(
        success,
        "Factor decomposition failed.");

    Matrix<double> L;
    decomposer.Convert(Lblock, L);
    double error = L1Norm(A - L * Transpose(L));
    UTAssert(
        error <= 1e-14,
        "Incorrect factorization.");

    Vector<double> B = { 2.0, 5.0, 7.0, 13.0 };
    BCD::BlockVector Bblock;
    decomposer.Convert(B, Bblock);
    BCD::BlockVector Xblock = Bblock;
    decomposer.SolveLower(Lblock, Xblock);
    decomposer.SolveUpper(Lblock, Xblock);
    Vector<double> X;
    decomposer.Convert(Xblock, X);
    error = Length(A * X - B);
    UTAssert(
        error <= 1e-14,
        "Incorrect solution to AX = B.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/MatrixAnalysis/BlockCholeskyDecomposition.h>

namespace gtl
{
    template class BlockCholeskyDecomposition<float, 2, 4>;
    template class BlockCholeskyDecomposition<float>;

    template class BlockCholeskyDecomposition<double, 2, 4>;
    template class BlockCholeskyDecomposition<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class BlockCholeskyDecomposition<Rational, 2, 4>;
    template class BlockCholeskyDecomposition<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(BlockCholeskyDecomposition)
