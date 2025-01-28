#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Intersection/3D/IntrLine3Ellipsoid3.h>
using namespace gtl;

namespace gtl
{
    class UnitTestIntrLine3Ellipsoid3
    {
    public:
        UnitTestIntrLine3Ellipsoid3();

    private:
        void Test();
    };
}

UnitTestIntrLine3Ellipsoid3::UnitTestIntrLine3Ellipsoid3()
{
    UTInformation("Mathematics/Intersection/3D/IntrLine3Ellipsoid3");

    Test();
}

void UnitTestIntrLine3Ellipsoid3::Test()
{
    Ellipsoid3<double> ellipsoid{};
    ellipsoid.center = { 0.5, 0.5, 0.5 };
    ellipsoid.axis[0] = { 1.0, 0.0, 0.0 };
    ellipsoid.axis[1] = { 0.0, 1.0, 0.0 };
    ellipsoid.axis[2] = { 0.0, 0.0, 1.0 };
    ellipsoid.extent = { 0.5, 0.51, 0.52 };

    Line3<double> line{};
    line.origin = { -0.1, -0.2, -0.3 };
    line.direction = { 1.0, 1.0, 1.0 };
    Normalize(line.direction);

    FIQuery<double, Line3<double>, Ellipsoid3<double>> fiQuery{};
    TIQuery<double, Line3<double>, Ellipsoid3<double>> tiQuery{};

    auto fiOutput = fiQuery(line, ellipsoid);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 2,
        "Invalid FIQuery.");

    // The fiOutput.point[] values are computed from the line and the
    // line parameters. Verify that the points are on the ellipsoid.
    Matrix3x3<double> M;
    ellipsoid.GetM(M);
    Vector3<double> diff = fiOutput.point[0] - ellipsoid.center;
    double error = std::fabs(Dot(diff, M * diff) - 1.0);
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");

    diff = fiOutput.point[1] - ellipsoid.center;
    error = std::fabs(Dot(diff, M * diff) - 1.0);
    UTAssert(
        error <= 1e-14,
        "Invalid FIQuery.");

    auto tiOutput = tiQuery(line, ellipsoid);
    UTAssert(
        tiOutput.intersect == true,
        "Invalid TIQuery.");

    // no intersection
    line.direction = { 1.0, 0.0, 0.0 };
    fiOutput = fiQuery(line, ellipsoid);
    UTAssert(
        fiOutput.intersect == false,
        "Invalid FIQuery.");

    tiOutput = tiQuery(line, ellipsoid);
    UTAssert(
        tiOutput.intersect == false,
        "Invalid TIQuery.");

    // tangential intersection
    ellipsoid.center = { 0.0, 0.0, 0.0 };
    ellipsoid.extent = { 1.0, 2.0, 4.0 };
    line.origin = { 1.0, -1.0, 0.0 };
    line.direction = { 0.0, 1.0, 0.0 };
    fiOutput = fiQuery(line, ellipsoid);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 1,
        "Invalid FIQuery.");
    diff = fiOutput.point[0] - Vector3<double>{ 1.0, 0.0, 0.0 };
    error = Length(diff);
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");

    tiOutput = tiQuery(line, ellipsoid);
    UTAssert(
        tiOutput.intersect == true,
        "Invalid TIQuery.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Intersection/3D/IntrLine3Ellipsoid3.h>

namespace gtl
{
    template class TIQuery<float, Line3<float>, Ellipsoid3<float>>;
    template class FIQuery<float, Line3<float>, Ellipsoid3<float>>;

    template class TIQuery<double, Line3<double>, Ellipsoid3<double>>;
    template class FIQuery<double, Line3<double>, Ellipsoid3<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class TIQuery<Rational, Line3<Rational>, Ellipsoid3<Rational>>;
    template class FIQuery<Rational, Line3<Rational>, Ellipsoid3<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntrLine3Ellipsoid3)
