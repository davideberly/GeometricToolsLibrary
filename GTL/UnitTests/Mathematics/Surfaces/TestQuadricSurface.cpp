#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Surfaces/QuadricSurface.h>
#include <GTL/Mathematics/Algebra/Polynomial.h>
#include <memory>
using namespace gtl;

namespace gtl
{
    class UnitTestQuadricSurface
    {
    public:
        using Rational = QuadricSurface<double>::Rational;
        using Quadric = QuadricSurface<Rational>;
        using Classification = Quadric::Classification;
        UnitTestQuadricSurface();

    private:
        void TestDescartesRuleOfSigns();
        void TestOrthogonalSetTwoNonzero();
        void TestOrthogonalSetOneNonzero();
        void TestAllNonzero();
        void TestTwoNonzero();
        void TestOneNonzero();
        void TestAllZeros();

        Matrix3x3<Rational> mRotation;
    };
}

UnitTestQuadricSurface::UnitTestQuadricSurface()
    :
    mRotation{}
{
    UTInformation("Mathematics/Surfaces/QuadricSurface");

    // Create random rotation to compute A = R*D*R^T.
    Rational x = 1.0, y = 3.0, z = -1.0, w = 2.0;
    Rational xx = x * x, yy = y * y, zz = z * z, ww = w * w;
    Rational xy = x * y, xz = x * z, xw = x * w;
    Rational yz = y * z, yw = y * w, zw = z * w;
    Rational sqrLen = ww + xx + yy + zz;
    Rational two(2);
    mRotation(0, 0) = ww + xx - yy - zz;
    mRotation(0, 1) = two * (xy - zw);
    mRotation(0, 2) = two * (xz + yw);
    mRotation(1, 0) = two * (xy + zw);
    mRotation(1, 1) = ww - xx + yy - zz;
    mRotation(1, 2) = two * (yz - xw);
    mRotation(2, 0) = two * (xz - yw);
    mRotation(2, 1) = two * (yz + xw);
    mRotation(2, 2) = ww - xx - yy + zz;
    mRotation /= sqrLen;

    TestDescartesRuleOfSigns();
    TestOrthogonalSetTwoNonzero();
    TestOrthogonalSetOneNonzero();
    TestAllNonzero();
    TestTwoNonzero();
    TestOneNonzero();
    TestAllZeros();
}

void UnitTestQuadricSurface::TestDescartesRuleOfSigns()
{
    Polynomial1<double> p(3);
    Polynomial1<double> factor0(1), factor1(1), factor2(1);
    factor0[1] = 1.0;
    factor1[1] = 1.0;
    factor2[1] = 1.0;

    std::size_t numPositive, numNegative, numZero;
    std::array<Rational, 4> rP{};

    // x^3 + 6x^2 + 11x + 6, (p,n,z) = (0,3,0)
    factor0[0] = 3.0;
    factor1[0] = 2.0;
    factor2[0] = 1.0;
    p = factor0 * factor1 * factor2;
    rP[0] = p[0];
    rP[1] = p[1];
    rP[2] = p[2];
    rP[3] = p[3];
    QuadricSurface<double>::ComputeRootSigns(rP, numPositive, numNegative, numZero);
    UTAssert(numPositive == 0 && numNegative == 3 && numZero == 0, "Incorrect counts");

    // x^3 + 4x^2 + x - 6, (p,n,z) = (1,2,0)
    factor0[0] = 3.0;
    factor1[0] = 2.0;
    factor2[0] = -1.0;
    p = factor0 * factor1 * factor2;
    rP[0] = p[0];
    rP[1] = p[1];
    rP[2] = p[2];
    rP[3] = p[3];
    QuadricSurface<double>::ComputeRootSigns(rP, numPositive, numNegative, numZero);
    UTAssert(numPositive == 1 && numNegative == 2 && numZero == 0, "Incorrect counts");

    // x^3 - 7x - 6, (p,n,z) = (2,1,0)
    factor0[0] = 3.0;
    factor1[0] = -2.0;
    factor2[0] = -1.0;
    p = factor0 * factor1 * factor2;
    rP[0] = p[0];
    rP[1] = p[1];
    rP[2] = p[2];
    rP[3] = p[3];
    QuadricSurface<double>::ComputeRootSigns(rP, numPositive, numNegative, numZero);
    UTAssert(numPositive == 2 && numNegative == 1 && numZero == 0, "Incorrect counts");

    // x^3 - 6x^2 + 11x - 6, (p,n,z) = (3,0,0)
    factor0[0] = -3.0;
    factor1[0] = -2.0;
    factor2[0] = -1.0;
    p = factor0 * factor1 * factor2;
    rP[0] = p[0];
    rP[1] = p[1];
    rP[2] = p[2];
    rP[3] = p[3];
    QuadricSurface<double>::ComputeRootSigns(rP, numPositive, numNegative, numZero);
    UTAssert(numPositive == 3 && numNegative == 0 && numZero == 0, "Incorrect counts");

    // x^3 + 5x^2 + 6x, (p,n,z) = (0,2,1)
    factor0[0] = 3.0;
    factor1[0] = 2.0;
    factor2[0] = 0.0;
    p = factor0 * factor1 * factor2;
    rP[0] = p[0];
    rP[1] = p[1];
    rP[2] = p[2];
    rP[3] = p[3];
    QuadricSurface<double>::ComputeRootSigns(rP, numPositive, numNegative, numZero);
    UTAssert(numPositive == 0 && numNegative == 2 && numZero == 1, "Incorrect counts");

    // x^3 + x^2 - 6x, (p,n,z) = (1,1,1)
    factor0[0] = 3.0;
    factor1[0] = -2.0;
    factor2[0] = 0.0;
    p = factor0 * factor1 * factor2;
    rP[0] = p[0];
    rP[1] = p[1];
    rP[2] = p[2];
    rP[3] = p[3];
    QuadricSurface<double>::ComputeRootSigns(rP, numPositive, numNegative, numZero);
    UTAssert(numPositive == 1 && numNegative == 1 && numZero == 1, "Incorrect counts");

    // x^3 - 5x^2 + 6x, (p,n,z) = (2,0,1)
    factor0[0] = -3.0;
    factor1[0] = -2.0;
    factor2[0] = 0.0;
    p = factor0 * factor1 * factor2;
    rP[0] = p[0];
    rP[1] = p[1];
    rP[2] = p[2];
    rP[3] = p[3];
    QuadricSurface<double>::ComputeRootSigns(rP, numPositive, numNegative, numZero);
    UTAssert(numPositive == 2 && numNegative == 0 && numZero == 1, "Incorrect counts");

    // x^3 + 3x^2, (p,n,z) = (0,1,2)
    factor0[0] = 3.0;
    factor1[0] = 0.0;
    factor2[0] = 0.0;
    p = factor0 * factor1 * factor2;
    rP[0] = p[0];
    rP[1] = p[1];
    rP[2] = p[2];
    rP[3] = p[3];
    QuadricSurface<double>::ComputeRootSigns(rP, numPositive, numNegative, numZero);
    UTAssert(numPositive == 0 && numNegative == 1 && numZero == 2, "Incorrect counts");

    // x^3 - 3x^2, (p,n,z) = (1,0,2)
    factor0[0] = -3.0;
    factor1[0] = 0.0;
    factor2[0] = 0.0;
    p = factor0 * factor1 * factor2;
    rP[0] = p[0];
    rP[1] = p[1];
    rP[2] = p[2];
    rP[3] = p[3];
    QuadricSurface<double>::ComputeRootSigns(rP, numPositive, numNegative, numZero);
    UTAssert(numPositive == 1 && numNegative == 0 && numZero == 2, "Incorrect counts");

    // x^3, (p,n,z) = (0,0,3)
    factor0[0] = 0.0;
    factor1[0] = 0.0;
    factor2[0] = 0.0;
    p = factor0 * factor1 * factor2;
    rP[0] = p[0];
    rP[1] = p[1];
    rP[2] = p[2];
    rP[3] = p[3];
    QuadricSurface<double>::ComputeRootSigns(rP, numPositive, numNegative, numZero);
    UTAssert(numPositive == 0 && numNegative == 0 && numZero == 3, "Incorrect counts");
}

void UnitTestQuadricSurface::TestOrthogonalSetTwoNonzero()
{
    Matrix3x3<Rational> A{};
    A(0, 0) = 2; A(0, 1) = 3; A(0, 2) = 5;
    A(1, 0) = 7; A(1, 1) = 11; A(1, 2) = 13;
    A(2, 0) = Rational(3) * A(0, 0) + Rational(2) * A(1, 0);
    A(2, 1) = Rational(3) * A(0, 1) + Rational(2) * A(1, 1);
    A(2, 2) = Rational(3) * A(0, 2) + Rational(2) * A(1, 2);

    Vector3<Rational> w0{}, w1{}, w2{};
    QuadricSurface<double>::ComputeOrthogonalSetTwoNonzero(A, w0, w1, w2);
    double dot00 = (double)Dot(w0, w0);
    double dot01 = (double)Dot(w0, w1);
    double dot02 = (double)Dot(w0, w2);
    double dot11 = (double)Dot(w1, w1);
    double dot12 = (double)Dot(w1, w2);
    double dot22 = (double)Dot(w2, w2);
    UTAssert(dot00 == 338.0, "Invalid orthogonal w0*w0");
    UTAssert(dot01 == 0.0, "Invalid orthogonal w0*w1");
    UTAssert(dot02 == 0.0, "Invalid orthogonal w0*w2");
    UTAssert(dot11 == 38.0, "Invalid orthogonal w1*w1");
    UTAssert(dot12 == 0.0, "Invalid orthogonal w1*w2");
    UTAssert(dot22 == 12844.0, "Invalid orthogonal w2*w2");

    A(0, 0) = 0; A(0, 1) = 0; A(0, 2) = 0;
    A(1, 0) = 2; A(1, 1) = 3; A(1, 2) = 5;
    A(2, 0) = 7; A(2, 1) = 11; A(2, 2) = 13;
    QuadricSurface<double>::ComputeOrthogonalSetTwoNonzero(A, w0, w1, w2);
    dot00 = (double)Dot(w0, w0);
    dot01 = (double)Dot(w0, w1);
    dot02 = (double)Dot(w0, w2);
    dot11 = (double)Dot(w1, w1);
    dot12 = (double)Dot(w1, w2);
    dot22 = (double)Dot(w2, w2);
    UTAssert(dot00 == 338.0, "Invalid orthogonal w0*w0");
    UTAssert(dot01 == 0.0, "Invalid orthogonal w0*w1");
    UTAssert(dot02 == 0.0, "Invalid orthogonal w0*w2");
    UTAssert(dot11 == 38.0, "Invalid orthogonal w1*w1");
    UTAssert(dot12 == 0.0, "Invalid orthogonal w1*w2");
    UTAssert(dot22 == 12844.0, "Invalid orthogonal w2*w2");
}

void UnitTestQuadricSurface::TestOrthogonalSetOneNonzero()
{
    Matrix3x3<Rational> A{};
    A(0, 0) = 2; A(0, 1) = 3; A(0, 2) = 5;
    A(1, 0) = 4; A(1, 1) = 6; A(1, 2) = 10;
    A(2, 0) = 10; A(2, 1) = 15; A(2, 2) = 25;

    Vector3<Rational> w0{}, w1{}, w2{};
    QuadricSurface<double>::ComputeOrthogonalSetOneNonzero(A, w0, w1, w2);
    double dot00 = (double)Dot(w0, w0);
    double dot01 = (double)Dot(w0, w1);
    double dot02 = (double)Dot(w0, w2);
    double dot11 = (double)Dot(w1, w1);
    double dot12 = (double)Dot(w1, w2);
    double dot22 = (double)Dot(w2, w2);
    UTAssert(dot00 == 34.0, "Invalid orthogonal w0*w0");
    UTAssert(dot01 == 0.0, "Invalid orthogonal w0*w1");
    UTAssert(dot02 == 0.0, "Invalid orthogonal w0*w2");
    UTAssert(dot11 == 1292.0, "Invalid orthogonal w1*w1");
    UTAssert(dot12 == 0.0, "Invalid orthogonal w1*w2");
    UTAssert(dot22 == 38.0, "Invalid orthogonal w2*w2");

    A(0, 0) = 0; A(0, 1) = 0; A(0, 2) = 0;
    A(1, 0) = 4; A(1, 1) = 6; A(1, 2) = 10;
    A(2, 0) = 10; A(2, 1) = 15; A(2, 2) = 25;
    QuadricSurface<double>::ComputeOrthogonalSetOneNonzero(A, w0, w1, w2);
    dot00 = (double)Dot(w0, w0);
    dot01 = (double)Dot(w0, w1);
    dot02 = (double)Dot(w0, w2);
    dot11 = (double)Dot(w1, w1);
    dot12 = (double)Dot(w1, w2);
    dot22 = (double)Dot(w2, w2);
    UTAssert(dot00 == 136.0, "Invalid orthogonal w0*w0");
    UTAssert(dot01 == 0.0, "Invalid orthogonal w0*w1");
    UTAssert(dot02 == 0.0, "Invalid orthogonal w0*w2");
    UTAssert(dot11 == 20672.0, "Invalid orthogonal w1*w1");
    UTAssert(dot12 == 0.0, "Invalid orthogonal w1*w2");
    UTAssert(dot22 == 152.0, "Invalid orthogonal w2*w2");

    A(0, 0) = 0; A(0, 1) = 0; A(0, 2) = 0;
    A(1, 0) = 0; A(1, 1) = 0; A(1, 2) = 0;
    A(2, 0) = 10; A(2, 1) = 15; A(2, 2) = 25;
    QuadricSurface<double>::ComputeOrthogonalSetOneNonzero(A, w0, w1, w2);
    dot00 = (double)Dot(w0, w0);
    dot01 = (double)Dot(w0, w1);
    dot02 = (double)Dot(w0, w2);
    dot11 = (double)Dot(w1, w1);
    dot12 = (double)Dot(w1, w2);
    dot22 = (double)Dot(w2, w2);
    UTAssert(dot00 == 850.0, "Invalid orthogonal w0*w0");
    UTAssert(dot01 == 0.0, "Invalid orthogonal w0*w1");
    UTAssert(dot02 == 0.0, "Invalid orthogonal w0*w2");
    UTAssert(dot11 == 807500.0, "Invalid orthogonal w1*w1");
    UTAssert(dot12 == 0.0, "Invalid orthogonal w1*w2");
    UTAssert(dot22 == 950.0, "Invalid orthogonal w2*w2");
}

void UnitTestQuadricSurface::TestAllNonzero()
{
    Matrix3x3<Rational> A{}, D{};
    Vector3<Rational> b{};
    Rational c{}, pivot{};
    Quadric::Classification classification{};
    Quadric surface{};

    // numPositive = 3, numNegative = 0, numZero = 0
    D(0, 0) = 1;
    D(1, 1) = 2;
    D(2, 2) = 3;
    A = mRotation * D * Transpose(mRotation);
    b = { -1, 1, -1 };
    pivot = Dot(b, GetInverse(A) * b) / Rational(4);

    // r > 0
    c = pivot - Rational(1);
    surface = Quadric(A, b, c);
    classification = surface.GetClassification();
    UTAssert(classification == Classification::ELLIPSOID, "Expecting ellipsoid.");

    // r = 0
    c = pivot;
    surface = QuadricSurface<Rational>(A, b, c);
    classification = surface.GetClassification();
    UTAssert(classification == Classification::POINT, "Expecting point.");

    // r < 0
    c = pivot + Rational(1);
    surface = QuadricSurface<Rational>(A, b, c);
    classification = surface.GetClassification();
    UTAssert(classification == Classification::NO_SOLUTION, "Expecting no solution.");

    // numPositive = 2, numNegative = 1, numZero = 0
    D(0, 0) = 1;
    D(1, 1) = 2;
    D(2, 2) = -3;
    A = mRotation * D * Transpose(mRotation);
    b = { -1, 1, -1 };
    pivot = Dot(b, GetInverse(A) * b) / Rational(4);

    // r > 0
    c = pivot - Rational(1);
    surface = Quadric(A, b, c);
    classification = surface.GetClassification();
    UTAssert(classification == Classification::HYPERBOLOID_ONE_SHEET, "Expecting hyperboloid one sheet.");

    // r = 0
    c = pivot;
    surface = QuadricSurface<Rational>(A, b, c);
    classification = surface.GetClassification();
    UTAssert(classification == Classification::ELLIPTIC_CONE, "Expecting elliptic cone.");

    // r < 0
    c = pivot + Rational(1);
    surface = QuadricSurface<Rational>(A, b, c);
    classification = surface.GetClassification();
    UTAssert(classification == Classification::HYPERBOLOID_TWO_SHEETS, "Expecting hyperboloid two sheets.");

    // numPositive = 1, numNegative = 2, numZero = 0
    D(0, 0) = 1;
    D(1, 1) = -2;
    D(2, 2) = -3;
    A = mRotation * D * Transpose(mRotation);
    b = { -1, 1, -1 };
    pivot = Dot(b, GetInverse(A) * b) / Rational(4);

    // r > 0
    c = pivot - Rational(1);
    surface = Quadric(A, b, c);
    classification = surface.GetClassification();
    UTAssert(classification == Classification::HYPERBOLOID_TWO_SHEETS, "Expecting hyperboloid two sheets.");

    // r = 0
    c = pivot;
    surface = QuadricSurface<Rational>(A, b, c);
    classification = surface.GetClassification();
    UTAssert(classification == Classification::ELLIPTIC_CONE, "Expecting elliptic cone.");

    // r < 0
    c = pivot + Rational(1);
    surface = QuadricSurface<Rational>(A, b, c);
    classification = surface.GetClassification();
    UTAssert(classification == Classification::HYPERBOLOID_ONE_SHEET, "Expecting hyperboloid one sheet.");

    // numPositive = 0, numNegative = 3, numZero = 0
    D(0, 0) = -1;
    D(1, 1) = -2;
    D(2, 2) = -3;
    A = mRotation * D * Transpose(mRotation);
    b = { -1, 1, -1 };
    pivot = Dot(b, GetInverse(A) * b) / Rational(4);

    // r > 0
    c = pivot - Rational(1);
    surface = Quadric(A, b, c);
    classification = surface.GetClassification();
    UTAssert(classification == Classification::NO_SOLUTION, "Expecting no solution.");

    // r = 0
    c = pivot;
    surface = QuadricSurface<Rational>(A, b, c);
    classification = surface.GetClassification();
    UTAssert(classification == Classification::POINT, "Expecting point.");

    // r < 0
    c = pivot + Rational(1);
    surface = QuadricSurface<Rational>(A, b, c);
    classification = surface.GetClassification();
    UTAssert(classification == Classification::ELLIPSOID, "Expecting ellipsoid.");
}

void UnitTestQuadricSurface::TestTwoNonzero()
{
    Matrix3x3<Rational> A{}, D{};
    Vector3<Rational> b{}, w0{}, w1{}, w2{};
    Matrix2x2<Rational> E{};
    Vector2<Rational> f{};
    Rational c{}, cpivot{};
    Quadric::Classification classification{};
    Quadric surface{};
    Rational const two(2);

    // numPositive = 2, numNegative = 0, numZero = 1
    D(0, 0) = 1;
    D(1, 1) = 2;
    D(2, 2) = 0;
    A = mRotation * D * Transpose(mRotation);
    Quadric::ComputeOrthogonalSetTwoNonzero(A, w0, w1, w2);
    E(0, 0) = Dot(w1, A * w1);
    E(0, 1) = Dot(w1, A * w2);
    E(1, 0) = E(0, 1);
    E(1, 1) = Dot(w2, A * w2);

    // d0 > 0, r irrelevant
    b = two * w0 + w1 + w2;
    surface = QuadricSurface<Rational>(A, b, c);
    classification = surface.GetClassification();
    UTAssert(classification == Classification::ELLIPTIC_PARABOLOID, "Expecting elliptic paraboloid.");

    // d0 < 0, r irrelevant
    b = -two * w0 + w1 + w2;
    surface = QuadricSurface<Rational>(A, b, c);
    classification = surface.GetClassification();
    UTAssert(classification == Classification::ELLIPTIC_PARABOLOID, "Expecting elliptic paraboloid.");

    // d0 = 0, r > 0
    b = w1 + w2;
    f = { Dot(b, w1), Dot(b, w2) };
    cpivot = Dot(f, GetInverse(E) * f) / Rational(4);
    c = cpivot - Rational(1);
    surface = Quadric(A, b, c);
    classification = surface.GetClassification();
    UTAssert(classification == Classification::ELLIPTIC_CYLINDER, "Expecting elliptic cylinder.");

    // d0 = 0, r = 0
    c = cpivot;
    surface = Quadric(A, b, c);
    classification = surface.GetClassification();
    UTAssert(classification == Classification::LINE, "Expecting line.");

    // d0 = 0, r < 0
    c = cpivot + Rational(1);
    surface = Quadric(A, b, c);
    classification = surface.GetClassification();
    UTAssert(classification == Classification::NO_SOLUTION, "Expecting no solution.");


    // numPositive = 1, numNegative = 1, numZero = 1
    D(0, 0) = 1;
    D(1, 1) = -2;
    D(2, 2) = 0;
    A = mRotation * D * Transpose(mRotation);
    Quadric::ComputeOrthogonalSetTwoNonzero(A, w0, w1, w2);
    E(0, 0) = Dot(w1, A * w1);
    E(0, 1) = Dot(w1, A * w2);
    E(1, 0) = E(0, 1);
    E(1, 1) = Dot(w2, A * w2);

    // d0 > 0, r irrelevant
    b = two * w0 + w1 + w2;
    surface = QuadricSurface<Rational>(A, b, c);
    classification = surface.GetClassification();
    UTAssert(classification == Classification::HYPERBOLIC_PARABOLOID, "Expecting hyperbolic paraboloid.");

    // d0 < 0, r irrelevant
    b = -two * w0 + w1 + w2;
    surface = QuadricSurface<Rational>(A, b, c);
    classification = surface.GetClassification();
    UTAssert(classification == Classification::HYPERBOLIC_PARABOLOID, "Expecting hyperbolic paraboloid.");

    // d0 = 0, r > 0
    b = w1 + w2;
    f = { Dot(b, w1), Dot(b, w2) };
    cpivot = Dot(f, GetInverse(E) * f) / Rational(4);
    c = cpivot - Rational(1);
    surface = Quadric(A, b, c);
    classification = surface.GetClassification();
    UTAssert(classification == Classification::HYPERBOLIC_CYLINDER, "Expecting hyperbolic cylinder.");

    // d0 = 0, r = 0
    c = cpivot;
    surface = Quadric(A, b, c);
    classification = surface.GetClassification();
    UTAssert(classification == Classification::TWO_PLANES, "Expecting two planes.");

    // d0 = 0, r < 0
    c = cpivot + Rational(1);
    surface = Quadric(A, b, c);
    classification = surface.GetClassification();
    UTAssert(classification == Classification::HYPERBOLIC_CYLINDER, "Expecting hyperbolic cylinder.");


    // numPositive = 0, numNegative = 2, numZero = 1
    D(0, 0) = -1;
    D(1, 1) = -2;
    D(2, 2) = 0;
    A = mRotation * D * Transpose(mRotation);
    Quadric::ComputeOrthogonalSetTwoNonzero(A, w0, w1, w2);
    E(0, 0) = Dot(w1, A * w1);
    E(0, 1) = Dot(w1, A * w2);
    E(1, 0) = E(0, 1);
    E(1, 1) = Dot(w2, A * w2);

    // d0 > 0, r irrelevant
    b = two * w0 + w1 + w2;
    surface = QuadricSurface<Rational>(A, b, c);
    classification = surface.GetClassification();
    UTAssert(classification == Classification::ELLIPTIC_PARABOLOID, "Expecting elliptic paraboloid.");

    // d0 < 0, r irrelevant
    b = -two * w0 + w1 + w2;
    surface = QuadricSurface<Rational>(A, b, c);
    classification = surface.GetClassification();
    UTAssert(classification == Classification::ELLIPTIC_PARABOLOID, "Expecting elliptic paraboloid.");

    // d0 = 0, r > 0
    b = w1 + w2;
    f = { Dot(b, w1), Dot(b, w2) };
    cpivot = Dot(f, GetInverse(E) * f) / Rational(4);
    c = cpivot - Rational(1);
    surface = Quadric(A, b, c);
    classification = surface.GetClassification();
    UTAssert(classification == Classification::NO_SOLUTION, "Expecting no solution.");

    // d0 = 0, r = 0
    c = cpivot;
    surface = Quadric(A, b, c);
    classification = surface.GetClassification();
    UTAssert(classification == Classification::LINE, "Expecting line.");

    // d0 = 0, r < 0
    c = cpivot + Rational(1);
    surface = Quadric(A, b, c);
    classification = surface.GetClassification();
    UTAssert(classification == Classification::ELLIPTIC_CYLINDER, "Expecting elliptic cylinder.");
}

void UnitTestQuadricSurface::TestOneNonzero()
{
    Matrix3x3<Rational> A{}, D{};
    Vector3<Rational> b{}, w0{}, w1{}, w2{};
    Rational E{}, f{}, c{}, cpivot{};
    Quadric::Classification classification{};
    Quadric surface{};
    Rational const two(2);

    // numPositive = 1, numNegative = 0, numZero = 2
    D(0, 0) = 1;
    D(1, 1) = 0;
    D(2, 2) = 0;
    A = mRotation * D * Transpose(mRotation);
    Quadric::ComputeOrthogonalSetOneNonzero(A, w0, w1, w2);
    E = Dot(w2, A * w2);

    // d0 > 0, d1 > 0, r irrelevant
    b = w0 + w1 + w2;
    surface = QuadricSurface<Rational>(A, b, c);
    classification = surface.GetClassification();
    UTAssert(classification == Classification::PARABOLIC_CYLINDER, "Expecting parabolic cylinder.");

    // d0 > 0, d1 < 0, r irrelevant
    b = w0 - w1 + w2;
    surface = QuadricSurface<Rational>(A, b, c);
    classification = surface.GetClassification();
    UTAssert(classification == Classification::PARABOLIC_CYLINDER, "Expecting parabolic cylinder.");

    // d0 < 0, d1 > 0, r irrelevant
    b = -w0 + w1 + w2;
    surface = QuadricSurface<Rational>(A, b, c);
    classification = surface.GetClassification();
    UTAssert(classification == Classification::PARABOLIC_CYLINDER, "Expecting parabolic cylinder.");

    // d0 < 0, d1 < 0, r irrelevant
    b = -w0 - w1 + w2;
    surface = QuadricSurface<Rational>(A, b, c);
    classification = surface.GetClassification();
    UTAssert(classification == Classification::PARABOLIC_CYLINDER, "Expecting parabolic cylinder.");

    // d0 = 0, d1 = 0, r > 0
    b = w2;
    f = Dot(b, w2);
    cpivot = f * f / (Rational(4) * E);
    c = cpivot - Rational(1);
    surface = Quadric(A, b, c);
    classification = surface.GetClassification();
    UTAssert(classification == Classification::TWO_PLANES, "Expecting two planes.");

    // d0 = 0, d1 = 0, r = 0
    c = cpivot;
    surface = Quadric(A, b, c);
    classification = surface.GetClassification();
    UTAssert(classification == Classification::PLANE, "Expecting plane.");

    // d0 = 0, d1 = 0, r < 0
    c = cpivot + Rational(1);
    surface = Quadric(A, b, c);
    classification = surface.GetClassification();
    UTAssert(classification == Classification::NO_SOLUTION, "Expecting no solution.");
}

void UnitTestQuadricSurface::TestAllZeros()
{
    Matrix3x3<Rational> A{};  // zero matrix
    Vector3<Rational> b{};
    Rational c{};
    Quadric::Classification classification{};
    Quadric surface{};

    // b != (0,0,0), c irrelevant
    b = { 1, 0, 1 };
    c = 0;
    surface = Quadric(A, b, c);
    classification = surface.GetClassification();
    UTAssert(classification == Classification::PLANE, "Expecting plane.");

    // b = (0,0,0), c = 0
    b = { 0, 0, 0 };
    c = 0;
    surface = Quadric(A, b, c);
    classification = surface.GetClassification();
    UTAssert(classification == Classification::ENTIRE_SPACE, "Expecting entire space.");

    // b = (0,0,0), c ! 0
    b = { 0, 0, 0 };
    c = 1;
    surface = Quadric(A, b, c);
    classification = surface.GetClassification();
    UTAssert(classification == Classification::NO_SOLUTION, "Expecting no solution.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Surfaces/QuadricSurface.h>

namespace gtl
{
    template class QuadricSurface<float>;
    template class QuadricSurface<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class QuadricSurface<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(QuadricSurface)
