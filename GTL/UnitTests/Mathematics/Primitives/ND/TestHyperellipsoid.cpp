#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Primitives/2D/Ellipse2.h>
#include <GTL/Mathematics/Primitives/3D/Ellipsoid3.h>
#include <GTL/Mathematics/Algebra/Matrix.h>
#include <GTL/Mathematics/Algebra/Vector.h>
using namespace gtl;

namespace gtl
{
    class UnitTestHyperellipsoid
    {
    public:
        UnitTestHyperellipsoid();

    private:
        void TestEllipse2();
        void TestEllipsoid3();
    };
}

UnitTestHyperellipsoid::UnitTestHyperellipsoid()
{
    UTInformation("Mathematics/Primitives/ND");

    TestEllipse2();
    TestEllipsoid3();
}

void UnitTestHyperellipsoid::TestEllipse2()
{
    double const maxError = 1e-12;
    double error0, error1, error2, error3;
    Ellipse2<double> ellipse;
    Vector<double, 2> C { 1.0001199536250087, 2.0000081669237559 };
    Matrix<double, 2, 2> A =
    {
        { 0.17437567938985582, -0.1008116113293356 },
        { -0.1008116113293356, 0.11563439445158194 }
    };

    SymmetricEigensolver<double, 2> es;
    es(A(0, 0), A(0, 1), A(1, 1));
    Vector<double, 2> U0 = es.GetEigenvector(0);
    Vector<double, 2> U1 = es.GetEigenvector(1);
    Vector<double, 2> extent{ 1.0 / std::sqrt(es.GetEigenvalue(0)), 1.0 / std::sqrt(es.GetEigenvalue(1)) };

    bool success;
    success = ellipse.FromCoefficients(A, -2.0 * A * C, Dot(C, A * C) - 1);
    error0 = Length(ellipse.center - C);
    error1 = Length(ellipse.axis[0] - U0);
    error2 = Length(ellipse.axis[1] - U1);
    error3 = Length(ellipse.extent - extent);
    UTAssert(
        success &&
        error0 <= maxError &&
        error1 <= maxError &&
        error2 <= maxError &&
        error3 <= maxError,
        "FromCoefficients(A,B,c) failed.");

    Matrix<double, 2, 2> M;
    ellipse.GetM(M);
    error0 = L1Norm(M - A);
    UTAssert(
        error0 <= maxError,
        "GetM failed.");

    Matrix<double, 2, 2> invM;
    ellipse.GetMInverse(invM);
    error0 = L1Norm(invM - Inverse(A));
    UTAssert(
        error0 <= maxError,
        "GetMInverse failed.");

    std::array<double, 6> expectedCoeff  // from Mathematica
    {
        -6.627246118699621,
        0.4709230281151898,
        -2.2561804357873703,
        1.507991460645127,
        -1.7436267437115711,
        1.0
    };

    std::array<double, 6> coeff;
    ellipse.ToCoefficients(coeff);
    error0 = Length(Vector<double, 6>(coeff) - Vector<double, 6>(expectedCoeff));
    UTAssert(
        error0 <= maxError,
        "ToCoefficients(coeff) failed.");

    success = ellipse.FromCoefficients(coeff);
    error0 = Length(ellipse.center - C);
    error1 = Length(ellipse.axis[0] - U0);
    error2 = Length(ellipse.axis[1] - U1);
    error3 = Length(ellipse.extent - extent);
    UTAssert(
        success &&
        error0 <= maxError &&
        error1 <= maxError &&
        error2 <= maxError &&
        error3 <= maxError,
        "FromCoefficients(coeff) failed.");

    Matrix<double, 2, 2> AA;
    Vector<double, 2> BB;
    double cc;
    ellipse.ToCoefficients(AA, BB, cc);
    error0 = L1Norm(AA - A);
    error1 = Length(BB + 2.0 * A * C);
    error2 = std::fabs(cc - (Dot(C, A * C) - 1));
    UTAssert(
        error0 <= maxError &&
        error1 <= maxError &&
        error2 <= maxError,
        "ToCoefficients(A,B,c) failed.");

    success = ellipse.FromCoefficients(AA, BB, cc);
    error0 = Length(ellipse.center - C);
    error1 = Length(ellipse.axis[0] - U0);
    error2 = Length(ellipse.axis[1] - U1);
    error3 = Length(ellipse.extent - extent);
    UTAssert(
        success &&
        error0 <= maxError &&
        error1 <= maxError &&
        error2 <= maxError &&
        error3 <= maxError,
        "FromCoefficients(A,B,c) failed.");
}

void UnitTestHyperellipsoid::TestEllipsoid3()
{
    double const maxError = 1e-12;
    double error0, error1, error2, error3, error4;

    Vector<double, 3> center{ 1.0, 2.0, 3.0 };
    Vector<double, 3> extent{ 2.0, 3.0, 5.0 };
    Vector<double, 3> axis0{ 1.0, 1.0, 1.0 };
    Vector<double, 3> axis1{ 0.0, 1.0, -1.0 };
    Vector<double, 3> axis2 = Cross(axis0, axis1);
    Normalize(axis0);
    Normalize(axis1);
    Normalize(axis2);

    Ellipsoid3<double> ellipsoid;
    ellipsoid.center = center;
    ellipsoid.axis[0] = axis0;
    ellipsoid.axis[1] = axis1;
    ellipsoid.axis[2] = axis2;
    ellipsoid.extent = extent;

    Matrix<double, 3, 3> M;
    ellipsoid.GetM(M);
    Matrix<double, 3, 3> expectedM
    {
        { 11.0 / 100.0, 7.0 / 100.0, 7.0 / 100.0 },
        { 7.0 / 100.0, 131.0 / 900.0, 31.0 / 900.0 },
        { 7.0 / 100.0, 31.0 / 900.0, 131.0 / 900.0 }
    };
    error0 = L1Norm(M - expectedM);
    UTAssert(
        error0 <= maxError,
        "GetM failed.");

    Matrix<double, 3, 3> invM;
    ellipsoid.GetMInverse(invM);
    error0 = L1Norm(invM - Inverse(expectedM));
    UTAssert(
        error0 <= maxError,
        "GetMInverse failed.");

    std::array<double, 10> coeff;
    ellipsoid.ToCoefficients(coeff);
    std::array<double, 10> expectedCoeff =
    {
        1904.0 / 131.0,
        -828.0 / 131.0,
        -836.0 / 131.0,
        -1036.0 / 131.0,
        99.0 / 131.0,
        2.0 * 63.0 / 131.0,
        2.0 * 63.0 / 131.0,
        1.0,
        2.0 * 31.0 / 131.0,
        1.0
    };
    error0 = Length(Vector<double, 10>(coeff) - Vector<double, 10>(expectedCoeff));
    UTAssert(
        error0 <= maxError,
        "ToCoefficients(coeff) failed.");

    // ellipsoid.axis[] = { -axis2, -axis1, -axis0 }
    // ellipsoid.extent = { extent2, extent1, extent0 }
    bool success = ellipsoid.FromCoefficients(coeff);
    error0 = Length(ellipsoid.center - center);
    error1 = Length(ellipsoid.axis[0] + axis2);
    error2 = Length(ellipsoid.axis[1] + axis1);
    error3 = Length(ellipsoid.axis[2] + axis0);
    error4 = Length(ellipsoid.extent - Vector<double, 3>{ extent[2], extent[1], extent[0] });
    UTAssert(
        success &&
        error0 <= maxError &&
        error1 <= maxError &&
        error2 <= maxError &&
        error3 <= maxError &&
        error4 <= maxError,
        "FromCoefficients(coeff) failed.");

    Matrix<double, 3, 3> A;
    Vector<double, 3> B;
    double c;
    ellipsoid.ToCoefficients(A, B, c);
    Matrix<double, 3, 3> expectedA = expectedM;
    Vector<double, 3> expectedB = -2.0 * (expectedM * center);
    double expectedC = Dot(center, expectedM * center) - 1.0;
    error0 = L1Norm(A - expectedA);
    error1 = Length(B - expectedB);
    error2 = std::fabs(c - expectedC);
    UTAssert(
        error0 <= maxError &&
        error1 <= maxError &&
        error2 <= maxError,
        "ToCoefficients(A,B,c) failed.");

    success = ellipsoid.FromCoefficients(A, B, c);
    error0 = Length(ellipsoid.center - center);
    error1 = Length(ellipsoid.axis[0] + axis2);
    error2 = Length(ellipsoid.axis[1] + axis1);
    error3 = Length(ellipsoid.axis[2] + axis0);
    error4 = Length(ellipsoid.extent - Vector<double, 3>{ extent[2], extent[1], extent[0] });
    UTAssert(
        success&&
        error0 <= maxError &&
        error1 <= maxError &&
        error2 <= maxError &&
        error3 <= maxError &&
        error4 <= maxError,
        "FromCoefficients(A,B,c) failed.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Primitives/ND/Hyperellipsoid.h>

namespace gtl
{
    template class Hyperellipsoid<float, 2>;
    template bool operator==(Hyperellipsoid<float, 2> const&, Hyperellipsoid<float, 2> const&);
    template bool operator!=(Hyperellipsoid<float, 2> const&, Hyperellipsoid<float, 2> const&);
    template bool operator< (Hyperellipsoid<float, 2> const&, Hyperellipsoid<float, 2> const&);
    template bool operator<=(Hyperellipsoid<float, 2> const&, Hyperellipsoid<float, 2> const&);
    template bool operator> (Hyperellipsoid<float, 2> const&, Hyperellipsoid<float, 2> const&);
    template bool operator>=(Hyperellipsoid<float, 2> const&, Hyperellipsoid<float, 2> const&);

    template class Hyperellipsoid<double, 3>;
    template bool operator==(Hyperellipsoid<double, 3> const&, Hyperellipsoid<double, 3> const&);
    template bool operator!=(Hyperellipsoid<double, 3> const&, Hyperellipsoid<double, 3> const&);
    template bool operator< (Hyperellipsoid<double, 3> const&, Hyperellipsoid<double, 3> const&);
    template bool operator<=(Hyperellipsoid<double, 3> const&, Hyperellipsoid<double, 3> const&);
    template bool operator> (Hyperellipsoid<double, 3> const&, Hyperellipsoid<double, 3> const&);
    template bool operator>=(Hyperellipsoid<double, 3> const&, Hyperellipsoid<double, 3> const&);

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class Hyperellipsoid<Rational, 4>;
    template bool operator==(Hyperellipsoid<Rational, 4> const&, Hyperellipsoid<Rational, 4> const&);
    template bool operator!=(Hyperellipsoid<Rational, 4> const&, Hyperellipsoid<Rational, 4> const&);
    template bool operator< (Hyperellipsoid<Rational, 4> const&, Hyperellipsoid<Rational, 4> const&);
    template bool operator<=(Hyperellipsoid<Rational, 4> const&, Hyperellipsoid<Rational, 4> const&);
    template bool operator> (Hyperellipsoid<Rational, 4> const&, Hyperellipsoid<Rational, 4> const&);
    template bool operator>=(Hyperellipsoid<Rational, 4> const&, Hyperellipsoid<Rational, 4> const&);
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(Hyperellipsoid)
