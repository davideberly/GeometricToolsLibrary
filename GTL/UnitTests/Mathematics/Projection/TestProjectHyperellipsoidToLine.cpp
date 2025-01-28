#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Projection/ProjectHyperellipsoidToLine.h>
#include <GTL/Mathematics/Primitives/2D/Ellipse2.h>
#include <GTL/Mathematics/Primitives/3D/Ellipsoid3.h>
#include <random>
using namespace gtl;

namespace gtl
{
    class UnitTestProjectHyperellipsoidToLine
    {
    public:
        UnitTestProjectHyperellipsoidToLine();

    private:
        void TestProject2();
        void TestProject3();
    };
}

UnitTestProjectHyperellipsoidToLine::UnitTestProjectHyperellipsoidToLine()
{
    UTInformation("Mathematics/Projection/ProjectHyperellipsoidToLine");

    TestProject2();
    TestProject3();
}

void UnitTestProjectHyperellipsoidToLine::TestProject2()
{
    Ellipse2<double> ellipse{};
    ellipse.center = { 0.003, -0.002 };
    ellipse.axis[0] = { 1.0, -2.0 };
    ellipse.axis[1] = { 2.0, 1.0 };
    ellipse.extent = { 7.0, 3.0 };
    Normalize(ellipse.axis[0]);
    Normalize(ellipse.axis[1]);

    Vector2<double> origin{ 1.0, 2.0 };
    Vector2<double> direction{ 3.0, 4.0 };
    Normalize(direction);

    double sMin = 0.0, sMax = 0.0;
    OrthogonalProject(ellipse, origin, direction, sMin, sMax);

    // Compute the extreme points.
    Matrix2x2<double> M{}, invM{};
    ellipse.GetM(M);
    ellipse.GetMInverse(invM);
    double t = 1.0 / std::sqrt(Dot(direction, invM * direction));
    Vector2<double> Xp = ellipse.center + t * (invM * direction);
    double qform = Dot(Xp - ellipse.center, M * (Xp - ellipse.center)) - 1.0;
    UTAssert(
        std::fabs(qform) <= 1e-15,
        "Invalid extreme point Xp.");

    Vector2<double> Xn = ellipse.center - t * (invM * direction);
    qform = Dot(Xn - ellipse.center, M * (Xn - ellipse.center)) - 1.0;
    UTAssert(
        std::fabs(qform) <= 1e-15,
        "Invalid extreme point Xn.");

    double sMax0 = Dot(direction, Xp - origin);
    double sMin0 = Dot(direction, Xn - origin);
    double sMaxDiff = sMax0 - sMax;
    double sMinDiff = sMin0 - sMin;
    UTAssert(
        std::fabs(sMinDiff) <= 1e-15,
        "Invalid smin.");
    UTAssert(
        std::fabs(sMaxDiff) <= 1e-15,
        "Invalid smin.");
}

void UnitTestProjectHyperellipsoidToLine::TestProject3()
{
    std::default_random_engine dre;
    std::uniform_real_distribution<double> rnd(-1.0, 1.0);

    Ellipsoid3<double> ellipsoid{};
    ellipsoid.center = { rnd(dre), rnd(dre), rnd(dre) };
    ellipsoid.axis[0] = { rnd(dre), rnd(dre), rnd(dre) };
    ComputeOrthonormalBasis(1, ellipsoid.axis[0], ellipsoid.axis[1], ellipsoid.axis[2]);
    ellipsoid.extent = { rnd(dre) + 1.1, rnd(dre) + 1.1, rnd(dre) + 1.1 };

    Vector3<double> origin{ 1.0, 2.0, 3.0 };
    Vector3<double> direction{ 3.0, 4.0, 5.0 };
    Normalize(direction);

    double sMin = 0.0, sMax = 0.0;
    OrthogonalProject(ellipsoid, origin, direction, sMin, sMax);

    // Compute the extreme points.
    Matrix3x3<double> M{}, invM{};
    ellipsoid.GetM(M);
    ellipsoid.GetMInverse(invM);
    double t = 1.0 / std::sqrt(Dot(direction, invM * direction));
    Vector3<double> Xp = ellipsoid.center + t * (invM * direction);
    double qform = Dot(Xp - ellipsoid.center, M * (Xp - ellipsoid.center)) - 1.0;
    UTAssert(
        std::fabs(qform) <= 1e-15,
        "Invalid extreme point Xp.");

    Vector3<double> Xn = ellipsoid.center - t * (invM * direction);
    qform = Dot(Xn - ellipsoid.center, M * (Xn - ellipsoid.center)) - 1.0;
    UTAssert(
        std::fabs(qform) <= 1e-15,
        "Invalid extreme point Xn.");

    double sMax0 = Dot(direction, Xp - origin);
    double sMin0 = Dot(direction, Xn - origin);
    double sMaxDiff = sMax0 - sMax;
    double sMinDiff = sMin0 - sMin;
    UTAssert(
        std::fabs(sMinDiff) <= 1e-15,
        "Invalid smin.");
    UTAssert(
        std::fabs(sMaxDiff) <= 1e-15,
        "Invalid smin.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Projection/ProjectHyperellipsoidToLine.h>

namespace gtl
{
    template void OrthogonalProject(Hyperellipsoid<float, 2> const&,
        Vector<float, 2> const&, Vector<float, 2> const&, float&, float&);

    template void OrthogonalProject(Hyperellipsoid<double, 3> const&,
        Vector<double, 3> const&, Vector<double, 3> const&, double&, double&);

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template void OrthogonalProject(Hyperellipsoid<Rational, 4> const&,
        Vector<Rational, 4> const&, Vector<Rational, 4> const&, Rational&, Rational&);
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ProjectHyperellipsoidToLine)
