// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/MatrixAnalysis/LinearSystem.h>
#include <random>
#include <sstream>
using namespace gtl;

namespace gtl
{
    class UnitTestLinearSystem
    {
    public:
        UnitTestLinearSystem();

    private:
        void TestSolve2x2();
        void TestSolve3x3();
        void TestSolve4x4();
        void TestSolveGeneral();
        void TestTridiagonal();
        void TestConstantTridiagonal();
        void TestSymmetricCGDense();
        void TestSymmetricCGSparse();

        std::default_random_engine mDRE;
        std::uniform_real_distribution<double> mURD;
    };
}

UnitTestLinearSystem::UnitTestLinearSystem()
    :
    mURD(-1.0, 1.0)
{
    UTInformation("Mathematics/MatrixAnalysis/LinearSystem");

    TestSolve2x2();
    TestSolve3x3();
    TestSolve4x4();
    TestSolveGeneral();
    TestTridiagonal();
    TestConstantTridiagonal();
    TestSymmetricCGDense();
    TestSymmetricCGSparse();
}

void UnitTestLinearSystem::TestSolve2x2()
{
    Matrix2x2<double> A;
    Vector2<double> B, X;
    for (std::size_t r = 0; r < 2; ++r)
    {
        for (std::size_t c = 0; c < 2; ++c)
        {
            A(r, c) = mURD(mDRE);
        }
        B[r] = mURD(mDRE);
    }

    bool success = LinearSystem<double>::Solve(A, B, X);
    UTAssert(
        success,
        "Singular matrix for 2x2 system.");
    Vector2<double> test = A * X - B;
    double error = Length(test);
    std::stringstream strError{};
    strError << std::setprecision(20) << error;
    UTAssert(
        error <= 1e-14,
        "Solve failed for 2x2 system, error = " + strError.str());
}

void UnitTestLinearSystem::TestSolve3x3()
{
    Matrix3x3<double> A;
    Vector3<double> B, X;
    for (std::size_t r = 0; r < 3; ++r)
    {
        for (std::size_t c = 0; c < 3; ++c)
        {
            A(r, c) = mURD(mDRE);
        }
        B[r] = mURD(mDRE);
    }

    bool success = LinearSystem<double>::Solve(A, B, X);
    UTAssert(
        success,
        "Singular matrix for 3x3 system.");
    Vector3<double> test = A * X - B;
    double error = Length(test);
    std::stringstream strError{};
    strError << std::setprecision(20) << error;
    UTAssert(
        error <= 1e-14,
        "Solve failed for 3x3 system, error = " + strError.str());
}

void UnitTestLinearSystem::TestSolve4x4()
{
    Matrix4x4<double> A;
    Vector4<double> B, X;
    for (std::size_t r = 0; r < 4; ++r)
    {
        for (std::size_t c = 0; c < 4; ++c)
        {
            A(r, c) = mURD(mDRE);
        }
        B[r] = mURD(mDRE);
    }

    bool success = LinearSystem<double>::Solve(A, B, X);
    UTAssert(
        success,
        "Singular matrix for 4x4 system.");
    Vector4<double> test = A * X - B;
    double error = Length(test);
    std::stringstream strError{};
    strError << std::setprecision(20) << error;
    UTAssert(
        error <= 1e-14,
        "Solve failed for 4x4 system, error = " + strError.str());
}

void UnitTestLinearSystem::TestSolveGeneral()
{
    std::size_t const N = 10;
    Matrix<double> A(N, N);
    Vector<double> B(N), X(N);
    for (std::size_t r = 0; r < N; ++r)
    {
        for (std::size_t c = 0; c < N; ++c)
        {
            A(r, c) = mURD(mDRE);
        }
        B[r] = mURD(mDRE);
    }

    bool success = LinearSystem<double>::Solve(N, 1, A.data(), B.data(), X.data());
    UTAssert(
        success,
        "Singular matrix for 10x10 system.");
    Vector<double> test = A * X - B;
    double error = Length(test);
    UTAssert(
        error <= 1e-14,
        "Solve failed for 4x4 system.");

    Matrix<double> BB(N, 3), XX(N, 3);
    for (std::size_t r = 0; r < N; ++r)
    {
        for (std::size_t c = 0; c < BB.GetNumCols(); ++c)
        {
            BB(r, c) = mURD(mDRE);
        }
    }

    success = LinearSystem<double>::Solve(N, BB.GetNumCols(), A.data(), BB.data(), XX.data());
    UTAssert(
        success,
        "Singular matrix for 10x10 system.");
    Matrix<double> testM = A * XX - BB;
    error = L1Norm(testM);
    UTAssert(
        error <= 1e-13,
        "Solve failed for 4x4/4x3 system.");
}

void UnitTestLinearSystem::TestTridiagonal()
{
    std::size_t constexpr N = 4;
    std::vector<double> diagonal(N), subdiagonal(N - 1), superdiagonal(N - 1);
    Matrix<double, N, N> A;
    Vector<double, N> B, X;
    for (std::size_t i = 0; i < diagonal.size(); ++i)
    {
        diagonal[i] = mURD(mDRE);
        B[i] = mURD(mDRE);
        A(i, i) = diagonal[i];
    }
    for (std::size_t i = 0; i < subdiagonal.size(); ++i)
    {
        subdiagonal[i] = mURD(mDRE);
        A(i + 1, i) = subdiagonal[i];
        superdiagonal[i] = mURD(mDRE);
        A(i, i + 1) = superdiagonal[i];
    }

    bool success = LinearSystem<double>::SolveTridiagonal(N, subdiagonal.data(),
        diagonal.data(), superdiagonal.data(), B.data(), X.data());
    UTAssert(
        success,
        "SolveTridiagonal failed.");
    Vector<double, N> test = A * X - B;
    double error = Length(test);
    UTAssert(
        error <= 1e-14,
        "Solve failed for 4x4/4x3 system.");
}

void UnitTestLinearSystem::TestConstantTridiagonal()
{
    std::size_t constexpr N = 4;
    double const diagonal = 3.0;
    double const subdiagonal = 1.0;
    double const superdiagonal = -2.0;
    Matrix<double, N, N> A;
    Vector<double, N> B, X;
    for (std::size_t i = 0; i < N; ++i)
    {
        B[i] = mURD(mDRE);
        A(i, i) = diagonal;
    }
    for (std::size_t i = 0; i + 1 < N; ++i)
    {
        A(i + 1, i) = subdiagonal;
        A(i, i + 1) = superdiagonal;
    }

    bool success = LinearSystem<double>::SolveConstantTridiagonal(N, subdiagonal,
        diagonal, superdiagonal, B.data(), X.data());
    UTAssert(
        success,
        "SolveConstantTridiagonal failed.");
    Vector<double, N> test = A * X - B;
    double error = Length(test);
    UTAssert(
        error <= 1e-15,
        "Solve failed for 4x4/4x3 system.");
}

void UnitTestLinearSystem::TestSymmetricCGDense()
{
    std::size_t const N = 10;
    Matrix<double> A(N, N);
    Vector<double> B(N), X(N);
    for (std::size_t r = 0; r < N; ++r)
    {
        for (std::size_t c = 0; c < r; ++c)
        {
            A(r, c) = A(c, r);
        }
        for (std::size_t c = r; c < N; ++c)
        {
            A(r, c) = mURD(mDRE);
        }
        B[r] = mURD(mDRE);
    }

    std::size_t const maxIterations = 1024;
    double const tolerance = 1e-08;
    bool success = LinearSystem<double>::SolveSymmetricCG(N, A.data(),
        B.data(), X.data(), maxIterations, tolerance);
    UTAssert(
        success,
        "SolveSymmetricCG (dense) failed.");
    Vector<double> test = A * X - B;
    double error = Length(test);
    std::stringstream strError{};
    strError << std::setprecision(20) << error;
    UTAssert(
        error <= 1e-9,
        "Solve failed using conjugate gradient(dense), error = " + strError.str());
}

void UnitTestLinearSystem::TestSymmetricCGSparse()
{
    std::size_t const N = 10;
    Matrix<double> A(N, N);
    Vector<double> B(N), X(N);
    std::map<std::array<std::size_t, 2>, double> Asparse;
    for (std::size_t i = 0; i < N; ++i)
    {
        Asparse.insert(std::make_pair(std::array<std::size_t, 2>{ i, i }, 4.0));
        A(i, i) = 4.0;
        B[i] = mURD(mDRE);
    }

    std::vector<std::array<std::size_t, 2>> occupied(4);
    occupied[0] = { 0, 5 };
    occupied[1] = { 1, 3 };
    occupied[2] = { 7, 8 };
    occupied[3] = { 9, 4 };
    for (auto const& location : occupied)
    {
        double value = mURD(mDRE);
        Asparse.insert(std::make_pair(location, value));
        A(location[0], location[1]) = value;
        A(location[1], location[0]) = value;
    }

    std::size_t const maxIterations = 1024;
    double const tolerance = 1e-8;
    std::size_t numIterations = LinearSystem<double>::SolveSymmetricCG(N, Asparse,
        B.data(), X.data(), maxIterations, tolerance);
    UTAssert(
        numIterations > 0,
        "SolveSymmetricCG (sparse) failed.");
    Vector<double> test = A * X - B;
    double error = Length(test);
    std::stringstream strError{};
    strError << std::setprecision(20) << error;
    UTAssert(
        error <= 1e-8,
        "Solve failed using conjugate gradient(sparse), error = " + strError.str());
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/MatrixAnalysis/LinearSystem.h>

namespace gtl
{
    template class LinearSystem<float>;
    template class LinearSystem<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class LinearSystem<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(LinearSystem)
