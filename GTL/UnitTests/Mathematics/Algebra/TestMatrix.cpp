// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#include <GTL/Mathematics/Algebra/Matrix.h>
#include <GTL/Mathematics/MatrixAnalysis/GaussianElimination.h>
using namespace gtl;

namespace gtl
{
    class UnitTestMatrix
    {
    public:
        UnitTestMatrix();

    private:
        using Rational = BSRational<UIntegerAP32>;

        // TN   = Matrix<T,R,C>
        // T    = Matrix<T>

        void ConstructorsTN();
        void AccessorsTN();
        void ComparisonsTN();
        void SpecialObjectsTN();
        void UnaryOperationsTN();
        void LinearAlgebraicOperationsTN();
        void GeometricOperationsTN();
        void MatrixAndVectorOperationsTN();
        void LiftProjectTN();

        void ConstructorsT();
        void AccessorsT();
        void ComparisonsT();
        void SpecialObjectsT();
        void UnaryOperationsT();
        void LinearAlgebraicOperationsT();
        void GeometricOperationsT();
        void MatrixAndVectorOperationsT();
        void LiftProjectT();

        template <typename T, std::size_t NumRows, std::size_t NumCols>
        void DoComparisons(Matrix<T, NumRows, NumCols> const& mat0, Matrix<T, NumRows, NumCols> const& mat1, bool equal, bool lessThan)
        {
            bool result = (mat0 == mat1);
            UTAssert(result == equal, "Incorrect operator==");
            result = (mat0 < mat1);
            UTAssert(result == lessThan, "Incorrect operator<");
            UTAssert((mat0 != mat1) == !equal, "Incorrect operator!=");
            UTAssert((mat0 <= mat1) == equal || lessThan, "Incorrect operator<=");
            UTAssert((mat0 > mat1) == (!equal && !lessThan), "Incorrect operator>");
            UTAssert((mat0 >= mat1) == !lessThan, "Incorrect operator>=");
        }

        template <typename T>
        void DoComparisons(Matrix<T> const& mat0, Matrix<T> const& mat1, bool equal, bool lessThan)
        {
            bool result = (mat0 == mat1);
            UTAssert(result == equal, "Incorrect operator==");
            result = (mat0 < mat1);
            UTAssert(result == lessThan, "Incorrect operator<");
            UTAssert((mat0 != mat1) == !equal, "Incorrect operator!=");
            UTAssert((mat0 <= mat1) == equal || lessThan, "Incorrect operator<=");
            UTAssert((mat0 > mat1) == (!equal && !lessThan), "Incorrect operator>");
            UTAssert((mat0 >= mat1) == !lessThan, "Incorrect operator>=");
        }

        // Tests for Matrix2x2, Matrix3x3 and Matrix4x4.
        void Geometric2x2();
        void Geometric3x3();
        void Geometric4x4();
        void ObliqueProjection();
        void PerspectiveProjection();
        void Reflection();
    };
}

UnitTestMatrix::UnitTestMatrix()
{
    UTInformation("Mathematics/Algebra/Matrix");

    ConstructorsTN();
    AccessorsTN();
    ComparisonsTN();
    SpecialObjectsTN();
    UnaryOperationsTN();
    LinearAlgebraicOperationsTN();
    GeometricOperationsTN();
    MatrixAndVectorOperationsTN();
    LiftProjectTN();

    ConstructorsT();
    AccessorsT();
    ComparisonsT();
    SpecialObjectsT();
    UnaryOperationsT();
    LinearAlgebraicOperationsT();
    GeometricOperationsT();
    MatrixAndVectorOperationsT();
    LiftProjectT();

    Geometric2x2();
    Geometric3x3();
    Geometric4x4();
    ObliqueProjection();
    PerspectiveProjection();
    Reflection();
}

void UnitTestMatrix::ConstructorsTN()
{
    Matrix<float, 2, 3> mrow;
    UTAssert(
        mrow(0, 0) == 0.0f &&
        mrow(0, 1) == 0.0f &&
        mrow(0, 2) == 0.0f &&
        mrow(1, 0) == 0.0f &&
        mrow(1, 1) == 0.0f &&
        mrow(1, 2) == 0.0f,
        "Default constructor failed");

    Matrix<float, 2, 3> mlistrow{ {1.0f, 2.0f, 3.0f}, {4.0f, 5.0f, 6.0f} };
    UTAssert(
        mlistrow(0, 0) == 1.0f &&
        mlistrow(0, 1) == 2.0f &&
        mlistrow(0, 2) == 3.0f &&
        mlistrow(1, 0) == 4.0f &&
        mlistrow(1, 1) == 5.0f &&
        mlistrow(1, 2) == 6.0f,
        "Initializer constructor failed");

    Matrix<float, 2, 3> mcopy = mlistrow;
    UTAssert(
        mcopy(0, 0) == 1.0f &&
        mcopy(0, 1) == 2.0f &&
        mcopy(0, 2) == 3.0f && 
        mcopy(1, 0) == 4.0f &&
        mcopy(1, 1) == 5.0f &&
        mcopy(1, 2) == 6.0f,
        "Create from Matrix<T,R,C> failed");

    Matrix<float> mdynamic{ {1.0f, 2.0f, 3.0f}, {4.0f, 5.0f, 6.0f} };
    Matrix<float> mdcopy = mdynamic;
    UTAssert(
        mdcopy(0, 0) == 1.0f &&
        mdcopy(0, 1) == 2.0f &&
        mdcopy(0, 2) == 3.0f &&
        mdcopy(1, 0) == 4.0f &&
        mdcopy(1, 1) == 5.0f &&
        mdcopy(1, 2) == 6.0f,
        "Create from Matrix<T> failed");
}

void UnitTestMatrix::AccessorsTN()
{
    Matrix<float, 2, 3> mat{ {1.0f, 2.0f, 3.0f}, {4.0f, 5.0f, 6.0f} };

    std::size_t numElements = mat.size();
    UTAssert(numElements == 6, "size() failed");

    float const* cdata = mat.data();
    for (std::size_t i = 0; i < 6; ++i)
    {
        UTAssert(cdata[i] == mat[i], "data() or operator[] failed");
    }

    float* data = mat.data();
    for (std::size_t i = 0; i < 6; ++i)
    {
        UTAssert(data[i] == mat[i], "data() or operator[] failed");
    }

    std::size_t numRows = mat.GetNumRows();
    UTAssert(numRows == 2, "GetNumRows failed");

    std::size_t numCols = mat.GetNumCols();
    UTAssert(numCols == 3, "GetNumCols failed");

    std::size_t r1c0 = mat.GetIndex(1, 0);
    UTAssert(r1c0 == 3, "GetIndex failed");

    Vector<float, 3> setrow = { -3.0f, -2.0f, -1.0f };
    mat.SetRow(0, setrow);
    UTAssert(mat(0, 0) == setrow[0] && mat(0, 1) == setrow[1]
        && mat(0, 2) == setrow[2], "SetRow failed");
    Vector<float, 3> getrow = mat.GetRow(0);
    UTAssert(mat(0, 0) == getrow[0] && mat(0, 1) == getrow[1]
        && mat(0, 2) == getrow[2], "GetRow failed");
    mat.SetRow(0, Vector<float, 3>{ 1.0f, 2.0f, 3.0f });
    mat.SetRow(1, setrow);
    UTAssert(mat(1, 0) == setrow[0] && mat(1, 1) == setrow[1]
        && mat(1, 2) == setrow[2], "SetRow failed");
    getrow = mat.GetRow(1);
    UTAssert(mat(1, 0) == getrow[0] && mat(1, 1) == getrow[1]
        && mat(1, 2) == getrow[2], "GetRow failed");
    mat.SetRow(0, Vector<float, 3>{ 1.0f, 2.0f, 3.0f });

    Vector<float, 2> setcol = { -4.0f, -1.0f };
    mat.SetCol(0, setcol);
    UTAssert(mat(0, 0) == setcol[0] && mat(1, 0) == setcol[1], "SetCol failed");
    Vector<float, 2> getcol = mat.GetCol(0);
    UTAssert(mat(0, 0) == getcol[0] && mat(1, 0) == getcol[1], "GetCol failed");
    mat.SetCol(0, Vector<float, 2>{ 1.0f, 4.0f });
    mat.SetCol(1, setcol);
    UTAssert(mat(0, 1) == setcol[0] && mat(1, 1) == setcol[1], "SetCol failed");
    getcol = mat.GetCol(1);
    UTAssert(mat(0, 1) == getcol[0] && mat(1, 1) == getcol[1], "GetCol failed");
    mat.SetCol(0, Vector<float, 2>{ 1.0f, 4.0f });
    mat.SetCol(2, setcol);
    UTAssert(mat(0, 2) == setcol[0] && mat(1, 2) == setcol[1], "SetCol failed");
    getcol = mat.GetCol(2);
    UTAssert(mat(0, 2) == getcol[0] && mat(1, 2) == getcol[1], "GetCol failed");
    mat.SetCol(0, Vector<float, 2>{ 1.0f, 4.0f });
}

void UnitTestMatrix::ConstructorsT()
{
    Matrix<float> mrow(2, 3);
    UTAssert(
        mrow(0, 0) == 0.0f &&
        mrow(0, 1) == 0.0f &&
        mrow(0, 2) == 0.0f &&
        mrow(1, 0) == 0.0f &&
        mrow(1, 1) == 0.0f &&
        mrow(1, 2) == 0.0f,
        "Default constructor failed");

    Matrix<float> mlistrow{ {1.0f, 2.0f, 3.0f}, {4.0f, 5.0f, 6.0f} };
    UTAssert(
        mlistrow(0, 0) == 1.0f &&
        mlistrow(0, 1) == 2.0f &&
        mlistrow(0, 2) == 3.0f &&
        mlistrow(1, 0) == 4.0f &&
        mlistrow(1, 1) == 5.0f &&
        mlistrow(1, 2) == 6.0f,
        "Initializer constructor failed");

    Matrix<float> mcopy = mlistrow;
    UTAssert(
        mcopy(0, 0) == 1.0f &&
        mcopy(0, 1) == 2.0f &&
        mcopy(0, 2) == 3.0f &&
        mcopy(1, 0) == 4.0f &&
        mcopy(1, 1) == 5.0f &&
        mcopy(1, 2) == 6.0f,
        "Create from <DYN,DYN,ADAPTER_NONE> failed");
}

void UnitTestMatrix::AccessorsT()
{
    Matrix<float> mat{ {1.0f, 2.0f, 3.0f}, {4.0f, 5.0f, 6.0f} };

    std::size_t numElements = mat.size();
    UTAssert(numElements == 6, "size() failed");

    float const* cdata = mat.data();
    for (std::size_t i = 0; i < 6; ++i)
    {
        UTAssert(cdata[i] == mat[i], "data() or operator[] failed");
    }

    float* data = mat.data();
    for (std::size_t i = 0; i < 6; ++i)
    {
        UTAssert(data[i] == mat[i], "data() or operator[] failed");
    }

    std::size_t numRows = mat.GetNumRows();
    UTAssert(numRows == 2, "GetNumRows failed");

    std::size_t numCols = mat.GetNumCols();
    UTAssert(numCols == 3, "GetNumCols failed");

    std::size_t r1c0 = mat.GetIndex(1, 0);
    UTAssert(r1c0 == 3, "GetIndex failed");

    Vector<float> setrow = { -3.0f, -2.0f, -1.0f };
    mat.SetRow(0, setrow);
    UTAssert(mat(0, 0) == setrow[0] && mat(0, 1) == setrow[1]
        && mat(0, 2) == setrow[2], "SetRow failed");
    Vector<float> getrow = mat.GetRow(0);
    UTAssert(mat(0, 0) == getrow[0] && mat(0, 1) == getrow[1]
        && mat(0, 2) == getrow[2], "GetRow failed");
    mat.SetRow(1, setrow);
    UTAssert(mat(1, 0) == setrow[0] && mat(1, 1) == setrow[1]
        && mat(1, 2) == setrow[2], "SetRow failed");
    getrow = mat.GetRow(1);
    UTAssert(mat(1, 0) == getrow[0] && mat(1, 1) == getrow[1]
        && mat(1, 2) == getrow[2], "GetRow failed");
    mat.SetRow(0, Vector<float>{ 1.0f, 2.0f, 3.0f });

    Vector<float> setcol = { -4.0f, -1.0f };
    mat.SetCol(0, setcol);
    UTAssert(mat(0, 0) == setcol[0] && mat(1, 0) == setcol[1], "SetCol failed");
    Vector<float> getcol = mat.GetCol(0);
    UTAssert(mat(0, 0) == getcol[0] && mat(1, 0) == getcol[1], "GetCol failed");
    mat.SetCol(1, setcol);
    UTAssert(mat(0, 1) == setcol[0] && mat(1, 1) == setcol[1], "SetCol failed");
    getcol = mat.GetCol(1);
    UTAssert(mat(0, 1) == getcol[0] && mat(1, 1) == getcol[1], "GetCol failed");
    mat.SetCol(2, setcol);
    UTAssert(mat(0, 2) == setcol[0] && mat(1, 2) == setcol[1], "SetCol failed");
    getcol = mat.GetCol(2);
    UTAssert(mat(0, 2) == getcol[0] && mat(1, 2) == getcol[1], "GetCol failed");
}

void UnitTestMatrix::ComparisonsTN()
{
    Matrix<float, 2, 3> m0 = { { 1.0f, 2.0f, 3.0f }, { 4.0f, 5.0f, 6.0f } };
    Matrix<float, 2, 3> m1 = { { 7.0f, 8.0f, 9.0f }, { 10.0f, 11.0f, 12.0f } };
    Matrix<float, 2, 3> m2 = { { 2.0f, 5.0f, 1.0f }, { 0.0f, 1.0f, 2.0f } };

    DoComparisons(m0, m0, true, false);
    DoComparisons(m0, m1, false, true);
    DoComparisons(m1, m2, false, false);
}

void UnitTestMatrix::ComparisonsT()
{
    Matrix<float> m0 = { { 1.0f, 2.0f, 3.0f }, { 4.0f, 5.0f, 6.0f } };
    Matrix<float> m1 = { { 7.0f, 8.0f, 9.0f }, { 10.0f, 11.0f, 12.0f } };
    Matrix<float> m2 = { { 2.0f, 5.0f, 1.0f }, { 0.0f, 1.0f, 2.0f } };

    DoComparisons(m0, m0, true, false);
    DoComparisons(m0, m1, false, true);
    DoComparisons(m1, m2, false, false);
}

void UnitTestMatrix::SpecialObjectsTN()
{
    Matrix<float, 2, 3> mat;
    mat.fill(3.0f);
    for (std::size_t i = 0; i < 6; ++i)
    {
        UTAssert(mat[i] == 3.0f, "Fill failed");
    }

    MakeZero(mat);
    for (std::size_t i = 0; i < 6; ++i)
    {
        UTAssert(mat[i] == 0.0f, "MakeZero failed");
    }
    bool isZero = IsZero(mat);
    UTAssert(isZero, "IsZero failed");

    MakeUnit(1, 1, mat);
    UTAssert(mat(0, 0) == 0.0f && mat(0, 1) == 0.0f && mat(0, 2) == 0.0f
        && mat(1, 0) == 0.0f && mat(1, 1) == 1.0f && mat(1, 2) == 0.0f,
        "MakeUnit failed");
    bool isUnit = IsUnit(1, 1, mat);
    UTAssert(isUnit, "IsUnit failed");

    Matrix<float, 2, 2> sqr;
    MakeIdentity(sqr);
    UTAssert(sqr(0, 0) == 1.0f && sqr(0, 1) == 0.0f && sqr(1, 0) == 0.0f
        && sqr(1, 1) == 1.0f, "MakeIdentity failed");
    bool isIdentity = IsIdentity(sqr);
    UTAssert(isIdentity, "IsIdentity failed");

    std::array<float, 2> diagonal = { 2.0f, 3.0f };
    MakeDiagonal(diagonal, sqr);
    UTAssert(sqr(0, 0) == diagonal[0] && sqr(0, 1) == 0.0f && sqr(1, 0) == 0.0f
        && sqr(1, 1) == diagonal[1], "MakeDiagonal failed");
    bool isDiagonal = IsDiagonal(sqr);
    UTAssert(isDiagonal, "IsDiagonal failed");

    Vector<float, 2> u = { 1.0f, 2.0f };
    Vector<float, 3> v = { 3.0f, 4.0f, 5.0f };
    mat = OuterProduct(u, v);
    UTAssert(
        mat(0, 0) == 3.0f &&
        mat(0, 1) == 4.0f &&
        mat(0, 2) == 5.0f &&
        mat(1, 0) == 6.0f &&
        mat(1, 1) == 8.0f &&
        mat(1, 2) == 10.0f,
        "OuterProduct failed");
}

void UnitTestMatrix::SpecialObjectsT()
{
    Matrix<float> mat(2, 3);
    mat.fill(3.0f);
    for (std::size_t i = 0; i < 6; ++i)
    {
        UTAssert(mat[i] == 3.0f, "Fill failed");
    }

    MakeZero(mat);
    for (std::size_t i = 0; i < 6; ++i)
    {
        UTAssert(mat[i] == 0.0f, "MakeZero failed");
    }
    bool isZero = IsZero(mat);
    UTAssert(isZero, "IsZero failed");

    MakeUnit(1, 1, mat);
    UTAssert(mat(0, 0) == 0.0f && mat(0, 1) == 0.0f && mat(0, 2) == 0.0f
        && mat(1, 0) == 0.0f && mat(1, 1) == 1.0f && mat(1, 2) == 0.0f,
        "MakeUnit failed");
    bool isUnit = IsUnit(1, 1, mat);
    UTAssert(isUnit, "IsUnit failed");

    Matrix<float> sqr(2, 2);
    MakeIdentity(sqr);
    UTAssert(sqr(0, 0) == 1.0f && sqr(0, 1) == 0.0f && sqr(1, 0) == 0.0f
        && sqr(1, 1) == 1.0f, "MakeIdentity failed");
    bool isIdentity = IsIdentity(sqr);
    UTAssert(isIdentity, "IsIdentity failed");

    std::vector<float> diagonal = { 2.0f, 3.0f };
    MakeDiagonal(diagonal, sqr);
    UTAssert(sqr(0, 0) == diagonal[0] && sqr(0, 1) == 0.0f && sqr(1, 0) == 0.0f
        && sqr(1, 1) == diagonal[1], "MakeDiagonal failed");
    bool isDiagonal = IsDiagonal(sqr);
    UTAssert(isDiagonal, "IsDiagonal failed");

    Vector<float> u = { 1.0f, 2.0f };
    Vector<float> v = { 3.0f, 4.0f, 5.0f };
    mat = OuterProduct(u, v);
    UTAssert(
        mat(0, 0) == 3.0f &&
        mat(0, 1) == 4.0f &&
        mat(0, 2) == 5.0f &&
        mat(1, 0) == 6.0f &&
        mat(1, 1) == 8.0f &&
        mat(1, 2) == 10.0f,
        "OuterProduct failed");
}

void UnitTestMatrix::UnaryOperationsTN()
{
    Matrix<float, 2, 3> m = { { 1.0f, 2.0f, 3.0f }, { 4.0f, 5.0f, 6.0f } };

    m = +m;
    UTAssert(
        m(0, 0) == 1.0f &&
        m(0, 1) == 2.0f &&
        m(0, 2) == 3.0f &&
        m(1, 0) == 4.0f &&
        m(1, 1) == 5.0f &&
        m(1, 2) == 6.0f,
        "unary positive failed");

    m = -m;
    UTAssert(
        m(0, 0) == -1.0f &&
        m(0, 1) == -2.0f &&
        m(0, 2) == -3.0f &&
        m(1, 0) == -4.0f &&
        m(1, 1) == -5.0f &&
        m(1, 2) == -6.0f,
        "unary negation failed");
}

void UnitTestMatrix::UnaryOperationsT()
{
    Matrix<float> m = { { 1.0f, 2.0f, 3.0f }, { 4.0f, 5.0f, 6.0f } };

    m = +m;
    UTAssert(
        m(0, 0) == 1.0f &&
        m(0, 1) == 2.0f &&
        m(0, 2) == 3.0f &&
        m(1, 0) == 4.0f &&
        m(1, 1) == 5.0f &&
        m(1, 2) == 6.0f,
        "unary positive failed");

    m = -m;
    UTAssert(
        m(0, 0) == -1.0f &&
        m(0, 1) == -2.0f &&
        m(0, 2) == -3.0f &&
        m(1, 0) == -4.0f &&
        m(1, 1) == -5.0f &&
        m(1, 2) == -6.0f,
        "unary negation failed");
}

void UnitTestMatrix::LinearAlgebraicOperationsTN()
{
    Matrix<float, 2, 3> m0 = { { 1.0f, 2.0f, 3.0f }, { 4.0f, 5.0f, 6.0f } };
    Matrix<float, 2, 3> m1 = { { -1.0f, 0.0f, 6.0f }, { 1.0f, -9.0f, -7.0f } };
    Matrix<float, 2, 3> m2 = { { 2.0f, 5.0f, 1.0f }, { 0.0f, 1.0f, 2.0f } };

    m2 = m0 + m1;
    UTAssert(
        m2(0, 0) == 0.0f &&
        m2(0, 1) == 2.0f &&
        m2(0, 2) == 9.0f &&
        m2(1, 0) == 5.0f &&
        m2(1, 1) == -4.0f &&
        m2(1, 2) == -1.0f,
        "operator+ failed");

    m2 = m0 - m1;
    UTAssert(
        m2(0, 0) == 2.0f &&
        m2(0, 1) == 2.0f &&
        m2(0, 2) == -3.0f &&
        m2(1, 0) == 3.0f &&
        m2(1, 1) == 14.0f &&
        m2(1, 2) == 13.0f,
        "operator- failed");

    m2 = 2.0f * m0;
    UTAssert(
        m2(0, 0) == 2.0f &&
        m2(0, 1) == 4.0f &&
        m2(0, 2) == 6.0f &&
        m2(1, 0) == 8.0f &&
        m2(1, 1) == 10.0f &&
        m2(1, 2) == 12.0f,
        "operator* failed");

    m2 = m0 * 2.0f;
    UTAssert(
        m2(0, 0) == 2.0f &&
        m2(0, 1) == 4.0f &&
        m2(0, 2) == 6.0f &&
        m2(1, 0) == 8.0f &&
        m2(1, 1) == 10.0f &&
        m2(1, 2) == 12.0f,
        "operator* failed");

    m2 = m0 / 2.0f;
    UTAssert(
        m2(0, 0) == 0.5f &&
        m2(0, 1) == 1.0f &&
        m2(0, 2) == 1.5f &&
        m2(1, 0) == 2.0f &&
        m2(1, 1) == 2.5f &&
        m2(1, 2) == 3.0f,
        "operator* failed");
}

void UnitTestMatrix::LinearAlgebraicOperationsT()
{
    Matrix<float> m0 = { { 1.0f, 2.0f, 3.0f }, { 4.0f, 5.0f, 6.0f } };
    Matrix<float> m1 = { { -1.0f, 0.0f, 6.0f }, { 1.0f, -9.0f, -7.0f } };
    Matrix<float> m2 = { { 2.0f, 5.0f, 1.0f }, { 0.0f, 1.0f, 2.0f } };

    m2 = m0 + m1;
    UTAssert(
        m2(0, 0) == 0.0f &&
        m2(0, 1) == 2.0f &&
        m2(0, 2) == 9.0f &&
        m2(1, 0) == 5.0f &&
        m2(1, 1) == -4.0f &&
        m2(1, 2) == -1.0f,
        "operator+ failed");

    m2 = m0 - m1;
    UTAssert(
        m2(0, 0) == 2.0f &&
        m2(0, 1) == 2.0f &&
        m2(0, 2) == -3.0f &&
        m2(1, 0) == 3.0f &&
        m2(1, 1) == 14.0f &&
        m2(1, 2) == 13.0f,
        "operator- failed");

    m2 = 2.0f * m0;
    UTAssert(
        m2(0, 0) == 2.0f &&
        m2(0, 1) == 4.0f &&
        m2(0, 2) == 6.0f &&
        m2(1, 0) == 8.0f &&
        m2(1, 1) == 10.0f &&
        m2(1, 2) == 12.0f,
        "operator* failed");

    m2 = m0 * 2.0f;
    UTAssert(
        m2(0, 0) == 2.0f &&
        m2(0, 1) == 4.0f &&
        m2(0, 2) == 6.0f &&
        m2(1, 0) == 8.0f &&
        m2(1, 1) == 10.0f &&
        m2(1, 2) == 12.0f,
        "operator* failed");

    m2 = m0 / 2.0f;
    UTAssert(
        m2(0, 0) == 0.5f &&
        m2(0, 1) == 1.0f &&
        m2(0, 2) == 1.5f &&
        m2(1, 0) == 2.0f &&
        m2(1, 1) == 2.5f &&
        m2(1, 2) == 3.0f,
        "operator* failed");
}

void UnitTestMatrix::GeometricOperationsTN()
{
    Matrix<float, 2, 3> mrow = { { 1.0f, 0.0f, 3.0f }, { -1.0f, -4.0f, 2.0f } };

    float norm = L1Norm(mrow);
    UTAssert(norm == 11.0f, "L1Norm failed");

    norm = L2Norm(mrow);
    UTAssert(norm == std::sqrt(31.0f), "L2Norm failed");

    norm = LInfinityNorm(mrow);
    UTAssert(norm == 4.0f, "LInfinityNorm failed");

    Matrix<float, 2, 2> mat0 = { { 1.0f, 2.0f }, { 3.0f, 4.0f } };
    float trace = Trace(mat0);
    UTAssert(trace == 5.0f, "Trace failed");

    Matrix<float, 3, 2> mrowtrn = Transpose(mrow);
    UTAssert(
        mrowtrn(0, 0) == 1.0f &&
        mrowtrn(0, 1) == -1.0f &&
        mrowtrn(1, 0) == 0.0f &&
        mrowtrn(1, 1) == -4.0f &&
        mrowtrn(2, 0) == 3.0f &&
        mrowtrn(2, 1) == 2.0f,
        "Transpose failed");

    Matrix<float, 3, 3> A
    {
        {1.0f, 2.0f, 4.0f },
        {0.0f, -1.0f, 1.0f},
        {4.0f, 8.0f, 2.0f}
    };

    Matrix<float, 3, 3> trueInverseA
    {
        {-5.0f / 7.0f, 2.0f, 3.0f / 7.0f},
        {2.0f / 7.0f, -1.0f, -1.0f / 14.0f},
        {2.0f / 7.0f, 0.0f, -1.0f / 14.0f}
    };

    float trueDeterminant = 14.0f;

    Matrix<float, 3, 3> inverseA;

    float const epsilon = 1e-06f;
    float determinant = 0.0f;
    inverseA = Inverse(A, &determinant);
    UTAssert(determinant != 0.0f, "Inverse failed");
    Matrix<float, 3, 3> diff3x3 = inverseA - trueInverseA;
    for (std::size_t i = 0; i < 9; ++i)
    {
        UTAssert(std::fabs(diff3x3[i]) <= epsilon, "Inverse inverse failed");
    }
    determinant = Determinant(A);
    UTAssert(std::fabs(determinant - trueDeterminant) <= epsilon, "Determinant failed");
}

void UnitTestMatrix::GeometricOperationsT()
{
    Matrix<float> mrow = { { 1.0f, 0.0f, 3.0f }, { -1.0f, -4.0f, 2.0f } };

    float norm = L1Norm(mrow);
    UTAssert(norm == 11.0f, "L1Norm failed");

    norm = L2Norm(mrow);
    UTAssert(norm == std::sqrt(31.0f), "L2Norm failed");

    norm = LInfinityNorm(mrow);
    UTAssert(norm == 4.0f, "LInfinityNorm failed");

    Matrix<float> mat0 = { { 1.0f, 2.0f }, { 3.0f, 4.0f } };
    float trace = Trace(mat0);
    UTAssert(trace == 5.0f, "Trace failed");

    Matrix<float> mrowtrn = Transpose(mrow);
    UTAssert(
        mrowtrn(0, 0) == 1.0f &&
        mrowtrn(0, 1) == -1.0f &&
        mrowtrn(1, 0) == 0.0f &&
        mrowtrn(1, 1) == -4.0f &&
        mrowtrn(2, 0) == 3.0f &&
        mrowtrn(2, 1) == 2.0f,
        "Transpose failed");

    Matrix<float> A
    {
        {1.0f, 2.0f, 4.0f },
        {0.0f, -1.0f, 1.0f},
        {4.0f, 8.0f, 2.0f}
    };

    Matrix<float> trueInverseA
    {
        {-5.0f / 7.0f, 2.0f, 3.0f / 7.0f},
        {2.0f / 7.0f, -1.0f, -1.0f / 14.0f},
        {2.0f / 7.0f, 0.0f, -1.0f / 14.0f}
    };

    float trueDeterminant = 14.0f;

    Matrix<float> inverseA;

    float const epsilon = 1e-06f;
    float determinant = 0.0f;
    inverseA = Inverse(A, &determinant);
    UTAssert(determinant != 0.0f, "Inverse failed");
    Matrix<float> diff3x3 = inverseA - trueInverseA;
    for (std::size_t i = 0; i < 9; ++i)
    {
        UTAssert(std::fabs(diff3x3[i]) <= epsilon, "Inverse inverse failed");
    }
    determinant = Determinant(A);
    UTAssert(std::fabs(determinant - trueDeterminant) <= epsilon, "Determinant failed");
}

void UnitTestMatrix::MatrixAndVectorOperationsTN()
{
    Matrix<float, 2, 3> A = { { 1.0f, 0.0f, 3.0f }, { -1.0f, -4.0f, 2.0f } };
    Matrix<float, 3, 2> B = { { -2.0f, 5.0f}, { 1.0f, 6.0f }, { 1.0f, -3.0f } };

    Matrix<float, 2, 2> C0;
    Matrix<float, 3, 3> C1;
    Matrix<float, 2, 3> A1;

    Vector<float, 2> u = { 3.0f, 4.0f };
    Vector<float, 3> v = { -5.0f, 2.0f, 1.0f };
    Vector<float, 2> w0;
    Vector<float, 3> w1;
    
    w0 = A * v;
    UTAssert(w0[0] == -2.0f && w0[1] == -1.0f, "M*v failed");

    w1 = u * A;
    UTAssert(w1[0] == -1.0f && w1[1] == -16.0f && w1[2] == 17.0f, "v*M failed");

    C0 = MultiplyAB(A, B);
    UTAssert(
        C0(0, 0) == 1.0f
        && C0(0, 1) == -4.0f
        && C0(1, 0) == 0.0f
        && C0(1, 1) == -35.0f,
        "MultiplyAB failed");

    C0 = MultiplyABT(A, A);
    UTAssert(
        C0(0, 0) == 10.0f
        && C0(0, 1) == 5.0f
        && C0(1, 0) == 5.0f
        && C0(1, 1) == 21.0f,
        "MultiplyABT failed");

    C1 = MultiplyATB(A, A);
    UTAssert(
        C1(0, 0) == 2.0f
        && C1(0, 1) == 4.0f
        && C1(0, 2) == 1.0f
        && C1(1, 0) == 4.0f
        && C1(1, 1) == 16.0f
        && C1(1, 2) == -8.0f
        && C1(2, 0) == 1.0f
        && C1(2, 1) == -8.0f
        && C1(2, 2) == 13.0f,
        "MultiplyATB failed");

    C1 = MultiplyATBT(A, B);
    UTAssert(
        C1(0, 0) == -7.0f
        && C1(0, 1) == -5.0f
        && C1(0, 2) == 4.0f
        && C1(1, 0) == -20.0f
        && C1(1, 1) == -24.0f
        && C1(1, 2) == 12.0f
        && C1(2, 0) == 4.0f
        && C1(2, 1) == 15.0f
        && C1(2, 2) == -3.0f,
        "MultiplyATBT failed");

    std::array<float, 3> d0 = { 3.0f, 2.0f, -1.0f };
    A1 = MultiplyMD(A, d0);
    UTAssert(
        A1(0, 0) == 3.0f
        && A1(0, 1) == 0.0f
        && A1(0, 2) == -3.0f
        && A1(1, 0) == -3.0f
        && A1(1, 1) == -8.0f
        && A1(1, 2) == -2.0f,
        "MultiplyMD failed");

    std::array<float, 2> d1 = { 3.0f, -2.0f };
    A1 = MultiplyDM(d1, A);
    UTAssert(
        A1(0, 0) == 3.0f
        && A1(0, 1) == 0.0f
        && A1(0, 2) == 9.0f
        && A1(1, 0) == 2.0f
        && A1(1, 1) == 8.0f
        && A1(1, 2) == -4.0f,
        "MultiplyDM failed");
}

void UnitTestMatrix::MatrixAndVectorOperationsT()
{
    Matrix<float> A = { { 1.0f, 0.0f, 3.0f }, { -1.0f, -4.0f, 2.0f } };
    Matrix<float> B = { { -2.0f, 5.0f}, { 1.0f, 6.0f }, { 1.0f, -3.0f } };

    Matrix<float> C0(2, 2);
    Matrix<float> C1(3, 3);
    Matrix<float> A1(2, 3);

    Vector<float> u = { 3.0f, 4.0f };
    Vector<float> v = { -5.0f, 2.0f, 1.0f };
    Vector<float> w0(2);
    Vector<float> w1(3);

    w0 = A * v;
    UTAssert(w0[0] == -2.0f && w0[1] == -1.0f, "M*v failed");

    w1 = u * A;
    UTAssert(w1[0] == -1.0f && w1[1] == -16.0f && w1[2] == 17.0f, "v*M failed");

    C0 = MultiplyAB(A, B);
    UTAssert(
        C0(0, 0) == 1.0f
        && C0(0, 1) == -4.0f
        && C0(1, 0) == 0.0f
        && C0(1, 1) == -35.0f,
        "MultiplyAB failed");

    C0 = MultiplyABT(A, A);
    UTAssert(
        C0(0, 0) == 10.0f
        && C0(0, 1) == 5.0f
        && C0(1, 0) == 5.0f
        && C0(1, 1) == 21.0f,
        "MultiplyABT failed");

    C1 = MultiplyATB(A, A);
    UTAssert(
        C1(0, 0) == 2.0f
        && C1(0, 1) == 4.0f
        && C1(0, 2) == 1.0f
        && C1(1, 0) == 4.0f
        && C1(1, 1) == 16.0f
        && C1(1, 2) == -8.0f
        && C1(2, 0) == 1.0f
        && C1(2, 1) == -8.0f
        && C1(2, 2) == 13.0f,
        "MultiplyATB failed");

    C1 = MultiplyATBT(A, B);
    UTAssert(
        C1(0, 0) == -7.0f
        && C1(0, 1) == -5.0f
        && C1(0, 2) == 4.0f
        && C1(1, 0) == -20.0f
        && C1(1, 1) == -24.0f
        && C1(1, 2) == 12.0f
        && C1(2, 0) == 4.0f
        && C1(2, 1) == 15.0f
        && C1(2, 2) == -3.0f,
        "MultiplyATBT failed");

    std::vector<float> d0 = { 3.0f, 2.0f, -1.0f };
    A1 = MultiplyMD(A, d0);
    UTAssert(
        A1(0, 0) == 3.0f
        && A1(0, 1) == 0.0f
        && A1(0, 2) == -3.0f
        && A1(1, 0) == -3.0f
        && A1(1, 1) == -8.0f
        && A1(1, 2) == -2.0f,
        "MultiplyMD failed");

    std::vector<float> d1 = { 3.0f, -2.0f };
    A1 = MultiplyDM(d1, A);
    UTAssert(
        A1(0, 0) == 3.0f
        && A1(0, 1) == 0.0f
        && A1(0, 2) == 9.0f
        && A1(1, 0) == 2.0f
        && A1(1, 1) == 8.0f
        && A1(1, 2) == -4.0f,
        "MultiplyDM failed");
}

void UnitTestMatrix::LiftProjectTN()
{
    Matrix<float, 2, 2> A = { { 1.0f, 2.0f }, { 3.0f, 4.0f } };
    Matrix<float, 3, 3> HA = HLift(A);
    UTAssert(
        HA(0, 0) == 1.0f
        && HA(0, 1) == 2.0f
        && HA(0, 2) == 0.0f
        && HA(1, 0) == 3.0f
        && HA(1, 1) == 4.0f
        && HA(1, 2) == 0.0f
        && HA(2, 0) == 0.0f
        && HA(2, 1) == 0.0f
        && HA(2, 2) == 1.0f,
        "HLift failed");

    Matrix<float, 3, 3> B = { {1.0f, 2.0f, 3.0f}, {4.0f, 5.0f, 6.0f}, {7.0f, 8.0f, 9.0f} };
    Matrix<float, 2, 2> PB = HProject(B);
    UTAssert(
        PB(0, 0) == 1.0f
        && PB(0, 1) == 2.0f
        && PB(1, 0) == 4.0f
        && PB(1, 1) == 5.0f,
        "HProject failed");
}

void UnitTestMatrix::LiftProjectT()
{
    Matrix<float> A = { {1.0f, 2.0f}, {3.0f, 4.0f} };
    Matrix<float> HA = HLift(A);
    UTAssert(
        HA(0, 0) == 1.0f
        && HA(0, 1) == 2.0f
        && HA(0, 2) == 0.0f
        && HA(1, 0) == 3.0f
        && HA(1, 1) == 4.0f
        && HA(1, 2) == 0.0f
        && HA(2, 0) == 0.0f
        && HA(2, 1) == 0.0f
        && HA(2, 2) == 1.0f,
        "HLift failed");

    Matrix<float> B = { {1.0f, 2.0f, 3.0f}, {4.0f, 5.0f, 6.0f}, {7.0f, 8.0f, 9.0f} };
    Matrix<float> PB = HProject(B);
    UTAssert(
        PB(0, 0) == 1.0f
        && PB(0, 1) == 2.0f
        && PB(1, 0) == 4.0f
        && PB(1, 1) == 5.0f,
        "HProject failed");
}

void UnitTestMatrix::Geometric2x2()
{
    Matrix2x2<float> A = { { 1.0f, 2.0f }, { 3.0f, 4.0f } };

    auto invA = GetInverse(A);
    UTAssert(
        invA(0, 0) == -2.0f &&
        invA(0, 1) == 1.0f &&
        invA(1, 0) == 1.5f &&
        invA(1, 1) == -0.5f,
        "Incorrect inverse.");

    auto adjA = GetAdjoint(A);
    UTAssert(
        adjA(0, 0) == 4.0f &&
        adjA(0, 1) == -2.0f &&
        adjA(1, 0) == -3.0f &&
        adjA(1, 1) == 1.0f,
        "Incorrect adjoint.");

    auto detA = GetDeterminant(A);
    UTAssert(detA == -2.0f, "Incorrect determinant.");

    auto traceA = GetTrace(A);
    UTAssert(traceA == 5.0f, "Incorrect trace.");
}

void UnitTestMatrix::Geometric3x3()
{
    Matrix3x3<Rational> A = { { 2, 3, 5 }, { 7, 11, 13 }, { 17, 19, 23 } };

    auto invA = GetInverse(A);
    UTAssert(
        invA(0, 0) == Rational(6, -78) &&
        invA(0, 1) == Rational(26, -78) &&
        invA(0, 2) == Rational(-16, -78) &&
        invA(1, 0) == Rational(60, -78) &&
        invA(1, 1) == Rational(-39, -78) &&
        invA(1, 2) == Rational(9, -78) &&
        invA(2, 0) == Rational(-54, -78) &&
        invA(2, 1) == Rational(13, -78) &&
        invA(2, 2) == Rational(1, -78),
        "Incorrect inverse.");

    auto adjA = GetAdjoint(A);
    UTAssert(
        adjA(0, 0) == Rational(6) &&
        adjA(0, 1) == Rational(26) &&
        adjA(0, 2) == Rational(-16) &&
        adjA(1, 0) == Rational(60) &&
        adjA(1, 1) == Rational(-39) &&
        adjA(1, 2) == Rational(9) &&
        adjA(2, 0) == Rational(-54) &&
        adjA(2, 1) == Rational(13) &&
        adjA(2, 2) == Rational(1),
        "Incorrect adjoint.");

    auto detA = GetDeterminant(A);
    UTAssert(detA == Rational(-78), "Incorrect determinant.");

    auto traceA = GetTrace(A);
    UTAssert(traceA == Rational(36), "Incorrect trace.");
}

void UnitTestMatrix::Geometric4x4()
{
    Matrix4x4<Rational> A =
    {
        { 2, 3, 5, 7 },
        { 11, 13, 17, 19 },
        { 23, 29, 31, 37 },
        { 41, 43, 47, 53 },
    };

    auto invA = GetInverse(A);
    UTAssert(
        invA(0, 0) == Rational(240, 880) &&
        invA(0, 1) == Rational(-192, 880) &&
        invA(0, 2) == Rational(-176, 880) &&
        invA(0, 3) == Rational(160, 880) &&
        invA(1, 0) == Rational(-400, 880) &&
        invA(1, 1) == Rational(-32, 880) &&
        invA(1, 2) == Rational(264, 880) &&
        invA(1, 3) == Rational(-120, 880) &&
        invA(2, 0) == Rational(-520, 880) &&
        invA(2, 1) == Rational(614, 880) &&
        invA(2, 2) == Rational(-88, 880) &&
        invA(2, 3) == Rational(-90, 880) &&
        invA(3, 0) == Rational(600, 880) &&
        invA(3, 1) == Rational(-370, 880) &&
        invA(3, 2) == Rational(0, 880) &&
        invA(3, 3) == Rational(70, 880),
        "Incorrect inverse.");

    auto adjA = GetAdjoint(A);
    UTAssert(
        adjA(0, 0) == Rational(240) &&
        adjA(0, 1) == Rational(-192) &&
        adjA(0, 2) == Rational(-176) &&
        adjA(0, 3) == Rational(160) &&
        adjA(1, 0) == Rational(-400) &&
        adjA(1, 1) == Rational(-32) &&
        adjA(1, 2) == Rational(264) &&
        adjA(1, 3) == Rational(-120) &&
        adjA(2, 0) == Rational(-520) &&
        adjA(2, 1) == Rational(614) &&
        adjA(2, 2) == Rational(-88) &&
        adjA(2, 3) == Rational(-90) &&
        adjA(3, 0) == Rational(600) &&
        adjA(3, 1) == Rational(-370) &&
        adjA(3, 2) == Rational(0) &&
        adjA(3, 3) == Rational(70),
        "Incorrect adjoint.");

    auto detA = GetDeterminant(A);
    UTAssert(detA == Rational(880), "Incorrect determinant.");

    auto traceA = GetTrace(A);
    UTAssert(traceA == Rational(99), "Incorrect trace.");
}

void UnitTestMatrix::ObliqueProjection()
{
    Vector4<Rational> origin{ 1, 2, 3, 1 };
    Vector4<Rational> normal{ Rational(11, 15), Rational(2, 15), Rational(2, 3), 0 };
    Vector4<Rational> direction{ 0, Rational(3, 5), Rational(4, 5), 0 };
    Matrix4x4<Rational> oblique = MakeObliqueProjection(origin, normal, direction);
    Vector4<Rational> Q{ 0.1234, 0.5678, 0.9012, 1 };
    Vector4<Rational> project = oblique * Q;
    project /= project[3];

    Rational t = -Dot(normal, Q - origin) / Dot(normal, direction);
    Vector4<Rational> Qprime = Q + t * direction;
    UTAssert(Qprime == project, "Invalid projection.");
}

void UnitTestMatrix::PerspectiveProjection()
{
    Vector4<Rational> origin{ 1, 2, 3, 1 };
    Vector4<Rational> normal{ Rational(4, 30), Rational(28, 30), Rational(-10, 30), 0 };
    Vector4<Rational> eye{ -1, -1, -1, 1 };
    Matrix4x4<Rational> perspective = MakePerspectiveProjection(origin, normal, eye);
    Vector4<Rational> Q{ 0.1234, 0.5678, 0.9012, 1 };
    Vector4<Rational> project = perspective * Q;
    project /= project[3];

    Rational t = Dot(normal, origin - eye) / Dot(normal, Q - eye);
    Vector4<Rational> Qprime = eye + t * (Q - eye);
    UTAssert(project == Qprime, "Invalid projection.");
}

void UnitTestMatrix::Reflection()
{
    Vector4<Rational> origin{ 1, 2, 3, 1 };
    Vector4<Rational> normal{ Rational(4, 30), Rational(28, 30), Rational(-10, 30), 0 };
    Matrix4x4<Rational> reflection = MakeReflection(origin, normal);
    Vector4<Rational> Q{ 0.1234, 0.5678, 0.9012, 1 };
    Vector4<Rational> project = reflection * Q;
    project /= project[3];

    Vector4<Rational> average = (Q + project) / Rational(2);
    Rational dot = Dot(normal, average - origin);
    UTAssert(dot.GetSign() == 0, "Invalid projection.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Algebra/Matrix.h>

namespace gtl
{
    template class Matrix<float, 2, 3>;
    using MF22 = Matrix<float, 2, 2>;
    using MF23 = Matrix<float, 2, 3>;
    using MF32 = Matrix<float, 3, 2>;
    using MF33 = Matrix<float, 3, 3>;
    using VF2 = Vector<float, 2>;
    using VF3 = Vector<float, 3>;
    template bool operator==(MF23 const&, MF23 const&);
    template bool operator!=(MF23 const&, MF23 const&);
    template bool operator<(MF23 const&, MF23 const&);
    template bool operator<=(MF23 const&, MF23 const&);
    template bool operator>(MF23 const&, MF23 const&);
    template bool operator>=(MF23 const&, MF23 const&);
    template void MakeZero(MF23&);
    template bool IsZero(MF23 const&);
    template void MakeUnit(std::size_t, std::size_t, MF23&);
    template bool IsUnit(std::size_t, std::size_t, MF23 const&);
    template void MakeIdentity(MF22&);
    template bool IsIdentity(MF22 const&);
    template void MakeDiagonal(std::array<float, 2> const&, MF22&);
    template bool IsDiagonal(MF22 const&);
    template MF23 OuterProduct(VF2 const&, VF3 const&);
    template MF23 operator+(MF23 const&);
    template MF23 operator-(MF23 const&);
    template MF23 operator+(MF23 const&, MF23 const&);
    template MF23& operator+=(MF23&, MF23 const&);
    template MF23 operator-(MF23 const&, MF23 const&);
    template MF23& operator-=(MF23&, MF23 const&);
    template MF23 operator*(MF23 const&, float const&);
    template MF23 operator*(float const&, MF23 const&);
    template MF23& operator*=(MF23&, float const&);
    template MF23 operator/(MF23 const&, float const&);
    template MF23& operator/=(MF23&, float const&);
    template float L1Norm(MF23 const&);
    template float L2Norm(MF23 const&);
    template float LInfinityNorm(MF23 const&);
    template float Trace(MF22 const&);
    template MF32 Transpose(MF23 const&);
    template VF2 operator*(MF23 const&, VF3 const&);
    template VF3 operator*(VF2 const&, MF23 const&);
    template MF22 operator*(MF23 const&, MF32 const&);
    template MF22 MultiplyAB(MF23 const&, MF32 const&);
    template MF22 MultiplyABT(MF23 const&, MF23 const&);
    template MF33 MultiplyATB(MF23 const&, MF23 const&);
    template MF33 MultiplyATBT(MF23 const&, MF32 const&);
    template MF23 MultiplyMD(MF23 const&, std::array<float, 3> const&);
    template MF23 MultiplyDM(std::array<float, 2> const&, MF23 const&);
    template MF33 HLift(MF22 const&);
    template MF22 HProject(MF33 const&);

    template class Matrix<float>;
    using MFD = Matrix<float>;
    using VFD = Vector<float>;
    template bool operator==(MFD const&, MFD const&);
    template bool operator!=(MFD const&, MFD const&);
    template bool operator<(MFD const&, MFD const&);
    template bool operator<=(MFD const&, MFD const&);
    template bool operator>(MFD const&, MFD const&);
    template bool operator>=(MFD const&, MFD const&);
    template void MakeZero(MFD&);
    template bool IsZero(MFD const&);
    template void MakeUnit(std::size_t, std::size_t, MFD&);
    template bool IsUnit(std::size_t, std::size_t, MFD const&);
    template void MakeIdentity(MFD&);
    template bool IsIdentity(MFD const&);
    template void MakeDiagonal(std::vector<float> const&, MFD&);
    template bool IsDiagonal(MFD const&);
    template MFD OuterProduct(VFD const&, VFD const&);
    template MFD operator+(MFD const&);
    template MFD operator-(MFD const&);
    template MFD operator+(MFD const&, MFD const&);
    template MFD& operator+=(MFD&, MFD const&);
    template MFD operator-(MFD const&, MFD const&);
    template MFD& operator-=(MFD&, MFD const&);
    template MFD operator*(MFD const&, float const&);
    template MFD operator*(float const&, MFD const&);
    template MFD& operator*=(MFD&, float const&);
    template MFD operator/(MFD const&, float const&);
    template MFD& operator/=(MFD&, float const&);
    template float L1Norm(MFD const&);
    template float L2Norm(MFD const&);
    template float LInfinityNorm(MFD const&);
    template float Trace(MFD const&);
    template MFD Transpose(MFD const&);
    template VFD operator*(MFD const&, VFD const&);
    template VFD operator*(VFD const&, MFD const&);
    template MFD operator*(MFD const&, MFD const&);
    template MFD MultiplyAB(MFD const&, MFD const&);
    template MFD MultiplyABT(MFD const&, MFD const&);
    template MFD MultiplyATB(MFD const&, MFD const&);
    template MFD MultiplyATBT(MFD const&, MFD const&);
    template MFD MultiplyMD(MFD const&, std::vector<float> const&);
    template MFD MultiplyDM(std::vector<float> const&, MFD const&);
    template MFD HLift(MFD const&);
    template MFD HProject(MFD const&);

    template class Matrix<double, 2, 3>;
    using MD22 = Matrix<double, 2, 2>;
    using MD23 = Matrix<double, 2, 3>;
    using MD32 = Matrix<double, 3, 2>;
    using MD33 = Matrix<double, 3, 3>;
    using VD2 = Vector<double, 2>;
    using VD3 = Vector<double, 3>;
    template bool operator==(MD23 const&, MD23 const&);
    template bool operator!=(MD23 const&, MD23 const&);
    template bool operator<(MD23 const&, MD23 const&);
    template bool operator<=(MD23 const&, MD23 const&);
    template bool operator>(MD23 const&, MD23 const&);
    template bool operator>=(MD23 const&, MD23 const&);
    template void MakeZero(MD23&);
    template bool IsZero(MD23 const&);
    template void MakeUnit(std::size_t, std::size_t, MD23&);
    template bool IsUnit(std::size_t, std::size_t, MD23 const&);
    template void MakeIdentity(MD22&);
    template bool IsIdentity(MD22 const&);
    template void MakeDiagonal(std::array<double, 2> const&, MD22&);
    template bool IsDiagonal(MD22 const&);
    template MD23 OuterProduct(VD2 const&, VD3 const&);
    template MD23 operator+(MD23 const&);
    template MD23 operator-(MD23 const&);
    template MD23 operator+(MD23 const&, MD23 const&);
    template MD23& operator+=(MD23&, MD23 const&);
    template MD23 operator-(MD23 const&, MD23 const&);
    template MD23& operator-=(MD23&, MD23 const&);
    template MD23 operator*(MD23 const&, double const&);
    template MD23 operator*(double const&, MD23 const&);
    template MD23& operator*=(MD23&, double const&);
    template MD23 operator/(MD23 const&, double const&);
    template MD23& operator/=(MD23&, double const&);
    template double L1Norm(MD23 const&);
    template double L2Norm(MD23 const&);
    template double LInfinityNorm(MD23 const&);
    template double Trace(MD22 const&);
    template MD32 Transpose(MD23 const&);
    template VD2 operator*(MD23 const&, VD3 const&);
    template VD3 operator*(VD2 const&, MD23 const&);
    template MD22 operator*(MD23 const&, MD32 const&);
    template MD22 MultiplyAB(MD23 const&, MD32 const&);
    template MD22 MultiplyABT(MD23 const&, MD23 const&);
    template MD33 MultiplyATB(MD23 const&, MD23 const&);
    template MD33 MultiplyATBT(MD23 const&, MD32 const&);
    template MD23 MultiplyMD(MD23 const&, std::array<double, 3> const&);
    template MD23 MultiplyDM(std::array<double, 2> const&, MD23 const&);
    template MD33 HLift(MD22 const&);
    template MD22 HProject(MD33 const&);

    template class Matrix<double>;
    using MDD = Matrix<double>;
    using VDD = Vector<double>;
    template bool operator==(MDD const&, MDD const&);
    template bool operator!=(MDD const&, MDD const&);
    template bool operator<(MDD const&, MDD const&);
    template bool operator<=(MDD const&, MDD const&);
    template bool operator>(MDD const&, MDD const&);
    template bool operator>=(MDD const&, MDD const&);
    template void MakeZero(MDD&);
    template bool IsZero(MDD const&);
    template void MakeUnit(std::size_t, std::size_t, MDD&);
    template bool IsUnit(std::size_t, std::size_t, MDD const&);
    template void MakeIdentity(MDD&);
    template bool IsIdentity(MDD const&);
    template void MakeDiagonal(std::vector<double> const&, MDD&);
    template bool IsDiagonal(MDD const&);
    template MDD OuterProduct(VDD const&, VDD const&);
    template MDD operator+(MDD const&);
    template MDD operator-(MDD const&);
    template MDD operator+(MDD const&, MDD const&);
    template MDD& operator+=(MDD&, MDD const&);
    template MDD operator-(MDD const&, MDD const&);
    template MDD& operator-=(MDD&, MDD const&);
    template MDD operator*(MDD const&, double const&);
    template MDD operator*(double const&, MDD const&);
    template MDD& operator*=(MDD&, double const&);
    template MDD operator/(MDD const&, double const&);
    template MDD& operator/=(MDD&, double const&);
    template double L1Norm(MDD const&);
    template double L2Norm(MDD const&);
    template double LInfinityNorm(MDD const&);
    template double Trace(MDD const&);
    template MDD Transpose(MDD const&);
    template VDD operator*(MDD const&, VDD const&);
    template VDD operator*(VDD const&, MDD const&);
    template MDD operator*(MDD const&, MDD const&);
    template MDD MultiplyAB(MDD const&, MDD const&);
    template MDD MultiplyABT(MDD const&, MDD const&);
    template MDD MultiplyATB(MDD const&, MDD const&);
    template MDD MultiplyATBT(MDD const&, MDD const&);
    template MDD MultiplyMD(MDD const&, std::vector<double> const&);
    template MDD MultiplyDM(std::vector<double> const&, MDD const&);
    template MDD HLift(MDD const&);
    template MDD HProject(MDD const&);

    //using MF22 = Matrix<float, 2, 2>;
    template MF22 GetInverse(MF22 const&, float*);
    template MF22 GetAdjoint(MF22 const&);
    template float GetDeterminant(MF22 const&);
    template float GetTrace(MF22 const&);

    //using MD22 = Matrix<double, 2, 2>;
    template MD22 GetInverse(MD22 const&, double*);
    template MD22 GetAdjoint(MD22 const&);
    template double GetDeterminant(MD22 const&);
    template double GetTrace(MD22 const&);

    //using MF33 = Matrix<float, 3, 3>;
    template MF33 GetInverse(MF33 const&, float*);
    template MF33 GetAdjoint(MF33 const&);
    template float GetDeterminant(MF33 const&);
    template float GetTrace(MF33 const&);

    //using MD33 = Matrix<double, 3, 3>;
    template MD33 GetInverse(MD33 const&, double*);
    template MD33 GetAdjoint(MD33 const&);
    template double GetDeterminant(MD33 const&);
    template double GetTrace(MD33 const&);

    using MF44 = Matrix<float, 4, 4>;
    using VF4 = Vector<float, 4>;
    template MF44 GetInverse(MF44 const&, float*);
    template MF44 GetAdjoint(MF44 const&);
    template float GetDeterminant(MF44 const&);
    template float GetTrace(MF44 const&);
    template MF44 MakeObliqueProjection(VF4 const&, VF4 const&, VF4 const&);
    template MF44 MakePerspectiveProjection(VF4 const&, VF4 const&, VF4 const&);
    template MF44 MakeReflection(VF4 const&, VF4 const&);

    using MD44 = Matrix<double, 4, 4>;
    using VD4 = Vector<double, 4>;
    template MD44 GetInverse(MD44 const&, double*);
    template MD44 GetAdjoint(MD44 const&);
    template double GetDeterminant(MD44 const&);
    template double GetTrace(MD44 const&);
    template MD44 MakeObliqueProjection(VD4 const&, VD4 const&, VD4 const&);
    template MD44 MakePerspectiveProjection(VD4 const&, VD4 const&, VD4 const&);
    template MD44 MakeReflection(VD4 const&, VD4 const&);

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;

    template class Matrix<Rational, 2, 3>;
    using MR22 = Matrix<Rational, 2, 2>;
    using MR23 = Matrix<Rational, 2, 3>;
    using MR32 = Matrix<Rational, 3, 2>;
    using MR33 = Matrix<Rational, 3, 3>;
    using VR2 = Vector<Rational, 2>;
    using VR3 = Vector<Rational, 3>;
    template bool operator==(MR23 const&, MR23 const&);
    template bool operator!=(MR23 const&, MR23 const&);
    template bool operator<(MR23 const&, MR23 const&);
    template bool operator<=(MR23 const&, MR23 const&);
    template bool operator>(MR23 const&, MR23 const&);
    template bool operator>=(MR23 const&, MR23 const&);
    template void MakeZero(MR23&);
    template bool IsZero(MR23 const&);
    template void MakeUnit(std::size_t, std::size_t, MR23&);
    template bool IsUnit(std::size_t, std::size_t, MR23 const&);
    template void MakeIdentity(MR22&);
    template bool IsIdentity(MR22 const&);
    template void MakeDiagonal(std::array<Rational, 2> const&, MR22&);
    template bool IsDiagonal(MR22 const&);
    template MR23 OuterProduct(VR2 const&, VR3 const&);
    template MR23 operator+(MR23 const&);
    template MR23 operator-(MR23 const&);
    template MR23 operator+(MR23 const&, MR23 const&);
    template MR23& operator+=(MR23&, MR23 const&);
    template MR23 operator-(MR23 const&, MR23 const&);
    template MR23& operator-=(MR23&, MR23 const&);
    template MR23 operator*(MR23 const&, Rational const&);
    template MR23 operator*(Rational const&, MR23 const&);
    template MR23& operator*=(MR23&, Rational const&);
    template MR23 operator/(MR23 const&, Rational const&);
    template MR23& operator/=(MR23&, Rational const&);
    template Rational L1Norm(MR23 const&);
    template Rational L2Norm(MR23 const&);
    template Rational LInfinityNorm(MR23 const&);
    template Rational Trace(MR22 const&);
    template MR32 Transpose(MR23 const&);
    template VR2 operator*(MR23 const&, VR3 const&);
    template VR3 operator*(VR2 const&, MR23 const&);
    template MR22 operator*(MR23 const&, MR32 const&);
    template MR22 MultiplyAB(MR23 const&, MR32 const&);
    template MR22 MultiplyABT(MR23 const&, MR23 const&);
    template MR33 MultiplyATB(MR23 const&, MR23 const&);
    template MR33 MultiplyATBT(MR23 const&, MR32 const&);
    template MR23 MultiplyMD(MR23 const&, std::array<Rational, 3> const&);
    template MR23 MultiplyDM(std::array<Rational, 2> const&, MR23 const&);
    template MR33 HLift(MR22 const&);
    template MR22 HProject(MR33 const&);

    template class Matrix<Rational>;
    using MRD = Matrix<Rational>;
    using VRD = Vector<Rational>;
    template bool operator==(MRD const&, MRD const&);
    template bool operator!=(MRD const&, MRD const&);
    template bool operator<(MRD const&, MRD const&);
    template bool operator<=(MRD const&, MRD const&);
    template bool operator>(MRD const&, MRD const&);
    template bool operator>=(MRD const&, MRD const&);
    template void MakeZero(MRD&);
    template bool IsZero(MRD const&);
    template void MakeUnit(std::size_t, std::size_t, MRD&);
    template bool IsUnit(std::size_t, std::size_t, MRD const&);
    template void MakeIdentity(MRD&);
    template bool IsIdentity(MRD const&);
    template void MakeDiagonal(std::vector<Rational> const&, MRD&);
    template bool IsDiagonal(MRD const&);
    template MRD OuterProduct(VRD const&, VRD const&);
    template MRD operator+(MRD const&);
    template MRD operator-(MRD const&);
    template MRD operator+(MRD const&, MRD const&);
    template MRD& operator+=(MRD&, MRD const&);
    template MRD operator-(MRD const&, MRD const&);
    template MRD& operator-=(MRD&, MRD const&);
    template MRD operator*(MRD const&, Rational const&);
    template MRD operator*(Rational const&, MRD const&);
    template MRD& operator*=(MRD&, Rational const&);
    template MRD operator/(MRD const&, Rational const&);
    template MRD& operator/=(MRD&, Rational const&);
    template Rational L1Norm(MRD const&);
    template Rational L2Norm(MRD const&);
    template Rational LInfinityNorm(MRD const&);
    template Rational Trace(MRD const&);
    template MRD Transpose(MRD const&);
    template VRD operator*(MRD const&, VRD const&);
    template VRD operator*(VRD const&, MRD const&);
    template MRD operator*(MRD const&, MRD const&);
    template MRD MultiplyAB(MRD const&, MRD const&);
    template MRD MultiplyABT(MRD const&, MRD const&);
    template MRD MultiplyATB(MRD const&, MRD const&);
    template MRD MultiplyATBT(MRD const&, MRD const&);
    template MRD MultiplyMD(MRD const&, std::vector<Rational> const&);
    template MRD MultiplyDM(std::vector<Rational> const&, MRD const&);
    template MRD HLift(MRD const&);
    template MRD HProject(MRD const&);

    using Rational = BSRational<UIntegerAP32>;
    using MR22 = Matrix<Rational, 2, 2>;
    template MR22 GetInverse(MR22 const&, Rational*);
    template MR22 GetAdjoint(MR22 const&);
    template Rational GetDeterminant(MR22 const&);
    template Rational GetTrace(MR22 const&);

    using Rational = BSRational<UIntegerAP32>;
    using MR33 = Matrix<Rational, 3, 3>;
    template MR33 GetInverse(MR33 const&, Rational*);
    template MR33 GetAdjoint(MR33 const&);
    template Rational GetDeterminant(MR33 const&);
    template Rational GetTrace(MR33 const&);

    using MR44 = Matrix<Rational, 4, 4>;
    using VR4 = Vector<Rational, 4>;
    template MR44 GetInverse(MR44 const&, Rational*);
    template MR44 GetAdjoint(MR44 const&);
    template Rational GetDeterminant(MR44 const&);
    template Rational GetTrace(MR44 const&);
    template MR44 MakeObliqueProjection(VR4 const&, VR4 const&, VR4 const&);
    template MR44 MakePerspectiveProjection(VR4 const&, VR4 const&, VR4 const&);
    template MR44 MakeReflection(VR4 const&, VR4 const&);
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(Matrix)
