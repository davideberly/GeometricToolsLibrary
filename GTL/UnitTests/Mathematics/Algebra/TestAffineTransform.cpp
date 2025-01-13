// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Algebra/AffineTransform.h>
using namespace gtl;

namespace gtl
{
    class UnitTestAffineTransform
    {
    public:
        UnitTestAffineTransform();

    private:
        void Test();
    };
}

UnitTestAffineTransform::UnitTestAffineTransform()
{
    UTInformation("Mathematics/Algebra/AffineTransform");

    Test();
}

void UnitTestAffineTransform::Test()
{
    // Constructor test.
    AffineTransform<double> transform{};
    UTAssert(transform.IsIdentity(), "Transform must be the identity.");
    UTAssert(transform.IsRSMatrix(), "Transform must be an RS matrix.");
    UTAssert(transform.IsUniformScale(), "Transform must be uniform scale.");
    UTAssert(IsIdentity(transform.mHMatrix), "mHMatrix must be the identity.");
    UTAssert(IsIdentity(transform.mInvHMatrix), "mInvHMatrix must be the identity.");
    UTAssert(IsIdentity(transform.mMatrix), "mMatrix must be the identity.");
    UTAssert(IsZero(transform.mTranslate), "mTranslate must be zero.");
    UTAssert(IsOne(transform.mScale), "mScale must be the 1-vector.");
    UTAssert(transform.mInverseNeedsUpdate == false, "mInverseNeedsUpdate must be false.");

    // SetMatrix
    Matrix3x3<double> mat0{};
    mat0.SetCol(0, { 1.0, 2.0, 3.0 });
    mat0.SetCol(1, { 4.0, 5.0, 6.0 });
    mat0.SetCol(2, { 1.0, 0.0, -1.0 });
    transform.SetMatrix(mat0);
    UTAssert(transform.GetMatrix() == mat0, "Transform must be mat0.");
    UTAssert(transform.mInverseNeedsUpdate == true, "Inverse requires an update.");

    // GetInverseH
    Matrix4x4<double> invMat0 = transform.GetInverseH();
    UTAssert(transform.mInverseNeedsUpdate == false, "Inverse does not require an update.");
    double norm = LInfinityNorm(invMat0);
    UTAssert(norm == 0.0, "The matrix is not invertible.");
    mat0.SetCol(0, { 1.0, 2.0, 1.0 });
    transform.SetMatrix(mat0);
    invMat0 = transform.GetInverseH();
    norm = LInfinityNorm(invMat0);
    UTAssert(norm != 0.0, "The matrix is invertible.");
    Matrix4x4<double> test = invMat0 * transform.GetH() - Matrix4x4<double>::Identity();
    norm = LInfinityNorm(test);
    UTAssert(norm <= 1e-15, "The matrix must be the identity.");

    // {Set,Get}Rotation
    Quaternion<double> q(1.0, 2.0, 3.0, 4.0);
    Normalize(q);
    Matrix3x3<double> rotate{};
    AxisAngle<double> aa{};
    EulerAngles<double> euler(0, 1, 2);
    Rotation<double>::Convert(q, rotate);
    Rotation<double>::Convert(q, aa);
    Rotation<double>::Convert(q, euler);

    transform.SetRotation(rotate);
    UTAssert(transform.IsIdentity() == false, "Matrix is not the identity.");
    UTAssert(transform.IsRSMatrix() == true, "Matrix is an RS matrix.");
    UTAssert(transform.IsUniformScale() == false, "Matrix is not a scaling matrix.");
    Matrix3x3<double> testRotate = transform.GetRotation();
    UTAssert(testRotate == rotate, "Set/GetRotation failed.");

    transform.SetRotation(q);
    testRotate = transform.GetRotation();
    norm = LInfinityNorm(rotate - testRotate);
    UTAssert(norm <= 1e-15, "Set/GetRotation failed.");

    transform.SetRotation(aa);
    testRotate = transform.GetRotation();
    norm = LInfinityNorm(rotate - testRotate);
    UTAssert(norm <= 1e-15, "Set/GetRotation failed.");

    transform.SetRotation(euler);
    testRotate = transform.GetRotation();
    norm = LInfinityNorm(rotate - testRotate);
    UTAssert(norm <= 1e-15, "Set/GetRotation failed.");

    // {Set,Get}Translation
    Vector3<double> trn{ 1.0, 2.0, 3.0 };
    transform.SetTranslation(trn[0], trn[1], trn[2]);
    Vector3<double> testTrn3 = transform.GetTranslation();
    norm = Length(testTrn3 - trn);
    UTAssert(norm <= 1e-15, "Set/GetTranslation failed.");

    transform.SetTranslation(trn);
    testTrn3 = transform.GetTranslation();
    norm = Length(testTrn3 - trn);
    UTAssert(norm <= 1e-15, "Set/GetTranslation failed.");

    // {Set,Get}Scale
    Vector3<double> scale{ 1.0, 2.0, 3.0 };
    transform.SetScale(scale[0], scale[1], scale[2]);
    Vector3<double> testSca3 = transform.GetScale();
    UTAssert(testSca3 == scale, "Set/GetScale failed.");

    transform.SetScale(scale);
    testSca3 = transform.GetScale();
    UTAssert(testSca3 == scale, "Set/GetScale failed.");

    transform.SetUniformScale(17.0);
    UTAssert(transform.GetUniformScale() == 17.0, "Set/GetUniformScale failed.");

    // GetNorm
    transform.SetRotation(rotate);
    transform.SetScale(scale);
    norm = transform.GetNorm();
    UTAssert(norm == scale[2], "GetNorm failed.");

    transform.SetMatrix(mat0);
    norm = transform.GetNorm();
    UTAssert(norm == 8.0, "GetNorm failed.");

    // GetInverseH blocks
    // 1. identity
    // 2. RSMatrix and uniform scale
    // 3. RSMatrix and not uniform scale
    // 4. not RSMatrix
    // Also test AffineTransform::Inverse.
    transform.MakeIdentity();
    Matrix4x4<double> testInverseH = transform.GetInverseH();
    UTAssert(testInverseH == Matrix4x4<double>::Identity(), "GetInverseH failed.");
    AffineTransform<double> invTransform = transform.Inverse();
    norm = LInfinityNorm(invTransform.GetH() - Matrix4x4<double>::Identity());
    UTAssert(norm <= 1e-15, "Inverse failed.");

    transform.SetRotation(rotate);
    transform.SetUniformScale(17.0);
    transform.SetTranslation(1.0, 2.0, 3.0);
    testInverseH = transform.GetInverseH();
    test = testInverseH * transform.GetH() - Matrix4x4<double>::Identity();
    norm = LInfinityNorm(test);
    UTAssert(norm <= 1e-15, "GetInverseH failed.");
    invTransform = transform.Inverse();
    auto mtest = invTransform.GetH() * transform.GetH();
    norm = LInfinityNorm(mtest - Matrix4x4<double>::Identity());
    UTAssert(norm <= 1e-15, "mtest failed.");
    AffineTransform<double> atest = invTransform * transform;
    norm = LInfinityNorm(atest.GetH() - Matrix4x4<double>::Identity());
    UTAssert(norm <= 1e-15, "atest failed.");

    // Transform has rotation, nonuniform scale, and translation.
    transform.SetScale(1.0, 2.0, 3.0);
    Matrix4x4<double> testH = transform.GetH();
    testInverseH = transform.GetInverseH();
    test = testInverseH * testH - Matrix4x4<double>::Identity();
    norm = LInfinityNorm(test);
    UTAssert(norm <= 1e-15, "GetInverseH failed.");
    invTransform = transform.Inverse();
    Matrix4x4<double> testInvTransformH = invTransform.GetH();
    mtest = testInvTransformH * testH;
    norm = LInfinityNorm(mtest - Matrix4x4<double>::Identity());
    UTAssert(norm <= 1e-15, "mtest failed.");
    atest = invTransform * transform;
    norm = LInfinityNorm(atest.GetH() - Matrix4x4<double>::Identity());
    UTAssert(norm <= 1e-15, "atest failed.");

    transform.SetMatrix(mat0);
    testInverseH = transform.GetInverseH();
    test = testInverseH * transform.GetH() - Matrix4x4<double>::Identity();
    norm = LInfinityNorm(test);
    UTAssert(norm <= 1e-15, "GetInverseH failed.");
    invTransform = transform.Inverse();
    mtest = invTransform.GetH() * transform.GetH();
    norm = LInfinityNorm(mtest - Matrix4x4<double>::Identity());
    UTAssert(norm <= 1e-15, "mtest failed.");
    atest = invTransform * transform;
    norm = LInfinityNorm(atest.GetH() - Matrix4x4<double>::Identity());
    UTAssert(norm <= 1e-15, "atest failed.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Algebra/AffineTransform.h>

namespace gtl
{
    template class AffineTransform<float>;
    template Vector4<float> operator*(AffineTransform<float> const&, Vector4<float> const&);
    template Vector4<float> operator*(Vector4<float> const&, AffineTransform<float> const&);
    template AffineTransform<float> operator*(AffineTransform<float> const&, AffineTransform<float> const&);
    template Matrix4x4<float> operator*(Matrix4x4<float> const&, AffineTransform<float> const&);
    template Matrix4x4<float> operator*(AffineTransform<float> const&, Matrix4x4<float> const&);

    template class AffineTransform<double>;
    template Vector4<double> operator*(AffineTransform<double> const&, Vector4<double> const&);
    template Vector4<double> operator*(Vector4<double> const&, AffineTransform<double> const&);
    template AffineTransform<double> operator*(AffineTransform<double> const&, AffineTransform<double> const&);
    template Matrix4x4<double> operator*(Matrix4x4<double> const&, AffineTransform<double> const&);
    template Matrix4x4<double> operator*(AffineTransform<double> const&, Matrix4x4<double> const&);

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class AffineTransform<Rational>;
    template Vector4<Rational> operator*(AffineTransform<Rational> const&, Vector4<Rational> const&);
    template Vector4<Rational> operator*(Vector4<Rational> const&, AffineTransform<Rational> const&);
    template AffineTransform<Rational> operator*(AffineTransform<Rational> const&, AffineTransform<Rational> const&);
    template Matrix4x4<Rational> operator*(Matrix4x4<Rational> const&, AffineTransform<Rational> const&);
    template Matrix4x4<Rational> operator*(AffineTransform<Rational> const&, Matrix4x4<Rational> const&);
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(AffineTransform)
