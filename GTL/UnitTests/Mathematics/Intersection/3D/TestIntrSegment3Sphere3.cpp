#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Intersection/3D/IntrSegment3Sphere3.h>
using namespace gtl;

namespace gtl
{
    class UnitTestIntrSegment3Sphere3
    {
    public:
        UnitTestIntrSegment3Sphere3();

    private:
        void Test();
    };
}

UnitTestIntrSegment3Sphere3::UnitTestIntrSegment3Sphere3()
{
    UTInformation("Mathematics/Intersection/3D/IntrSegment3Sphere3");

    Test();
}

void UnitTestIntrSegment3Sphere3::Test()
{
    Sphere3<double> sphere{};
    sphere.center = { 0.5, 0.5, 0.5 };
    sphere.radius = 0.5;

    Segment3<double> segment{};
    segment.p[0] = { -0.1, -0.2, -0.3 };
    segment.p[1] = segment.p[0] + Vector3<double>{ 2.0, 2.0, 2.0 };

    FIQuery<double, Segment3<double>, Sphere3<double>> fiQuery{};
    TIQuery<double, Segment3<double>, Sphere3<double>> tiQuery{};

    auto fiOutput = fiQuery(segment, sphere);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 2,
        "Invalid FIQuery.");

    // The fiOutput.point[] values are computed from the segment and the
    // segment parameters. Verify that the points are on the sphere.
    Vector3<double> diff = fiOutput.point[0] - sphere.center;
    double radius = Length(diff);
    double error = std::fabs(radius - sphere.radius);
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");

    diff = fiOutput.point[1] - sphere.center;
    radius = Length(diff);
    error = std::fabs(radius - sphere.radius);
    UTAssert(
        error <= 1e-14,
        "Invalid FIQuery.");

    auto tiOutput = tiQuery(segment, sphere);
    UTAssert(
        tiOutput.intersect == true,
        "Invalid TIQuery.");

    // The segment p[0] is inside the sphere.
    segment.p[0] = 0.5 * (fiOutput.point[0] + fiOutput.point[1]);
    segment.p[1] = segment.p[0] + Vector3<double>{ 1.0, 1.0, 1.0 };
    fiOutput = fiQuery(segment, sphere);
    diff = fiOutput.point[0] - segment.p[0];
    error = Length(diff);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 2 &&
        error <= 1e-15,
        "Invalid FIQuery.");

    // The intersection point[1] is on the sphere.
    diff = fiOutput.point[1] - sphere.center;
    radius = Length(diff);
    error = std::fabs(radius - sphere.radius);
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");

    tiOutput = tiQuery(segment, sphere);
    UTAssert(
        tiOutput.intersect == true,
        "Invalid TIQuery.");

    // The segment p[1] is inside the sphere.
    segment.p[1] = 0.5 * (fiOutput.point[0] + fiOutput.point[1]);
    segment.p[0] = segment.p[1] - Vector3<double>{ 1.0, 1.0, 1.0 };
    fiOutput = fiQuery(segment, sphere);
    diff = fiOutput.point[1] - segment.p[1];
    error = Length(diff);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 2 &&
        error <= 1e-15,
        "Invalid FIQuery.");

    // The intersection point[0] is on the sphere.
    diff = fiOutput.point[0] - sphere.center;
    radius = Length(diff);
    error = std::fabs(radius - sphere.radius);
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");

    tiOutput = tiQuery(segment, sphere);
    UTAssert(
        tiOutput.intersect == true,
        "Invalid TIQuery.");

    // no intersection
    segment.p[0] = { 1.1, 1.2, 1.3 };
    segment.p[1] = segment.p[0] + Vector3<double>{ 1.0, 1.0, 1.0 };
    fiOutput = fiQuery(segment, sphere);
    UTAssert(
        fiOutput.intersect == false,
        "Invalid FIQuery.");

    tiOutput = tiQuery(segment, sphere);
    UTAssert(
        tiOutput.intersect == false,
        "Invalid TIQuery.");

    segment.p[1] = { -1.1, -1.2, -1.3 };
    segment.p[0] = segment.p[1] - Vector3<double>{ 1.0, 1.0, 1.0 };
    fiOutput = fiQuery(segment, sphere);
    UTAssert(
        fiOutput.intersect == false,
        "Invalid FIQuery.");

    tiOutput = tiQuery(segment, sphere);
    UTAssert(
        tiOutput.intersect == false,
        "Invalid TIQuery.");

    segment.p[0] = { 0.0, -0.001, 0.5 };
    segment.p[1] = segment.p[0] + Vector3<double>{ 1.0, 0.0, 0.0 };
    fiOutput = fiQuery(segment, sphere);
    UTAssert(
        fiOutput.intersect == false,
        "Invalid FIQuery.");

    tiOutput = tiQuery(segment, sphere);
    UTAssert(
        tiOutput.intersect == false,
        "Invalid TIQuery.");

    // tangential intersection
    segment.p[0] = { 0.0, 0.0, 0.5 };
    segment.p[1] = segment.p[0] + Vector3<double>{ 1.0, 0.0, 0.0 };
    fiOutput = fiQuery(segment, sphere);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 1,
        "Invalid FIQuery.");
    diff = fiOutput.point[0] - Vector3<double>{ 0.5, 0.0, 0.5 };
    error = Length(diff);
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");

    tiOutput = tiQuery(segment, sphere);
    UTAssert(
        tiOutput.intersect == true,
        "Invalid TIQuery.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Intersection/3D/IntrSegment3Sphere3.h>

namespace gtl
{
    template class TIQuery<float, Segment3<float>, Sphere3<float>>;
    template class FIQuery<float, Segment3<float>, Sphere3<float>>;

    template class TIQuery<double, Segment3<double>, Sphere3<double>>;
    template class FIQuery<double, Segment3<double>, Sphere3<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class TIQuery<Rational, Segment3<Rational>, Sphere3<Rational>>;
    template class FIQuery<Rational, Segment3<Rational>, Sphere3<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntrSegment3Sphere3)
