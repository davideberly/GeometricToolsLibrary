// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/MatrixAnalysis/CholeskyDecomposition.h>
#include <GTL/Mathematics/Algebra/RigidMotion.h>
#include <random>
using namespace gtl;

namespace gtl
{
    class UnitTestCholeskyDecomposition
    {
    public:
        UnitTestCholeskyDecomposition();

    private:
        void TestNInPlace();
        void TestNPreserve();
        void TestDynamicInPlace();
        void TestDynamicPreserve();
    };
}

UnitTestCholeskyDecomposition::UnitTestCholeskyDecomposition()
{
    UTInformation("Mathematics/MatrixAnalysis/CholeskyDecomposition");

    TestNInPlace();
    TestNPreserve();
    TestDynamicInPlace();
    TestDynamicPreserve();
}

void UnitTestCholeskyDecomposition::TestNInPlace()
{
    Matrix<double, 3, 3> D, R, A, saveA, L;
    D(0, 0) = 1.0;
    D(1, 1) = 2.0;
    D(2, 2) = 3.0;
    Quaternion<double> q(-1.0, 1.0, -3.0, 5.0);
    Normalize(q);
    RigidMotion<double>::Convert(q, R);
    A = R * D * Transpose(R);
    saveA = A;

    CholeskyDecomposition<double, 3> decomposer;
    bool success = decomposer.Factor(A);
    UTAssert(
        success,
        "Factor decomposition failed.");

    MakeZero(L);
    for (std::size_t r = 0; r < 3; ++r)
    {
        for (std::size_t c = 0; c <= r; ++c)
        {
            L(r, c) = A(r, c);
        }
    }

    double error = L1Norm(saveA - L * Transpose(L));
    UTAssert(
        error <= 1e-15,
        "Incorrect factorization.");

    Vector<double, 3> B{ 2.0, 5.0, 7.0 }, X = B;
    decomposer.SolveLower(A, X);
    decomposer.SolveUpper(A, X);
    Vector<double, 3> test = saveA * X - B;
    error = Length(test);
    UTAssert(
        error <= 1e-15,
        "Incorrect solution to AX = B.");
}

void UnitTestCholeskyDecomposition::TestNPreserve()
{
    Matrix<double, 3, 3> D, R, A, L;
    D(0, 0) = 1.0;
    D(1, 1) = 2.0;
    D(2, 2) = 3.0;
    Quaternion<double> q(-1.0, 1.0, -3.0, 5.0);
    Normalize(q);
    RigidMotion<double>::Convert(q, R);
    A = R * D * Transpose(R);

    CholeskyDecomposition<double, 3> decomposer;
    bool success = decomposer.Factor(A, L);
    UTAssert(
        success,
        "Factor decomposition failed.");

    double error = L1Norm(A - L * Transpose(L));
    UTAssert(
        error <= 1e-15,
        "Incorrect factorization.");

    Vector<double, 3> B{ 2.0, 5.0, 7.0 }, X = B;
    decomposer.SolveLower(L, X);
    decomposer.SolveUpper(L, X);
    Vector<double, 3> test = A * X - B;
    error = Length(test);
    UTAssert(
        error <= 1e-15,
        "Incorrect solution to AX = B.");
}

void UnitTestCholeskyDecomposition::TestDynamicInPlace()
{
    Matrix<double> D(3, 3), R(3, 3), A(3, 3), saveA(3, 3), L(3, 3);
    D(0, 0) = 1.0;
    D(1, 1) = 2.0;
    D(2, 2) = 3.0;
    Quaternion<double> q(-1.0, 1.0, -3.0, 5.0);
    Normalize(q);
    Matrix<double, 3, 3> tempR;
    RigidMotion<double>::Convert(q, tempR);
    for (std::size_t i = 0; i < 9; ++i)
    {
        R[i] = tempR[i];
    }
    A = R * D * Transpose(R);
    saveA = A;

    CholeskyDecomposition<double> decomposer(3);
    bool success = decomposer.Factor(A);
    UTAssert(
        success,
        "Factor decomposition failed.");

    MakeZero(L);
    for (std::size_t r = 0; r < 3; ++r)
    {
        for (std::size_t c = 0; c <= r; ++c)
        {
            L(r, c) = A(r, c);
        }
    }

    double error = L1Norm(saveA - L * Transpose(L));
    UTAssert(
        error <= 1e-15,
        "Incorrect factorization.");

    Vector<double> B{ 2.0, 5.0, 7.0 }, X = B;
    decomposer.SolveLower(A, X);
    decomposer.SolveUpper(A, X);
    Vector<double> test = saveA * X - B;
    error = Length(test);
    UTAssert(
        error <= 1e-15,
        "Incorrect solution to AX = B.");
}

void UnitTestCholeskyDecomposition::TestDynamicPreserve()
{
    Matrix<double> D(3, 3), R(3, 3), A(3, 3), L(3, 3);
    D(0, 0) = 1.0;
    D(1, 1) = 2.0;
    D(2, 2) = 3.0;
    Quaternion<double> q(-1.0, 1.0, -3.0, 5.0);
    Normalize(q);
    Matrix<double, 3, 3> tempR;
    RigidMotion<double>::Convert(q, tempR);
    for (std::size_t i = 0; i < 9; ++i)
    {
        R[i] = tempR[i];
    }
    A = R * D * Transpose(R);

    CholeskyDecomposition<double> decomposer(3);
    bool success = decomposer.Factor(A, L);
    UTAssert(
        success,
        "Factor decomposition failed.");

    double error = L1Norm(A - L * Transpose(L));
    UTAssert(
        error <= 1e-15,
        "Incorrect factorization.");

    Vector<double> B{ 2.0, 5.0, 7.0 }, X = B;
    decomposer.SolveLower(L, X);
    decomposer.SolveUpper(L, X);
    Vector<double> test = A * X - B;
    error = Length(test);
    UTAssert(
        error <= 1e-15,
        "Incorrect solution to AX = B.");
}


#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/MatrixAnalysis/CholeskyDecomposition.h>

namespace gtl
{
    template class CholeskyDecomposition<float, 2>;
    template class CholeskyDecomposition<float>;

    template class CholeskyDecomposition<double, 2>;
    template class CholeskyDecomposition<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class CholeskyDecomposition<Rational, 2>;
    template class CholeskyDecomposition<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(CholeskyDecomposition)
