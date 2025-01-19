// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Algebra/Vector.h>
#include <GTL/Mathematics/MatrixAnalysis/SymmetricEigensolver.h>
#include <GTL/Mathematics/MatrixAnalysis/GaussianElimination.h>
#include <random>
using namespace gtl;

namespace gtl
{
    class UnitTestSymmetricEigensolver
    {
    public:
        UnitTestSymmetricEigensolver();

    private:
        static double constexpr maxError = 1e-12;

        void Test2x2();
        void Test3x3();
        void TestNxNDynamic();
        void TestNxNDynamic(std::size_t N, double& normA, double& normE, std::size_t& numIterations);

        void TestNxNStatic();

        template <std::size_t N>
        void TestNxNStatic(double& normA, double& normE, std::size_t& numIterations)
        {
            std::mt19937 mte;
            std::uniform_real_distribution<double> rnd(0.0, 1.0);

            Matrix<double> A(N, N);
            for (std::size_t r = 0; r < N; ++r)
            {
                std::size_t c;
                for (c = 0; c < r; ++c)
                {
                    A(r, c) = A(c, r);
                }
                for (/**/; c < N; ++c)
                {
                    A(r, c) = rnd(mte);
                }
            }
            normA = L2Norm(A);

            std::size_t const maxIterations = 8192;
            SymmetricEigensolver<double> solver;
            numIterations = solver(N, A.data(), maxIterations);
            Matrix<double> R(N, N), D(N, N);
            for (std::size_t i = 0; i < N; ++i)
            {
                D(i, i) = solver.GetEigenvalue(i);
                R.SetCol(i, solver.GetEigenvector(i));
            }
            Matrix<double> E = A - MultiplyABT(R * D, R);
            normE = L2Norm(E);
        }
    };
}

UnitTestSymmetricEigensolver::UnitTestSymmetricEigensolver()
{
    // Also tested by GTL/VisualTests/MatrixAnalysis/SymmetricEigensolver3x3.
    UTInformation("Mathematics/MatrixAnalysis/SymmetricEigensolver");

    Test2x2();
    Test3x3();
    TestNxNDynamic();
    TestNxNStatic();
}

void UnitTestSymmetricEigensolver::Test2x2()
{
    Matrix<double, 2, 2> A, R, D, test;
    SymmetricEigensolver<double, 2> solver;
    double detm1, norm;
    double const epsilon = 1e-12;

    A(0, 0) = -729.04599140643904;
    A(0, 1) = 670.01717998915910;
    A(1, 1) = 937.73554224846271;
    A(1, 0) = A(0, 1);
    solver(A(0, 0), A(0, 1), A(1, 1));
    for (std::size_t i = 0; i < 2; ++i)
    {
        D(i, i) = solver.GetEigenvalue(i);
        R.SetCol(i, solver.GetEigenvector(i));
    }
    detm1 = std::fabs(Determinant(R) - 1.0);
    UTAssert(detm1 <= epsilon, "Invalid ordering.");
    test = A - MultiplyABT(R * D, R);
    norm = L1Norm(test);
    UTAssert(norm <= epsilon, "Incorrect result.");

    double angle = 0.12345;
    R(0, 0) = std::cos(angle);
    R(1, 0) = std::sin(angle);
    R(0, 1) = -R(1, 0);
    R(1, 1) = R(0, 0);
    D(0, 0) = 1.0;
    D(1, 1) = 1.0 + 1e-8;
    A = MultiplyABT(R * D, R);
    solver(A(0, 0), A(0, 1), A(1, 1));
    for (std::size_t i = 0; i < 2; ++i)
    {
        D(i, i) = solver.GetEigenvalue(i);
        R.SetCol(i, solver.GetEigenvector(i));
    }
    detm1 = std::fabs(Determinant(R) - 1.0);
    UTAssert(detm1 <= epsilon, "Invalid ordering.");
    test = A - MultiplyABT(R * D, R);
    norm = L1Norm(test);
    UTAssert(norm <= epsilon, "Incorrect result.");

    D(0, 0) = 1.0e-8;
    D(1, 1) = 1.0e+8;
    A = MultiplyABT(R * D, R);
    solver(A(0, 0), A(0, 1), A(1, 1));
    for (std::size_t i = 0; i < 2; ++i)
    {
        D(i, i) = solver.GetEigenvalue(i);
        R.SetCol(i, solver.GetEigenvector(i));
    }
    detm1 = std::fabs(Determinant(R) - 1.0);
    UTAssert(detm1 <= epsilon, "Invalid ordering.");
    test = A - MultiplyABT(R * D, R);
    norm = L1Norm(test);
    UTAssert(norm <= 1e-7, "Incorrect result.");
}

void UnitTestSymmetricEigensolver::Test3x3()
{
    SymmetricEigensolver<double, 3> solver;
    Matrix<double, 3, 3> A, R, D, test;
    Vector<double, 3> vDiff;
    double detm1, norm, vLength;

    // Example 1.
    A(0, 0) = -0.72904599140643900;
    A(0, 1) = 0.67001717998915900;
    A(0, 2) = 0.93773554224846278;
    A(1, 1) = -0.55793191403459019;
    A(1, 2) = -0.38366589898599346;
    A(2, 2) = 0.094441192735703838;
    A(1, 0) = A(0, 1);
    A(2, 0) = A(0, 2);
    A(2, 1) = A(1, 2);

    // noniterative
    solver(A(0, 0), A(0, 1), A(0, 2), A(1, 1), A(1, 2), A(2, 2), true);
    for (std::size_t i = 0; i < 3; ++i)
    {
        D(i, i) = solver.GetEigenvalue(i);
        R.SetCol(i, solver.GetEigenvector(i));
    }

    // verify A * V - lambda * V = 0
    vDiff = A * R.GetCol(0) - D(0, 0) * R.GetCol(0);
    vLength = Length(vDiff);
    UTAssert(vLength <= maxError, "Incorrect eigenstuff(0).");
    vDiff = A * R.GetCol(1) - D(1, 1) * R.GetCol(1);
    vLength = Length(vDiff);
    UTAssert(vLength <= maxError, "Incorrect eigenstuff(1).");
    vDiff = A * R.GetCol(2) - D(2, 2) * R.GetCol(2);
    vLength = Length(vDiff);
    UTAssert(vLength <= maxError, "Incorrect eigenstuff(2).");

    // verify rotation (not reflection)
    detm1 = std::fabs(Determinant(R) - 1.0);
    UTAssert(detm1 <= maxError, "Invalid ordering.");

    // verify A - R * D * R^T = 0
    test = A - MultiplyABT(R * D, R);
    norm = L1Norm(test);
    UTAssert(norm <= maxError, "Incorrect result.");

    // iterative, nonaggressive
    solver(A(0, 0), A(0, 1), A(0, 2), A(1, 1), A(1, 2), A(2, 2), false, false);
    for (std::size_t i = 0; i < 3; ++i)
    {
        D(i, i) = solver.GetEigenvalue(i);
        R.SetCol(i, solver.GetEigenvector(i));
    }

    // verify A * V - lambda * V = 0
    vDiff = A * R.GetCol(0) - D(0, 0) * R.GetCol(0);
    vLength = Length(vDiff);
    UTAssert(vLength <= maxError, "Incorrect eigenstuff(0).");
    vDiff = A * R.GetCol(1) - D(1, 1) * R.GetCol(1);
    vLength = Length(vDiff);
    UTAssert(vLength <= maxError, "Incorrect eigenstuff(1).");
    vDiff = A * R.GetCol(2) - D(2, 2) * R.GetCol(2);
    vLength = Length(vDiff);
    UTAssert(vLength <= maxError, "Incorrect eigenstuff(2).");

    // verify rotation (not reflection)
    detm1 = std::fabs(Determinant(R) - 1.0);
    UTAssert(detm1 <= maxError, "Invalid ordering.");

    // verify A - R * D * R^T = 0
    test = A - MultiplyABT(R * D, R);
    norm = L1Norm(test);
    UTAssert(norm <= maxError, "Incorrect result.");

    // iterative, aggressive
    solver(A(0, 0), A(0, 1), A(0, 2), A(1, 1), A(1, 2), A(2, 2), false, true);
    for (std::size_t i = 0; i < 3; ++i)
    {
        D(i, i) = solver.GetEigenvalue(i);
        R.SetCol(i, solver.GetEigenvector(i));
    }

    // verify A * V - lambda * V = 0
    vDiff = A * R.GetCol(0) - D(0, 0) * R.GetCol(0);
    vLength = Length(vDiff);
    UTAssert(vLength <= maxError, "Incorrect eigenstuff(0).");
    vDiff = A * R.GetCol(1) - D(1, 1) * R.GetCol(1);
    vLength = Length(vDiff);
    UTAssert(vLength <= maxError, "Incorrect eigenstuff(1).");
    vDiff = A * R.GetCol(2) - D(2, 2) * R.GetCol(2);
    vLength = Length(vDiff);
    UTAssert(vLength <= maxError, "Incorrect eigenstuff(2).");

    // verify rotation (not reflection)
    detm1 = std::fabs(Determinant(R) - 1.0);
    UTAssert(detm1 <= maxError, "Invalid ordering.");

    // verify A - R * D * R^T = 0
    test = A - MultiplyABT(R * D, R);
    norm = L1Norm(test);
    UTAssert(norm <= maxError, "Incorrect result.");

    // Example 2.  Nearly repeated eigenvalue.  The iterative and noniterative
    // eigensolvers appear to compute different eigenvectors evec[1] and evec[2].
    // However, the eigenspace is effectively 2-dimensional, so numerical
    // eigensolvers can return any 2 orthogonal eigenvectors in this space.
    A(0, 0) = 1.0500019652102639;
    A(0, 1) = 0.20839505975204270;
    A(0, 2) = 0.063822157252153844;
    A(1, 1) = 1.8685358814685582;
    A(1, 2) = 0.26599399079891739;
    A(2, 2) = 1.0814621533211755;
    A(1, 0) = A(0, 1);
    A(2, 0) = A(0, 2);
    A(2, 1) = A(1, 2);

    // noniterative
    solver(A(0, 0), A(0, 1), A(0, 2), A(1, 1), A(1, 2), A(2, 2), true);
    for (std::size_t i = 0; i < 3; ++i)
    {
        D(i, i) = solver.GetEigenvalue(i);
        R.SetCol(i, solver.GetEigenvector(i));
    }

    // verify A * V - lambda * V = 0
    vDiff = A * R.GetCol(0) - D(0, 0) * R.GetCol(0);
    vLength = Length(vDiff);
    UTAssert(vLength <= maxError, "Incorrect eigenstuff(0).");
    vDiff = A * R.GetCol(1) - D(1, 1) * R.GetCol(1);
    vLength = Length(vDiff);
    UTAssert(vLength <= maxError, "Incorrect eigenstuff(1).");
    vDiff = A * R.GetCol(2) - D(2, 2) * R.GetCol(2);
    vLength = Length(vDiff);
    UTAssert(vLength <= maxError, "Incorrect eigenstuff(2).");

    // verify rotation (not reflection)
    detm1 = std::fabs(Determinant(R) - 1.0);
    UTAssert(detm1 <= maxError, "Invalid ordering.");

    // verify A - R * D * R^T = 0
    test = A - MultiplyABT(R * D, R);
    norm = L1Norm(test);
    UTAssert(norm <= maxError, "Incorrect result.");

    // iterative, nonaggressive
    solver(A(0, 0), A(0, 1), A(0, 2), A(1, 1), A(1, 2), A(2, 2), false, false);
    for (std::size_t i = 0; i < 3; ++i)
    {
        D(i, i) = solver.GetEigenvalue(i);
        R.SetCol(i, solver.GetEigenvector(i));
    }

    // verify A * V - lambda * V = 0
    vDiff = A * R.GetCol(0) - D(0, 0) * R.GetCol(0);
    vLength = Length(vDiff);
    UTAssert(vLength <= maxError, "Incorrect eigenstuff(0).");
    vDiff = A * R.GetCol(1) - D(1, 1) * R.GetCol(1);
    vLength = Length(vDiff);
    UTAssert(vLength <= maxError, "Incorrect eigenstuff(1).");
    vDiff = A * R.GetCol(2) - D(2, 2) * R.GetCol(2);
    vLength = Length(vDiff);
    UTAssert(vLength <= maxError, "Incorrect eigenstuff(2).");

    // verify rotation (not reflection)
    detm1 = std::fabs(Determinant(R) - 1.0);
    UTAssert(detm1 <= maxError, "Invalid ordering.");

    // verify A - R * D * R^T = 0
    test = A - MultiplyABT(R * D, R);
    norm = L1Norm(test);
    UTAssert(norm <= maxError, "Incorrect result.");

    // iterative, aggressive
    solver(A(0, 0), A(0, 1), A(0, 2), A(1, 1), A(1, 2), A(2, 2), false, true);
    for (std::size_t i = 0; i < 3; ++i)
    {
        D(i, i) = solver.GetEigenvalue(i);
        R.SetCol(i, solver.GetEigenvector(i));
    }

    // verify A * V - lambda * V = 0
    vDiff = A * R.GetCol(0) - D(0, 0) * R.GetCol(0);
    vLength = Length(vDiff);
    UTAssert(vLength <= maxError, "Incorrect eigenstuff(0).");
    vDiff = A * R.GetCol(1) - D(1, 1) * R.GetCol(1);
    vLength = Length(vDiff);
    UTAssert(vLength <= maxError, "Incorrect eigenstuff(1).");
    vDiff = A * R.GetCol(2) - D(2, 2) * R.GetCol(2);
    vLength = Length(vDiff);
    UTAssert(vLength <= maxError, "Incorrect eigenstuff(2).");

    // verify rotation (not reflection)
    detm1 = std::fabs(Determinant(R) - 1.0);
    UTAssert(detm1 <= maxError, "Invalid ordering.");

    // verify A - R * D * R^T = 0
    test = A - MultiplyABT(R * D, R);
    norm = L1Norm(test);
    UTAssert(norm <= maxError, "Incorrect result.");

    // Example 3. Large condition number.
    double d0 = 1.0e-4, d1 = 1.0, d2 = 1.0e+4;
    double const maxNoniterativeError = 1e-08;
    MakeDiagonal({ d0, d1, d2 }, D);
    Vector<double, 3> U0 = { 1.0, 2.0, 3.0 }, U1, U2;
    ComputeOrthonormalBasis(1, U0, U1, U2);
    // U0 = (0.26726124191242440, 0.53452248382484879, 0.80178372573727319)
    // U1 = (0.0000000000000000, 0.83205029433784372, -0.55470019622522915)
    // U2 = (-0.96362411165943151, 0.14824986333222023, 0.22237479499833032)

    R.SetCol(0, U0);
    R.SetCol(1, U1);
    R.SetCol(2, U2);
    A = MultiplyABT(R, D * R);

    // noniterative
    solver(A(0, 0), A(0, 1), A(0, 2), A(1, 1), A(1, 2), A(2, 2), true);
    for (std::size_t i = 0; i < 3; ++i)
    {
        D(i, i) = solver.GetEigenvalue(i);
        R.SetCol(i, solver.GetEigenvector(i));
    }

    // verify A * V - lambda * V = 0
    vDiff = A * R.GetCol(0) - D(0, 0) * R.GetCol(0);
    vLength = Length(vDiff);
    UTAssert(vLength <= maxNoniterativeError, "Incorrect eigenstuff(0).");
    vDiff = A * R.GetCol(1) - D(1, 1) * R.GetCol(1);
    vLength = Length(vDiff);
    UTAssert(vLength <= maxNoniterativeError, "Incorrect eigenstuff(1).");
    vDiff = A * R.GetCol(2) - D(2, 2) * R.GetCol(2);
    vLength = Length(vDiff);
    UTAssert(vLength <= maxNoniterativeError, "Incorrect eigenstuff(2).");

    // verify rotation (not reflection)
    detm1 = std::fabs(Determinant(R) - 1.0);
    UTAssert(detm1 <= maxNoniterativeError, "Invalid ordering.");

    // verify A - R * D * R^T = 0
    test = A - MultiplyABT(R * D, R);
    norm = L1Norm(test);
    UTAssert(norm <= maxNoniterativeError, "Incorrect result.");

    // iterative, nonaggressive
    solver(A(0, 0), A(0, 1), A(0, 2), A(1, 1), A(1, 2), A(2, 2), false, false);
    for (std::size_t i = 0; i < 3; ++i)
    {
        D(i, i) = solver.GetEigenvalue(i);
        R.SetCol(i, solver.GetEigenvector(i));
    }

    // verify A * V - lambda * V = 0
    vDiff = A * R.GetCol(0) - D(0, 0) * R.GetCol(0);
    vLength = Length(vDiff);
    UTAssert(vLength <= maxError, "Incorrect eigenstuff(0).");
    vDiff = A * R.GetCol(1) - D(1, 1) * R.GetCol(1);
    vLength = Length(vDiff);
    UTAssert(vLength <= maxError, "Incorrect eigenstuff(1).");
    vDiff = A * R.GetCol(2) - D(2, 2) * R.GetCol(2);
    vLength = Length(vDiff);
    UTAssert(vLength <= maxError, "Incorrect eigenstuff(2).");

    // verify rotation (not reflection)
    detm1 = std::fabs(Determinant(R) - 1.0);
    UTAssert(detm1 <= maxError, "Invalid ordering.");

    // verify A - R * D * R^T = 0
    test = A - MultiplyABT(R * D, R);
    norm = L1Norm(test);
    UTAssert(norm <= maxError, "Incorrect result.");

    // iterative, aggressive
    solver(A(0, 0), A(0, 1), A(0, 2), A(1, 1), A(1, 2), A(2, 2), false, true);
    for (std::size_t i = 0; i < 3; ++i)
    {
        D(i, i) = solver.GetEigenvalue(i);
        R.SetCol(i, solver.GetEigenvector(i));
    }

    // verify A * V - lambda * V = 0
    vDiff = A * R.GetCol(0) - D(0, 0) * R.GetCol(0);
    vLength = Length(vDiff);
    UTAssert(vLength <= maxError, "Incorrect eigenstuff(0).");
    vDiff = A * R.GetCol(1) - D(1, 1) * R.GetCol(1);
    vLength = Length(vDiff);
    UTAssert(vLength <= maxError, "Incorrect eigenstuff(1).");
    vDiff = A * R.GetCol(2) - D(2, 2) * R.GetCol(2);
    vLength = Length(vDiff);
    UTAssert(vLength <= maxError, "Incorrect eigenstuff(2).");

    // verify rotation (not reflection)
    detm1 = std::fabs(Determinant(R) - 1.0);
    UTAssert(detm1 <= maxError, "Invalid ordering.");

    // verify A - R * D * R^T = 0
    test = A - MultiplyABT(R * D, R);
    norm = L1Norm(test);
    UTAssert(norm <= maxError, "Incorrect result.");
}

void UnitTestSymmetricEigensolver::TestNxNDynamic()
{
    double normA, normE;
    std::size_t numIterations;
    for (std::size_t i = 2; i < 32; ++i)
    {
        TestNxNDynamic(i, normA, normE, numIterations);

        std::ostringstream ostream;
        ostream
            << "N = " << i
            << ", |A| = " << std::setprecision(14) << std::scientific << normA
            << ", |E| = " << std::setprecision(14) << std::scientific << normE
            << ", |E|/|A| = " << std::setprecision(14) << std::scientific << normE / normA
            << ", iterations = " << numIterations;

        UTInformation(ostream.str());
    }
}

void UnitTestSymmetricEigensolver::TestNxNDynamic(std::size_t N, double& normA,
    double& normE, std::size_t& numIterations)
{
    std::mt19937 mte;
    std::uniform_real_distribution<double> rnd(0.0, 1.0);

    Matrix<double> A(N, N);
    for (std::size_t r = 0; r < N; ++r)
    {
        std::size_t c;
        for (c = 0; c < r; ++c)
        {
            A(r, c) = A(c, r);
        }
        for (/**/; c < N; ++c)
        {
            A(r, c) = rnd(mte);
        }
    }
    normA = L2Norm(A);

    std::size_t const maxIterations = 8192;
    SymmetricEigensolver<double> solver;
    numIterations = solver(N, A.data(), maxIterations);
    Matrix<double> R(N, N), D(N, N);
    for (std::size_t i = 0; i < N; ++i)
    {
        D(i, i) = solver.GetEigenvalue(i);
        R.SetCol(i, solver.GetEigenvector(i));
    }
    Matrix<double> E = A - MultiplyABT(R * D, R);
    normE = L2Norm(E);
}

#define TEST_NXN_STATIC(N)\
{\
    TestNxNStatic<N>(normA, normE, numIterations);\
    std::ostringstream ostream;\
    ostream\
        << "N = " << N\
        << ", |A| = " << std::setprecision(14) << std::scientific << normA\
        << ", |E| = " << std::setprecision(14) << std::scientific << normE\
        << ", |E|/|A| = " << std::setprecision(14) << std::scientific << normE / normA\
        << ", iterations = " << numIterations;\
    UTInformation(ostream.str());\
}

void UnitTestSymmetricEigensolver::TestNxNStatic()
{
    double normA, normE;
    std::size_t numIterations;

    TEST_NXN_STATIC(2);
    TEST_NXN_STATIC(3);
    TEST_NXN_STATIC(4);
    TEST_NXN_STATIC(5);
    TEST_NXN_STATIC(6);
    TEST_NXN_STATIC(7);
    TEST_NXN_STATIC(8);
    TEST_NXN_STATIC(9);
    TEST_NXN_STATIC(10);
    TEST_NXN_STATIC(11);
    TEST_NXN_STATIC(12);
    TEST_NXN_STATIC(13);
    TEST_NXN_STATIC(14);
    TEST_NXN_STATIC(15);
    TEST_NXN_STATIC(16);
    TEST_NXN_STATIC(17);
    TEST_NXN_STATIC(18);
    TEST_NXN_STATIC(19);
    TEST_NXN_STATIC(20);
    TEST_NXN_STATIC(21);
    TEST_NXN_STATIC(22);
    TEST_NXN_STATIC(23);
    TEST_NXN_STATIC(24);
    TEST_NXN_STATIC(25);
    TEST_NXN_STATIC(26);
    TEST_NXN_STATIC(27);
    TEST_NXN_STATIC(28);
    TEST_NXN_STATIC(29);
    TEST_NXN_STATIC(30);
    TEST_NXN_STATIC(31);
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/MatrixAnalysis/SymmetricEigensolver.h>

namespace gtl
{
    template class SymmetricEigensolver<float, 2>;
    template class SymmetricEigensolver<float, 3>;
    template class SymmetricEigensolver<float>;

    template class SymmetricEigensolver<double, 2>;
    template class SymmetricEigensolver<double, 3>;
    template class SymmetricEigensolver<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class SymmetricEigensolver<Rational, 2>;
    template class SymmetricEigensolver<Rational, 3>;
    template class SymmetricEigensolver<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(SymmetricEigensolver)
