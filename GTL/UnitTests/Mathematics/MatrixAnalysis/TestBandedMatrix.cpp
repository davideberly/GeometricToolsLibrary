// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/MatrixAnalysis/BandedMatrix.h>
#include <GTL/Mathematics/Algebra/Matrix.h>
using namespace gtl;

namespace gtl
{
    class UnitTestBandedMatrix
    {
    public:
        UnitTestBandedMatrix();

    private:
        void TestConstruction();
        void TestCholesky();
        void TestSolveSystem1();
        void TestSolveSystem2();
        void TestComputeInverse();
    };
}

UnitTestBandedMatrix::UnitTestBandedMatrix()
{
    UTInformation("Mathematics/MatrixAnalysis/BandedMatrix");

    TestConstruction();
    TestCholesky();
    TestSolveSystem1();
    TestSolveSystem2();
    TestComputeInverse();
}

void UnitTestBandedMatrix::TestConstruction()
{
    BandedMatrix<double> A(4, 2, 1);

    UTAssert(
        A.GetDBand().size() == 4,
        "Incorrect number of D-band elements.");

    UTAssert(
        A.GetLBands().size() == 2,
        "Incorrect number of L-bands.");

    UTAssert(
        A.GetLBand(0).size() == 3,
        "Incorrect number of L[0]-band elements.");

    UTAssert(
        A.GetLBand(1).size() == 2,
        "Incorrect number of L[1]-band elements.");

    UTAssert(
        A.GetUBands().size() == 1,
        "Incorrect number of U-bands.");

    UTAssert(
        A.GetUBand(0).size() == 3,
        "Incorrect number of U[0]-band elements.");

    for (std::size_t row = 0; row < A.GetSize(); ++row)
    {
        for (std::size_t col = 0; col < A.GetSize(); ++col)
        {
            UTAssert(
                A(row, col) == 0.0,
                "Invalid operator() result.");
        }
    }
}

void UnitTestBandedMatrix::TestCholesky()
{
    BandedMatrix<double> A(5, 2, 2);
    auto& DBand = A.GetDBand();
    DBand[0] = 1.0;
    DBand[1] = 1.0;
    DBand[2] = 1.0;
    DBand[3] = 1.0;
    DBand[4] = 1.0;

    auto& LBand0 = A.GetLBand(0);
    auto& UBand0 = A.GetUBand(0);
    LBand0[0] = 0.1;
    LBand0[1] = 0.2;
    LBand0[2] = 0.3;
    LBand0[3] = 0.4;
    UBand0 = LBand0;

    auto& LBand1 = A.GetLBand(1);
    auto& UBand1 = A.GetUBand(1);
    LBand1[0] = 0.01;
    LBand1[1] = 0.02;
    LBand1[2] = 0.03;
    UBand1 = LBand1;

    Vector<double, 5> expDBand
    {
       1.0,
       0.99498743710661997,
       0.97974434925596277,
       0.95306460837805629,
       0.91157461512459093
    };

    Vector<double, 4> expLBand0
    {
        0.10000000000000001,
        0.20000252523658321,
        0.30209901001681805,
        0.40999283214148635
    };

    Vector<double, 3> expLBand1
    {
        0.010000000000000000,
        0.020100756305184243,
        0.030620232740084283
    };

    bool factored = A.CholeskyFactor();
    UTAssert(
        factored,
        "CholeskyFactor failed.");

    double error = Length(expDBand - Vector<double, 5>(A.GetDBand()));
    UTAssert(
        error <= 1e-15,
        "CholeskyFactor failed.");

    error = Length(expLBand0 - Vector<double, 4>(A.GetLBand(0)));
    UTAssert(
        error <= 1e-15,
        "CholeskyFactor failed.");

    error = Length(expLBand1 - Vector<double, 3>(A.GetLBand(1)));
    UTAssert(
        error <= 1e-15,
        "CholeskyFactor failed.");

    error = Length(expLBand0 - Vector<double, 4>(A.GetUBand(0)));
    UTAssert(
        error <= 1e-15,
        "CholeskyFactor failed.");

    error = Length(expLBand1 - Vector<double, 3>(A.GetUBand(1)));
    UTAssert(
        error <= 1e-15,
        "CholeskyFactor failed.");
}

void UnitTestBandedMatrix::TestSolveSystem1()
{
    BandedMatrix<double> A(5, 2, 2);
    A(0, 0) = 1.0;
    A(1, 1) = 1.0;
    A(2, 2) = 1.0;
    A(3, 3) = 1.0;
    A(4, 4) = 1.0;
    A(1, 0) = 0.1;
    A(2, 1) = 0.2;
    A(3, 2) = 0.3;
    A(4, 3) = 0.4;
    A(0, 1) = A(1, 0);
    A(1, 2) = A(2, 1);
    A(2, 3) = A(3, 2);
    A(3, 4) = A(4, 3);
    A(2, 0) = 0.01;
    A(3, 1) = 0.02;
    A(4, 2) = 0.03;
    A(0, 2) = A(2, 0);
    A(1, 3) = A(3, 1);
    A(2, 4) = A(4, 2);

    Matrix<double, 5, 5> ASave{};
    for (std::size_t row = 0; row < 5; ++row)
    {
        for (std::size_t col = 0; col < 5; ++col)
        {
            ASave(row, col) = A(row, col);
        }
    }

    Vector<double, 5> B{ 5.0, -4.0, 3.0, -2.0, 1.0 }, X = B;
    bool solved = A.SolveSystem(X.data());
    UTAssert(
        solved,
        "SolveSystem (vector) failed.");

    Vector<double, 5> test = ASave * X - B;
    double error = Length(test);
    UTAssert(
        error <= 1e-14,
        "SolveSystem (vector) failed.");
}

void UnitTestBandedMatrix::TestSolveSystem2()
{
    BandedMatrix<double> A(5, 2, 2);
    A(0, 0) = 1.0;
    A(1, 1) = 1.0;
    A(2, 2) = 1.0;
    A(3, 3) = 1.0;
    A(4, 4) = 1.0;
    A(1, 0) = 0.1;
    A(2, 1) = 0.2;
    A(3, 2) = 0.3;
    A(4, 3) = 0.4;
    A(0, 1) = A(1, 0);
    A(1, 2) = A(2, 1);
    A(2, 3) = A(3, 2);
    A(3, 4) = A(4, 3);
    A(2, 0) = 0.01;
    A(3, 1) = 0.02;
    A(4, 2) = 0.03;
    A(0, 2) = A(2, 0);
    A(1, 3) = A(3, 1);
    A(2, 4) = A(4, 2);

    Matrix<double, 5, 5> ASave{};
    for (std::size_t row = 0; row < 5; ++row)
    {
        for (std::size_t col = 0; col < 5; ++col)
        {
            ASave(row, col) = A(row, col);
        }
    }

    Matrix<double, 5, 3> B
    {
        { 1.0, 2.0, 3.0 },
        { 4.0, 5.0, 6.0 },
        { 7.0, 8.0, 9.0 },
        { 10.0, 11.0, 12.0 },
        { 13.0, 14.0, 15.0 }
    };
    Matrix<double, 5, 3> X = B;
    bool solved = A.SolveSystem(X.data(), 3);
    UTAssert(
        solved,
        "SolveSystem (matrix) failed.");

    Matrix<double, 5, 3> test = ASave * X - B;
    double error = L1Norm(test);
    UTAssert(
        error <= 1e-13,
        "SolveSystem (matrix) failed.");
}

void UnitTestBandedMatrix::TestComputeInverse()
{
    BandedMatrix<double> A(5, 2, 2);
    A(0, 0) = 1.0;
    A(1, 1) = 1.0;
    A(2, 2) = 1.0;
    A(3, 3) = 1.0;
    A(4, 4) = 1.0;
    A(1, 0) = 0.1;
    A(2, 1) = 0.2;
    A(3, 2) = 0.3;
    A(4, 3) = 0.4;
    A(0, 1) = A(1, 0);
    A(1, 2) = A(2, 1);
    A(2, 3) = A(3, 2);
    A(3, 4) = A(4, 3);
    A(2, 0) = 0.01;
    A(3, 1) = 0.02;
    A(4, 2) = 0.03;
    A(0, 2) = A(2, 0);
    A(1, 3) = A(3, 1);
    A(2, 4) = A(4, 2);

    Matrix<double, 5, 5> ASave{};
    for (std::size_t row = 0; row < 5; ++row)
    {
        for (std::size_t col = 0; col < 5; ++col)
        {
            ASave(row, col) = A(row, col);
        }
    }

    Matrix<double, 5, 5> invA{};
    bool inverted = A.ComputeInverse(invA.data());
    UTAssert(
        inverted,
        "ComputeInverse failed.");

    Matrix<double, 5, 5> identity{};
    MakeIdentity(identity);
    Matrix<double, 5, 5> test = ASave * invA - identity;
    double error = L1Norm(test);
    UTAssert(
        error <= 1e-15,
        "ComputeInverse failed.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/MatrixAnalysis/BandedMatrix.h>

namespace gtl
{
    template class BandedMatrix<float>;
    template class BandedMatrix<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class BandedMatrix<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(BandedMatrix)
