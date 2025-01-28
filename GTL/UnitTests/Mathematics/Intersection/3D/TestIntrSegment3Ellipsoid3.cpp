#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Intersection/3D/IntrSegment3Ellipsoid3.h>
using namespace gtl;

namespace gtl
{
    class UnitTestIntrSegment3Ellipsoid3
    {
    public:
        UnitTestIntrSegment3Ellipsoid3();

    private:
        void Test();
    };
}

UnitTestIntrSegment3Ellipsoid3::UnitTestIntrSegment3Ellipsoid3()
{
    UTInformation("Mathematics/Intersection/3D/IntrSegment3Ellipsoid3");

    Test();
}

void UnitTestIntrSegment3Ellipsoid3::Test()
{
    Ellipsoid3<double> ellipsoid{};
    ellipsoid.center = { 0.5, 0.5, 0.5 };
    ellipsoid.axis[0] = { 1.0, 0.0, 0.0 };
    ellipsoid.axis[1] = { 0.0, 1.0, 0.0 };
    ellipsoid.axis[2] = { 0.0, 0.0, 1.0 };
    ellipsoid.extent = { 0.5, 0.51, 0.52 };

    Segment3<double> segment{};
    segment.p[0] = { -0.1, -0.2, -0.3 };
    segment.p[1] = segment.p[0] + Vector3<double>{ 2.0, 2.0, 2.0 };

    FIQuery<double, Segment3<double>, Ellipsoid3<double>> fiQuery{};
    TIQuery<double, Segment3<double>, Ellipsoid3<double>> tiQuery{};

    auto fiOutput = fiQuery(segment, ellipsoid);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 2,
        "Invalid FIQuery.");

    // The fiOutput.point[] values are computed from the segment and the
    // segment parameters. Verify that the points are on the ellipsoid.
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
        error <= 1e-15,
        "Invalid FIQuery.");

    auto tiOutput = tiQuery(segment, ellipsoid);
    UTAssert(
        tiOutput.intersect == true,
        "Invalid TIQuery.");

    // The segment p[0] is inside the ellipsoid.
    segment.p[0] = 0.5 * (fiOutput.point[0] + fiOutput.point[1]);
    segment.p[1] = segment.p[0] + Vector3<double>{ 1.0, 1.0, 1.0 };
    fiOutput = fiQuery(segment, ellipsoid);
    diff = fiOutput.point[0] - segment.p[0];
    error = Length(diff);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 2 &&
        error <= 1e-15,
        "Invalid FIQuery.");

    // The intersection point[1] is on the ellipsoid.
    diff = fiOutput.point[1] - ellipsoid.center;
    error = std::fabs(Dot(diff, M * diff) - 1.0);
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");

    tiOutput = tiQuery(segment, ellipsoid);
    UTAssert(
        tiOutput.intersect == true,
        "Invalid TIQuery.");

    // The segment p[1] is inside the ellipsoid.
    segment.p[1] = 0.5 * (fiOutput.point[0] + fiOutput.point[1]);
    segment.p[0] = segment.p[1] - Vector3<double>{ 1.0, 1.0, 1.0 };
    fiOutput = fiQuery(segment, ellipsoid);
    diff = fiOutput.point[1] - segment.p[1];
    error = Length(diff);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 2 &&
        error <= 1e-15,
        "Invalid FIQuery.");

    // The intersection point[0] is on the ellipsoid.
    diff = fiOutput.point[0] - ellipsoid.center;
    error = std::fabs(Dot(diff, M * diff) - 1.0);
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");

    tiOutput = tiQuery(segment, ellipsoid);
    UTAssert(
        tiOutput.intersect == true,
        "Invalid TIQuery.");

    // no intersection
    segment.p[0] = { 1.1, 1.2, 1.3 };
    segment.p[1] = segment.p[0] + Vector3<double>{ 1.0, 1.0, 1.0 };
    fiOutput = fiQuery(segment, ellipsoid);
    UTAssert(
        fiOutput.intersect == false,
        "Invalid FIQuery.");

    tiOutput = tiQuery(segment, ellipsoid);
    UTAssert(
        tiOutput.intersect == false,
        "Invalid TIQuery.");

    segment.p[1] = { -1.1, -1.2, -1.3 };
    segment.p[0] = segment.p[1] - Vector3<double>{ 1.0, 1.0, 1.0 };
    fiOutput = fiQuery(segment, ellipsoid);
    UTAssert(
        fiOutput.intersect == false,
        "Invalid FIQuery.");

    tiOutput = tiQuery(segment, ellipsoid);
    UTAssert(
        tiOutput.intersect == false,
        "Invalid TIQuery.");

    // tangential intersection
    ellipsoid.center = { 0.0, 0.0, 0.0 };
    ellipsoid.extent = { 1.0, 2.0, 4.0 };
    segment.p[0] = { 1.0, -1.0, 0.0 };
    segment.p[1] = segment.p[0] + Vector3<double>{ 0.0, 2.0, 0.0 };
    fiOutput = fiQuery(segment, ellipsoid);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 1,
        "Invalid FIQuery.");
    diff = fiOutput.point[0] - Vector3<double>{ 1.0, 0.0, 0.0 };
    error = Length(diff);
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");

    tiOutput = tiQuery(segment, ellipsoid);
    UTAssert(
        tiOutput.intersect == true,
        "Invalid TIQuery.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Intersection/3D/IntrSegment3Ellipsoid3.h>

namespace gtl
{
    template class TIQuery<float, Segment3<float>, Ellipsoid3<float>>;
    template class FIQuery<float, Segment3<float>, Ellipsoid3<float>>;

    template class TIQuery<double, Segment3<double>, Ellipsoid3<double>>;
    template class FIQuery<double, Segment3<double>, Ellipsoid3<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class TIQuery<Rational, Segment3<Rational>, Ellipsoid3<Rational>>;
    template class FIQuery<Rational, Segment3<Rational>, Ellipsoid3<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntrSegment3Ellipsoid3)
